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
 * Driver for ST M41T94 SPI RTC
 *
 * Taken from the Linux kernel drivier:
 * Copyright (C) 2008 Kim B. Heino
 *
 * Adaptation for U-Boot:
 * Copyright (C) 2009
 * Albin Tonnerre, Free Electrons <albin.tonnerre@free-electrons.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <rtc.h>
#include <spi.h>

static struct spi_slave *slave;

#define M41T94_REG_SECONDS	0x01
#define M41T94_REG_MINUTES	0x02
#define M41T94_REG_HOURS	0x03
#define M41T94_REG_WDAY		0x04
#define M41T94_REG_DAY		0x05
#define M41T94_REG_MONTH	0x06
#define M41T94_REG_YEAR		0x07
#define M41T94_REG_HT		0x0c

#define M41T94_BIT_HALT		0x40
#define M41T94_BIT_STOP		0x80
#define M41T94_BIT_CB		0x40
#define M41T94_BIT_CEB		0x80

int rtc_set(struct rtc_time *tm)
{
	u8 buf[8]; /* write cmd + 7 registers */
	int ret;

	if (!slave) {
		slave = spi_setup_slave(CONFIG_M41T94_SPI_BUS,
					CONFIG_M41T94_SPI_CS, 1000000,
					SPI_MODE_3);
		if (!slave)
			return -1;
	}
	spi_claim_bus(slave);

	buf[0] = 0x80 | M41T94_REG_SECONDS; /* write time + date */
	buf[M41T94_REG_SECONDS] = bin2bcd(tm->tm_sec);
	buf[M41T94_REG_MINUTES] = bin2bcd(tm->tm_min);
	buf[M41T94_REG_HOURS]   = bin2bcd(tm->tm_hour);
	buf[M41T94_REG_WDAY]    = bin2bcd(tm->tm_wday + 1);
	buf[M41T94_REG_DAY]     = bin2bcd(tm->tm_mday);
	buf[M41T94_REG_MONTH]   = bin2bcd(tm->tm_mon + 1);

	buf[M41T94_REG_HOURS] |= M41T94_BIT_CEB;
	if (tm->tm_year >= 100)
		buf[M41T94_REG_HOURS] |= M41T94_BIT_CB;
	buf[M41T94_REG_YEAR] = bin2bcd(tm->tm_year % 100);

	ret = spi_xfer(slave, 64, buf, NULL, SPI_XFER_BEGIN | SPI_XFER_END);
	spi_release_bus(slave);
	return ret;
}

int rtc_get(struct rtc_time *tm)
{
	u8 buf[2];
	int ret, hour;

	if (!slave) {
		slave = spi_setup_slave(CONFIG_M41T94_SPI_BUS,
					CONFIG_M41T94_SPI_CS, 1000000,
					SPI_MODE_3);
		if (!slave)
			return -1;
	}
	spi_claim_bus(slave);

	/* clear halt update bit */
	ret = spi_w8r8(slave, M41T94_REG_HT);
	if (ret < 0)
		return ret;
	if (ret & M41T94_BIT_HALT) {
		buf[0] = 0x80 | M41T94_REG_HT;
		buf[1] = ret & ~M41T94_BIT_HALT;
		spi_xfer(slave, 16, buf, NULL, SPI_XFER_BEGIN | SPI_XFER_END);
	}

	/* clear stop bit */
	ret = spi_w8r8(slave, M41T94_REG_SECONDS);
	if (ret < 0)
		return ret;
	if (ret & M41T94_BIT_STOP) {
		buf[0] = 0x80 | M41T94_REG_SECONDS;
		buf[1] = ret & ~M41T94_BIT_STOP;
		spi_xfer(slave, 16, buf, NULL, SPI_XFER_BEGIN | SPI_XFER_END);
	}

	tm->tm_sec  = bcd2bin(spi_w8r8(slave, M41T94_REG_SECONDS));
	tm->tm_min  = bcd2bin(spi_w8r8(slave, M41T94_REG_MINUTES));
	hour = spi_w8r8(slave, M41T94_REG_HOURS);
	tm->tm_hour = bcd2bin(hour & 0x3f);
	tm->tm_wday = bcd2bin(spi_w8r8(slave, M41T94_REG_WDAY)) - 1;
	tm->tm_mday = bcd2bin(spi_w8r8(slave, M41T94_REG_DAY));
	tm->tm_mon  = bcd2bin(spi_w8r8(slave, M41T94_REG_MONTH)) - 1;
	tm->tm_year = bcd2bin(spi_w8r8(slave, M41T94_REG_YEAR));
	if ((hour & M41T94_BIT_CB) || !(hour & M41T94_BIT_CEB))
		tm->tm_year += 100;

	spi_release_bus(slave);
	return 0;
}

void rtc_reset(void)
{
	/*
	 * Could not be tested as the reset pin is not wired on
	 * the sbc35-ag20 board
	 */
}
