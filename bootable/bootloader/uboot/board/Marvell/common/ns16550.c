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
 * COM1 NS16550 support
 * originally from linux source (arch/powerpc/boot/ns16550.c)
 * modified to use CONFIG_SYS_ISA_MEM and new defines
 *
 * further modified by Josh Huber <huber@mclx.com> to support
 * the DUART on the Galileo Eval board. (db64360)
 */

#include <config.h>
#include "ns16550.h"

#ifdef ZUMA_NTL
/* no 16550 device */
#else
const NS16550_t COM_PORTS[] = { (NS16550_t) (CONFIG_SYS_DUART_IO + 0),
	(NS16550_t) (CONFIG_SYS_DUART_IO + 0x20)
};

volatile struct NS16550 *NS16550_init (int chan, int baud_divisor)
{
	volatile struct NS16550 *com_port;

	com_port = (struct NS16550 *) COM_PORTS[chan];
	com_port->ier = 0x00;
	com_port->lcr = LCR_BKSE;	/* Access baud rate */
	com_port->dll = baud_divisor & 0xff;	/* 9600 baud */
	com_port->dlm = (baud_divisor >> 8) & 0xff;
	com_port->lcr = LCR_8N1;	/* 8 data, 1 stop, no parity */
	com_port->mcr = MCR_DTR | MCR_RTS;	/* RTS/DTR */

	/* Clear & enable FIFOs */
	com_port->fcr = FCR_FIFO_EN | FCR_RXSR | FCR_TXSR;
	return (com_port);
}

void NS16550_reinit (volatile struct NS16550 *com_port, int baud_divisor)
{
	com_port->ier = 0x00;
	com_port->lcr = LCR_BKSE;	/* Access baud rate */
	com_port->dll = baud_divisor & 0xff;	/* 9600 baud */
	com_port->dlm = (baud_divisor >> 8) & 0xff;
	com_port->lcr = LCR_8N1;	/* 8 data, 1 stop, no parity */
	com_port->mcr = MCR_DTR | MCR_RTS;	/* RTS/DTR */

	/* Clear & enable FIFOs */
	com_port->fcr = FCR_FIFO_EN | FCR_RXSR | FCR_TXSR;
}

void NS16550_putc (volatile struct NS16550 *com_port, unsigned char c)
{
	while ((com_port->lsr & LSR_THRE) == 0);
	com_port->thr = c;
}

unsigned char NS16550_getc (volatile struct NS16550 *com_port)
{
	while ((com_port->lsr & LSR_DR) == 0);
	return (com_port->rbr);
}

int NS16550_tstc (volatile struct NS16550 *com_port)
{
	return ((com_port->lsr & LSR_DR) != 0);
}
#endif
