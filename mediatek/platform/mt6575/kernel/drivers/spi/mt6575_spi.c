#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/spi/spi.h>
#include <linux/workqueue.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/wakelock.h>
#include <mach/irqs.h>

#include <mach/mt6575_spi.h>
#include <mach/mt6575_gpio.h>
#include <mach/mt6575_clock_manager.h>
//#define SPI_DEBUG
#ifdef SPI_DEBUG
  #define SPI_DBG(fmt, args...)  printk ( KERN_INFO "SPI:%5d: <%s>" fmt, __LINE__,__func__,##args )
#else
  #define SPI_DBG(fmt, args...)
#endif

#define SPI_ERR(fmt, args...)  printk ( KERN_ERR "SPI:%5d:<%s>" fmt, __LINE__, __func__, ##args)

#define IDLE 0
#define INPROGRESS 1
#define PAUSED 2
#define PACKET_SIZE 0x400
#define FIFO_SIZE 32
#define FIFO_TX 0
#define FIFO_RX 1
#define INVALID_DMA_ADDRESS 0xffffffff

DECLARE_COMPLETION_ONSTACK(done);

struct mt6575_spi {
  struct platform_device *pdev;

  spinlock_t spin_lock;

  void __iomem *regs;

  int irq;
  int running;

  struct wake_lock wk_lock;

  struct mt6575_chip_conf *config;
  struct spi_master *master;
        struct spi_transfer *cur_transfer;
  struct spi_transfer *next_transfer;
  struct spi_message *cur_msg;
};

static void spi_gpio_set(struct mt6575_spi *mtspi)
{
  mt_set_gpio_mode(GPIO_SPI_CS_PIN, GPIO_SPI_CS_PIN_M_SPI_CS_N);
  mt_set_gpio_mode(GPIO_SPI_SCK_PIN, GPIO_SPI_SCK_PIN_M_SPI_SCK);
  mt_set_gpio_mode(GPIO_SPI_MISO_PIN, GPIO_SPI_MISO_PIN_M_SPI_MISO);
  mt_set_gpio_mode(GPIO_SPI_MOSI_PIN, GPIO_SPI_MOSI_PIN_M_SPI_MOSI);

  return;
}

static void spi_gpio_reset(struct mt6575_spi *mtspi)
{
  //when in sleep mode,set cs pin :imput mode /disalbe pull up
  mt_set_gpio_mode(GPIO_SPI_CS_PIN, GPIO_SPI_CS_PIN_M_GPIO);
  mt_set_gpio_dir(GPIO_SPI_CS_PIN, GPIO_DIR_IN);
  mt_set_gpio_pull_enable(GPIO_SPI_CS_PIN, GPIO_PULL_DISABLE);

  mt_set_gpio_mode(GPIO_SPI_SCK_PIN, GPIO_SPI_SCK_PIN_M_GPIO);
  mt_set_gpio_mode(GPIO_SPI_MISO_PIN, GPIO_SPI_MISO_PIN_M_GPIO);
  mt_set_gpio_mode(GPIO_SPI_MOSI_PIN, GPIO_SPI_MOSI_PIN_M_GPIO);

  return;
}

static void enable_clk(void)
{
  enable_clock(MT65XX_PDN_MM_SPI, "spi");
  return;
}

static void disable_clk(void)
{
  disable_clock(MT65XX_PDN_MM_SPI, "spi");
  return;
}

static int is_pause_mode(struct mt6575_spi *mtspi)
{
  struct mt6575_chip_conf *conf;
  conf = mtspi->config;
  return conf->pause;
}

static int set_pause_bit(struct mt6575_spi *mtspi)
{
  u32 reg_val;
  unsigned long flags;

  reg_val = spi_readl(mtspi, SPI_CMD_REG);
  reg_val |= 1 << SPI_CMD_PAUSE_EN_OFFSET;
  spi_writel(mtspi, SPI_CMD_REG, reg_val);

  return 0;
}

static int clear_pause_bit(struct mt6575_spi *mtspi)
{
  u32 reg_val;
  unsigned long flags;

  spin_lock(&mtspi->spin_lock);
  reg_val = spi_readl(mtspi, SPI_CMD_REG);
  reg_val &= ~SPI_CMD_PAUSE_EN_MASK;
  spi_writel(mtspi, SPI_CMD_REG, reg_val);
  spin_unlock(&mtspi->spin_lock);

  return 0;
}

static int spi_clear_fifo(struct mt6575_spi *mtspi, int fifo)
{
  u32 volatile reg_val;
  unsigned long flags;
  int i;

  for ( i = 0; i < FIFO_SIZE/4; i++ )
  {
    if ( fifo == FIFO_TX )
    {
      spi_writel ( mtspi, SPI_TX_DATA_REG, 0x0 );
    }
    else if ( fifo == FIFO_RX )
    {
      reg_val = spi_readl ( mtspi, SPI_RX_DATA_REG );
    }
    else
    {
      SPI_DBG("The fifo is not right.\n");
    }
  }
  return 0;
}

static int spi_is_busy(struct mt6575_spi *mtspi)
{
  u32 reg_val;
  unsigned long flags;

  reg_val = spi_readl(mtspi, SPI_STATUS1_REG);
  if ( reg_val & 0x1)
  {
    SPI_DBG("is not busy.\n");
    return 0;
  }
  else
  {
    SPI_DBG("is busy.\n");
    return 1;
  }
}

static int spi_start_transfer(struct mt6575_spi *mtspi)
{
  u32 reg_val;
  unsigned long flags;

  reg_val = spi_readl ( mtspi, SPI_CMD_REG );
  reg_val |= 1 << SPI_CMD_ACT_OFFSET;
  spi_writel ( mtspi, SPI_CMD_REG, reg_val);

  return 0;
}

static void spi_resume_transfer(struct mt6575_spi *mtspi)
{
  u32 reg_val;
  unsigned long flags;

  reg_val = spi_readl(mtspi, SPI_CMD_REG);
  reg_val &= ~SPI_CMD_RESUME_MASK;
  reg_val |= 1 << SPI_CMD_RESUME_OFFSET;
  spi_writel ( mtspi,  SPI_CMD_REG, reg_val );
  return;
}

