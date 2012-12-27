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
 * AU1X00 UART support
 *
 * Hardcoded to UART 0 for now
 * Speed and options also hardcoded to 115200 8N1
 *
 *  Copyright (c) 2003	Thomas.Lange@corelatus.se
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

#include <config.h>
#include <common.h>
#include <asm/au1x00.h>

/******************************************************************************
*
* serial_init - initialize a channel
*
* This routine initializes the number of data bits, parity
* and set the selected baud rate. Interrupts are disabled.
* Set the modem control signals if the option is selected.
*
* RETURNS: N/A
*/

int serial_init (void)
{
	volatile u32 *uart_fifoctl = (volatile u32*)(UART0_ADDR+UART_FCR);
	volatile u32 *uart_enable = (volatile u32*)(UART0_ADDR+UART_ENABLE);

	/* Enable clocks first */
	*uart_enable = UART_EN_CE;

	/* Then release reset */
	/* Must release reset before setting other regs */
	*uart_enable = UART_EN_CE|UART_EN_E;

	/* Activate fifos, reset tx and rx */
	/* Set tx trigger level to 12 */
	*uart_fifoctl = UART_FCR_ENABLE_FIFO|UART_FCR_CLEAR_RCVR|
		UART_FCR_CLEAR_XMIT|UART_FCR_T_TRIGGER_12;

	serial_setbrg();

	return 0;
}


void serial_setbrg (void)
{
	volatile u32 *uart_clk = (volatile u32*)(UART0_ADDR+UART_CLK);
	volatile u32 *uart_lcr = (volatile u32*)(UART0_ADDR+UART_LCR);
	volatile u32 *sys_powerctrl = (u32 *)SYS_POWERCTRL;
	int sd;
	int divisorx2;

	/* sd is system clock divisor			*/
	/* see section 10.4.5 in au1550 datasheet	*/
	sd = (*sys_powerctrl & 0x03) + 2;

	/* calulate 2x baudrate and round */
	divisorx2 = ((CONFIG_SYS_MIPS_TIMER_FREQ/(sd * 16 * CONFIG_BAUDRATE)));

	if (divisorx2 & 0x01)
		divisorx2 = divisorx2 + 1;

	*uart_clk = divisorx2 / 2;

	/* Set parity, stop bits and word length to 8N1 */
	*uart_lcr = UART_LCR_WLEN8;
}

void serial_putc (const char c)
{
	volatile u32 *uart_lsr = (volatile u32*)(UART0_ADDR+UART_LSR);
	volatile u32 *uart_tx = (volatile u32*)(UART0_ADDR+UART_TX);

	if (c == '\n') serial_putc ('\r');

	/* Wait for fifo to shift out some bytes */
	while((*uart_lsr&UART_LSR_THRE)==0);

	*uart_tx = (u32)c;
}

void serial_puts (const char *s)
{
	while (*s)
	{
		serial_putc (*s++);
	}
}

int serial_getc (void)
{
	volatile u32 *uart_rx = (volatile u32*)(UART0_ADDR+UART_RX);
	char c;

	while (!serial_tstc());

	c = (*uart_rx&0xFF);
	return c;
}

int serial_tstc (void)
{
	volatile u32 *uart_lsr = (volatile u32*)(UART0_ADDR+UART_LSR);

	if(*uart_lsr&UART_LSR_DR){
		/* Data in rfifo */
		return(1);
	}
	return 0;
}
