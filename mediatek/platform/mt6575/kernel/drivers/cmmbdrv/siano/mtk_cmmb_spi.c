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
#include <mach/irqs.h>

#include <mach/mt6575_spi.h>
#include <mach/mt6575_gpio.h>
#include <mach/mt6575_clock_manager.h>

#include "mtk_cmmb_spi.h"

#define SPI_base 0xc2097000

#define sms_msg(fmt, arg...)	printk(KERN_ERR "[cmmb-drv]%s: " fmt "\n", __func__, ##arg)
#define sms_dbg(fmt, arg...)	//printk(KERN_ERR "[cmmb-drv]%s: "fmt "\n", __func__, ##arg)
#define sms_err(fmt, arg...)	printk(KERN_ERR "[cmmb-drv]ERROR!!! %s: "fmt "\n", __func__, ##arg) 
volatile int *SPI_CONF0_REG;
volatile int *SPI_CONF1_REG;
volatile int *SPI_TX_ADDR_REG;
volatile int *SPI_RX_ADDR_REG;
volatile int *SPI_TX_FIFO_REG_ADDR;
volatile int *SPI_TX_FIFO_REG;
volatile int *SPI_RX_FIFO_REG_ADDR;
volatile int *SPI_RX_FIFO_REG;
volatile int *SPI_COMM_REG;

volatile int *SPI_STATUS2_REG;

volatile int* SPI_STATUS1_REG1;
#define GPIO_SPI_CS_N_PIN      83             
#define GPIO_SPI_CS_N_PIN_MODE   1    
#define GPIO_SPI_CS_N_PIN_PULLEN   1
#define GPIO_SPI_CS_N_PIN_PULLDOWN 0
#define GPIO_SPI_CS_N_PIN_PULLUP   1
#define GPIO_DIR_OUT 1
#define GPIO_OUT_DATA 0

#define GPIO_SPI_SCK_N_PIN 86
#define GPIO_SPI_SCK_N_PIN_MODE 1
#define GPIO_SPI_MISO_N_PIN 84
#define GPIO_SPI_MISO_N_PIN_MODE 1
#define GPIO_SPI_MOSI_N_PIN 85
#define GPIO_SPI_MOSI_N_PIN_MODE 1

#define CLKSRC (122.88*1000*1000)

#define PACKET_SIZE 0x400

#define FIFO_SIZE 32

#define INVALID_DMA_ADDRESS 0xffffffff

#define SPI_CMD_CPHA_MASK 0x100
#define SPI_CMD_CPOL_MASK 0x200

#define SPI_CMD_CPHA_OFFSET 8
#define SPI_CMD_CPOL_OFFSET 9

#define SPI_CFG1_CS_IDLE 0xff

#define SPI_CMD_RX_ENDIAN_OFFSET 14
#define SPI_CMD_TX_ENDIAN_OFFSET 15
#define SPI_CMD_RX_ENDIAN_MASK 0x4000
#define SPI_CMD_TX_ENDIAN_MASK 0x8000

#define SPI_CMD_RX_MSB_OFFSET 13
#define SPI_CMD_TX_MSB_OFFSET 12
#define SPI_CMD_RX_MSB_MASK 0x4000
#define SPI_CMD_TX_MSB_MASK 0x8000
struct completion spi_int_done;

unsigned char* tmp_txbuf=NULL;
struct
{
	int pause_mode_count;
	bool   deassert_mode;
	bool   pause_mode;
	bool   ultra_high_mode;
	int ultra_high_thresh;
	bool   slow_down_mode;
	bool   split_burst_enable;
	int slow_down_thresh;
	int get_tick_mode;
} spi_internal_state = {0};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
enum SPI_DIRECTION_TYPE {
	SPI_TX = 0,
	/**<
	 * \ingroup spi
	 * Means transmission
	 */
	SPI_RX
		/**<
		 * \ingroup spi
		 * Means reception
		 */
};
typedef enum SPI_DIRECTION_TYPE SPI_DIRECTION_TYPE;