static void reset_spi(struct mt6575_spi *mtspi)
{
  u32 reg_val;
  unsigned long flags;

  /*set the software reset bit in SPI_CMD_REG.*/
  spin_lock(&mtspi->spin_lock);
  reg_val=spi_readl(mtspi, SPI_CMD_REG);
  reg_val &= ~SPI_CMD_RST_MASK;
  reg_val |= 1 << SPI_CMD_RST_OFFSET;
  spi_writel(mtspi, SPI_CMD_REG, reg_val);
  //clear reset bit
  reg_val = spi_readl ( mtspi, SPI_CMD_REG );
  reg_val &= ~SPI_CMD_RST_MASK;
  spi_writel ( mtspi, SPI_CMD_REG, reg_val );
  spin_unlock(&mtspi->spin_lock);

  return;
}

static int transfer_dma_mapping (
  struct mt6575_spi *mtspi,
  struct spi_transfer *trans )
{
  struct device *dev =& mtspi->pdev->dev;

  SPI_DBG("transfer_dma_mapping. \n");

  trans->tx_dma = trans->rx_dma = INVALID_DMA_ADDRESS;
  if (trans->tx_buf==NULL && trans->rx_buf==NULL )
  {
    SPI_ERR("the tx_buf and rx_buf is not set");
    return -EINVAL;
  }

  if (trans->tx_buf)
  {
    trans->tx_dma = dma_map_single(dev,(void *)trans->tx_buf,trans->len,DMA_TO_DEVICE);
    SPI_DBG("dma mapping address is:0x%x.\n", trans->tx_dma);
    if (dma_mapping_error(dev, trans->tx_dma))
    {
      SPI_DBG("dma mapping tx_buf error.\n");
      return -ENOMEM;
    }
  }
  if (trans->rx_buf)
  {
    trans->rx_dma = dma_map_single ( dev,trans->rx_buf,trans->len, DMA_FROM_DEVICE);
    SPI_DBG("dma mapping rx_dma is:0x%x.\n", trans->rx_dma);
    if ( dma_mapping_error ( dev,trans->rx_dma ) )
    {
      if (trans->tx_buf)
      {
        //readwrite at the same time
        dma_unmap_single ( dev, trans->tx_dma,trans->len, DMA_TO_DEVICE );
      }
      SPI_DBG("dma mapping rx_buf error.\n");
      return -ENOMEM;
    }
  }
  return 0;
}

static int transfer_dma_unmapping (
  struct mt6575_spi *mtspi,
  struct spi_transfer *trans )
{
  struct device *dev=&mtspi->pdev->dev;
  SPI_DBG("transfer_dma_unmapping.\n");

  if ( trans->tx_dma != INVALID_DMA_ADDRESS )
  {
    dma_unmap_single(dev, trans->tx_dma,trans->len, DMA_TO_DEVICE);
  }
  if ( trans->rx_dma != INVALID_DMA_ADDRESS )
  {
    dma_unmap_single ( dev,  trans->rx_dma,trans->len, DMA_FROM_DEVICE );
  }
  return 0;
}

static int setup_next_transfer (
  struct spi_master *master,
  struct spi_transfer *trans )
{
  struct mt6575_spi *mtspi;
  struct spi_message *msg;
  u32 reg_val;
  u32 flags;
  int i;
  int ret =0;

  if ( !master )
  {
    dev_dbg(&master->dev, "master  %s: is invalid \n", dev_name(&master->dev));
    return -EINVAL;
  }

  mtspi = spi_master_get_devdata ( master );
  msg = mtspi->cur_msg;

  SPI_DBG ( "setup_next_transfer.\n" );
  reg_val = spi_readl ( mtspi, SPI_CMD_REG);
  /* if interrupt is enabled,then wait for interrupt complete. */
  if ((( reg_val & 0x30000 ) >> 16 ))
  {
    ret = wait_for_completion_timeout ( &done, 2*HZ);
	if(ret == 0){
		dev_err(&msg->spi->dev,"xfer error or timeout %d\n",ret);
    mtspi->cur_transfer = NULL;
    mtspi->running = IDLE;
		return -1;	
	}
  }else{
		 dev_err(&msg->spi->dev,"xfer interrupt disable %d,\n",mtspi->config->finish_intr);
		  mtspi->cur_transfer = NULL;
		   mtspi->running = IDLE;
		return -1;	

  }

  /*The last transfer of the message has been transferred.*/
  if ( trans->transfer_list.next == &msg->transfers )
  {
    SPI_DBG("The last transfer of the message has been transferred.\n");
    msg->complete ( msg->context );
    SPI_DBG ( "msg completion.\n" );
    mtspi->cur_transfer = NULL;
    mtspi->running = IDLE;
    return 0;
  }

  SPI_DBG("mtspi next_transfer is not NULL, so this is not the last tranfer.\n");
  mtspi->cur_transfer = mtspi->next_transfer;
  msg->actual_length += mtspi->cur_transfer->len;

  return msg->actual_length;
}

static int do_dma_transfer (
  struct spi_master *master,
  struct spi_transfer *transfer )
{
  unsigned long flags;
  u32 packet_loop;
  u32 packet_size;
  int ret = 0;
  int i;
  u32 cfg0, cfg1, cmd, tx, rx,dma_reg;
  struct mt6575_spi *mtspi;
  struct spi_message *msg;

  SPI_DBG("do_dma_transfer.\n");

  if ( !master )
  {
    dev_dbg( &master->dev,"master %s: is invalid, pls check it.\n",dev_name(&master->dev));
    return -EINVAL;
  }

  mtspi = spi_master_get_devdata ( master );
  msg = mtspi->cur_msg;
  msg->actual_length += mtspi->cur_transfer->len;

