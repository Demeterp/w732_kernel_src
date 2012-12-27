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
 * (C) Copyright 2004, Li-Pro.Net <www.li-pro.net>
 * Stephan Linz <linz@li-pro.net>
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
#include <linux/ctype.h>

#if defined(CONFIG_NIOS_SPI)
#include <nios-io.h>
#include <spi.h>

#if !defined(CONFIG_SYS_NIOS_SPIBASE)
#error "*** CONFIG_SYS_NIOS_SPIBASE not defined ***"
#endif

#if !defined(CONFIG_SYS_NIOS_SPIBITS)
#error "*** CONFIG_SYS_NIOS_SPIBITS not defined ***"
#endif

#if (CONFIG_SYS_NIOS_SPIBITS != 8) && (CONFIG_SYS_NIOS_SPIBITS != 16)
#error "*** CONFIG_SYS_NIOS_SPIBITS should be either 8 or 16 ***"
#endif

static nios_spi_t	*spi	= (nios_spi_t *)CONFIG_SYS_NIOS_SPIBASE;

/* Warning:
 * You cannot enable DEBUG for early system initalization, i. e. when
 * this driver is used to read environment parameters like "baudrate"
 * from EEPROM which are used to initialize the serial port which is
 * needed to print the debug messages...
 */
#undef	DEBUG

#ifdef  DEBUG

#define	DPRINT(a)	printf a;
/* -----------------------------------------------
 * Helper functions to peek into tx and rx buffers
 * ----------------------------------------------- */
static const char * const hex_digit = "0123456789ABCDEF";

static char quickhex (int i)
{
	return hex_digit[i];
}

static void memdump (const void *pv, int num)
{
	int i;
	const unsigned char *pc = (const unsigned char *) pv;

	for (i = 0; i < num; i++)
		printf ("%c%c ", quickhex (pc[i] >> 4), quickhex (pc[i] & 0x0f));
	printf ("\t");
	for (i = 0; i < num; i++)
		printf ("%c", isprint (pc[i]) ? pc[i] : '.');
	printf ("\n");
}
#else   /* !DEBUG */

#define	DPRINT(a)
#define	memdump(p,n)

#endif  /* DEBUG */


struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct spi_slave *slave;

	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	slave = malloc(sizeof(struct spi_slave));
	if (!slave)
		return NULL;

	slave->bus = bus;
	slave->cs = cs;

	/* TODO: Add support for different modes and speeds */

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

}

/*
 * SPI transfer:
 *
 * See include/spi.h and http://www.altera.com/literature/ds/ds_nios_spi.pdf
 * for more informations.
 */
int spi_xfer(struct spi_slave *slave, int bitlen, const void *dout,
		void *din, unsigned long flags)
{
	const u8 *txd = dout;
	u8 *rxd = din;
	int j;

	DPRINT(("spi_xfer: slave %u:%u dout %08X din %08X bitlen %d\n",
		slave->bus, slave->cs, *(uint *)dout, *(uint *)din, bitlen));

	memdump(dout, (bitlen + 7) / 8);

	if (flags & SPI_XFER_BEGIN)
		spi_cs_activate(slave);

	if (!(flags & SPI_XFER_END) || bitlen > CONFIG_SYS_NIOS_SPIBITS) {
		/* leave chip select active */
		spi->control |= NIOS_SPI_SSO;
	}

	for (	j = 0;				/* count each byte in */
		j < ((bitlen + 7) / 8);		/* dout[] and din[] */

#if	(CONFIG_SYS_NIOS_SPIBITS == 8)
		j++) {

		while ((spi->status & NIOS_SPI_TRDY) == 0)
			;
		spi->txdata = (unsigned)(txd[j]);

		while ((spi->status & NIOS_SPI_RRDY) == 0)
			;
		rxd[j] = (unsigned char)(spi->rxdata & 0xff);

#elif	(CONFIG_SYS_NIOS_SPIBITS == 16)
		j++, j++) {

		while ((spi->status & NIOS_SPI_TRDY) == 0)
			;
		if ((j+1) < ((bitlen + 7) / 8))
			spi->txdata = (unsigned)((txd[j] << 8) | txd[j+1]);
		else
			spi->txdata = (unsigned)(txd[j] << 8);

		while ((spi->status & NIOS_SPI_RRDY) == 0)
			;
		rxd[j] = (unsigned char)((spi->rxdata >> 8) & 0xff);
		if ((j+1) < ((bitlen + 7) / 8))
			rxd[j+1] = (unsigned char)(spi->rxdata & 0xff);

#else
#error "*** unsupported value of CONFIG_SYS_NIOS_SPIBITS ***"
#endif

	}

	if (bitlen > CONFIG_SYS_NIOS_SPIBITS && (flags & SPI_XFER_END)) {
		spi->control &= ~NIOS_SPI_SSO;
	}

	if (flags & SPI_XFER_END)
		spi_cs_deactivate(slave);

	memdump(din, (bitlen + 7) / 8);

	return 0;
}

#endif /* CONFIG_NIOS_SPI */
