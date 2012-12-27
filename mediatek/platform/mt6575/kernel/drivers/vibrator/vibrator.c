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

/******************************************************************************
 * mt6575_vibrator.c - MT6575 Android Linux Vibrator Device Driver
 * 
 * Copyright 2009-2010 MediaTek Co.,Ltd.
 * 
 * DESCRIPTION:
 *     This file provid the other drivers vibrator relative functions
 *
 ******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/workqueue.h>

#include "timed_output.h"
#include <linux/hrtimer.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>

#include <linux/jiffies.h>
#include <linux/timer.h>

#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_pm_ldo.h>

#include <cust_vibrator.h>

#define VERSION					        "v 0.1"
#define VIB_DEVICE	        			"mt6575_vibrator"


/******************************************************************************
Error Code No.
******************************************************************************/
#define RSUCCESS        0

/******************************************************************************
Debug Message Settings
******************************************************************************/

/* Debug message event */
#define DBG_EVT_NONE		0x00000000	/* No event */
#define DBG_EVT_INT			0x00000001	/* Interrupt related event */
#define DBG_EVT_TASKLET		0x00000002	/* Tasklet related event */

#define DBG_EVT_ALL			0xffffffff
 
#define DBG_EVT_MASK      	(DBG_EVT_TASKLET)

