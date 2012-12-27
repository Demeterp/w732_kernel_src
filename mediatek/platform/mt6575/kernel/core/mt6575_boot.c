/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/kfifo.h>

#include <linux/firmware.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>

#include <mach/mt6575_boot.h>
#include <mach/mt6575_reg_base.h>
#include <mach/mt6575_typedefs.h>
#include <mach/sbchk_base.h>
#include <linux/version.h>

//#include <mach/sbchk_base.h>
#include <mach/eint.h>
#include <mach/mt6575_gpio.h>



#define MOD "BOOT"

/* hardware version register */
#define APHW_CODE           (VER_BASE)
#define APHW_SUBCODE        (VER_BASE + 0x04)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)

/* this vairable will be set by mt6575_fixup */
BOOTMODE g_boot_mode = UNKNOWN_BOOT;
META_COM_TYPE g_meta_com_type = META_UNKNOWN_COM;
unsigned int g_meta_com_id = 0;

static struct device_driver meta_com_type_info =
{
    .name = "meta_com_type_info",
    .bus = &platform_bus_type,
    .owner = THIS_MODULE,
};

static struct device_driver meta_com_id_info =
{
    .name = "meta_com_id_info",
    .bus = &platform_bus_type,
    .owner = THIS_MODULE,
};

static ssize_t (*md_show)(char*) = NULL;
static ssize_t (*md_store)(const char*,size_t) = NULL;
extern int IMM_get_adc_channel_num(char *channel_name, int len);

void boot_register_md_func(ssize_t (*show)(char*), ssize_t (*store)(const char*,size_t))
{
    md_show = show;
    md_store = store;
}

static char* get_chip_eco_str(CHIP_VER ver)
{
    if (CHIP_E1 == ver) 
        return "E1";
    else if (CHIP_E2 == ver)
        return "E2";
    else if (CHIP_E3 == ver)
        return "E3";
    return "";
}

static ssize_t boot_show(struct kobject *kobj, struct attribute *a, char *buf)
{
    if (!strncmp(a->name, MD_SYSFS_ATTR, strlen(MD_SYSFS_ATTR)) && md_show) 
    {
        return md_show(buf);
    }
    else if (!strncmp(a->name, INFO_SYSFS_ATTR, strlen(INFO_SYSFS_ATTR)))
    {
        return sprintf(buf, "%x%x%x%x%s\n", get_chip_code(), get_chip_hw_ver(), 
                       get_chip_hw_subcode(), get_chip_sw_ver(), get_chip_eco_str(get_chip_eco_ver()));
    }
    else
    {
        return sprintf(buf, "%d\n", g_boot_mode);
    }
}

static ssize_t boot_store(struct kobject *kobj, struct attribute *a, const char *buf, size_t count)
{
    sbchk_base();

    if (!strncmp(a->name, MD_SYSFS_ATTR, strlen(MD_SYSFS_ATTR)) && md_store) 
    {
        return md_store(buf, count);
    }
    
    return count;
}


/* boot object */
static struct kobject boot_kobj;
static struct sysfs_ops boot_sysfs_ops = {
    .show = boot_show,
    .store = boot_store
};

/* boot attribute */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
struct attribute boot_attr = {BOOT_SYSFS_ATTR, THIS_MODULE, 0644};
struct attribute md_attr = {MD_SYSFS_ATTR, THIS_MODULE, 0644};
struct attribute info_attr = {INFO_SYSFS_ATTR, THIS_MODULE, 0644};
#else
struct attribute boot_attr = {BOOT_SYSFS_ATTR, 0644};
struct attribute md_attr = {MD_SYSFS_ATTR, 0644};
struct attribute info_attr = {INFO_SYSFS_ATTR, 0644};
#endif
static struct attribute *boot_attrs[] = {
    &boot_attr,
    &md_attr,
    &info_attr,
    NULL
};

/* boot type */
static struct kobj_type boot_ktype = {
    .sysfs_ops = &boot_sysfs_ops,
    .default_attrs = boot_attrs
};

