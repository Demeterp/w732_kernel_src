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

#include <mach/mtk_ccci_helper.h>


//#define android_bring_up_prepare 1

ccci_kern_func_info ccci_func_table[MAX_REGISTER_API];


/***************************************************************************
 * Make device node helper function section
 ***************************************************************************/
 #if 0
static void* create_dev_class(struct module *owner, const char *name)
{
	int err = 0;
	struct class *dev_class = class_create(owner, name);
	if(IS_ERR(dev_class)){
		err = PTR_ERR(dev_class);
		printk("create %s class fail, %d\n", name, err);
		return NULL;
	}

	return dev_class;
}

static void release_dev_class(void *dev_class)
{
	if(NULL != dev_class)
		class_destroy(dev_class);
}

static int registry_dev_node(void *dev_class, const char *name, int major_id, int minor_start_id, int index)
{
	int ret=0;
	dev_t dev;
	struct device *devices;

	if(index>=0){
		dev = MKDEV(major_id, minor_start_id) + index;
		devices = device_create( (struct class *)dev_class, NULL, dev, NULL, "%s%d", name, index );
	}else{
		dev = MKDEV(major_id, minor_start_id);
		devices = device_create( (struct class *)dev_class, NULL, dev, NULL, "%s", name );
	}

	if(IS_ERR(devices)){
		ret = PTR_ERR(devices);
		printk("create %s device fail, %d\n", name, ret);
	}
	return ret;
}


static void release_dev_node(void *dev_class, int major_id, int minor_start_id, int index)
{
	device_destroy(dev_class,MKDEV(major_id, minor_start_id) + index);
}

#endif

/***************************************************************************
 * provide API called by ccci module
 ***************************************************************************/

int get_dram_type_clk(int *clk, int *type)
{
	return get_dram_info(clk, type);
}
EXPORT_SYMBOL(get_dram_type_clk);


/***************************************************************************
 * Make sysfs node helper function section
 ***************************************************************************/
ssize_t mtk_ccci_attr_show(struct kobject *kobj, struct attribute *attr, char *buffer);
ssize_t mtk_ccci_attr_store(struct kobject *kobj, struct attribute *attr, const char *buffer, size_t size);
ssize_t mtk_ccci_filter_show(struct kobject *kobj, char *page);
ssize_t mtk_ccci_filter_store(struct kobject *kobj, const char *page, size_t size);

struct sysfs_ops mtk_ccci_sysfs_ops = {
	.show   = mtk_ccci_attr_show,
	.store  = mtk_ccci_attr_store,
};

struct mtk_ccci_sys_entry {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj, char *page);
	ssize_t (*store)(struct kobject *kobj, const char *page, size_t size);
};

static struct mtk_ccci_sys_entry filter_setting_entry = {
	{ .name = "filter",  .mode = S_IRUGO | S_IWUSR },
	mtk_ccci_filter_show,
	mtk_ccci_filter_store,
};

struct attribute *mtk_ccci_attributes[] = {
	&filter_setting_entry.attr,
	NULL,
};

struct kobj_type mtk_ccci_ktype = {
	.sysfs_ops = &mtk_ccci_sysfs_ops,
	.default_attrs = mtk_ccci_attributes,
};

static struct mtk_ccci_sysobj {
	struct kobject kobj;
} ccci_sysobj;

static int mtk_ccci_sysfs(void) 
{
	struct mtk_ccci_sysobj *obj = &ccci_sysobj;

	memset(&obj->kobj, 0x00, sizeof(obj->kobj));

	obj->kobj.parent = kernel_kobj;
	if (kobject_init_and_add(&obj->kobj, &mtk_ccci_ktype, NULL, "ccci")) {
		kobject_put(&obj->kobj);
		return -ENOMEM;
	}
	kobject_uevent(&obj->kobj, KOBJ_ADD);

	return 0;
}

ssize_t mtk_ccci_attr_show(struct kobject *kobj, struct attribute *attr, char *buffer) 
{
	struct mtk_ccci_sys_entry *entry = container_of(attr, struct mtk_ccci_sys_entry, attr);
	return entry->show(kobj, buffer);
}

ssize_t mtk_ccci_attr_store(struct kobject *kobj, struct attribute *attr, const char *buffer, size_t size) 
{
	struct mtk_ccci_sys_entry *entry = container_of(attr, struct mtk_ccci_sys_entry, attr);
	return entry->store(kobj, buffer, size);
}