static void mtkspi_release(struct device *dev)
{
	sms_dbg("mtkspi_release nothing to do");
	/* Nothing to release */
}
static struct platform_device mtk_spi_device = {
	.name = "mtkspi2",
	.id = 1,
	.dev = {
		.release = mtkspi_release,
	},

};
static int transfer_dma_mapping(void* tx_buf, void* rx_buf,unsigned long*  tx_dma, unsigned long*  rx_dma,int len)
{
	struct device *dev=&(mtk_spi_device.dev);
	*tx_dma=*rx_dma=INVALID_DMA_ADDRESS;
	if (tx_buf) {
		if((int)tx_buf %4 !=0){
			sms_err("error tx_buf not 4 align");
			return -1;
		}
		*tx_dma=dma_map_single(dev, (void *)tx_buf, len, DMA_TO_DEVICE);	
		//sms_msg("dma mapping tx_dma is:0x%x.", *tx_dma);
		if (dma_mapping_error(dev, *tx_dma)) {
			sms_err("dma mapping tx_buf error.");
			return -ENOMEM;	
		}
	}
	if (rx_buf) {
		if((int)rx_buf %4 !=0){
			sms_err("error rx_buf not 4 align");
			return -1;
		}
		*rx_dma = dma_map_single(dev, rx_buf, len, DMA_FROM_DEVICE);
		//sms_msg("dma mapping *rx_dma is:0x%x.", *rx_dma);
		if (dma_mapping_error(dev, *rx_dma)){
			if (tx_buf)
				dma_unmap_single(dev, *tx_dma, len, DMA_TO_DEVICE);
			sms_err("dma mapping rx_buf error.");
			return -ENOMEM;
		}
	}
	return 0;	
}

static int transfer_dma_unmapping(unsigned long  tx_dma, unsigned long  rx_dma,int len)
{
	struct device *dev=&(mtk_spi_device.dev);
	//sms_msg("transfer_dma_unmapping.");

	if (tx_dma != INVALID_DMA_ADDRESS) 
		dma_unmap_single(dev, tx_dma, len,DMA_TO_DEVICE);
	if (rx_dma != INVALID_DMA_ADDRESS)
		dma_unmap_single(dev, rx_dma, len, DMA_FROM_DEVICE);
	return 0;
}
bool spi_set_rwaddr(int const id,SPI_DIRECTION_TYPE const type, int addr)
{
	//sms_msg("spi_set_rwaddr.");
	if ((type != SPI_TX) && (type != SPI_RX))
	{
		sms_err("error direction type");
		return false;
	}
	//sms_msg("NULL == addr.");
	if (NULL == addr)
	{
		return false;
	}

	if (SPI_TX == type)
	{
		//sms_msg("   *SPI_TX_ADDR_REG = cpu_to_le32(addr);.");
		*SPI_TX_ADDR_REG = cpu_to_le32(addr);
	}
	else
	{
		//sms_msg("   *SPI_RX_ADDR_REG = cpu_to_le32(addr); .");
		*SPI_RX_ADDR_REG = cpu_to_le32(addr); 
	}
	return true;
}

bool spi_set_deassert_mode(int const id,
		bool const status)
{
	CLEAR_SPI_COMM_REG(SPI_COMM_BIT_CS_DEASSERT_EN);
	SET_SPI_COMM_REG(SPI_COMM_BIT_CS_DEASSERT_EN, status);
	return true;
}

bool spi_transfer_select_mode(int const id, SPI_DIRECTION_TYPE const type, SPI_MODE const mode)
{
	int bit_pos;

	if ((mode != SPI_MODE_DMA) && (mode != SPI_MODE_FIFO))
	{
		return false;
	}

	switch (type)
	{
		case SPI_TX: bit_pos = SPI_COMM_BIT_TX_DMA_EN; break;
		case SPI_RX: bit_pos = SPI_COMM_BIT_RX_DMA_EN; break;

		default:
			     return false;
	}
	CLEAR_SPI_COMM_REG(bit_pos);
	SET_SPI_COMM_REG(bit_pos, mode);

	return true;
}

