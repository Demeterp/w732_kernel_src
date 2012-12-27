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
#include <mach/mt6575_pm_ldo.h>
#include <cm3623_cust_alsps.h>
//#include <mach/mt6575_pm_ldo.h>
//amy0514 
static struct alsps_hw cm3623_cust_alsps_hw = {
    .i2c_num    = 0,
#if defined(ACER_C8)
	.polling_mode_ps =0,	/*punk, for c8*/
#else
	.polling_mode_ps =0,
#endif
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .i2c_addr   = {0x0C, 0x48, 0x78, 0x00},
#if defined(ACER_C8)
    .als_level  = { 100,  5210,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,20000},
    .als_value  = { 31, 1000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000},
#else
    .als_level  = { 0,  1,  1,   7,  15,  15,  100, 1000, 2000,  3000,  6000, 10000, 14000, 18000, 20000},
    .als_value  = {40, 40, 90,  90, 160, 160,  225,  320,  640,  1280,  1280,  2600,  2600, 2600,  10240, 10240},
#endif
#if defined(I5000_P150)||defined(I5000_P14)
     .ps_threshold = 4,
#else	
     .ps_threshold = 5, /**/  
#endif
};
struct alsps_hw *cm3623_get_cust_alsps_hw(void) {
    return &cm3623_cust_alsps_hw;   
}


#if defined(ACER_C8)
int C_CUST_ALS_FACTOR=136;
#elif defined(SIMCOM_I5000) //amy0504 
int C_CUST_ALS_FACTOR  =  150;
#else
int C_CUST_ALS_FACTOR=1000;
#endif