  do {
    if ( mtspi->cur_transfer->transfer_list.next == &msg->transfers )
    {
      SPI_DBG("This is the last transfer of this message which will be transferred.\n");
      mtspi->next_transfer = NULL;
      clear_pause_bit ( mtspi );
    }
    else
    {
      SPI_DBG("This is not last transfer of this message.\n");
      mtspi->next_transfer = list_entry (
        mtspi->cur_transfer->transfer_list.next,
        struct spi_transfer, transfer_list );
    }
    SPI_DBG("Set the SPI registers to start DMA transfer.\n");
    ret = transfer_dma_mapping ( mtspi, mtspi->cur_transfer );
    if ( ret < 0 )
    {
      SPI_DBG("transfer_dma_mapping failed.\n");
      return ret;
    }
    else
    {
      SPI_DBG("transfer_dma_mapping success.\n");
    }

    SPI_DBG("mtspi->cur_transfer->tx_buf:0x%x\n",(u32 *)mtspi->cur_transfer->tx_buf);
    SPI_DBG("mtspi->cur_transfer->rx_buf:0x%x\n",(u32 *)mtspi->cur_transfer->rx_buf);

    SPI_DBG("mtspi->cur_transfer->tx_dma:0x%x\n",(u32 *)mtspi->cur_transfer->tx_dma);
    SPI_DBG("mtspi->cur_transfer->rx_dma:0x%x\n",(u32 *)mtspi->cur_transfer->rx_dma);
    //check data size,data size must less than 1024 or be equal with x*1024(x<=256)
    if ( (mtspi->cur_transfer->len > PACKET_SIZE)&&(mtspi->cur_transfer->len % PACKET_SIZE != 0) )
    {
      SPI_ERR("The packet_loop is: 0x%x\n", packet_loop);
      return -EINVAL;
    }
    //check data size
    if ( mtspi->cur_transfer->len > 256*PACKET_SIZE )
    {
      SPI_ERR("The data size must not exceed 256k bytes");
      return -EINVAL;
    }

    //sixian:caculate the packet_loop and packet_count
    if ( mtspi->cur_transfer->len < PACKET_SIZE )
    {
      packet_size = mtspi->cur_transfer->len;
    }
    else
    {
      packet_size = PACKET_SIZE;
    }
    if ( mtspi->cur_transfer->len % packet_size )
    {
      packet_loop = mtspi->cur_transfer->len/packet_size + 1;
    }
    else
    {
      packet_loop = ( mtspi->cur_transfer->len ) /packet_size;
    }

    spin_lock ( &mtspi->spin_lock );//start write to H/W register
    /*set packet loop and count in this message.*/
    cfg1 = spi_readl ( mtspi, SPI_CFG1_REG );
    cfg1 &= ~SPI_CFG1_PACKET_LENGTH_MASK;
    cfg1 &= ~SPI_CFG1_PACKET_LOOP_MASK;
    cfg1 |= ( packet_size - 1 ) << SPI_CFG1_PACKET_LENGTH_OFFSET;
    cfg1 |= ( packet_loop - 1 ) << SPI_CFG1_PACKET_LOOP_OFFSET;
    spi_writel ( mtspi, SPI_CFG1_REG, cfg1 );

    //enable tx_dma or rx_dma
    cmd = spi_readl ( mtspi, SPI_CMD_REG );
    cmd &= ~SPI_CMD_TX_DMA_MASK;
    cmd &= ~SPI_CMD_RX_DMA_MASK;
      spi_clear_fifo ( mtspi, FIFO_RX );
    if ( mtspi->cur_transfer->tx_buf )
    {
      //spi_write,clear RX fifo,and set the TX buffer

      spi_writel ( mtspi, SPI_TX_SRC_REG,cpu_to_le32( mtspi->cur_transfer->tx_dma ));

      //set TX DMA mode
      cmd |= 1 << SPI_CMD_TX_DMA_OFFSET;
    }
      spi_clear_fifo ( mtspi, FIFO_TX );
    if ( mtspi->cur_transfer->rx_buf )
    {
      //spi_read,clear TX fifo,and set the RX buffer

      spi_writel ( mtspi, SPI_RX_DST_REG,cpu_to_le32( mtspi->cur_transfer->rx_dma ));

      //set RX DMA mode
      cmd |= 1 << SPI_CMD_RX_DMA_OFFSET;

    }
  spi_writel(mtspi, SPI_CMD_REG, cmd);//enable rx_dma/tx_dma,

    #ifdef SPI_DEBUG //read it only in debug mode
      cfg0 = spi_readl ( mtspi, SPI_CFG0_REG );
      SPI_DBG ( "  cfg0:0x%x\n", cfg0 );
      cfg1 = spi_readl ( mtspi, SPI_CFG1_REG );
      SPI_DBG ( "  cfg1:0x%x\n", cfg1 );
      cmd = spi_readl ( mtspi, SPI_CMD_REG );
      SPI_DBG ( "  cmd:0x%x\n", cmd );
      dma_reg = spi_readl ( mtspi, SPI_TX_DATA_REG );
      SPI_DBG ( "  spi_tx_data_reg:0x%x\n", dma_reg );
      dma_reg = spi_readl ( mtspi, SPI_RX_DATA_REG );
      SPI_DBG ( "  spi_rx_data_reg:0x%x\n", dma_reg );
      tx = spi_readl ( mtspi, SPI_TX_SRC_REG );
      SPI_DBG ( "  spi_tx_src_reg:0x%x\n", tx );
      rx = spi_readl ( mtspi, SPI_RX_DST_REG );
      SPI_DBG ( "  spi_rx_dst_reg:0x%x\n", rx );
      dma_reg = spi_readl ( mtspi, SPI_STATUS0_REG);
      SPI_DBG ( "  spi_status0_reg:0x%x\n", dma_reg );
      dma_reg = spi_readl ( mtspi, SPI_STATUS1_REG );
      SPI_DBG ( "  spi_status1_reg:0x%x\n", dma_reg );
    #endif

    /*tell the controller to start to transfer*/
    if ( mtspi->running == PAUSED)
    {
      SPI_DBG("pause status to resume.\n");
      spi_resume_transfer ( mtspi );
    }
    else if ( mtspi->running == IDLE)
    {
      SPI_DBG("The first transfer. spi_start_transfer.\n");
      /*if there is only one transfer, pause bit should not be set.*/
      if (is_pause_mode (mtspi) &&
            ( mtspi->cur_transfer->transfer_list.next != &msg->transfers ) )
      {
        SPI_DBG("set pause mode.\n");
        set_pause_bit ( mtspi );
      }
      spi_start_transfer ( mtspi );
    }
    else
    {
      SPI_ERR("Wrong status %d.\n",mtspi->running);
      ret = -1;
      goto fail;
    }
    mtspi->running = INPROGRESS;
    spin_unlock ( &mtspi->spin_lock );
    /*prepare the buffer for the next transfer*/
    SPI_DBG ( "prepare the buffer of the next transfer.\n" );
    ret = setup_next_transfer ( master, mtspi->cur_transfer );
	if(ret < 0)
		return ret;
  }while ( mtspi->cur_transfer );

