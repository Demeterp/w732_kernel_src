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

/* GRLIB APBUART Serial controller driver
 *
 * (C) Copyright 2007
 * Daniel Hellstrom, Gaisler Research, daniel@gaisler.com.
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

#include <common.h>
#include <asm/processor.h>
#include <asm/leon.h>
#include <ambapp.h>

DECLARE_GLOBAL_DATA_PTR;

/* Force cache miss each time a serial controller reg is read */
#define CACHE_BYPASS 1

#ifdef CACHE_BYPASS
#define READ_BYTE(var)  SPARC_NOCACHE_READ_BYTE((unsigned int)&(var))
#define READ_HWORD(var) SPARC_NOCACHE_READ_HWORD((unsigned int)&(var))
#define READ_WORD(var)  SPARC_NOCACHE_READ((unsigned int)&(var))
#define READ_DWORD(var) SPARC_NOCACHE_READ_DWORD((unsigned int)&(var))
#endif

ambapp_dev_apbuart *leon3_apbuart = NULL;

int serial_init(void)
{
	ambapp_apbdev apbdev;
	unsigned int tmp;

	/* find UART */
	if (ambapp_apb_first(VENDOR_GAISLER, GAISLER_APBUART, &apbdev) == 1) {

		leon3_apbuart = (ambapp_dev_apbuart *) apbdev.address;

		/* found apbuart, let's init...
		 *
		 * Set scaler / baud rate
		 *
		 * Receiver & transmitter enable
		 */
		leon3_apbuart->scaler = CONFIG_SYS_GRLIB_APBUART_SCALER;

		/* Let bit 11 be unchanged (debug bit for GRMON) */
		tmp = READ_WORD(leon3_apbuart->ctrl);

		leon3_apbuart->ctrl = ((tmp & LEON_REG_UART_CTRL_DBG) |
				       LEON_REG_UART_CTRL_RE |
				       LEON_REG_UART_CTRL_TE);

		return 0;
	}
	return -1;		/* didn't find hardware */
}

void serial_putc(const char c)
{
	if (c == '\n')
		serial_putc_raw('\r');

	serial_putc_raw(c);
}

void serial_putc_raw(const char c)
{
	if (!leon3_apbuart)
		return;

	/* Wait for last character to go. */
	while (!(READ_WORD(leon3_apbuart->status) & LEON_REG_UART_STATUS_THE)) ;

	/* Send data */
	leon3_apbuart->data = c;

#ifdef LEON_DEBUG
	/* Wait for data to be sent */
	while (!(READ_WORD(leon3_apbuart->status) & LEON_REG_UART_STATUS_TSE)) ;
#endif
}

void serial_puts(const char *s)
{
	while (*s) {
		serial_putc(*s++);
	}
}

int serial_getc(void)
{
	if (!leon3_apbuart)
		return 0;

	/* Wait for a character to arrive. */
	while (!(READ_WORD(leon3_apbuart->status) & LEON_REG_UART_STATUS_DR)) ;

	/* read data */
	return READ_WORD(leon3_apbuart->data);
}

int serial_tstc(void)
{
	if (leon3_apbuart)
		return (READ_WORD(leon3_apbuart->status) &
			LEON_REG_UART_STATUS_DR);
	return 0;
}

/* set baud rate for uart */
void serial_setbrg(void)
{
	/* update baud rate settings, read it from gd->baudrate */
	unsigned int scaler;
	if (leon3_apbuart && (gd->baudrate > 0)) {
		scaler =
		    (((CONFIG_SYS_CLK_FREQ * 10) / (gd->baudrate * 8)) -
		     5) / 10;
		leon3_apbuart->scaler = scaler;
	}
	return;
}