/* boot device node */
static dev_t boot_dev_num;
static struct cdev boot_cdev;
static struct file_operations boot_fops = {
    .owner = THIS_MODULE,
    .open = NULL,
    .release = NULL,
    .write = NULL,
    .read = NULL,
    .unlocked_ioctl = NULL
};

/* boot device class */
static struct class *boot_class;
static struct device *boot_device;


/* return boot mode */
BOOTMODE get_boot_mode(void)
{
    return g_boot_mode;
}

/* return chip code */
unsigned int get_chip_code(void)
{   
    return DRV_Reg32(APHW_CODE);
}
EXPORT_SYMBOL(get_chip_code);

/* return hardware version */
unsigned int get_chip_hw_ver(void)
{
    return DRV_Reg32(APHW_VER);
}
EXPORT_SYMBOL(get_chip_hw_ver);

/* return software version */
unsigned int get_chip_sw_ver(void)
{   
    return DRV_Reg32(APSW_VER);
}
EXPORT_SYMBOL(get_chip_sw_ver);

/* return hardware subcode */
unsigned int get_chip_hw_subcode(void)
{
    return DRV_Reg32(APHW_SUBCODE);
}
EXPORT_SYMBOL(get_chip_hw_subcode);

CHIP_VER get_chip_eco_ver(void)
{   
    unsigned int hw_ver = DRV_Reg32(APHW_VER);
    if (CHIP_E1 == hw_ver)
        return CHIP_E1;
    else if (CHIP_E2 == hw_ver)
        return CHIP_E2;
    else 
        return CHIP_E2;
}
EXPORT_SYMBOL(get_chip_eco_ver);

AP_IMG_TYPE get_ap_img_ver(void)
{
	#if defined(MODEM_2G)
	return AP_IMG_2G;
	#elif  defined(MODEM_3G)
	return AP_IMG_3G;
	#else
	return AP_IMG_INVALID;
	#endif
}
EXPORT_SYMBOL(get_ap_img_ver);


int get_td_eint_info(char * eint_name, unsigned int len)
{
	return get_td_eint_num(eint_name, len);
}
EXPORT_SYMBOL(get_td_eint_info);


int get_md_gpio_info(char *gpio_name, unsigned int len)
{
	return mt_get_md_gpio(gpio_name, len);
}
EXPORT_SYMBOL(get_md_gpio_info);


int get_md_adc_info(char *adc_name, unsigned int len)
{
	return IMM_get_adc_channel_num(adc_name, len);
}
EXPORT_SYMBOL(get_md_adc_info);



void AudSys_Power_On(bool on)
{
	#if defined(CONFIG_MTK_LDVT)
	return;

	#else
	
        extern void CCCI_AudDrv_Clk_On(void);
        extern void CCCI_AudDrv_Clk_Off(void);

	if(on)
		return CCCI_AudDrv_Clk_On();
	else
		return CCCI_AudDrv_Clk_Off();
	#endif
}
EXPORT_SYMBOL(AudSys_Power_On);



/* for convenience, simply check is meta mode or not */
bool is_meta_mode(void)
{   
    if(g_boot_mode == META_BOOT)
    {   
        return true;
    }
    else
    {   
        return false;
    }
}