  return msg->actual_length;

fail:
  spin_unlock ( &mtspi->spin_lock );//if fail,unlock
  return ret;

}

/*do fifo transfer, this will be only using by transfer length is less than 32 bit.*/
static int do_fifo_transfer (
  struct spi_master *master,
  struct spi_transfer *transfer )
{
  unsigned long flags;
  int reg_val;
  u32 packet_size;
  u32 packet_loop;
  u32 cfg0, cfg1, cmd, tx, rx, dma_reg;
  int cnt, i;

  struct mt6575_spi *mtspi;
  struct spi_message *msg;

  if (!master)
  {
    dev_dbg(&master->dev,
      " master device %s: error.\n",
      dev_name(&master->dev));
    return -EINVAL;
  }

  SPI_DBG ( "do_fifo_transfer.\n" );
  mtspi = spi_master_get_devdata ( master );
  msg = mtspi->cur_msg;

  if ( msg->transfers.next == &msg->transfers )
  {
    SPI_DBG( "message is NULL.\n");
    msg->actual_length=0;
    mtspi->cur_transfer=NULL;
    mtspi->next_transfer=NULL;
    msg->actual_length = 0;
    msg->status=-EINVAL;
    return -EINVAL;
  }

  msg->actual_length += mtspi->cur_transfer->len;

  if ( transfer->len > FIFO_SIZE )
  {
    SPI_DBG ( "transfer size excesses \
    more than FIFO_SIZE.\n" );
  }

  do {
    if ( mtspi->cur_transfer->transfer_list.next == &msg->transfers)
    {
      mtspi->next_transfer = NULL;
    }
    else
    {
      mtspi->next_transfer = list_entry (
        mtspi->cur_transfer->transfer_list.next,
        struct spi_transfer,
        transfer_list );
    }
    packet_size = transfer->len;
    packet_loop = 1;

    spin_lock(&mtspi->spin_lock);
    spi_clear_fifo ( mtspi, FIFO_TX );
    spi_clear_fifo ( mtspi, FIFO_RX );

    /*set packet loop and count.*/
    reg_val = spi_readl ( mtspi, SPI_CFG1_REG );
    reg_val &= ~ SPI_CFG1_PACKET_LENGTH_MASK;
    reg_val &= ~SPI_CFG1_PACKET_LOOP_MASK;
    reg_val |=  ( packet_loop - 1 ) << SPI_CFG1_PACKET_LOOP_OFFSET;
    reg_val |=  ( packet_size - 1 ) << SPI_CFG1_PACKET_LENGTH_OFFSET;
    spi_writel(mtspi, SPI_CFG1_REG, reg_val);

    #ifdef SPI_DEBUG
      cfg0 = spi_readl ( mtspi, SPI_CFG0_REG );
      SPI_DBG ( "before transfer cfg0:0x%x\n", cfg0 );
      cfg1 = spi_readl ( mtspi, SPI_CFG1_REG );
      SPI_DBG ( "before transfer cfg1:0x%x\n", cfg1 );
      cmd = spi_readl ( mtspi, SPI_CMD_REG );
      SPI_DBG ( "before transfer cmd:0x%x\n", cmd );
      dma_reg = spi_readl ( mtspi, SPI_TX_DATA_REG );
      SPI_DBG ( "before transfer spi_tx_data_reg:0x%x\n", dma_reg );
      dma_reg = spi_readl ( mtspi, SPI_RX_DATA_REG );
      SPI_DBG ( "before transfer spi_rx_data_reg:0x%x\n", dma_reg );
      tx = spi_readl ( mtspi, SPI_TX_SRC_REG );
      SPI_DBG ( "before transfer spi_tx_src_reg:0x%x\n", tx );
      rx = spi_readl ( mtspi, SPI_RX_DST_REG );
      SPI_DBG ( "before transfer spi_rx_dst_reg:0x%x\n", rx );
      dma_reg = spi_readl ( mtspi, SPI_STATUS0_REG );
      SPI_DBG ( "before transfer spi_status0_reg:0x%x\n", dma_reg );
      dma_reg = spi_readl ( mtspi, SPI_STATUS1_REG );
      SPI_DBG ( "before transfer spi_status1_reg:0x%x\n", dma_reg );
    #endif


    /*fill the register of SPI_TX_DATA_REG */
    if ( transfer->len%4 )
    {
      cnt = transfer->len/4 + 1;
    }
    else
    {
      cnt = transfer->len/4;
    }

    for ( i = 0; i < cnt; i++ )
    {
      spi_writel ( mtspi, SPI_TX_DATA_REG,*((u32 *) mtspi->cur_transfer->tx_buf + i));
      SPI_DBG ( "The tx_buf is:%x\n",*(u32 *)mtspi->cur_transfer->tx_buf );
      SPI_DBG ( "The address of tx_buf is:%x\n",(u32 *)mtspi->cur_transfer->tx_buf);
      SPI_DBG ( "The address of rx_buf is:%x\n",(u32 *)mtspi->cur_transfer->rx_buf);
    }

    /*tell the controller to start transfer*/
    spi_start_transfer(mtspi);

    mtspi->running = INPROGRESS;

    /*waiting for transfer ending*/
    while ( spi_is_busy ( mtspi ) );

    for ( i = 0; i < cnt; i++ )
    {
      /*get the data from register of SPI_RX_DATA_REG */
      reg_val = spi_readl(mtspi, SPI_RX_DATA_REG);
      SPI_DBG("SPI_RX_DATA_REG:0x%x", reg_val);
      *( ( u32 * ) mtspi->cur_transfer->rx_buf + i ) = reg_val;
    }

    /*Set ACT as 0 */
    spin_unlock ( &mtspi->spin_lock);
    mtspi->running = IDLE;
    setup_next_transfer ( master, mtspi->cur_transfer );
  }while ( mtspi->cur_transfer );

  return msg->actual_length;
}

static int do_other_transfer (
  struct spi_master *master,
  struct spi_transfer *transfer )
{
  unsigned long flags;
  u32 reg_val;
  u32 packet_size;
  u32 packet_loop;
  u32 cfg0, cfg1, cmd, tx, rx, dma_reg;
  int cnt, i;

