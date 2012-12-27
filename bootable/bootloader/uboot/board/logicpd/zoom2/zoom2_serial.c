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
 * Copyright (c) 2009 Wind River Systems, Inc.
 * Tom Rix <Tom.Rix@windriver.com>
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
 * This file was adapted from arch/powerpc/cpu/mpc5xxx/serial.c
 *
 */

#include <common.h>
#include <serial.h>
#include <ns16550.h>
#include <asm/arch/cpu.h>
#include "zoom2_serial.h"

int quad_init_dev (unsigned long base)
{
	/*
	 * The Quad UART is on the debug board.
	 * Check if the debug board is attached before using the UART
	 */
	if (zoom2_debug_board_connected ()) {
		NS16550_t com_port = (NS16550_t) base;
		int baud_divisor = CONFIG_SYS_NS16550_CLK / 16 /
			CONFIG_BAUDRATE;

		/*
		 * Zoom2 has a board specific initialization of its UART.
		 * This generic initialization has been copied from
		 * drivers/serial/ns16550.c. The macros have been expanded.
		 *
		 * Do the following instead of
		 *
		 * NS16550_init (com_port, clock_divisor);
		 */
		com_port->ier = 0x00;

		/*
		 * On Zoom2 board Set pre-scalar to 1
		 * CLKSEL is GND => MCR[7] is 1 => preslr is 4
		 * So change the prescl to 1
		 */
		com_port->lcr = 0xBF;
		com_port->fcr |= 0x10;
		com_port->mcr &= 0x7F;

		/* This is generic ns16550.c setup */
		com_port->lcr = UART_LCR_BKSE | UART_LCR_8N1;
		com_port->dll = 0;
		com_port->dlm = 0;
		com_port->lcr = UART_LCR_8N1;
		com_port->mcr = UART_MCR_DTR | UART_MCR_RTS;
		com_port->fcr = UART_FCR_FIFO_EN | UART_FCR_RXSR |
			UART_FCR_TXSR;
		com_port->lcr = UART_LCR_BKSE | UART_LCR_8N1;
		com_port->dll = baud_divisor & 0xff;
		com_port->dlm = (baud_divisor >> 8) & 0xff;
		com_port->lcr = UART_LCR_8N1;
	}
	/*
	 * We have to lie here, otherwise the board init code will hang
	 * on the check
	 */
	return 0;
}

void quad_putc_dev (unsigned long base, const char c)
{
	if (zoom2_debug_board_connected ()) {

		if (c == '\n')
			quad_putc_dev (base, '\r');

		NS16550_putc ((NS16550_t) base, c);
	} else {
		usbtty_putc(c);
	}
}

void quad_puts_dev (unsigned long base, const char *s)
{
	if (zoom2_debug_board_connected ()) {
		while ((s != NULL) && (*s != '\0'))
			quad_putc_dev (base, *s++);
	} else {
		usbtty_puts(s);
	}
}

int quad_getc_dev (unsigned long base)
{
	if (zoom2_debug_board_connected ())
		return NS16550_getc ((NS16550_t) base);

	return usbtty_getc();
}

int quad_tstc_dev (unsigned long base)
{
	if (zoom2_debug_board_connected ())
		return NS16550_tstc ((NS16550_t) base);

	return usbtty_tstc();
}

void quad_setbrg_dev (unsigned long base)
{
	if (zoom2_debug_board_connected ()) {

		int clock_divisor = CONFIG_SYS_NS16550_CLK / 16 /
			CONFIG_BAUDRATE;

		NS16550_reinit ((NS16550_t) base, clock_divisor);
	}
}

QUAD_INIT (0)
QUAD_INIT (1)
QUAD_INIT (2)
QUAD_INIT (3)
