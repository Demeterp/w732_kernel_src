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

/*
 * (C) Copyright 2008
 * Gururaja Hebbar gururajakr@sanyo.co.in
 *
 * reference linux-2.6.20.6/drivers/rtc/rtc-pl031.c
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <rtc.h>

#if defined(CONFIG_CMD_DATE)

#ifndef CONFIG_SYS_RTC_PL031_BASE
#error CONFIG_SYS_RTC_PL031_BASE is not defined!
#endif

/*
 * Register definitions
 */
#define	RTC_DR		0x00	/* Data read register */
#define	RTC_MR		0x04	/* Match register */
#define	RTC_LR		0x08	/* Data load register */
#define	RTC_CR		0x0c	/* Control register */
#define	RTC_IMSC	0x10	/* Interrupt mask and set register */
#define	RTC_RIS		0x14	/* Raw interrupt status register */
#define	RTC_MIS		0x18	/* Masked interrupt status register */
#define	RTC_ICR		0x1c	/* Interrupt clear register */

#define RTC_CR_START	(1 << 0)

#define	RTC_WRITE_REG(addr, val) \
			(*(volatile unsigned int *)(CONFIG_SYS_RTC_PL031_BASE + (addr)) = (val))
#define	RTC_READ_REG(addr)	\
			(*(volatile unsigned int *)(CONFIG_SYS_RTC_PL031_BASE + (addr)))

static int pl031_initted = 0;

/* Enable RTC Start in Control register*/
void rtc_init(void)
{
	RTC_WRITE_REG(RTC_CR, RTC_CR_START);

	pl031_initted = 1;
}

/*
 * Reset the RTC. We set the date back to 1970-01-01.
 */
void rtc_reset(void)
{
	RTC_WRITE_REG(RTC_LR, 0x00);
	if(!pl031_initted)
		rtc_init();
}

/*
 * Set the RTC
*/
int rtc_set(struct rtc_time *tmp)
{
	unsigned long tim;

	if(!pl031_initted)
		rtc_init();

	if (tmp == NULL) {
		puts("Error setting the date/time\n");
		return -1;
	}

	/* Calculate number of seconds this incoming time represents */
	tim = mktime(tmp->tm_year, tmp->tm_mon, tmp->tm_mday,
	                tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	RTC_WRITE_REG(RTC_LR, tim);

	return -1;
}

/*
 * Get the current time from the RTC
 */
int rtc_get(struct rtc_time *tmp)
{
	ulong tim;

	if(!pl031_initted)
		rtc_init();

	if (tmp == NULL) {
		puts("Error getting the date/time\n");
		return -1;
	}

	tim = RTC_READ_REG(RTC_DR);

	to_tm (tim, tmp);

	debug ( "Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	return 0;
}

#endif