  struct mt6575_spi *mtspi;
  struct spi_message *msg;

  if  ( !master )
  {
    dev_dbg(&master->dev,
      " master device %s: error.\n",
      dev_name(&master->dev ) );
    return -EINVAL;
  }

  if ( transfer->len > 32 || transfer->len < 1)
  {
    SPI_DBG("Transfer length excesses the limitation.\n");
    return -EINVAL;
  }

  SPI_DBG ( "do_dvt_transfer.\n" );
  mtspi = spi_master_get_devdata ( master );
  msg = mtspi->cur_msg;

  spin_lock ( &mtspi->spin_lock );
  /* Make sure tx and rx fifo mode before setting the mode*/
  spi_clear_fifo ( mtspi, FIFO_TX );
  spi_clear_fifo ( mtspi, FIFO_RX );

  cmd = spi_readl ( mtspi, SPI_CMD_REG );
  SPI_DBG("SPI_CMD_REG initialization is:0x%x\n", reg_val);
  cmd &= ~SPI_CMD_PAUSE_EN_MASK;
  cmd &= ~SPI_CMD_PAUSE_IE_MASK;
  cmd &= ~SPI_CMD_FINISH_IE_MASK;
  spi_writel(mtspi, SPI_CMD_REG,cmd);

  packet_size = transfer->len;
  packet_loop = 1;

  /*set the packet length of command register.*/
  cfg1 = spi_readl ( mtspi, SPI_CFG1_REG );
  cfg1 &= ~SPI_CFG1_PACKET_LENGTH_MASK;
  cfg1 &= ~SPI_CFG1_PACKET_LOOP_MASK;
  cfg1 |= ( packet_loop - 1 ) << SPI_CFG1_PACKET_LOOP_OFFSET;
  cfg1 |= (packet_size - 1) << SPI_CFG1_PACKET_LENGTH_OFFSET;
  spi_writel ( mtspi, SPI_CFG1_REG, cfg1 );

  #ifdef SPI_DEBUG
    cfg0 = spi_readl ( mtspi, SPI_CFG0_REG );
    SPI_DBG("before transfer cfg0:0x%x\n", cfg0 );
    cfg1 = spi_readl ( mtspi, SPI_CFG1_REG );
    SPI_DBG ("before transfer cfg1:0x%x\n", cfg1 );
    cmd = spi_readl ( mtspi, SPI_CMD_REG );
    SPI_DBG ( "before transfer cmd:0x%x\n", cmd );
    dma_reg = spi_readl ( mtspi, SPI_TX_DATA_REG );
    SPI_DBG ( "before transfer spi_tx_data_reg:0x%x\n", dma_reg );
    dma_reg = spi_readl ( mtspi, SPI_RX_DATA_REG );
    SPI_DBG ( "before transfer spi_rx_data_reg:0x%x\n", dma_reg );
    tx = spi_readl ( mtspi, SPI_TX_SRC_REG );
    SPI_DBG ("before transfer spi_tx_src_reg:0x%x\n", tx );
    rx = spi_readl ( mtspi, SPI_RX_DST_REG );
    SPI_DBG ( "before transfer spi_rx_dst_reg:0x%x\n", rx );
    dma_reg = spi_readl ( mtspi, SPI_STATUS0_REG );
    SPI_DBG ("before transfer spi_status0_reg:0x%x\n", dma_reg );
    dma_reg = spi_readl ( mtspi, SPI_STATUS1_REG );
    SPI_DBG ("before transfer spi_status1_reg:0x%x\n", dma_reg );
  #endif


  if ( mtspi->config->com_mod == OTHER1 )
  {
    reg_val = spi_readl ( mtspi, SPI_CMD_REG);
    reg_val &= ~ SPI_CMD_TX_DMA_MASK;
    reg_val |= 1 << SPI_CMD_TX_DMA_OFFSET;
    reg_val &= ~SPI_CMD_RX_DMA_MASK;
    spi_writel ( mtspi, SPI_CMD_REG, reg_val );

    transfer->tx_dma = dma_map_single(&mtspi->pdev->dev,(void *)transfer->tx_buf,transfer->len,
      DMA_TO_DEVICE);

    SPI_DBG("dma mapping address is:0x%x.\n",transfer->tx_dma);

    if(dma_mapping_error (&mtspi->pdev->dev,transfer->tx_dma ))
    {
      SPI_DBG("dma mapping tx_buf error.\n");
      return -ENOMEM;
    }

    spi_writel ( mtspi, SPI_TX_SRC_REG,(u32 *)transfer->tx_dma );
    #ifdef SPI_DEBUG
      reg_val = spi_readl ( mtspi, SPI_CMD_REG );
      SPI_DBG("SPI_CMD_REG value is:0x%x\n", reg_val);
    #endif

    if ( transfer->len%4 )
    {
      cnt = transfer->len/4 + 1;
    }
    else
    {
      cnt = transfer->len/4;
    }
    spi_start_transfer ( mtspi );
    while ( spi_is_busy ( mtspi ) );

    for ( i = 0; i < cnt; i++ )
    {
      reg_val =  spi_readl ( mtspi, SPI_RX_DATA_REG );
      *( ( u32 * ) mtspi->cur_transfer->rx_buf + i ) = reg_val;
      SPI_DBG("SPI_RX_DATA_REG is:%x\n", reg_val );
    }

    SPI_DBG ( "rx_buffer is:%x\n",*( u32 * ) mtspi->cur_transfer->rx_buf );
    dma_unmap_single(&mtspi->pdev->dev,transfer->tx_dma,transfer->len, DMA_TO_DEVICE);

  }
  else
  {
    reg_val = spi_readl ( mtspi, SPI_CMD_REG );
    reg_val &=~SPI_CMD_RX_DMA_MASK;
    reg_val |= 1 << SPI_CMD_RX_DMA_OFFSET;
    reg_val &=~SPI_CMD_TX_DMA_MASK;
    spi_writel ( mtspi, SPI_CMD_REG, reg_val );

    if (transfer->len%4)
      cnt = transfer->len/4 + 1;
    else
      cnt = transfer->len/4;

    for ( i = 0; i < cnt; i++ )
    {
      spi_writel ( mtspi, SPI_TX_DATA_REG,*((u32 *) transfer->tx_buf + i));
    }

    transfer->rx_dma = dma_map_single(&mtspi->pdev->dev,transfer->rx_buf,transfer->len,
      DMA_FROM_DEVICE );

    SPI_DBG("dma mapping rx_dma is:0x%x.\n", transfer->rx_dma);

    if( dma_mapping_error ( &mtspi->pdev->dev,transfer->rx_dma ) )
    {
      if(transfer->tx_buf)
      {
        dma_unmap_single (&mtspi->pdev->dev,transfer->tx_dma,transfer->len,DMA_TO_DEVICE);
      }
      SPI_DBG("dma mapping rx_buf error.\n" );
      return -ENOMEM;
    }
    spi_writel(mtspi, SPI_RX_DST_REG, (u32*)transfer->rx_dma);

    spi_start_transfer(mtspi);
    while (spi_is_busy(mtspi));
    dma_unmap_single (&mtspi->pdev->dev,transfer->rx_dma,transfer->len,DMA_FROM_DEVICE);
    SPI_DBG("rx_buffer is:%x\n",*(u32 *)mtspi->cur_transfer->rx_buf );

  }

