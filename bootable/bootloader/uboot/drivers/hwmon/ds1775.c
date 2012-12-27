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
 * Dallas Semiconductor's DS1775 Digital Thermometer and Thermostat
 */

#include <common.h>

#include <i2c.h>
#include <dtt.h>

#define DTT_I2C_DEV_CODE	CONFIG_SYS_I2C_DTT_ADDR /* Dallas Semi's DS1775 device code */
#define DTT_READ_TEMP		0x0
#define DTT_CONFIG		0x1
#define DTT_TEMP_HYST		0x2
#define DTT_TEMP_OS		0x3

int dtt_read(int sensor, int reg)
{
	int dlen;
	uchar data[2];

	/*
	 * Calculate sensor address and command
	 */
	sensor = DTT_I2C_DEV_CODE + (sensor & 0x07); /* Calculate addr of ds1775 */

	/*
	 * Prepare to handle 2 byte result
	 */
	if ((reg == DTT_READ_TEMP) ||
	    (reg == DTT_TEMP_OS) || (reg == DTT_TEMP_HYST))
		dlen = 2;
	else
		dlen = 1;

	/*
	 * Now try to read the register
	 */
	if (i2c_read(sensor, reg, 1, data, dlen) != 0)
		return 1;

	/*
	 * Handle 2 byte result
	 */
	if (dlen == 2)
		return ((int)((short)data[1] + (((short)data[0]) << 8)));

	return (int) data[0];
}


int dtt_write(int sensor, int reg, int val)
{
	int dlen;
	uchar data[2];

	/*
	 * Calculate sensor address and register
	 */
	sensor = DTT_I2C_DEV_CODE + (sensor & 0x07);

	/*
	 * Handle various data sizes
	 */
	if ((reg == DTT_READ_TEMP) ||
	    (reg == DTT_TEMP_OS) || (reg == DTT_TEMP_HYST)) {
		dlen = 2;
		data[0] = (char)((val >> 8) & 0xff); /* MSB first */
		data[1] = (char)(val & 0xff);
	} else {
		dlen = 1;
		data[0] = (char)(val & 0xff);
	}

	/*
	 * Write value to device
	 */
	if (i2c_write(sensor, reg, 1, data, dlen) != 0)
		return 1;

	return 0;
}


static int _dtt_init(int sensor)
{
	int val;

	/*
	 * Setup High Temp
	 */
	val = ((CONFIG_SYS_DTT_MAX_TEMP * 2) << 7) & 0xff80;
	if (dtt_write(sensor, DTT_TEMP_OS, val) != 0)
		return 1;
	udelay(50000);			/* Max 50ms */

	/*
	 * Setup Low Temp - hysteresis
	 */
	val = (((CONFIG_SYS_DTT_MAX_TEMP - CONFIG_SYS_DTT_HYSTERESIS) * 2) << 7) & 0xff80;
	if (dtt_write(sensor, DTT_TEMP_HYST, val) != 0)
		return 1;
	udelay(50000);			/* Max 50ms */

	/*
	 * Setup configuraton register
	 *
	 * Fault Tolerance limits 4, Thermometer resolution bits is 9,
	 * Polarity = Active Low,continuous conversion mode, Thermostat
	 * mode is interrupt mode
	 */
	val = 0xa;
	if (dtt_write(sensor, DTT_CONFIG, val) != 0)
		return 1;
	udelay(50000);			/* Max 50ms */

	return 0;
}


int dtt_init (void)
{
	int i;
	unsigned char sensors[] = CONFIG_DTT_SENSORS;

	for (i = 0; i < sizeof(sensors); i++) {
		if (_dtt_init(sensors[i]) != 0)
			printf("DTT%d:  FAILED\n", i+1);
		else
			printf("DTT%d:  %i C\n", i+1, dtt_get_temp(sensors[i]));
	}

	return (0);
}


int dtt_get_temp(int sensor)
{
	return (dtt_read(sensor, DTT_READ_TEMP) / 256);
}
