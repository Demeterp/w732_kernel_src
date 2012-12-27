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
 * Tor Krill, Excito Elektronik i Skåne , tor@excito.com
 *
 * Modelled after the ds1337 driver
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

/*
 * Date & Time support (no alarms) for Intersil
 * ISL1208 Real Time Clock (RTC).
 */

#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>

/*---------------------------------------------------------------------*/
#ifdef DEBUG_RTC
#define DEBUGR(fmt,args...) printf(fmt ,##args)
#else
#define DEBUGR(fmt,args...)
#endif
/*---------------------------------------------------------------------*/

/*
 * RTC register addresses
 */

#define RTC_SEC_REG_ADDR	0x0
#define RTC_MIN_REG_ADDR	0x1
#define RTC_HR_REG_ADDR		0x2
#define RTC_DATE_REG_ADDR	0x3
#define RTC_MON_REG_ADDR	0x4
#define RTC_YR_REG_ADDR		0x5
#define RTC_DAY_REG_ADDR	0x6
#define RTC_STAT_REG_ADDR	0x7
/*
 * RTC control register bits
 */

/*
 * RTC status register bits
 */
#define RTC_STAT_BIT_ARST	0x80	/* AUTO RESET ENABLE BIT */
#define RTC_STAT_BIT_XTOSCB	0x40	/* CRYSTAL OSCILLATOR ENABLE BIT */
#define RTC_STAT_BIT_WRTC	0x10	/* WRITE RTC ENABLE BIT */
#define RTC_STAT_BIT_ALM	0x04	/* ALARM BIT */
#define RTC_STAT_BIT_BAT	0x02	/* BATTERY BIT */
#define RTC_STAT_BIT_RTCF	0x01	/* REAL TIME CLOCK FAIL BIT */

static uchar rtc_read (uchar reg);
static void rtc_write (uchar reg, uchar val);

/*
 * Get the current time from the RTC
 */

int rtc_get (struct rtc_time *tmp)
{
	int rel = 0;
	uchar sec, min, hour, mday, wday, mon, year, status;

	status = rtc_read (RTC_STAT_REG_ADDR);
	sec = rtc_read (RTC_SEC_REG_ADDR);
	min = rtc_read (RTC_MIN_REG_ADDR);
	hour = rtc_read (RTC_HR_REG_ADDR);
	wday = rtc_read (RTC_DAY_REG_ADDR);
	mday = rtc_read (RTC_DATE_REG_ADDR);
	mon = rtc_read (RTC_MON_REG_ADDR);
	year = rtc_read (RTC_YR_REG_ADDR);

	DEBUGR ("Get RTC year: %02x mon: %02x mday: %02x wday: %02x "
		"hr: %02x min: %02x sec: %02x status: %02x\n",
		year, mon, mday, wday, hour, min, sec, status);

	if (status & RTC_STAT_BIT_RTCF) {
		printf ("### Warning: RTC oscillator has stopped\n");
		rtc_write(RTC_STAT_REG_ADDR,
			rtc_read(RTC_STAT_REG_ADDR) &~ (RTC_STAT_BIT_BAT|RTC_STAT_BIT_RTCF));
		rel = -1;
	}

	tmp->tm_sec  = bcd2bin (sec & 0x7F);
	tmp->tm_min  = bcd2bin (min & 0x7F);
	tmp->tm_hour = bcd2bin (hour & 0x3F);
	tmp->tm_mday = bcd2bin (mday & 0x3F);
	tmp->tm_mon  = bcd2bin (mon & 0x1F);
	tmp->tm_year = bcd2bin (year)+2000;
	tmp->tm_wday = bcd2bin (wday & 0x07);
	tmp->tm_yday = 0;
	tmp->tm_isdst= 0;

	DEBUGR ("Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	return rel;
}

/*
 * Set the RTC
 */
int rtc_set (struct rtc_time *tmp)
{
	DEBUGR ("Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	/* enable write */
	rtc_write(RTC_STAT_REG_ADDR,
		rtc_read(RTC_STAT_REG_ADDR) | RTC_STAT_BIT_WRTC);

	rtc_write (RTC_YR_REG_ADDR, bin2bcd (tmp->tm_year % 100));
	rtc_write (RTC_MON_REG_ADDR, bin2bcd (tmp->tm_mon));
	rtc_write (RTC_DAY_REG_ADDR, bin2bcd (tmp->tm_wday));
	rtc_write (RTC_DATE_REG_ADDR, bin2bcd (tmp->tm_mday));
	rtc_write (RTC_HR_REG_ADDR, bin2bcd (tmp->tm_hour) | 0x80 ); /* 24h clock */
	rtc_write (RTC_MIN_REG_ADDR, bin2bcd (tmp->tm_min));
	rtc_write (RTC_SEC_REG_ADDR, bin2bcd (tmp->tm_sec));

	/* disable write */
	rtc_write(RTC_STAT_REG_ADDR,
		rtc_read(RTC_STAT_REG_ADDR) & ~RTC_STAT_BIT_WRTC);

	return 0;
}

void rtc_reset (void)
{
}

/*
 * Helper functions
 */

static uchar rtc_read (uchar reg)
{
	return (i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg));
}

static void rtc_write (uchar reg, uchar val)
{
	i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
}