  //clear the finish flag
  reg_val = spi_readl(mtspi,SPI_STATUS0_REG);
  spin_unlock(&mtspi->spin_lock);
  msg->complete (msg->context);
  msg->actual_length = transfer->len;
  msg->status = msg->actual_length;
  return 0;
}

static irqreturn_t mt6575_spi_interrupt ( int irq, void * dev_id )
{
  unsigned long flags;
  u32 reg_val;
  struct mt6575_spi *mtspi = ( struct mt6575_spi * )dev_id;

  SPI_DBG("mt6575_spi_interrupt.\n");
  /*clear the interrupt status bits by reading the register*/
  reg_val = spi_readl(mtspi,SPI_STATUS0_REG);
  SPI_DBG ( "interrupt status is:0x%x\n", reg_val & 0x3);
  if ( (reg_val & 0x03) == 0)
  {
    goto out;
  }

  if ( mtspi->config->com_mod == DMA_TRANSFER )
  {
    transfer_dma_unmapping(mtspi, mtspi->cur_transfer);
  }
  complete ( &done );

  if ( is_pause_mode ( mtspi ) )
  {
    if ( mtspi->running == INPROGRESS )
    {
      mtspi->running = PAUSED;
    }
    else
    {
      SPI_DBG( "Wrong spi status.\n" );
    }
  }
  else
  {
    mtspi->running = IDLE;
  }
  SPI_DBG( "return IRQ_DONE\n" );
  #ifdef SPI_DEBUG
    reg_val = spi_readl(mtspi,SPI_STATUS0_REG);//do it again,only for debug
    SPI_DBG ( "interrupt status is:0x%x\n", reg_val & 0x3);
  #endif
  return IRQ_HANDLED;

out:
  SPI_DBG ( "return IRQ_NONE.\n" );
  return IRQ_NONE;
}

static int mt6575_spi_transfer (
  struct spi_device *spidev,
  struct spi_message *msg )
{
  struct spi_master *master ;
  struct mt6575_spi *mtspi;
  struct spi_transfer *transfer;
  int len = 0;

  master = spidev->master;
  mtspi = spi_master_get_devdata ( master );
  wake_lock ( &mtspi->wk_lock );
  SPI_DBG("calling mt6575_spi_transfer.\n" );
  if ( !msg )
  {
    SPI_DBG ( "msg is NULL pointer. \n" );
    msg->status = -EINVAL;
    goto out;
  }

  if ( msg->transfers.next == &msg->transfers )
  {
    SPI_DBG ( "the message is NULL.\n" );
    msg->status = -EINVAL;
    msg->actual_length = 0;
    goto out;
  }
  mtspi->cur_transfer = list_entry(msg->transfers.next,struct spi_transfer,transfer_list);

  transfer = mtspi->cur_transfer;
  SPI_DBG("The message is not NULL.\n");
  msg->actual_length = 0;
  mtspi->cur_msg = msg;
  mtspi->running = IDLE;

  spi_gpio_set ( mtspi );
  enable_clk ();
  master->setup(spidev);

  if ( mtspi->config->com_mod == DMA_TRANSFER )
  {
    len = do_dma_transfer ( master, transfer );
    SPI_DBG ( "After transfer: the len of the message is:%d\n", len );
  }
  else if ( mtspi->config->com_mod == FIFO_TRANSFER )
  {
    do_fifo_transfer ( master, transfer );
  }
  else if ( mtspi->config->com_mod == OTHER1 || mtspi->config->com_mod == OTHER2 )
  {
    do_other_transfer ( master, transfer );
  }
  else
  {
    SPI_DBG ( "INVALID transfer mode.\n" );
  }

  mtspi->running = IDLE;
  msg = mtspi->cur_msg;
  msg->status = 0;

  spi_gpio_reset ( mtspi );
  disable_clk ();
  wake_unlock ( &mtspi->wk_lock );
  if(len < 0)
	msg->status = -2;
out:
  wake_unlock ( &mtspi->wk_lock );
  return msg->status;
}

