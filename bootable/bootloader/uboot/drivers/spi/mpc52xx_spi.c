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
 * (C) Copyright 2009
 * Frank Bodammer <frank.bodammer@gcd-solutions.de>
 * (C) Copyright 2009 Semihalf, Grzegorz Bernacki
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
#include <asm/io.h>
#include <malloc.h>
#include <spi.h>
#include <mpc5xxx.h>

void spi_init(void)
{
	struct mpc5xxx_spi *spi = (struct mpc5xxx_spi *)MPC5XXX_SPI;
	/*
	 * Its important to use the correct order when initializing the
	 * registers
	 */
	out_8(&spi->ddr, 0x0F);	/* set all SPI pins as output */
	out_8(&spi->pdr, 0x00);	/* set SS low */
	/* SPI is master, SS is general purpose output */
	out_8(&spi->cr1, SPI_CR_MSTR | SPI_CR_SPE);
	out_8(&spi->cr2, 0x00);	/* normal operation */
	out_8(&spi->brr, 0x77);	/* baud rate: IPB clock / 2048 */
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct spi_slave *slave;

	slave = malloc(sizeof(struct spi_slave));
	if (!slave)
		return NULL;

	slave->bus = bus;
	slave->cs = cs;

	return slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	free(slave);
}

int spi_claim_bus(struct spi_slave *slave)
{
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	return;
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	struct mpc5xxx_spi *spi = (struct mpc5xxx_spi *)MPC5XXX_SPI;
	int i, iter = bitlen >> 3;
	const uchar *txp = dout;
	uchar *rxp = din;

	debug("spi_xfer: slave %u:%u dout %08X din %08X bitlen %u\n",
	      slave->bus, slave->cs, *(uint *) dout, *(uint *) din, bitlen);

	if (flags & SPI_XFER_BEGIN)
		setbits_8(&spi->pdr, SPI_PDR_SS);

	for (i = 0; i < iter; i++) {
		udelay(1000);
		debug("spi_xfer: sending %x\n", txp[i]);
		out_8(&spi->dr, txp[i]);
		while (!(in_8(&spi->sr) & SPI_SR_SPIF)) {
			udelay(1000);
			if (in_8(&spi->sr) & SPI_SR_WCOL) {
				rxp[i] = in_8(&spi->dr);
				puts("spi_xfer: write collision\n");
				return -1;
			}
		}
		rxp[i] = in_8(&spi->dr);
		debug("spi_xfer: received %x\n", rxp[i]);
	}
	if (flags & SPI_XFER_END)
		clrbits_8(&spi->pdr, SPI_PDR_SS);

	return 0;
}
