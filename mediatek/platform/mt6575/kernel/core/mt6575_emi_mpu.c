#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/irq.h>
#include <linux/sched.h>

#include "mach/mt6575_reg_base.h"
#include "mach/mt6575_emi_mpu.h"
#include "mach/mt6575_device_apc.h"
#include "mach/mtk_mau.h"
#include "mach/sync_write.h"
#include "mach/irqs.h"
#include "mach/dma.h"
#ifdef CONFIG_MTK_AEE_FEATURE
  #include <linux/aee.h>
#endif  

static DEFINE_SPINLOCK(g_mpu_lock);
static unsigned long g_mpu_flags;

#define ABORT_EMI  0x40000008

static struct device_driver emi_mpu_ctrl =
{
    .name = "emi_mpu_ctrl",
    .bus = &platform_bus_type,
    .owner = THIS_MODULE,
};


static irqreturn_t mpu_violation_irq(int irq, void *dev_id)
{  
    unsigned int dbg_s, dbg_t;
    unsigned int master_ID;
    unsigned int domain_ID;
    unsigned int r_w_violation;
    unsigned int region_abort;
    char *s_region = NULL;
    int i;
    
    if ( readl(AP_DEVAPC0_DXS_VIO_STA) != 0x2)
    {
        printk("[EMI MPU] ERROR AP_DEVAPC0_DXS_VIO_STA not device apc 1 violation!");
        return IRQ_NONE;
    }

    spin_lock_irqsave(&g_mpu_lock, g_mpu_flags);

    dbg_s = readl(EMI_MPUS);
    dbg_t = readl(EMI_MPUT);
    
    master_ID = dbg_s & 0x000007FF;
    domain_ID = (dbg_s >>12) & 0x00000003;
    r_w_violation = (dbg_s >> 28) & 0x00000003;
    
    printk("\r\n [EMI MPU] Violation!!! EMI_MPUS = %x, EMI_MPUT = %x\n\r", dbg_s, dbg_t);
    printk("\r [EMI MPU] Debug info start ----------------------------------------\n\r");
    printk("\r Current Process is \"%s \" (pid: %i)  \n\r", current->comm, current->pid);
    printk("\r Violation Address is 0x%x \n\r", dbg_t);
    printk("\r Violation Master ID is 0x%x \n\r", master_ID);
    printk("\r Violation Domain ID is 0x%x \n\r", domain_ID);
    if(r_w_violation == 1)
    {
      printk("\r R/W Violation : Write \n\r");
    }
    else
    {
      printk("\r R/W Violation : Read \n\r");
    }
    region_abort = (dbg_s >> 16) & 0xFF;
    for (i = 0 ; i < 8 ; i++) {
        if ((region_abort >> i) & 1) {
            break;
        }
    }
    printk("Abort Region is %d\n\r", i);

    /*show running DMA*/
    mt65xx_dma_running_status();

    MAU_get_port_with_m4u(dbg_t, dbg_t);

    printk("\r [EMI MPU] Debug info end------------------------------------------\n\r");
    printk("\n");
#ifdef CONFIG_MTK_AEE_FEATURE
    aee_kernel_exception("EMI MPU", "\r----[EMI MPU] Debug info start----\n\r\rCurrent Process is \"%s \" (pid: %i)  \n\r\r Violation Address is 0x%x \n\r\r Violation Master ID is 0x%x \n\r\r Violation Domain ID is 0x%x \n\r\r Abort Region is %s\n\r", current->comm, current->pid, dbg_t, master_ID, domain_ID, s_region);
#endif

    writel(0x000003FF, EMI_MPUP); //clean Domain 0 violation status
    //writel(0x000003FF, EMI_MPUQ); //clean Domain 1 violation status
    //writel(0x000003FF, EMI_MPUR); //clean Domain 2 violation status

    writel(readl(AP_DEVAPC1_D0_VIO_STA) | ABORT_EMI , AP_DEVAPC1_D0_VIO_STA);

    while((readl(AP_DEVAPC1_D0_VIO_STA) & ABORT_EMI) != 0);
    
    writel(0x2, AP_DEVAPC0_DXS_VIO_STA);

    while(( readl(AP_DEVAPC0_DXS_VIO_STA) & 0x2) != 0);

    // clear the violation
  mt65xx_reg_sync_writel(0x80000000 , EMI_MPUS);

  dbg_s = readl(EMI_MPUS);
  dbg_t = readl(EMI_MPUT);
  
  if ((dbg_s !=0) || (dbg_t !=0)) 
  {
      printk("\r\n [EMI MPU] FAILED!\n\r");
      printk("\r\n [EMI MPU] EMI_MPUS = %x, EMI_MPUT = %x", dbg_s, dbg_t);
  }

  spin_unlock_irqrestore(&g_mpu_lock, g_mpu_flags);
  
  return IRQ_HANDLED;
}