static int mt6575_spi_setup ( struct spi_device *spidev )
{
  struct spi_master *master;
  struct mt6575_spi *mtspi;
  struct mt6575_chip_conf *chip_config=NULL;
  unsigned long flags;
  u32 reg_val;

  master = spidev->master;
  mtspi = spi_master_get_devdata ( master );

  SPI_DBG("mt6575_spi_setup.\n");
  if  ( !spidev )
  {
    dev_dbg ( &spidev->dev,
      "spi device %s: error.\n",
      dev_name ( &spidev->dev ) );
  }

  if (spidev->chip_select >= master->num_chipselect)
  {
    SPI_DBG("spi device chip select excesses the number of master's chipselect.\n");
    return -EINVAL;
  }

  chip_config = ( struct mt6575_chip_conf * ) spidev->controller_data;
  if ( !chip_config )
  {
    chip_config =  ( struct mt6575_chip_conf *) kzalloc (
      sizeof ( struct mt6575_chip_conf ),  GFP_KERNEL );
    if (!chip_config)
    {
      dev_err(&spidev->dev,"spidev %s: can not get enough memory.\n",dev_name(&spidev->dev));
      return -ENOMEM;
    }
    dev_dbg ( &spidev->dev,"device %s: allocated memoy for controller's runtime state\n",
      dev_name ( &spidev->dev ) );

    chip_config->setuptime = 3;
    chip_config->holdtime = 3;
    chip_config->high_time = 10;
    chip_config->low_time = 10;
    chip_config->cs_idletime = 2;
    chip_config->ulthgh_thrsh = 0;

    chip_config->cpol = 0;
    chip_config->cpha = 1;

    chip_config->rx_mlsb = 1;
    chip_config->tx_mlsb = 1;

    chip_config->tx_endian = 0;
    chip_config->rx_endian = 0;

    chip_config->com_mod = DMA_TRANSFER;
    chip_config->pause = 0;
    chip_config->finish_intr = 1;
    chip_config->deassert = 0;
    chip_config->ulthigh = 0;
    chip_config->tckdly = 0;
    spidev->controller_data = chip_config;
  }

  SPI_DBG ( "The com_mod is:%d\n", chip_config->com_mod );

  //spi_gpio_set ( mtspi );
  spin_lock ( &mtspi->spin_lock );

  /*set the timing*/
  reg_val = spi_readl ( mtspi, SPI_CFG0_REG );
  reg_val &= ~ ( SPI_CFG0_SCK_HIGH_MASK |SPI_CFG0_SCK_LOW_MASK );
  reg_val &= ~ ( SPI_CFG0_CS_HOLD_MASK |SPI_CFG0_CS_SETUP_MASK );
  reg_val |= ( (chip_config->high_time-1) << SPI_CFG0_SCK_HIGH_OFFSET );
  reg_val |= ( (chip_config->low_time-1) << SPI_CFG0_SCK_LOW_OFFSET );
  reg_val |= ( (chip_config->holdtime-1) << SPI_CFG0_CS_HOLD_OFFSET );
  reg_val |= ( (chip_config->setuptime-1) << SPI_CFG0_CS_SETUP_OFFSET );
  spi_writel ( mtspi, SPI_CFG0_REG, reg_val );

  reg_val = spi_readl ( mtspi, SPI_CFG1_REG );
  reg_val &= ~(SPI_CFG1_CS_IDLE_MASK);
  reg_val |= ( (chip_config->cs_idletime-1 ) << SPI_CFG1_CS_IDLE_OFFSET );

  reg_val &= ~(SPI_CFG1_GET_TICK_DLY_MASK);
  reg_val |= ( ( chip_config->tckdly ) << SPI_CFG1_GET_TICK_DLY_OFFSET );
  spi_writel ( mtspi, SPI_CFG1_REG, reg_val );

  /*set the mlsbx and mlsbtx*/
  reg_val = spi_readl ( mtspi, SPI_CMD_REG );
  reg_val &= ~ ( SPI_CMD_TX_ENDIAN_MASK | SPI_CMD_RX_ENDIAN_MASK );
  reg_val &= ~ ( SPI_CMD_TXMSBF_MASK| SPI_CMD_RXMSBF_MASK );
  reg_val &= ~ ( SPI_CMD_CPHA_MASK | SPI_CMD_CPOL_MASK );
  reg_val |= ( chip_config->tx_mlsb << SPI_CMD_TXMSBF_OFFSET );
  reg_val |= ( chip_config->rx_mlsb << SPI_CMD_RXMSBF_OFFSET );
  reg_val |= (chip_config->tx_endian << SPI_CMD_TX_ENDIAN_OFFSET );
  reg_val |= (chip_config->rx_endian << SPI_CMD_RX_ENDIAN_OFFSET );
  reg_val |= (chip_config->cpha << SPI_CMD_CPHA_OFFSET );
  reg_val |= (chip_config->cpol << SPI_CMD_CPOL_OFFSET );
  spi_writel(mtspi, SPI_CMD_REG, reg_val);

  /*set pause mode*/
  reg_val = spi_readl ( mtspi, SPI_CMD_REG );
  reg_val &= ~SPI_CMD_PAUSE_EN_MASK;
  reg_val &= ~ SPI_CMD_PAUSE_IE_MASK;
  //if  ( chip_config->com_mod == DMA_TRANSFER )
  reg_val |= ( chip_config->pause << SPI_CMD_PAUSE_IE_OFFSET );

  /*set finish interrupt enable bit*/
  reg_val &= ~ SPI_CMD_FINISH_IE_MASK;
  reg_val |= ( chip_config->finish_intr << SPI_CMD_FINISH_IE_OFFSET );

  /*set the communication of mode*/
  reg_val &= ~ SPI_CMD_TX_DMA_MASK;
  reg_val &= ~ SPI_CMD_RX_DMA_MASK;

  /*set deassert mode*/
  reg_val &= ~SPI_CMD_DEASSERT_MASK;
  reg_val |= ( chip_config->deassert << SPI_CMD_DEASSERT_OFFSET );
  spi_writel ( mtspi, SPI_CMD_REG, reg_val );

  /*set ultra high priority*/
  reg_val = spi_readl ( mtspi, SPI_GMC_SLOW_REG );
  reg_val &= ~SPI_ULTRA_HIGH_EN_MASK;
  reg_val |=  chip_config->ulthigh << SPI_ULTRA_HIGH_EN_OFFSET;

  reg_val &= ~SPI_ULTRA_HIGH_THRESH_MASK;
  reg_val |= ( chip_config->ulthgh_thrsh << SPI_ULTRA_HIGH_THRESH_OFFSET );
  spi_writel ( mtspi, SPI_ULTRA_HIGH_REG, reg_val );

  spin_unlock ( &mtspi->spin_lock );

  mtspi->config = chip_config;

  return 0;
}

static void mt6575_spi_cleanup ( struct spi_device *spidev )
{
  struct spi_master *master;
  struct mt6575_spi *mtspi;

  master=spidev->master;
  mtspi=spi_master_get_devdata ( master );

  SPI_DBG("Calling mt6575_spi_cleanup.\n");

  spidev->controller_data = NULL;
  spidev->master = NULL;
  if ( mtspi->regs )
  {
    iounmap ( &mtspi->regs );
  }
  if ( mtspi->config )
  {
    kfree ( mtspi->config );
  }
  return;
}

