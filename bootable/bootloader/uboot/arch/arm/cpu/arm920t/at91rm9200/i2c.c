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
 *  i2c Support for Atmel's AT91RM9200 Two-Wire Interface
 *
 *  (c) Rick Bronson
 *
 *  Borrowed heavily from original work by:
 *  Copyright (c) 2000 Philip Edelbrock <phil@stimpy.netroedge.com>
 *
 *  Modified to work with u-boot by (C) 2004 Gary Jennejohn garyj@denx.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
*/
#include <common.h>

#ifdef CONFIG_HARD_I2C

#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>

#include <at91rm9200_i2c.h>

/* define DEBUG */

/*
 * Poll the i2c status register until the specified bit is set.
 * Returns 0 if timed out (100 msec)
 */
static short at91_poll_status(AT91PS_TWI twi, unsigned long bit) {
	int loop_cntr = 10000;
	do {
		udelay(10);
	} while (!(twi->TWI_SR & bit) && (--loop_cntr > 0));

	return (loop_cntr > 0);
}

/*
 * Generic i2c master transfer entrypoint
 *
 * rw == 1 means that this is a read
 */
static int
at91_xfer(unsigned char chip, unsigned int addr, int alen,
						unsigned char *buffer, int len, int rw)
{
	AT91PS_TWI twi = (AT91PS_TWI) AT91_TWI_BASE;
	int length;
	unsigned char *buf;
	/* Set the TWI Master Mode Register */
	twi->TWI_MMR = (chip << 16) | (alen << 8)
		| ((rw == 1) ? AT91C_TWI_MREAD : 0);

	/* Set TWI Internal Address Register with first messages data field */
	if (alen > 0)
		twi->TWI_IADR = addr;

	length = len;
	buf = buffer;
	if (length && buf) {	/* sanity check */
		if (rw) {
			twi->TWI_CR = AT91C_TWI_START;
			while (length--) {
				if (!length)
					twi->TWI_CR = AT91C_TWI_STOP;
				/* Wait until transfer is finished */
				if (!at91_poll_status(twi, AT91C_TWI_RXRDY)) {
					debug ("at91_i2c: timeout 1\n");
					return 1;
				}
				*buf++ = twi->TWI_RHR;
			}
			if (!at91_poll_status(twi, AT91C_TWI_TXCOMP)) {
				debug ("at91_i2c: timeout 2\n");
				return 1;
			}
		} else {
			twi->TWI_CR = AT91C_TWI_START;
			while (length--) {
				twi->TWI_THR = *buf++;
				if (!length)
					twi->TWI_CR = AT91C_TWI_STOP;
				if (!at91_poll_status(twi, AT91C_TWI_TXRDY)) {
					debug ("at91_i2c: timeout 3\n");
					return 1;
				}
			}
			/* Wait until transfer is finished */
			if (!at91_poll_status(twi, AT91C_TWI_TXCOMP)) {
				debug ("at91_i2c: timeout 4\n");
				return 1;
			}
		}
	}
	return 0;
}

int
i2c_probe(unsigned char chip)
{
	unsigned char buffer[1];

	return at91_xfer(chip, 0, 0, buffer, 1, 1);
}

int
i2c_read (unsigned char chip, unsigned int addr, int alen,
	  unsigned char *buffer, int len)
{
#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	/* we only allow one address byte */
	if (alen > 1)
		return 1;
	/* XXX assume an ATMEL AT24C16 */
	if (alen == 1) {
#if 0 /* EEPROM code already sets this correctly */
		chip |= (addr >> 8) & 0xff;
#endif
		addr = addr & 0xff;
	}
#endif
	return at91_xfer(chip, addr, alen, buffer, len, 1);
}

int
i2c_write(unsigned char chip, unsigned int addr, int alen,
	  unsigned char *buffer, int len)
{
#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	int i;
	unsigned char *buf;

	/* we only allow one address byte */
	if (alen > 1)
		return 1;
	/* XXX assume an ATMEL AT24C16 */
	if (alen == 1) {
		buf = buffer;
		/* do single byte writes */
		for (i = 0; i < len; i++) {
#if 0 /* EEPROM code already sets this correctly */
			chip |= (addr >> 8) & 0xff;
#endif
			addr = addr & 0xff;
			if (at91_xfer(chip, addr, alen, buf++, 1, 0))
				return 1;
			addr++;
		}
		return 0;
	}
#endif
	return at91_xfer(chip, addr, alen, buffer, len, 0);
}

/*
 * Main initialization routine
 */
void
i2c_init(int speed, int slaveaddr)
{
	AT91PS_TWI twi = (AT91PS_TWI) AT91_TWI_BASE;

	*AT91C_PIOA_PDR = AT91C_PA25_TWD | AT91C_PA26_TWCK;
	*AT91C_PIOA_ASR = AT91C_PA25_TWD | AT91C_PA26_TWCK;
	*AT91C_PIOA_MDER = AT91C_PA25_TWD | AT91C_PA26_TWCK;
	*AT91C_PMC_PCER = 1 << AT91C_ID_TWI;		/* enable peripheral clock */

	twi->TWI_IDR = 0x3ff;				/* Disable all interrupts */
	twi->TWI_CR = AT91C_TWI_SWRST;			/* Reset peripheral */
	twi->TWI_CR = AT91C_TWI_MSEN | AT91C_TWI_SVDIS;	/* Set Master mode */

	/* Here, CKDIV = 1 and CHDIV=CLDIV  ==> CLDIV = CHDIV = 1/4*((Fmclk/FTWI) -6) */
	twi->TWI_CWGR = AT91C_TWI_CKDIV1 | AT91C_TWI_CLDIV3 | (AT91C_TWI_CLDIV3 << 8);

	debug ("Found AT91 i2c\n");
	return;
}

#endif /* CONFIG_HARD_I2C */
