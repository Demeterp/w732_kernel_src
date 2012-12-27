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

#include <common.h>
#include <mpc8xx.h>
#include <malloc.h>
#include <galileo/gt64260R.h>
#include <galileo/core.h>

#define MAX_I2C_RETRYS	    10
#define I2C_DELAY	    1000  /* Should be at least the # of MHz of Tclk */
#undef	DEBUG_I2C

#ifdef DEBUG_I2C
#define DP(x) x
#else
#define DP(x)
#endif

/* Assuming that there is only one master on the bus (us) */

static void
i2c_init(int speed, int slaveaddr)
{
	unsigned int n, m, freq, margin, power;
	unsigned int actualFreq, actualN=0, actualM=0;
	unsigned int control, status;
	unsigned int minMargin = 0xffffffff;
	unsigned int tclk = 125000000;

	DP(puts("i2c_init\n"));

	for(n = 0 ; n < 8 ; n++)
	{
		for(m = 0 ; m < 16 ; m++)
		{
			power = 2<<n; /* power = 2^(n+1) */
			freq = tclk/(10*(m+1)*power);
			if (speed > freq)
				margin = speed - freq;
			else
				margin = freq - speed;
			if(margin < minMargin)
			{
				minMargin   = margin;
				actualFreq  = freq;
				actualN	    = n;
				actualM	    = m;
			}
		}
	}

	DP(puts("setup i2c bus\n"));

	/* Setup bus */

	GT_REG_WRITE(I2C_SOFT_RESET, 0);

	DP(puts("udelay...\n"));

	udelay(I2C_DELAY);

	DP(puts("set baudrate\n"));

	GT_REG_WRITE(I2C_STATUS_BAUDE_RATE, (actualM << 3) | actualN);
	GT_REG_WRITE(I2C_CONTROL, (0x1 << 2) | (0x1 << 6));

	udelay(I2C_DELAY * 10);

	DP(puts("read control, baudrate\n"));

	GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
	GT_REG_READ(I2C_CONTROL, &control);
}

static uchar
i2c_start(void)
{
	unsigned int control, status;
	int count = 0;

	DP(puts("i2c_start\n"));

	/* Set the start bit */

	GT_REG_READ(I2C_CONTROL, &control);
	control |= (0x1 << 5);
	GT_REG_WRITE(I2C_CONTROL, control);

	GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);

	count = 0;
	while ((status & 0xff) != 0x08) {
		udelay(I2C_DELAY);
		if (count > 20) {
			GT_REG_WRITE(I2C_CONTROL, (0x1 << 4)); /*stop*/
			return (status);
		}
		GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
		count++;
	}

	return (0);
}

static uchar
i2c_select_device(uchar dev_addr, uchar read, int ten_bit)
{
	unsigned int status, data, bits = 7;
	int count = 0;

	DP(puts("i2c_select_device\n"));

	/* Output slave address */

	if (ten_bit) {
		bits = 10;
	}

	data = (dev_addr << 1);
	/* set the read bit */
	data |= read;
	GT_REG_WRITE(I2C_DATA, data);
	/* assert the address */
	RESET_REG_BITS(I2C_CONTROL, BIT3);

	udelay(I2C_DELAY);

	GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
	count = 0;
	while (((status & 0xff) != 0x40) && ((status & 0xff) != 0x18)) {
		udelay(I2C_DELAY);
		if (count > 20) {
			GT_REG_WRITE(I2C_CONTROL, (0x1 << 4)); /*stop*/
			return(status);
		}
		GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
		count++;
	}

	if (bits == 10) {
		printf("10 bit I2C addressing not yet implemented\n");
		return (0xff);
	}

	return (0);
}

