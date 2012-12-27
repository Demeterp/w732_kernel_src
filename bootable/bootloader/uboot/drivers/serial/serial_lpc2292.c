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
 * (C) Copyright 2002-2004
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * Copyright (C) 1999 2000 2001 Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
#include <asm/arch/hardware.h>

DECLARE_GLOBAL_DATA_PTR;

void serial_setbrg (void)
{
	unsigned short divisor = 0;

	switch (gd->baudrate) {
	case   1200:	divisor = 3072;	break;
	case   9600:	divisor =  384;	break;
	case  19200:	divisor =  192;	break;
	case  38400:	divisor =   96;	break;
	case  57600:	divisor =   64;	break;
	case 115200:	divisor =   32;	break;
	default:	hang ();	break;
	}

	/* init serial UART0 */
	PUT8(U0LCR, 0);
	PUT8(U0IER, 0);
	PUT8(U0LCR, 0x80);	/* DLAB=1 */
	PUT8(U0DLL, (unsigned char)(divisor & 0x00FF));
	PUT8(U0DLM, (unsigned char)(divisor >> 8));
	PUT8(U0LCR, 0x03);	/* 8N1, DLAB=0  */
	PUT8(U0FCR, 1);		/* Enable RX and TX FIFOs */
}

int serial_init (void)
{
	unsigned long pinsel0;

	serial_setbrg ();

	pinsel0 = GET32(PINSEL0);
	pinsel0 &= ~(0x00000003);
	pinsel0 |= 5;
	PUT32(PINSEL0, pinsel0);

	return (0);
}

void serial_putc (const char c)
{
	if (c == '\n')
	{
		while((GET8(U0LSR) & (1<<5)) == 0); /* Wait for empty U0THR */
		PUT8(U0THR, '\r');
	}

	while((GET8(U0LSR) & (1<<5)) == 0); /* Wait for empty U0THR */
	PUT8(U0THR, c);
}

int serial_getc (void)
{
	while((GET8(U0LSR) & 1) == 0);
	return GET8(U0RBR);
}

void
serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

/* Test if there is a byte to read */
int serial_tstc (void)
{
	return (GET8(U0LSR) & 1);
}
