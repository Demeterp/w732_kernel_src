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
 * (C) Copyright 2004, Freescale, Inc
 * TsiChung Liew, Tsi-Chung.Liew@freescale.com.
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
 *
 */

/*
 * Minimal serial functions needed to use one of the PSC ports
 * as serial console interface.
 */

#include <common.h>
#include <mpc8220.h>

DECLARE_GLOBAL_DATA_PTR;

#define PSC_BASE   MMAP_PSC1

#if defined(CONFIG_PSC_CONSOLE)
int serial_init (void)
{
	volatile psc8220_t *psc = (psc8220_t *) PSC_BASE;
	u32 counter;

	/* write to SICR: SIM2 = uart mode,dcd does not affect rx */
	psc->cr = 0;
	psc->ipcr_acr = 0;
	psc->isr_imr = 0;

	/* write to CSR: RX/TX baud rate from timers */
	psc->sr_csr = 0xdd000000;

	psc->mr1_2 = PSC_MR1_BITS_CHAR_8 | PSC_MR1_NO_PARITY | PSC_MR2_STOP_BITS_1;

	/* Setting up BaudRate */
	counter = ((gd->bus_clk / gd->baudrate)) >> 5;
	counter++;

	/* write to CTUR: divide counter upper byte */
	psc->ctur = ((counter & 0xff00) << 16);
	/* write to CTLR: divide counter lower byte */
	psc->ctlr = ((counter & 0x00ff) << 24);

	psc->cr = PSC_CR_RST_RX_CMD;
	psc->cr = PSC_CR_RST_TX_CMD;
	psc->cr = PSC_CR_RST_ERR_STS_CMD;
	psc->cr = PSC_CR_RST_BRK_INT_CMD;
	psc->cr = PSC_CR_RST_MR_PTR_CMD;

	psc->cr = PSC_CR_RX_ENABLE | PSC_CR_TX_ENABLE;
	return (0);
}

void serial_putc (const char c)
{
	volatile psc8220_t *psc = (psc8220_t *) PSC_BASE;

	if (c == '\n')
		serial_putc ('\r');

	/* Wait for last character to go. */
	while (!(psc->sr_csr & PSC_SR_TXRDY));

	psc->xmitbuf[0] = c;
}

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

int serial_getc (void)
{
	volatile psc8220_t *psc = (psc8220_t *) PSC_BASE;

	/* Wait for a character to arrive. */
	while (!(psc->sr_csr & PSC_SR_RXRDY));
	return psc->xmitbuf[2];
}

int serial_tstc (void)
{
	volatile psc8220_t *psc = (psc8220_t *) PSC_BASE;

	return (psc->sr_csr & PSC_SR_RXRDY);
}

void serial_setbrg (void)
{
	volatile psc8220_t *psc = (psc8220_t *) PSC_BASE;
	u32 counter;

	counter = ((gd->bus_clk / gd->baudrate)) >> 5;
	counter++;

	/* write to CTUR: divide counter upper byte */
	psc->ctur = ((counter & 0xff00) << 16);
	/* write to CTLR: divide counter lower byte */
	psc->ctlr = ((counter & 0x00ff) << 24);

	psc->cr = PSC_CR_RST_RX_CMD;
	psc->cr = PSC_CR_RST_TX_CMD;

	psc->cr = PSC_CR_RX_ENABLE | PSC_CR_TX_ENABLE;
}
#endif /* CONFIG_PSC_CONSOLE */