#if 1
#define MSG(evt, fmt, args...) \
do {	\
	if ((DBG_EVT_##evt) & DBG_EVT_MASK) { \
		printk(fmt, ##args); \
	} \
} while(0)

#define MSG_FUNC_ENTRY(f)	MSG(FUC, "<FUN_ENT>: %s\n", __FUNCTION__)
#else
#define MSG(evt, fmt, args...) do{}while(0)
#define MSG_FUNC_ENTRY(f)	   do{}while(0)
#endif


/******************************************************************************
Global Definations
******************************************************************************/
static struct workqueue_struct *vibrator_queue;
static struct work_struct vibrator_work;
static struct hrtimer vibe_timer;
static spinlock_t vibe_lock;
static int vibe_state;
static int ldo_state;
extern void dct_pmic_VIBR_enable(kal_bool dctEnable);
extern void dct_pmic_VIBR_sel(kal_uint32 volt);//LK@add

static int vibr_Enable(void)
{
	if(!ldo_state) {
		printk("[vibrator]vibr_Enable \n");
		/*
		if(hwPowerOn(MT65XX_POWER_LDO_VIBR, VOL_2800, "VIBR")) {
			ldo_state=1;
		}
		*/
		dct_pmic_VIBR_sel(VOL_3000);//LK@add
		dct_pmic_VIBR_enable(1);
		ldo_state=1;
	}
	return 0;
}

static int vibr_Disable(void)
{
	if(ldo_state) {
		printk("[vibrator]vibr_Disable \n");
		/*
		if(hwPowerDown(MT65XX_POWER_LDO_VIBR, "VIBR")) {
			ldo_state=0;
		}
		*/
		dct_pmic_VIBR_enable(0);
		ldo_state=0;
	}
   	return 0;
}


static void update_vibrator(struct work_struct *work)
{
	if(!vibe_state)
		vibr_Disable();
	else
		vibr_Enable();
}

static int vibrator_get_time(struct timed_output_dev *dev)
{
	if (hrtimer_active(&vibe_timer)) 
	{
		ktime_t r = hrtimer_get_remaining(&vibe_timer);
		return ktime_to_ms(r);
	} 
	else
		return 0;
}

static void vibrator_enable(struct timed_output_dev *dev, int value)
{
		unsigned long   flags;
#if 1
		struct vibrator_hw* hw = get_cust_vibrator_hw(); 

#endif
		spin_lock_irqsave(&vibe_lock, flags);
		while(hrtimer_cancel(&vibe_timer))
                {
                      printk("[vibrator]vibrator_enable: try to cancel hrtimer \n");
                }

		if (value == 0)
			vibe_state = 0;
		else 
		{
#if 1
			printk("[vibrator]vibrator_enable: vibrator cust timer: %d \n", hw->vib_timer);
			if(value >= hw->vib_timer_min && value <= hw->vib_timer)
				value = hw->vib_timer;
#endif

			value = (value > 15000 ? 15000 : value);
			vibe_state = 1;
			hrtimer_start(&vibe_timer, 
							ktime_set(value / 1000, (value % 1000) * 1000000),
							HRTIMER_MODE_REL);
		}
		spin_unlock_irqrestore(&vibe_lock, flags);
                printk("[vibrator]vibrator_enable: vibrator start: %d \n", value); 
		queue_work(vibrator_queue, &vibrator_work);
}

static enum hrtimer_restart vibrator_timer_func(struct hrtimer *timer)
{
 		vibe_state = 0;
                printk("[vibrator]vibrator_timer_func: vibrator will disable \n");
 		queue_work(vibrator_queue, &vibrator_work);
 		return HRTIMER_NORESTART;
}

static struct timed_output_dev mt6575_vibrator = 
{
	.name = "vibrator",
	.get_time = vibrator_get_time,
	.enable = vibrator_enable,
};

static int vib_probe(struct platform_device *pdev)
{
	return 0;
}

static int vib_remove(struct platform_device *pdev)
{
	return 0;
}

static void vib_shutdown(struct platform_device *pdev)
{
	if(vibe_state) {
		vibe_state = 0;
		vibr_Disable();
	}
}
/******************************************************************************
Device driver structure
*****************************************************************************/
static struct platform_driver vibrator_driver = 
{
    .probe		= vib_probe,
	.remove	    = vib_remove,
    .shutdown = vib_shutdown,
    .driver     = {
    .name = VIB_DEVICE,
    },
};

static ssize_t store_vibr_on(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
{
	if(buf != NULL && size != 0)
	{
		printk("[vibrator]buf is %s and size is %d \n",buf,size);
		if(buf[0]== '0')
		{
			vibr_Disable();
		}else
		{
			vibr_Enable();
		}
	}
	return size;
}

static DEVICE_ATTR(vibr_on, 0664, NULL, store_vibr_on);

/******************************************************************************
 * vib_mod_init
 * 
 * DESCRIPTION:
 *   Register the vibrator device driver ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   RSUCCESS : Success
 * 
 ******************************************************************************/

static s32 __devinit vib_mod_init(void)
{	
	s32 ret;

	printk("MediaTek MT6575 vibrator driver register, version %s\n", VERSION);

	vibrator_queue = create_singlethread_workqueue(VIB_DEVICE);
	if(!vibrator_queue) {
		printk("[vibrator]Unable to create workqueue\n");
		return -ENODATA;
	}
	INIT_WORK(&vibrator_work, update_vibrator);

	spin_lock_init(&vibe_lock);
	vibe_state = 0;
	hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	vibe_timer.function = vibrator_timer_func;

	timed_output_dev_register(&mt6575_vibrator);

    ret = platform_driver_register(&vibrator_driver);

    if(ret) 
    {
		printk("[vibrator]Unable to register vibrator driver (%d)\n", ret);
		return ret;
    }	

	ret = device_create_file(mt6575_vibrator.dev,&dev_attr_vibr_on);
    if(ret)
    {
        printk("[vibrator]device_create_file vibr_on fail! \n");
    }
    
	printk("[vibrator]vib_mod_init Done \n");
 
    return RSUCCESS;
}

/******************************************************************************
 * vib_mod_exit
 * 
 * DESCRIPTION: 
 *   Free the device driver ! 
 * 
 * PARAMETERS: 
 *   None
 * 
 * RETURNS: 
 *   None
 * 
 * NOTES: 
 *   None
 * 
 ******************************************************************************/
 
static void __exit vib_mod_exit(void)
{
	printk("MediaTek MT6575 vibrator driver unregister, version %s \n", VERSION);
	if(vibrator_queue) {
		destroy_workqueue(vibrator_queue);
	}
	printk("[vibrator]vib_mod_exit Done \n");
}

module_init(vib_mod_init);
module_exit(vib_mod_exit);
MODULE_AUTHOR("MediaTek Inc.");
MODULE_DESCRIPTION("MT6575 Vibrator Driver (VIB)");
MODULE_LICENSE("GPL");