bool mtk_spi_mode_setting(SPI_MODE_T pSetMode)               
{
	switch (pSetMode.mode)
	{
		case SPI_MODE_DEASSERT:
			// deassert mode is conflict with pause mode.
			if (pSetMode.bEnable)
			{
				// ASSERT(spi_internal_state.pause_mode == false);
				// Enable deassert mode.
				spi_internal_state.deassert_mode = true;
				spi_set_deassert_mode(0, true);
			}
			else
			{
				spi_set_deassert_mode(0, false);
				spi_internal_state.deassert_mode = false;
			}
			break;
		case SPI_MODE_PAUSE:
			// pause mode is conflict with deassert mode.
			if (pSetMode.bEnable)
			{
				//  ASSERT(spi_internal_state.deassert_mode == false);
				//  ASSERT(pSetMode.Param <=1);
				spi_internal_state.pause_mode_count = pSetMode.Param;
				spi_internal_state.pause_mode = true;
			}
			else
			{
				// Normally pause mode will be disabled automatically.
				spi_internal_state.pause_mode_count = 1;
			}
			break;
		case SPI_MODE_GET_TICK:
			break;
		case SPI_MODE_ULTRA_HIGH:
			break;
		case SPI_MODE_SLOW_DOWN:
			break;
		default:
			//ASSERT(0);
			break;
	}
	return 0;
}

/* The real size is calculated by:
 * ((PACKET_LENGTH + 1) * (PACKET_LOOP_CNT + 1))
 */
bool spi_set_desired_size(int const id,int const pkg_length,int const pkg_count)
{
	/* Set 'PACKET_LOOP_CNT' field. */
	*SPI_CONF1_REG &= (~(0xFF << 8));
	*SPI_CONF1_REG |= ((pkg_count - 1) << 8);

	/* Set 'PACKET_LENGTH' field. */
	*SPI_CONF1_REG &= (~(((int)0xFFFF) << 16));
	*SPI_CONF1_REG |= ((pkg_length - 1) << 16);

	return true;
}
bool spi_activate(int const id)
{
	/* ensure non-cachable DMA address. */
	//sms_msg("spi_activate");
	*SPI_COMM_REG |= (1 << SPI_COMM_BIT_ACT);       
	//*SPI_COMM_REG |= ((1 << SPI_COMM_BIT_ACT) | (1 << SPI_COMM_BIT_RESET));          

	return true;
}

bool spi_resume(int const id)
{
	*SPI_COMM_REG |= (1 << SPI_COMM_BIT_RESUME);
	return true;
}

