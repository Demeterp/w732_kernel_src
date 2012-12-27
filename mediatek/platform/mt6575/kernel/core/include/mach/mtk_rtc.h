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

#ifndef _MTK_RTC_H_
#define _MTK_RTC_H_

#include <linux/kernel.h>
#include <linux/rtc.h>

#define RTC_GPIO_USER_MASK	(((1U << 13) - 1) & 0xff00)

typedef enum {
	RTC_GPIO_USER_WIFI	= 8,
	RTC_GPIO_USER_GPS	= 9,
	RTC_GPIO_USER_BT	= 10,
	RTC_GPIO_USER_FM	= 11,
	RTC_GPIO_USER_PMIC	= 12,
} rtc_gpio_user_t;

#ifdef CONFIG_MTK_RTC

/*
 * NOTE:
 * 1. RTC_GPIO always exports 32K enabled by some user even if the phone is powered off
 */
extern void rtc_gpio_enable_32k(rtc_gpio_user_t user);
extern void rtc_gpio_disable_32k(rtc_gpio_user_t user);

/* for AUDIOPLL (deprecated) */
extern void rtc_enable_abb_32k(void);
extern void rtc_disable_abb_32k(void);

/* NOTE: used in Sleep driver to workaround Vrtc-Vore level shifter issue */
extern void rtc_enable_writeif(void);
extern void rtc_disable_writeif(void);

extern void rtc_mark_recovery(void);

extern u16 rtc_rdwr_uart_bits(u16 *val);

extern void rtc_bbpu_power_down(void);

extern void rtc_read_pwron_alarm(struct rtc_wkalrm *alm);

#else

#define rtc_gpio_enable_32k(user)	do {} while (0)
#define rtc_gpio_disable_32k(user)	do {} while (0)
#define rtc_enable_abb_32k()		do {} while (0)
#define rtc_disable_abb_32k()		do {} while (0)
#define rtc_enable_writeif()		do {} while (0)
#define rtc_disable_writeif()		do {} while (0)
#define rtc_mark_recovery()		do {} while (0)
#define rtc_rdwr_uart_bits(val)		({ 0; })
#define rtc_bbpu_power_down()		do {} while (0)
#define rtc_read_pwron_alarm(alm)	do {} while (0)

#endif

#endif
