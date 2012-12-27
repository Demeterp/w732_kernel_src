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
 * File:         drivers/i2c/pca9564.c
 * Based on:     drivers/i2c/s3c44b0_i2c.c
 * Author:
 *
 * Created:      2009-06-23
 * Description:  PCA9564 i2c bridge driver
 *
 * Modified:
 *               Copyright 2009 CJSC "NII STT", http://www.niistt.ru/
 *
 * Bugs:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <common.h>
#include <i2c.h>
#include <pca9564.h>
#include <asm/io.h>

#define PCA_STA			(CONFIG_PCA9564_BASE + 0)
#define PCA_TO			(CONFIG_PCA9564_BASE + 0)
#define PCA_DAT			(CONFIG_PCA9564_BASE + (1 << 2))
#define PCA_ADR			(CONFIG_PCA9564_BASE + (2 << 2))
#define PCA_CON			(CONFIG_PCA9564_BASE + (3 << 2))

static unsigned char pca_read_reg(unsigned int reg)
{
	return readb((void *)reg);
}

static void pca_write_reg(unsigned int reg, unsigned char value)
{
	writeb(value, (void *)reg);
}

static int pca_wait_busy(void)
{
	unsigned int timeout = 10000;

	while (!(pca_read_reg(PCA_CON) & PCA_CON_SI) && --timeout)
		udelay(1);

	if (timeout == 0)
		debug("I2C timeout!\n");

	debug("CON = 0x%02x, STA = 0x%02x\n", pca_read_reg(PCA_CON),
	       pca_read_reg(PCA_STA));

	return timeout ? 0 : 1;
}

/*=====================================================================*/
/*                         Public Functions                            */
/*=====================================================================*/

/*-----------------------------------------------------------------------
 * Initialization
 */
void i2c_init(int speed, int slaveaddr)
{
	pca_write_reg(PCA_CON, PCA_CON_ENSIO | speed);
}

/*
 * Probe the given I2C chip address.  Returns 0 if a chip responded,
 * not 0 on failure.
 */

int i2c_probe(uchar chip)
{
	unsigned char res;

	pca_write_reg(PCA_CON, PCA_CON_STA | PCA_CON_ENSIO);
	pca_wait_busy();

	pca_write_reg(PCA_CON, PCA_CON_STA | PCA_CON_ENSIO);

	pca_write_reg(PCA_DAT, (chip << 1) | 1);
	res = pca_wait_busy();

	if ((res == 0) && (pca_read_reg(PCA_STA) == 0x48))
		res = 1;

	pca_write_reg(PCA_CON, PCA_CON_STO | PCA_CON_ENSIO);

	return res;
}

/*
 * Read/Write interface:
 *   chip:    I2C chip address, range 0..127
 *   addr:    Memory (register) address within the chip
 *   alen:    Number of bytes to use for addr (typically 1, 2 for larger
 *              memories, 0 for register type devices with only one
 *              register)
 *   buffer:  Where to read/write the data
 *   len:     How many bytes to read/write
 *
 *   Returns: 0 on success, not 0 on failure
 */
int i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int i;

	pca_write_reg(PCA_CON, PCA_CON_ENSIO | PCA_CON_STA);
	pca_wait_busy();

	pca_write_reg(PCA_CON, PCA_CON_ENSIO);

	pca_write_reg(PCA_DAT, (chip << 1));
	pca_wait_busy();
	pca_write_reg(PCA_CON, PCA_CON_ENSIO);

	if (alen > 0) {
		pca_write_reg(PCA_DAT, addr);
		pca_wait_busy();
		pca_write_reg(PCA_CON, PCA_CON_ENSIO);
	}

	pca_write_reg(PCA_CON, PCA_CON_ENSIO | PCA_CON_STO);

	udelay(500);

	pca_write_reg(PCA_CON, PCA_CON_ENSIO | PCA_CON_STA);
	pca_wait_busy();
	pca_write_reg(PCA_CON, PCA_CON_ENSIO);

	pca_write_reg(PCA_DAT, (chip << 1) | 1);
	pca_wait_busy();

	for (i = 0; i < len; ++i) {
		if (i == len - 1)
			pca_write_reg(PCA_CON, PCA_CON_ENSIO);
		else
			pca_write_reg(PCA_CON, PCA_CON_ENSIO | PCA_CON_AA);

		pca_wait_busy();
		buffer[i] = pca_read_reg(PCA_DAT);

	}

	pca_write_reg(PCA_CON, PCA_CON_ENSIO | PCA_CON_STO);

	return 0;
}

int i2c_write(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int i;

	pca_write_reg(PCA_CON, PCA_CON_ENSIO | PCA_CON_STA);
	pca_wait_busy();
	pca_write_reg(PCA_CON, PCA_CON_ENSIO);

	pca_write_reg(PCA_DAT, chip << 1);
	pca_wait_busy();
	pca_write_reg(PCA_CON, PCA_CON_ENSIO);

	if (alen > 0) {
		pca_write_reg(PCA_DAT, addr);
		pca_wait_busy();
		pca_write_reg(PCA_CON, PCA_CON_ENSIO);
	}

	for (i = 0; i < len; ++i) {
		pca_write_reg(PCA_DAT, buffer[i]);
		pca_wait_busy();
		pca_write_reg(PCA_CON, PCA_CON_ENSIO);
	}

	pca_write_reg(PCA_CON, PCA_CON_STO | PCA_CON_ENSIO);

	return 0;
}