/*
 * emi_mpu_set_region_protection: set EMI memory region protection attribute
 * @sce: concurrency scenario ID
 * @op: either ENABLE_CON_SCE or DISABLE_CON_SCE
 * Return 0 for success; return negative values for failure.
 */
int emi_mpu_set_region_protection(unsigned int start_addr, unsigned int end_addr, int region, unsigned int access_permission)
{
#ifdef USER_BUILD_KERNEL
  /*Disable MPU at USER BUILD*/
  return 0;
#endif

  switch(region){
    case 0:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUA ); //Define Region 0
      writel(readl(EMI_MPUI) |access_permission, EMI_MPUI);
      break; 
    case 1:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUB ); //Define Region 1
      writel(readl(EMI_MPUI) |(access_permission <<16), EMI_MPUI);
      break;
    case 2:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUC ); //Define Region 2
      writel(readl(EMI_MPUJ) |access_permission, EMI_MPUJ);
      break;
    case 3:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUD ); //Define Region 3
      writel(readl(EMI_MPUJ) |(access_permission <<16), EMI_MPUJ);
      break;        
    case 4:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUE ); //Define Region 4
      writel(readl(EMI_MPUK) |access_permission, EMI_MPUK);
      break;  
    case 5:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUF ); //Define Region 5
      writel(readl(EMI_MPUK) |(access_permission <<16), EMI_MPUK);
      break; 
    case 6:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUG ); //Define Region 6
      writel(readl(EMI_MPUL) |access_permission, EMI_MPUL);
      break;    
    case 7:
      writel(((start_addr >> 20) << 16) | (end_addr >>20) , EMI_MPUH ); //Define Region 7
      writel(readl(EMI_MPUL) |(access_permission <<16), EMI_MPUL);
      break;    
  }

  return 0;
  
}

/*
 * mpu_config_show: sysfs con_sce file show function.
 * @driver:
 * @buf:
 * Return the number of read bytes.
 */