static int __init mt6575_spi_probe ( struct platform_device *pdev )
{
  int  ret;
  int irq;
  struct resource *regs;

  struct spi_master *master;
  struct mt6575_spi *mtspi;

  u32 reg_val;

  if ( !request_mem_region ( pdev->resource[0].start,
               pdev->resource[0].end-pdev->resource[0].start + 1, pdev->name ) )
  {
    SPI_DBG("request_mem_region busy.\n");
    return -EBUSY;
  }

  regs = platform_get_resource ( pdev,IORESOURCE_MEM, 0 );
  if ( !regs )
  {
    SPI_DBG("get resource regs NULL.\n");
    return -ENXIO;
  }

  irq = platform_get_irq(pdev,0);
  if ( irq  < 0 )
  {
    SPI_DBG("platform_get_irq error. get invalid irq\n");
    return irq;
  }

  ret = -ENOMEM;
  master = spi_alloc_master ( &pdev->dev, sizeof ( struct mt6575_spi ) );
  if ( !master )
  {
    dev_dbg (&pdev->dev,"device %s: alloc spi master fail.\n", dev_name(&pdev->dev));
    goto out;
  }

  master->mode_bits = ( SPI_CPOL | SPI_CPHA );
  master->bus_num = pdev->id;

  /*hardware can only connect 1 slave.*/
  master->num_chipselect = 1;
  master->setup=mt6575_spi_setup;
  master->transfer=mt6575_spi_transfer;
  master->cleanup=mt6575_spi_cleanup;
  platform_set_drvdata ( pdev, master );

  mtspi = spi_master_get_devdata ( master );
  mtspi->regs = ioremap ( regs->start, ( regs->end-regs->start +1 ) );
  if  ( !mtspi->regs )
  {
    SPI_DBG ( "ioremap error.\n" );
    goto out;
  }

  SPI_DBG ( "the base address of regs is :0x%x\n", mtspi->regs );

  mtspi->pdev = pdev;
  mtspi->irq = irq;
  mtspi->running = IDLE;
  mtspi->cur_transfer = NULL;
  mtspi->next_transfer = NULL;
  wake_lock_init (&mtspi->wk_lock, WAKE_LOCK_SUSPEND, "spi_wakelock");

  SPI_DBG ( "mtspi->cur_transfer is NULL.\n" );
  SPI_DBG ( "mtspi->next_transfer is NULL.\n" );
  SPI_DBG ( "before spi_master_set_devdata.\n" );

  spin_lock_init ( &mtspi->spin_lock );

  ret = request_irq ( irq,mt6575_spi_interrupt, IRQF_TRIGGER_LOW,
    dev_name ( &pdev->dev ), mtspi );
  if ( ret )
  {
    SPI_DBG("registering interrupt handler fails.\n");
    goto out_free;
  }

  spi_master_set_devdata ( master, mtspi );

  reset_spi ( mtspi );
  spi_gpio_reset ( mtspi );//for low power

  SPI_DBG ( "after spi_master_set_devdata.\n" );
  ret = spi_register_master ( master );
  if ( ret )
  {
    SPI_DBG ( "spi_register_master fails.\n" );
    goto power_down;
  }
  else
  {
    SPI_DBG ( "spi_register_master success.\n" );
    #ifdef SPI_DEBUG
      reg_val = spi_readl ( mtspi, SPI_CMD_REG );
      reg_val &= SPI_CMD_RST_MASK;
	  SPI_DBG ( "SPI_CMD_RST bit is: %d\n", reg_val );
    #endif
    return 0;
  }

power_down:
out_free:
  iounmap ( regs );
out:
  spi_master_put ( master );
  return ret;
}

static int __exit mt6575_spi_remove ( struct platform_device *pdev )
{
  struct mt6575_spi *mtspi;
  struct spi_transfer *transfer;
  struct spi_message *msg;
  struct spi_master *master = platform_get_drvdata ( pdev );
  if (!master )
  {
    dev_dbg(&pdev->dev,"master %s: is invalid. \n",dev_name ( &pdev->dev ) );
    return -EINVAL;
  }
  mtspi = spi_master_get_devdata ( master );
  transfer = mtspi->cur_transfer;

  msg = list_entry (transfer->transfer_list.next,struct spi_message,transfers );

  /*if the transfer hasn't been finished, so stop this message*/
  if( transfer )
  {
    msg->complete ( msg->context );
    mtspi->running = IDLE;
  }

  reset_spi ( mtspi );
  disable_clk();
  spi_gpio_reset ( mtspi );
  iounmap ( mtspi->regs );
  free_irq ( mtspi->irq, master );
  spi_unregister_master ( master );
  return 0;
}

#ifdef CONFIG_PM
static int mt6575_spi_suspend (struct platform_device *pdev,pm_message_t message )
{
  /* if interrupt is enabled,then wait for interrupt complete. */
//  disable_clk ( );
  SPI_DBG ( "the pm status is:0x%x.\n", message.event );
  return 0;
}
static int mt6575_spi_resume ( struct platform_device *pdev )
{
//  enable_clk ( );
  SPI_DBG ( "calling mt6575_spi_resume.\n" );
  return 0;
}
#else
#define mt6575_spi_suspend NULL
#define mt6575_spi_resume NULL
#endif

struct platform_driver mt6575_spi_driver = {
  .driver = {
    .name = "mt6575_spi",
    .owner = THIS_MODULE,
  },
  .probe = mt6575_spi_probe,
  .suspend = mt6575_spi_suspend,
  .resume = mt6575_spi_resume,
  .remove = __exit_p ( mt6575_spi_remove ),
};

static int __init mt6575_spi_init ( void )
{
  int ret;
  ret = platform_driver_register ( &mt6575_spi_driver );
  return ret;
}

static void __init mt6575_spi_exit ( void )
{
  platform_driver_unregister ( &mt6575_spi_driver );
}

module_init ( mt6575_spi_init );
module_exit ( mt6575_spi_exit );

MODULE_DESCRIPTION ( "MT6575 SPI Controller driver" );
MODULE_AUTHOR ( "Li Zhang <cindy.zhang@mediatek.com>" );
MODULE_LICENSE ( "GPL" );
MODULE_ALIAS ( "platform: mt6575_spi" );