/*---------------------------------------------------------------------------*/
/* Filter table                                                              */
/*---------------------------------------------------------------------------*/
#define MAX_FILTER_MEMBER		(4)
typedef size_t (*ccci_sys_cb_func_t)(const char buf[], size_t len);
typedef struct _cmd_op_map{
	char cmd[8];
	int  cmd_len;
	ccci_sys_cb_func_t store;
	ccci_sys_cb_func_t show;
}cmd_op_map_t;

cmd_op_map_t cmd_map_table[MAX_FILTER_MEMBER] = {{"",0}, {"",0}, {"",0}, {"",0}};

ssize_t mtk_ccci_filter_show(struct kobject *kobj, char *buffer) 
{
	int i;
	int remain = PAGE_SIZE;
	unsigned long len;
	char *ptr = buffer;

	for(i=0; i<MAX_FILTER_MEMBER; i++){
		if( cmd_map_table[i].cmd_len !=0 ){
			// Find a mapped cmd
			if(NULL != cmd_map_table[i].show){
				len = cmd_map_table[i].show(ptr, remain);
				ptr += len;
        			remain -= len;
        		}
		}
	}

	return (PAGE_SIZE-remain);
}

ssize_t mtk_ccci_filter_store(struct kobject *kobj, const char *buffer, size_t size) 
{
	int i;

	for(i=0; i<MAX_FILTER_MEMBER; i++){
		if( strncmp(buffer, cmd_map_table[i].cmd, cmd_map_table[i].cmd_len)==0 ){
			// Find a mapped cmd
			if(NULL != cmd_map_table[i].store){
				return cmd_map_table[i].store(buffer, size);
			}
		}
	}
	printk("unsupport cmd\n");
	return size;
}

int register_filter_func(char cmd[], ccci_sys_cb_func_t store, ccci_sys_cb_func_t show)
{
	int i;
	int empty_slot = -1;
	int cmd_len;
	
	for(i=0; i<MAX_FILTER_MEMBER; i++){
		if( 0 == cmd_map_table[i].cmd_len ){
			// Find a empty slot
			if(-1 == empty_slot)
				empty_slot = i;
		}else if( strcmp(cmd, cmd_map_table[i].cmd)==0 ){
			// Find a duplicate cmd
			return -1;
		}
	}
	if( -1 != empty_slot){
		cmd_len = strlen(cmd);
		if(cmd_len > 7){
			return -2;
		}

		cmd_map_table[empty_slot].cmd_len = cmd_len;
		for(i=0; i<cmd_len; i++)
			cmd_map_table[empty_slot].cmd[i] = cmd[i];
		cmd_map_table[empty_slot].cmd[i] = 0; // termio char
		cmd_map_table[empty_slot].store = store;
		cmd_map_table[empty_slot].show = show;
		return 0;
	}
	return -3;
}
EXPORT_SYMBOL(register_filter_func);



/***************************************************************************/
/* Register kernel API for ccci driver invoking                                                           
/ **************************************************************************/
int register_ccci_kern_func(unsigned int id, ccci_kern_cb_func_t func)
{
	int ret = 0;
	
	if((id >= MAX_REGISTER_API) || (func == NULL)) {
		printk("ccci/ctl: invalid parameter(%d)!", id);
		return E_PARAM;
	}
	
	if(ccci_func_table[id].func == NULL) {
		ccci_func_table[id].id = id;
		ccci_func_table[id].func = func;
	}
	else
		printk("ccci/ctl: func%d has registered!", id);

	return ret;
}
EXPORT_SYMBOL(register_ccci_kern_func);


int exec_ccci_kern_func(unsigned int id, char *buf, unsigned int len)
{
	ccci_kern_cb_func_t func;
	int ret = 0;
	
	if(id >= MAX_REGISTER_API) {
		printk("ccci/ctl: invalid function id(%d)!", id);
		return E_PARAM;
	}
	
	func = ccci_func_table[id].func;
	if(func != NULL) {
		ret = func(buf, len);
	}
	else {
		ret = E_NO_EXIST;
		printk("ccci/ctl: func%d not register!", id);
	}

	return ret;
}
EXPORT_SYMBOL(exec_ccci_kern_func);


static int __init ccci_helper_init(void)
{
	// Init ccci helper sys fs
	memset( (void*)cmd_map_table, 0, sizeof(cmd_map_table) );
	mtk_ccci_sysfs();

	// init ccci kernel API register table
	memset((void*)ccci_func_table, 0, sizeof(ccci_func_table));

	return 0;
}

module_init(ccci_helper_init);

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Mediatek inc.");
MODULE_DESCRIPTION("The ccci helper function");
