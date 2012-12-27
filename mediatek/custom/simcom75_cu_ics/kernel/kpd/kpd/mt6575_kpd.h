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

/*
 * Copyright (C) 2010 MediaTek, Inc.
 *
 * Author: Terry Chang <terry.chang@mediatek.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _MT6575_KPD_H_
#define _MT6575_KPD_H_

#include <linux/kernel.h>
#include <linux/delay.h>
#include <cust_kpd.h>
/* include PMIC header file */
#include <mach/mt6575_typedefs.h>
#include <mach/pmic_mt6329_hw_bank1.h> 
#include <mach/pmic_mt6329_sw_bank1.h> 
#include <mach/pmic_mt6329_hw.h>
#include <mach/pmic_mt6329_sw.h>
#include <mach/upmu_common_sw.h>
#include <mach/upmu_hw.h>


#define KPD_PWRKEY_USE_EINT 0
#define KPD_PWRKEY_USE_PMIC 1
#define KPD_DRV_CTRL_BACKLIGHT	KPD_NO	/* retired, move to Lights framework */
#define KPD_BACKLIGHT_TIME	8	/* sec */
/* the keys can wake up the system and we should enable backlight */
#define KPD_BACKLIGHT_WAKE_KEY	\
{				\
	KEY_ENDCALL, KEY_POWER,	\
}

#define KPD_HAS_SLIDE_QWERTY	KPD_NO
#define KPD_SLIDE_EINT		CUST_EINT_KPD_SLIDE_NUM
#define KPD_SLIDE_DEBOUNCE	CUST_EINT_KPD_SLIDE_DEBOUNCE_CN		/* ms */
#define KPD_SLIDE_POLARITY	CUST_EINT_KPD_SLIDE_POLARITY
#define KPD_SLIDE_SENSITIVE	CUST_EINT_KPD_SLIDE_SENSITIVE

#if defined(PHILIPS_TITAN)
#define KPD_HAS_LOWPOWER_EINT	KPD_YES
#else
#define KPD_HAS_LOWPOWER_EINT	KPD_NO
#endif
#define KPD_LOWPOWER_EINT	CUST_EINT_LOWPOWER_NUM
#define KPD_LOWPOWER_DEBOUNCE	CUST_EINT_LOWPOWER_DEBOUNCE_CN		/* ms */
#define KPD_LOWPOWER_POLARITY	CUST_EINT_LOWPOWER_POLARITY
#define KPD_LOWPOWER_SENSITIVE	CUST_EINT_LOWPOWER_SENSITIVE

#if KPD_DRV_CTRL_BACKLIGHT
extern void kpd_enable_backlight(void);
extern void kpd_disable_backlight(void);
extern void kpd_backlight_handler(bool pressed, u16 linux_keycode);
#else
#define kpd_enable_backlight()		do {} while (0)
#define kpd_disable_backlight()		do {} while (0)
#define kpd_backlight_handler(pressed, linux_keycode)	do {} while (0)
#endif

/* for META tool */
extern void kpd_set_backlight(bool onoff, void *val1, void *val2);

#if KPD_PWRKEY_USE_PMIC
void kpd_pwrkey_pmic_handler(unsigned long pressed);
#endif

void kpd_pmic_rstkey_handler(unsigned long pressed);

//#define KPD_PMIC_RSTKEY_MAP KEY_VOLUMEDOWN
//#define KPD_PMIC_LPRST_TD 0 /* timeout period. 0: 5sec; 1: 7sec; 2: 9sec; 3: 11sec */

#if defined(SIMCOM_I5000)||defined(PHILIPS_TITAN) //amy0504 
#define KPD_PMIC_RSTKEY_MAP KEY_HOME
//#define KPD_PMIC_LPRST_TD 0 /* timeout period. 0: 5sec; 1: 7sec; 2: 9sec; 3: 11sec */
#endif
#endif