bool is_advanced_meta_mode(void)
{
    if (g_boot_mode == ADVMETA_BOOT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool com_is_enable(void)  // usb android will check whether is com port enabled default. in normal boot it is default enabled. 
{	if(g_boot_mode == NORMAL_BOOT)
	{	return false;
	}
	else
	{	return true;
	}
}

static int boot_mode_proc(char *page, char **start, off_t off,int count, int *eof, void *data)
{
    char *p = page;
    int len = 0; 

    p += sprintf(p, "\n\rMT6575 BOOT MODE : " );
    switch(g_boot_mode)
    {
        case NORMAL_BOOT :
            p += sprintf(p, "NORMAL BOOT\n");
            break;
        case META_BOOT :
            p += sprintf(p, "META BOOT\n");
            break;
        case ADVMETA_BOOT :
            p += sprintf(p, "Advanced META BOOT\n");
            break;   
        case ATE_FACTORY_BOOT :
            p += sprintf(p, "ATE_FACTORY BOOT\n");
            break;
        case ALARM_BOOT :
            p += sprintf(p, "ALARM BOOT\n");
            break;
        default :
            p += sprintf(p, "UNKNOWN BOOT\n");
            break;
    }  
    *start = page + off;
    len = p - page;
    if (len > off)
        len -= off;
    else
        len = 0;

    return len < count ? len  : count;     
}

static ssize_t meta_com_type_show(struct device_driver *driver, char *buf)
{
  return sprintf(buf, "%d\n", g_meta_com_type);
}

static ssize_t meta_com_type_store(struct device_driver *driver, const char *buf, size_t count)
{
  /*Do nothing*/
  return count;
}

DRIVER_ATTR(meta_com_type_info, 0644, meta_com_type_show, meta_com_type_store);


static ssize_t meta_com_id_show(struct device_driver *driver, char *buf)
{
  return sprintf(buf, "%d\n", g_meta_com_id);
}

static ssize_t meta_com_id_store(struct device_driver *driver, const char *buf, size_t count)
{
  /*Do nothing*/
  return count;
}

DRIVER_ATTR(meta_com_id_info, 0644, meta_com_id_show, meta_com_id_store);


static int __init boot_mod_init(void)
{
    int ret;

    /* allocate device major number */
    if (alloc_chrdev_region(&boot_dev_num, 0, 1, BOOT_DEV_NAME) < 0) {
        printk("[%s] fail to register chrdev\n",MOD);
        return -1;
    }

    /* add character driver */
    cdev_init(&boot_cdev, &boot_fops);
    ret = cdev_add(&boot_cdev, boot_dev_num, 1);
    if (ret < 0) {
        printk("[%s] fail to add cdev\n",MOD);
        return ret;
    }

    /* create class (device model) */
    boot_class = class_create(THIS_MODULE, BOOT_DEV_NAME);
    if (IS_ERR(boot_class)) {
        printk("[%s] fail to create class\n",MOD);
        return (int)boot_class;
    }

    boot_device = device_create(boot_class, NULL, boot_dev_num, NULL, BOOT_DEV_NAME);
    if (IS_ERR(boot_device)) {
        printk("[%s] fail to create device\n",MOD);
        return (int)boot_device;
    }

    /* add kobject */
    ret = kobject_init_and_add(&boot_kobj, &boot_ktype, &(boot_device->kobj), BOOT_SYSFS);
    if (ret < 0) {
        printk("[%s] fail to add kobject\n",MOD);
        return ret;
    }
    
    printk("[%s] APHW_CODE = 0x%x\n",MOD,get_chip_code());
    printk("[%s] APHW_VER  = 0x%x\n",MOD,get_chip_eco_ver());
    
    /* create proc entry at /proc/boot_mode */
    create_proc_read_entry("boot_mode", S_IRUGO, NULL, boot_mode_proc, NULL);

    if(g_boot_mode == META_BOOT || g_boot_mode == ADVMETA_BOOT)
    {
      /* register driver and create sysfs files */
      ret = driver_register(&meta_com_type_info);
      if (ret) 
      {
        printk("fail to register META COM TYPE driver\n");
      }
      ret = driver_create_file(&meta_com_type_info, &driver_attr_meta_com_type_info);
      if (ret) 
      {
        printk("[BOOT INIT] Fail to create META COM TPYE sysfs file\n");
      }

      ret = driver_register(&meta_com_id_info);
      if (ret) 
      {
        printk("fail to register META COM ID driver\n");
      }
      ret = driver_create_file(&meta_com_id_info, &driver_attr_meta_com_id_info);
      if (ret) 
      {
        printk("[BOOT INIT] Fail to create META COM ID sysfs file\n");
      }
    }    
    
    return 0;
}

static void __exit boot_mod_exit(void)
{
    cdev_del(&boot_cdev);
}

module_init(boot_mod_init);
module_exit(boot_mod_exit);
MODULE_DESCRIPTION("MT6575 Boot Information Querying Driver");
MODULE_LICENSE("Proprietary");
EXPORT_SYMBOL(is_meta_mode);
EXPORT_SYMBOL(is_advanced_meta_mode);
EXPORT_SYMBOL(get_boot_mode);
EXPORT_SYMBOL(boot_register_md_func);