static uchar
i2c_get_data(uchar* return_data, int len) {

	unsigned int data, status = 0;
	int count = 0;

	DP(puts("i2c_get_data\n"));

	while (len) {

		/* Get and return the data */

		RESET_REG_BITS(I2C_CONTROL, (0x1 << 3));

		udelay(I2C_DELAY * 5);

		GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
		count++;
		while ((status & 0xff) != 0x50) {
			udelay(I2C_DELAY);
			if(count > 2) {
				GT_REG_WRITE(I2C_CONTROL, (0x1 << 4)); /*stop*/
				return 0;
			}
			GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
			count++;
		}
		GT_REG_READ(I2C_DATA, &data);
		len--;
		*return_data = (uchar)data;
		return_data++;
	}
	RESET_REG_BITS(I2C_CONTROL, BIT2|BIT3);
	while ((status & 0xff) != 0x58) {
		udelay(I2C_DELAY);
		if(count > 200) {
			GT_REG_WRITE(I2C_CONTROL, (0x1 << 4)); /*stop*/
			return (status);
		}
		GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
		count++;
	}
	GT_REG_WRITE(I2C_CONTROL, (0x1 << 4)); /* stop */

	return (0);
}

static uchar
i2c_write_data(unsigned int data, int len)
{
	unsigned int status;
	int count = 0;

	DP(puts("i2c_write_data\n"));

	if (len > 4)
		return -1;

	while (len) {
		/* Set and assert the data */

		GT_REG_WRITE(I2C_DATA, (unsigned int)data);
		RESET_REG_BITS(I2C_CONTROL, (0x1 << 3));

		udelay(I2C_DELAY);

		GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
		count++;
		while ((status & 0xff) != 0x28) {
			udelay(I2C_DELAY);
			if(count > 20) {
				GT_REG_WRITE(I2C_CONTROL, (0x1 << 4)); /*stop*/
				return (status);
			}
			GT_REG_READ(I2C_STATUS_BAUDE_RATE, &status);
			count++;
		}
		len--;
	}
	GT_REG_WRITE(I2C_CONTROL, (0x1 << 3) | (0x1 << 4));
	GT_REG_WRITE(I2C_CONTROL, (0x1 << 4));

	udelay(I2C_DELAY * 10);

	return (0);
}

static uchar
i2c_set_dev_offset(uchar dev_addr, unsigned int offset, int ten_bit)
{
	uchar status;

	DP(puts("i2c_set_dev_offset\n"));

	status = i2c_select_device(dev_addr, 0, ten_bit);
	if (status) {
#ifdef DEBUG_I2C
		printf("Failed to select device setting offset: 0x%02x\n",
		       status);
#endif
		return status;
	}

	status = i2c_write_data(offset, 1);
	if (status) {
#ifdef DEBUG_I2C
		printf("Failed to write data: 0x%02x\n", status);
#endif
		return status;
	}

	return (0);
}

uchar
i2c_read(uchar dev_addr, unsigned int offset, int len, uchar* data,
	 int ten_bit)
{
	uchar status = 0;
	unsigned int i2cFreq = 400000;

	DP(puts("i2c_read\n"));

	i2c_init(i2cFreq,0);

	status = i2c_start();

	if (status) {
#ifdef DEBUG_I2C
		printf("Transaction start failed: 0x%02x\n", status);
#endif
		return status;
	}

	status = i2c_set_dev_offset(dev_addr, 0, 0);
	if (status) {
#ifdef DEBUG_I2C
		printf("Failed to set offset: 0x%02x\n", status);
#endif
		return status;
	}

	i2c_init(i2cFreq,0);

	status = i2c_start();
	if (status) {
#ifdef DEBUG_I2C
		printf("Transaction restart failed: 0x%02x\n", status);
#endif
		return status;
	}

	status = i2c_select_device(dev_addr, 1, ten_bit);
	if (status) {
#ifdef DEBUG_I2C
		printf("Address not acknowledged: 0x%02x\n", status);
#endif
		return status;
	}

	status = i2c_get_data(data, len);
	if (status) {
#ifdef DEBUG_I2C
		printf("Data not recieved: 0x%02x\n", status);
#endif
		return status;
	}

	return 0;
}
