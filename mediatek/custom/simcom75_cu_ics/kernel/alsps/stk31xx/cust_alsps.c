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
#include <linux/types.h>
#include <cust_alsps.h>
#ifdef MT6577
#include <mach/mt6577_pm_ldo.h>
#endif
#ifdef MT6575
#include <mach/mt6575_pm_ldo.h>
#endif

static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 0,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .i2c_addr   = {0x90, 0x00, 0x00, 0x00},	/*STK31xx*/
    /*Lenovo-sw chenlj2 add 2011-06-03,modify parameter below two lines*/
    .als_level  = { 4, 40,  80,   120,   160, 250,  400, 800, 1200,  1600, 2000, 3000, 5000, 10000, 65535},
    .als_value  = {10, 20,20,  120, 120, 280,  280,  280, 1600,  1600,  1600,  6000,  6000, 9000,  10240, 10240},
    //bl_level: 60,102,180,210,230,255
    //define in  alps/frameworks/base/core/res/res/values/config.xml

   //.als_cmd_val = 0x49,	/*ALS_GAIN=1, IT_ALS=400ms*/
   //.ps_cmd_val = 0x21,	/*SLP=30ms, IT_PS=0.2ms*/
   //.ps_gain_setting = 0x08, /*PS_GAIN=4X */
   // .ps_high_thd_val = 0x78,
   // .ps_low_thd_val = 0x6E,
	.ps_threshold_high = 0x78,
    .ps_threshold_low = 0x6E,
};

struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}


int stk31xx_als_cmd_val = 0x49;
int stk31xx_ps_cmd_val = 0x21;
int stk31xx_ps_gain_setting = 0x08;