bool spi_is_busy(void)
{
	//sms_msg("spi_is_busy");
	int const status = *SPI_STATUS2_REG;

	if (status & SPI_STATUS2_BIT_BUSY)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool spi_wait_not_busy(int const id)
{
	// sms_msg("wait for busy");
	//udelay(500);
	while (true == spi_is_busy()) {   ;}
	{
		int volatile tmp = *SPI_STATUS1_REG1;           
	}

	return true;
}

bool spi_is_in_pause_mode(void)
{

	if (*SPI_COMM_REG & (1 << SPI_COMM_BIT_PAUSE_EN))
	{
		return true;
	} 
	else
	{
		return false;
	}
}

bool spi_set_pause_mode(int const id,
		bool const status)
{
	CLEAR_SPI_COMM_REG(SPI_COMM_BIT_PAUSE_EN);
	SET_SPI_COMM_REG(SPI_COMM_BIT_PAUSE_EN, status);
	return true;
}

bool  spi_set_interrupt(int const id,
		SPI_INT_TYPE const type,
		SPI_BIT_STATUS const status)
{
	int  bit_pos;

	if ((status != SPI_ENABLE) && (status != SPI_DISABLE))
	{
		sms_err("error status interrupt");
		return false;
	}

	switch (type)
	{
		case SPI_INT_PAUSE:  bit_pos = SPI_COMM_BIT_PAUSE_IE;  break;
		case SPI_INT_FINISH: bit_pos = SPI_COMM_BIT_FINISH_IE; break;
		default:
				     return false;
	}

	CLEAR_SPI_COMM_REG(bit_pos);
	SET_SPI_COMM_REG(bit_pos, status);

	return 1;
}
void spi_transfer_internal(SPI_INTERNAL_HANDLE_T* pHandle, SPI_CALLBACK fCB)
{
	bool bInPauseMode;
	if (fCB)
	{
		spi_set_interrupt(pHandle->id, SPI_INT_PAUSE, SPI_ENABLE);
		spi_set_interrupt(pHandle->id, SPI_INT_FINISH, SPI_ENABLE);
		//spi_set_cb(pHandle->id, SPI_INT_PAUSE, fCB, NULL);
		//spi_set_cb(pHandle->id, SPI_INT_FINISH, fCB, NULL);
	}
	else
	{
		spi_set_interrupt(pHandle->id, SPI_INT_PAUSE, SPI_DISABLE);
		spi_set_interrupt(pHandle->id, SPI_INT_FINISH, SPI_DISABLE);
		//spi_set_cb(pHandle->id, NULL, fCB, NULL);
		//spi_set_cb(pHandle->id, NULL, fCB, NULL);
	}

	// Start SPI. Check pause mode count. If count > 0, use resume. If count = 0, use activate and disable pause mode.
	bInPauseMode = spi_is_in_pause_mode();
	if ((spi_internal_state.pause_mode_count > 0) && (spi_internal_state.pause_mode == true) && (bInPauseMode == true))
	{
		// Use resume
		spi_internal_state.pause_mode_count--;                 
		if (spi_internal_state.pause_mode_count == 0)
		{
			spi_internal_state.pause_mode = false;
			spi_set_pause_mode(pHandle->id, false);
		}
		spi_resume(pHandle->id);
	}
	else if (bInPauseMode == false)                        
	{
		if ((spi_internal_state.pause_mode_count > 1) && (spi_internal_state.pause_mode == true))
			spi_set_pause_mode(pHandle->id, true);
		if (spi_internal_state.pause_mode_count > 0)
			spi_internal_state.pause_mode_count--;
		if (spi_internal_state.pause_mode_count == 0)
			spi_internal_state.pause_mode = false;
		// Use activate
		spi_activate(pHandle->id);

	}
	// else
	// ASSERT(0);

#if 0	
	if(fCB){
		//sms_msg("dma using finish int");
		spi_set_interrupt(pHandle->id, SPI_INT_FINISH, SPI_ENABLE);
		//sms_msg("spi_transfer_internal");
		//spi_activate(pHandle->id);
		//   spi_set_interrupt(pHandle->id, SPI_INT_PAUSE, SPI_ENABLE);
		wait_for_completion(&spi_int_done);

	}
#endif
	if (fCB == NULL){
		//sms_msg("dma polling mode");
		//sms_msg("spi_transfer_internal");
		//spi_activate(pHandle->id);
		spi_wait_not_busy(pHandle->id);
	}
	else
		wait_for_completion(&spi_int_done);
}
/** \brief Activate the SPI operation.
 *
 * Start the SPI clock to start its transmission.
 *
 * \param id The return value of spi_open().
 * \return
 * - true if success.
 * - false if failure, caller can use spi_get_error_code()
 * to examine what was wrong.
 */


////////////////////////////////////////////////////////////////////
void gpio_setting(void)
{
	mt_set_gpio_mode(GPIO_SPI_CS_N_PIN, GPIO_SPI_CS_N_PIN_MODE);
	mt_set_gpio_dir(GPIO_SPI_CS_N_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_SPI_CS_N_PIN, GPIO_OUT_DATA);
#if 0
	mt_set_gpio_pull_enable(GPIO_SPI_CS_N_PIN, GPIO_SPI_CS_N_PIN_PULLEN);
	mt_set_gpio_pull_select(GPIO_SPI_CS_N_PIN,  GPIO_SPI_CS_N_PIN_PULLUP);
#endif

	mt_set_gpio_mode(GPIO_SPI_SCK_N_PIN, GPIO_SPI_SCK_N_PIN_MODE);
#if 0
	mt_set_gpio_pull_enable(GPIO_SPI_SCK_N_PIN, 1);
	mt_set_gpio_pull_select(GPIO_SPI_SCK_N_PIN,  1);
#endif
	mt_set_gpio_mode(GPIO_SPI_MISO_N_PIN, GPIO_SPI_MISO_N_PIN_MODE);
#if 0
	mt_set_gpio_pull_enable(GPIO_SPI_MISO_N_PIN, 1);
	mt_set_gpio_pull_select(GPIO_SPI_MISO_N_PIN,  1);
#endif
	mt_set_gpio_mode(GPIO_SPI_MOSI_N_PIN, GPIO_SPI_MOSI_N_PIN_MODE);
#if 0
	mt_set_gpio_pull_enable(GPIO_SPI_MOSI_N_PIN, 1);
	mt_set_gpio_pull_select(GPIO_SPI_MOSI_N_PIN,  1);
#endif

	return;
}

static irqreturn_t mtk_spi_interrupt(int irq, void * dev_id)
{
	int volatile tmp;
//	sms_msg("-->");
	tmp = *SPI_STATUS1_REG1;
	// sms_msg("mtk_spi_interrupt ");

	if (tmp & SPI_STATUS1_BIT_PAUSE ) 
	{
		// sms_msg("## spi int pause ");
		complete(&spi_int_done);
	}

	if (tmp & SPI_STATUS1_BIT_FINISH) 
	{
		//sms_msg("## spi int finish ");
		complete(&spi_int_done);
	}
//	sms_msg("<--");
	return 0;
}
#define SPI_FIFO_SIZE (32)
bool spi_clear_fifo(int const id,
		SPI_DIRECTION_TYPE const direction)
{
	int i;

	for (i = 0; i < (SPI_FIFO_SIZE / 4); ++i)
	{
		switch (direction)
		{
			case SPI_TX:
				SPI_TX_FIFO_REG = 0x0;
				break;

			case SPI_RX:
				{
					int volatile tmp = SPI_RX_FIFO_REG;
					break;
				}

			default:
				;
				return false;
		}
	}

	return 0;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//


bool mtk_spi_open()
{
	sms_msg("start");
	SPI_CONF0_REG =(int*)ioremap(SPI_base,4);
	SPI_CONF1_REG =(int*)ioremap(SPI_base+ 0x4,4);
	SPI_TX_ADDR_REG =(int*)ioremap(SPI_base+ 0x8,4);
	SPI_RX_ADDR_REG =(int*)ioremap(SPI_base+ 0xC,4);
	SPI_TX_FIFO_REG =(int*)ioremap(SPI_base+ 0x10,4);
	SPI_RX_FIFO_REG =(int*)ioremap(SPI_base+ 0x14,4);
	SPI_COMM_REG =(int*)ioremap(SPI_base+ 0x18,4);
	SPI_STATUS1_REG1 =(int*)ioremap(SPI_base + 0x1C,4);
	SPI_STATUS2_REG  =(int*)ioremap(SPI_base+ 0x20,4);
	/* Turn on SPI clock. */
	enable_clock(MT65XX_PDN_MM_SPI, "spi");         

	/* Switch GPIO register to SPI mode. */
	gpio_setting();                                   	
	int ret = platform_device_register(&mtk_spi_device);                     
	if (ret < 0) {
		sms_err("platform_device_register failed");
		return ret;
	}
	sms_msg("request_irq-->");
	ret = request_irq(112, mtk_spi_interrupt, IRQF_TRIGGER_LOW,"mtkspi", 0);
	if(ret){
                 sms_err("request_irq");
		   return -1;
	}
	sms_msg("request_irq<--");
	tmp_txbuf = kmalloc(256, GFP_KERNEL);
	if(tmp_txbuf==NULL)
		sms_err("error tmp_txbuf kmalloc");
	sms_msg("OK");
	return 1;
}

bool mtk_spi_close()
{
	/* Turn off SPI clock. */
	disable_clock(MT65XX_PDN_MM_SPI, "spi");     
	/* Switch GPIO register to not SPI mode. */
	platform_device_unregister(&mtk_spi_device);
	kfree(tmp_txbuf);
	return 1;
}

bool mtk_spi_config( spi_attr_t * const attr)
{ 
	sms_msg("mtk_spi_config start");
	*SPI_CONF0_REG = 0;
	*SPI_CONF1_REG = 0;
	//debug	 cfg0	
	sms_dbg("SPI CFG0:%08x",*SPI_CONF0_REG);
	*SPI_CONF0_REG &= 0x00000000;
	//*SPI_CONF0_REG |= 10 | (10<<8) |(15<<16)|(15<<24);                                //6m
	*SPI_CONF0_REG |= 15 | (15<<8) |(20<<16)|(20<<24);                                //4m
	//*SPI_CONF0_REG |= 30 | (30<<8) |(15<<16)|(15<<24);                              //1203  2m
	// *SPI_CONF0_REG |= 6 | (6<<8) |(15<<16)|(15<<24);                              //1203  10m
	//*SPI_CONF0_REG |= 10 | (10<<8);
	sms_dbg("SPI CFG0:%08x",*SPI_CONF0_REG);

	//debug	 cfg0	
	sms_dbg("SPI CFG1:%08x",*SPI_CONF1_REG);
	*SPI_CONF1_REG &= ~(SPI_CFG1_CS_IDLE);
	*SPI_CONF1_REG |= 20;                                
	sms_dbg("SPI CFG1:%08x",*SPI_CONF1_REG);

	//debug	 cmd
	//sms_dbg("SPI CMD:%8x",SPI_COMM_REG);
	*SPI_COMM_REG &= ~(SPI_CMD_CPHA_MASK);	
	*SPI_COMM_REG &= ~(SPI_CMD_CPOL_MASK);
	*SPI_COMM_REG &= ~(SPI_CMD_TX_ENDIAN_MASK|SPI_CMD_RX_ENDIAN_MASK);
	*SPI_COMM_REG |= (0<<SPI_CMD_CPOL_OFFSET|0<<SPI_CMD_CPHA_OFFSET);
	*SPI_COMM_REG |= (1<<SPI_CMD_RX_MSB_OFFSET|1<<SPI_CMD_TX_MSB_OFFSET);
	sms_msg("<--");
	return true;
}

//#define _buf_issue
bool mtk_spi_readwrite(SPI_INTERNAL_HANDLE_T* handle,void* pOutBuffer, void* pInBuffer, int length, int count, SPI_CALLBACK fCB)
{
	//sms_dbg("dma transfer");
	int i;
	// memset(pInBuffer,0,length*count);
#if 0
	unsigned char* tmp_rx ;
	sms_dbg("txbuflen=%d",length);
	tmp_rx = (unsigned char*)pOutBuffer;
	for(i=0;i<16;i++)
		printk(" 0x%02x",tmp_rx[i]);
	printk("\n");
#endif

	bool result;
	SPI_INTERNAL_HANDLE_T* pHandle = (SPI_INTERNAL_HANDLE_T*) handle;
	//sms_msg("--s len=%d",length);
	*SPI_COMM_REG |= (1 << SPI_COMM_BIT_RESET);  
	*SPI_COMM_REG &= ~(1 << SPI_COMM_BIT_RESET);  
	/* mtk_spi_config(NULL);
	 */
	unsigned long txdma=0,rxdma=0;
#ifndef _buf_issue                          //buf issue
	result =transfer_dma_mapping(pOutBuffer,pInBuffer,&txdma,&rxdma,length*count);   
	if(result<0){
		sms_err("transfer_dma_mapping error");
		return false;
	}   	
#else
	txdma = (unsigned long)pOutBuffer;
	rxdma = (unsigned long)pInBuffer;
#endif
	init_completion(&spi_int_done);
	// Set DMA mode and FIFO mode and     // Set addresses
	if(pOutBuffer){
		spi_transfer_select_mode(pHandle->id, SPI_TX, SPI_MODE_DMA);
		spi_set_rwaddr(pHandle->id, SPI_TX, txdma);
	}
	else{
		spi_transfer_select_mode(pHandle->id, SPI_TX, SPI_MODE_FIFO);
		spi_clear_fifo(pHandle->id, SPI_TX);
	}

	if(pInBuffer){		
		spi_set_rwaddr(pHandle->id, SPI_RX, rxdma);
		spi_transfer_select_mode(pHandle->id, SPI_RX, SPI_MODE_DMA);
	}
	else
		spi_transfer_select_mode(pHandle->id, SPI_RX, SPI_MODE_FIFO);

	spi_clear_fifo(pHandle->id, SPI_RX);
	// Set transfer size
	spi_set_desired_size(pHandle->id, length, count);
	// spi_clear_fifo(pHandle->id, SPI_RX);
	// Transfer
	//cs_activate();
	spi_transfer_internal(pHandle, fCB);
	// msleep(20);
	//unmap
#ifndef _buf_issue                                   //buf issue
	transfer_dma_unmapping(txdma,rxdma,length*count);    
#endif
#if 0	
	sms_dbg("rxbuf:len=%d",length);
	tmp_rx = (unsigned char*)pInBuffer;
	for(i=0;i<16;i++)
		printk(" 0x%02x",tmp_rx[i]);
	printk("\n");
#endif
	//sms_msg("mtk_spi_readwrite--e");
	return 0;
}

bool mtk_spi_readwrite_fifo(SPI_INTERNAL_HANDLE_T* handle,void* txPtr, void* rxPtr, int length, int count, SPI_CALLBACK fCB)
{
	//sms_msg("fifo transfer");
	int* txbuf = (int*)txPtr;
	int* rxbuf = (int*)rxPtr;
	bool result;
	SPI_INTERNAL_HANDLE_T* pHandle = (SPI_INTERNAL_HANDLE_T*) handle;
	// Set FIFO mode
	spi_transfer_select_mode(pHandle->id, SPI_TX, SPI_MODE_FIFO);
	spi_transfer_select_mode(pHandle->id, SPI_RX, SPI_MODE_FIFO);
	// Set transfer size
	//sms_msg("spi_transfer_internal");
	spi_set_desired_size(pHandle->id, length, count);
	if(txbuf)
		*SPI_TX_FIFO_REG = *txbuf;
	else
		*SPI_TX_FIFO_REG = NULL;	
	//*rxdata = *SPI_RX_FIFO_REG;
	// Transfer
	spi_transfer_internal(pHandle, fCB);
	if(rxbuf)
		*rxbuf = *SPI_RX_FIFO_REG;
	else
		sms_err("error  rxbuf ==NULL");
	return 1;
}

bool mtk_spi_read(SPI_INTERNAL_HANDLE_T* handle, void* pBuffer, int length, int count, SPI_CALLBACK fCB)
{
	bool result;
	SPI_INTERNAL_HANDLE_T* pHandle = (SPI_INTERNAL_HANDLE_T*) handle;
	sms_dbg("--s");
	//debug read buffer
	int i;
	// memset(pBuffer,0,length*count);
	unsigned char* tmp_rx ;
	/*  sms_msg("read bufffer:");
	    unsigned char* tmp_rx = (unsigned char*)pBuffer;
	    for(i=0;i<length*count;i++)
	    sms_msg(" 0x%02x",tmp_rx[i]);
	    sms_msg("end");
	    sms_msg("mtk_spi_read--e");
	 */   
	unsigned long txdma=0,rxdma=0;
	//sms_msg("transfer_dma_mapping");
	result =transfer_dma_mapping(NULL,pBuffer,&txdma,&rxdma,length*count);   
	if(result<0){
		sms_err("transfer_dma_mapping error");
		return false;
	}   	
	init_completion(&spi_int_done);
	// Set addresses
	spi_set_rwaddr(pHandle->id, SPI_RX, rxdma);
	// Set DMA mode and FIFO mode
	spi_transfer_select_mode(pHandle->id, SPI_RX, SPI_MODE_DMA);
	spi_transfer_select_mode(pHandle->id, SPI_TX, SPI_MODE_FIFO);
	spi_clear_fifo(pHandle->id, SPI_TX);
	// Set transfer size
	spi_set_desired_size(pHandle->id, length, (int) count);
	// Transfer
	spi_transfer_internal(pHandle, fCB);

	//sms_msg("transfer_dma_unmapping");
	transfer_dma_unmapping(txdma,rxdma,length*count);    
	//debug read buffer
#if 0
	sms_dbg("rxbuf:len=%d",length);
	tmp_rx = (unsigned char*)pBuffer;
	for(i=0;i<length*count;i++)
		printk(" 0x%02x",tmp_rx[i]);
	printk("\n");
#endif
	sms_dbg("--e");
	return SPI_RESULT_OK;
}

bool mtk_spi_write(SPI_INTERNAL_HANDLE_T* handle, void* pBuffer, int  length, int count, SPI_CALLBACK fCB)
{
	sms_dbg("--s");
	bool result;
	SPI_INTERNAL_HANDLE_T* pHandle = (SPI_INTERNAL_HANDLE_T*) handle;
	/*
	   SPI_INTERNAL_HANDLE_T tmp_handle;
	   handle = &tmp_handle;
	   pBuffer = tmp_txbuf;
	   length =256;
	   count =1;
	   fCB =2;
	 */

	//ASSERT(pHandle);    
	unsigned long txdma=0,rxdma=0;
	transfer_dma_mapping(pBuffer,NULL,&txdma,&rxdma,length*count);   
	init_completion(&spi_int_done);
	// Set addresses
	spi_set_rwaddr(pHandle->id, SPI_TX, txdma);
	// Set DMA mode and FIFO mode
	spi_transfer_select_mode(pHandle->id, SPI_TX, SPI_MODE_DMA);
	spi_transfer_select_mode(pHandle->id, SPI_RX, SPI_MODE_FIFO);
	spi_clear_fifo(pHandle->id, SPI_RX);
	// Set transfer size
	spi_set_desired_size(pHandle->id, length, (int)count);
	// Transfer
	spi_transfer_internal(pHandle, fCB);

	transfer_dma_unmapping(txdma,rxdma,length*count);    
	sms_dbg(" --e");
	return SPI_RESULT_OK;
}

bool mtk_spi_test()
{
	SPI_INTERNAL_HANDLE_T rtHandle;
	// mtk_spi_open();

	//mtk_spi_config(NULL);

	//transfer data
	int i,j;
	static u8 smsspi_startup[] = { 0, 0, 0xde, 0xc1, 0xa5, 0x51, 0xf1, 0xed };
	u8 *tx_buffer=(u8 *)kmalloc(256, GFP_KERNEL);
	memcpy(tx_buffer,smsspi_startup,8);
	for (i=8; i<256; i++)
		*(tx_buffer+i)=0x55;
	u8 *rx_buffer=(u8 *)kmalloc(256, GFP_KERNEL);
	memset(rx_buffer, 0, 256);


	// transfer data
	sms_msg("****0,check mtk_spi_readwrite interrupt");
	mtk_spi_readwrite(&rtHandle, tx_buffer,rx_buffer,256,1,1);
	//sms_msg result
	if(tx_buffer){
		sms_msg("tx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",tx_buffer[i]);
		sms_msg("");
	}
	if(rx_buffer){
		sms_msg("rx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",rx_buffer[i]);
		sms_msg("\n");
	}


	// transfer data
	sms_msg("****1,check mtk_spi_write NULL");
	mtk_spi_write(&rtHandle, tx_buffer,256,1,NULL);
	//sms_msg result
	if(tx_buffer){
		sms_msg("tx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",tx_buffer[i]);
		sms_msg("\n");
	}
	if(rx_buffer){
		sms_msg("rx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",rx_buffer[i]);
		sms_msg("\n");
	}

	sms_msg("****1.1 check mtk_spi_write interrupt");
	mtk_spi_write(&rtHandle, tx_buffer,256,1,1);
	//sms_msg result
	if(tx_buffer){
		sms_msg("tx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",tx_buffer[i]);
		sms_msg("\n");
	}
	if(rx_buffer){
		sms_msg("rx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",rx_buffer[i]);
		sms_msg("\n");
	}
	//////////////////
	sms_msg("****2.check mtk_spi_write no interrup");
	mtk_spi_read(&rtHandle, tx_buffer,256,1,0);
	//sms_msg result
	if(tx_buffer){
		sms_msg("tx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",tx_buffer[i]);
		sms_msg("\n");
	}
	if(rx_buffer){
		sms_msg("rx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",rx_buffer[i]);
		sms_msg("\n");
	}
	sms_msg("****2.2check mtk_spi_write interrupt");
	mtk_spi_write(&rtHandle, tx_buffer,256,1,1);
	//sms_msg result
	if(tx_buffer){
		sms_msg("tx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",tx_buffer[i]);
		sms_msg("\n");
	}
	if(rx_buffer){
		sms_msg("rx_buffer:  ");
		for(i=0;i<256;i++)
			sms_msg("%x ",rx_buffer[i]);
		sms_msg("\n");
	}	   
	////////////////////////////////////////////

	kfree(tx_buffer);
	kfree(rx_buffer);

	sms_msg(KERN_INFO "spi_test_init.\n");
	return 0;

}