static ssize_t mpu_config_show(struct device_driver *driver, char *buf)
{
    char *ptr = buf;
    unsigned int start, end;
    unsigned int reg_value;
    unsigned int d0, d1, d2;
     char *permission [6] = {"No protect",
                                  "Only R/W for secure access",
                                  "Only R/W for secure access, and non-secure read access",
                                  "Only R/W for secure access, and non-secure write access",
                                  "Only R for secure/non-secure",
                                  "Both R/W are forbidden"};

    reg_value = readl(EMI_MPUA);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 0 --> 0x%x to 0x%x\n", start, end);

    reg_value = readl(EMI_MPUB);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 1 --> 0x%x to 0x%x\n", start, end);

    reg_value = readl(EMI_MPUC);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 2 --> 0x%x to 0x%x\n", start, end);

    reg_value = readl(EMI_MPUD);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 3 --> 0x%x to 0x%x\n", start, end);

    reg_value = readl(EMI_MPUE);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 4 --> 0x%x to 0x%x\n", start, end);

    reg_value = readl(EMI_MPUF);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 5 --> 0x%x to 0x%x\n", start, end);

    reg_value = readl(EMI_MPUG);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 6 --> 0x%x to 0x%x\n", start, end);

    reg_value = readl(EMI_MPUH);
    start = ((reg_value >> 16) << 20);
    end = ((reg_value & 0xFFFF) << 20);
    ptr += sprintf(ptr, "Region 7 --> 0x%x to 0x%x\n", start, end);
    
    ptr += sprintf (ptr, "\n");

    reg_value = readl(EMI_MPUI);
    d0 = (reg_value & 0x7);
    d1 = (reg_value >> 3) & 0x7;
    d2 = (reg_value >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 0 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    d0 = ((reg_value>>16) & 0x7);
    d1 = ((reg_value>>16) >> 3) & 0x7;
    d2 = ((reg_value>>16) >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 1 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    reg_value = readl(EMI_MPUJ);
    d0 = (reg_value & 0x7);
    d1 = (reg_value >> 3) & 0x7;
    d2 = (reg_value >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 2 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    d0 = ((reg_value>>16) & 0x7);
    d1 = ((reg_value>>16) >> 3) & 0x7;
    d2 = ((reg_value>>16) >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 3 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    reg_value = readl(EMI_MPUK);
    d0 = (reg_value & 0x7);
    d1 = (reg_value >> 3) & 0x7;
    d2 = (reg_value >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 4 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    d0 = ((reg_value>>16) & 0x7);
    d1 = ((reg_value>>16) >> 3) & 0x7;
    d2 = ((reg_value>>16) >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 5 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    reg_value = readl(EMI_MPUL);
    d0 = (reg_value & 0x7);
    d1 = (reg_value >> 3) & 0x7;
    d2 = (reg_value >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 6 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    d0 = ((reg_value>>16) & 0x7);
    d1 = ((reg_value>>16) >> 3) & 0x7;
    d2 = ((reg_value>>16) >> 6) & 0x7;
    ptr += sprintf(ptr, "Region 7 --> d0 = %s, d1 = %s, d2 = %s\n", permission[d0],  permission[d1],  permission[d2]);

    return strlen(buf);
}


/*
 * mpu_config_store: sysfs con_sce file store function.
 * @driver:
 * @buf:
 * @count:
 * Return the number of write bytes.
 */
static ssize_t mpu_config_store(struct device_driver *driver, const char *buf, size_t count)
{
    int i;    
    unsigned int start_addr;
    unsigned int end_addr; 
    unsigned int region; 
    unsigned int access_permission;
    char *command;
    char *ptr;
    char *token [5];

    if((strlen(buf)+1) > 128)
    {
      printk("EMI MPU: command overflow!");
      return -1;
    }

    command = kmalloc((size_t)128, GFP_KERNEL);

    strcpy(command, buf);

    ptr = (char *)buf;

    printk("EMI MPU store: %s\n", ptr);

    if (!strncmp(buf, EN_MPU_STR, strlen(EN_MPU_STR))) //ON
    {
      i = 0;
      while(ptr != NULL)
      {
        ptr = strsep (&command, " ");
        token[i] = ptr;
        printk("token[%d] = %s\n",  i,  token[i]);
        i++;
      }

      for (i=0;i<5;i++)
        printk("token[%d] = %s\n", i, token[i]);

      start_addr = simple_strtoul(token[1], &token[1], 16);
      end_addr = simple_strtoul(token[2], &token[2], 16);
      region = simple_strtoul(token[3], &token[3], 16);
      access_permission = simple_strtoul(token[4], &token[4], 16);
      
      emi_mpu_set_region_protection(start_addr, end_addr, region, access_permission);
      printk("Setting EMI MPU: start_addr ==> 0x%x, end_addr ==> 0x%x, region ==> 0x%x, permission ==> 0x%x\n", start_addr, end_addr, region, access_permission);
    }
    else if (!strncmp(buf, DIS_MPU_STR, strlen(DIS_MPU_STR))) //OFF
    {
      i = 0;
      while(ptr != NULL)
      {
        ptr = strsep (&command, " ");
        token[i] = ptr;
        printk("token[%d] = %s\n",  i,  token[i]);
        i++;
      }

      for (i=0;i<5;i++)
        printk("token[%d] = %s\n", i, token[i]);
      start_addr = simple_strtoul(token[1], &token[1], 16);
      end_addr = simple_strtoul(token[2], &token[2], 16);
      region = simple_strtoul(token[3], &token[3], 16);
      emi_mpu_set_region_protection(0x0, 0x0, region, SET_ACCESS_PERMISSON(NO_PRETECTION, NO_PRETECTION, NO_PRETECTION));
      printk("Setting EMI MPU: start_addr ==> 0x%x, end_addr ==> 0x%x, region ==> 0x%x, permission ==> 0x%x\n", 0, 0, region, SET_ACCESS_PERMISSON(NO_PRETECTION, NO_PRETECTION, NO_PRETECTION));
    }
    else
    {
      printk("Unkown command for mpu_config_store! \n");
    }

    kfree(command);

    return count;
}


DRIVER_ATTR(mpu_config, 0644, mpu_config_show, mpu_config_store);


/*
 * emi_bwl_mod_init: module init function.
 */
static int __init emi_mpu_mod_init(void)
{
    int ret;
    unsigned int dbg_s, dbg_t;

    printk("[EMI MPU] MPU module init. \n");
  
    // clear the violation
    writel(0x80000000 , EMI_MPUS);
    
    dbg_s = readl(EMI_MPUS);
    dbg_t = readl(EMI_MPUT);
    
    if ((dbg_s !=0) || (dbg_t !=0)) 
    {
        printk("\r\n [EMI MPU] FAILED!\n\r");
        printk("\r\n [EMI MPU] EMI_MPUS = %x, EMI_MPUT = %x", dbg_s, dbg_t);
    }

    writel(0x000003FF, EMI_MPUP); //clean Domain 0 violation status
    //writel(0x000003FF, EMI_MPUQ); //clean Domain 1 violation status
    //writel(0x000003FF, EMI_MPUR); //clean Domain 2 violation status

    writel(readl(AP_DEVAPC0_APC_CON) &  (0xFFFFFFFF ^ (1<<2)), AP_DEVAPC0_APC_CON);
    writel(readl(AP_DEVAPC1_APC_CON) &  (0xFFFFFFFF ^ (1<<2)), AP_DEVAPC1_APC_CON);
    writel(readl(MM_DEVAPC0_APC_CON) &  (0xFFFFFFFF ^ (1<<2)), MM_DEVAPC0_APC_CON);
    writel(readl(MM_DEVAPC1_APC_CON) &  (0xFFFFFFFF ^ (1<<2)), MM_DEVAPC1_APC_CON);
    writel(0x0000007F, AP_DEVAPC0_DXS_VIO_STA);
    writel(0x00FF00FD, AP_DEVAPC0_DXS_VIO_MASK);

    writel(readl(AP_DEVAPC1_D0_VIO_STA) | ABORT_EMI , AP_DEVAPC1_D0_VIO_STA);
    writel(readl(AP_DEVAPC1_D0_VIO_MASK) & ~ABORT_EMI , AP_DEVAPC1_D0_VIO_MASK);
  
    writel(0x2, AP_DEVAPC0_DXS_VIO_STA);    
  
    /* 
     * NoteXXX: Interrupts of vilation (including SPC in SMI, or EMI MPU) are triggered by the device APC.
     *          Need to share the interrupt with the SPC driver. 
     */
    ret = request_irq(MT6575_APARM_DOMAIN_IRQ_ID, (irq_handler_t)mpu_violation_irq, IRQF_TRIGGER_LOW | IRQF_SHARED, "mt6575_emi_mpu", &emi_mpu_ctrl);    
    disable_irq(MT6575_APARM_DOMAIN_IRQ_ID);
    enable_irq(MT6575_APARM_DOMAIN_IRQ_ID);
    
    if(ret != 0)
    {
        printk( "[EMI MPU] Failed to request irq! (%d)\n", ret);
        return ret;
    }
  
  #ifndef USER_BUILD_KERNEL
    /* register driver and create sysfs files */
    ret = driver_register(&emi_mpu_ctrl);
    if (ret) 
    {
      printk("fail to register EMI MPU driver\n");
    }
    ret = driver_create_file(&emi_mpu_ctrl, &driver_attr_mpu_config);
    if (ret) 
    {
      printk("[EMI MPU] Fail to create EMI_MPU sysfs file\n");
    }
#endif
  
    return 0;
}

void start_mm_mau_protect(unsigned int mau_entry_start, unsigned int mau_entry_end, unsigned int entry_id)
{  
#ifdef USER_BUILD_KERNEL
  /*Disable MAU at USER BUILD*/
  return;
#else
 
  /*MAU Entry configuration */
  MTK_MAU_CONFIG mauConfig = {0};
  mauConfig.EntryID = entry_id;                                             //Entry 1 monitor modem memory
  mauConfig.Enable = true;
  mauConfig.Mode = 0;
  mauConfig.InvalidMasterLARB0 = 0xffffffff;    //enable LARB0 all Port
  mauConfig.InvalidMasterLARB1 = 0xffffffff;    //enable LARB1 all Port
  mauConfig.InvalidMasterLARB2 = 0xffffffff;    //enable LARB2 all Port
  mauConfig.InvalidMasterLARB3 = 0xffffffff;    //enable LARB3 all Port
  mauConfig.ReadEn = true;                                       //Monitor read
  mauConfig.WriteEn = true;                                      //Monitor write
  mauConfig.StartAddr = mau_entry_start;
  mauConfig.EndAddr = mau_entry_end;     
  MAU_Config(&mauConfig);
#endif
}
/*
 * emi_bwl_mod_exit: module exit function.
 */
static void __exit emi_mpu_mod_exit(void)
{
}

module_init(emi_mpu_mod_init);
module_exit(emi_mpu_mod_exit);
EXPORT_SYMBOL(emi_mpu_set_region_protection);
EXPORT_SYMBOL(start_mm_mau_protect);

