/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/ctype.h>

#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/workqueue.h>
#include <linux/switch.h>
#include <linux/delay.h>

#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/kthread.h>
#include <linux/input.h>
#include <linux/wakelock.h>
#include <linux/time.h>

#include <linux/string.h>

#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_reg_base.h>
#include <mach/irqs.h>
#include <accdet_custom.h>
#include <mach/reg_accdet.h>
//#include <mach/mt6575_pll.h>
#include <mach/mt6575_pmic_feature_api.h>
#include <mach/pmic_mt6329_sw_bank1.h>
#include <mach/pmic_mt6329_hw_bank1.h>
#include <mach/upmu_hw.h>
#include <mach/mt6575_clock_manager.h>

/*----------------------------------------------------------------------
IOCTL
----------------------------------------------------------------------*/
#define ACCDET_DEVNAME "accdet"
#define ACCDET_IOC_MAGIC 'A'
#define ACCDET_INIT _IO(ACCDET_IOC_MAGIC,0)
#define SET_CALL_STATE _IO(ACCDET_IOC_MAGIC,1)
#define GET_BUTTON_STATUS _IO(ACCDET_IOC_MAGIC,2)

#define KEY_CALL	KEY_SEND
#define KEY_ENDCALL	KEY_HANGEUL


enum accdet_report_state
{
    NO_DEVICE =0,
    HEADSET_MIC = 1,
    HEADSET_NO_MIC = 2,
    DOUBLE_CHECK_TV = 3
};

enum accdet_status
{
    PLUG_OUT = 0,
    MIC_BIAS = 1,
    DOUBLE_CHECK = 2,
    HOOK_SWITCH = 3,
    TV_OUT = 4,
    STAND_BY =5
};

char *accdet_status_string[6]=
{
    "Plug_out",
    "Headset_plug_in",
    "Double_check",
    "Hook_switch",
    "Tvout_plug_in",
    "Stand_by"
};

char *accdet_report_string[4]=
{
    "No_device",
    "Headset_mic",
    "Headset_no_mic",
    "Double_check"
};

enum hook_switch_result
{
    DO_NOTHING =0,
    ANSWER_CALL = 1,
    REJECT_CALL = 2
};

typedef enum
{	
    TVOUT_STATUS_OK = 0,
    TVOUT_STATUS_ALREADY_SET,
    TVOUT_STATUS_ERROR,
} TVOUT_STATUS;

static int debug_enable = 1;
#define ACCDET_DEBUG(format, args...) do{ \
	if(debug_enable) \
	{\
		printk(KERN_ERR format,##args);\
	}\
}while(0)

