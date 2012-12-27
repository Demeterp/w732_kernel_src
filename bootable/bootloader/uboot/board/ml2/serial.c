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
 * (C) Copyright 2002
 * Peter De Schrijver (p2@mind.be), Mind Linux Solutions, NV.
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
#include <asm/u-boot.h>
#include <asm/processor.h>
#include <command.h>
#include <configs/ML2.h>

#if (defined CONFIG_SYS_INIT_CHAN1) || (defined CONFIG_SYS_INIT_CHAN2)
#include <ns16550.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#if (defined CONFIG_SYS_INIT_CHAN1) || (defined CONFIG_SYS_INIT_CHAN2)
const NS16550_t COM_PORTS[] = { (NS16550_t) CONFIG_SYS_NS16550_COM1,
	(NS16550_t) CONFIG_SYS_NS16550_COM2
};
#endif

int serial_init (void)
{
	int clock_divisor = CONFIG_SYS_NS16550_CLK / 16 / gd->baudrate;

#ifdef CONFIG_SYS_INIT_CHAN1
	(void) NS16550_init (COM_PORTS[0], clock_divisor);
#endif
#ifdef CONFIG_SYS_INIT_CHAN2
	(void) NS16550_init (COM_PORTS[1], clock_divisor);
#endif
	return 0;

}

void serial_putc (const char c)
{
	if (c == '\n')
		NS16550_putc (COM_PORTS[CONFIG_SYS_DUART_CHAN], '\r');

	NS16550_putc (COM_PORTS[CONFIG_SYS_DUART_CHAN], c);
}

int serial_getc (void)
{
	return NS16550_getc (COM_PORTS[CONFIG_SYS_DUART_CHAN]);
}

int serial_tstc (void)
{
	return NS16550_tstc (COM_PORTS[CONFIG_SYS_DUART_CHAN]);
}

void serial_setbrg (void)
{
	int clock_divisor = CONFIG_SYS_NS16550_CLK / 16 / gd->baudrate;

#ifdef CONFIG_SYS_INIT_CHAN1
	NS16550_reinit (COM_PORTS[0], clock_divisor);
#endif
#ifdef CONFIG_SYS_INIT_CHAN2
	NS16550_reinit (COM_PORTS[1], clock_divisor);
#endif
}

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

#if defined(CONFIG_CMD_KGDB)
void kgdb_serial_init (void)
{
}

void putDebugChar (int c)
{
	serial_putc (c);
}

void putDebugStr (const char *str)
{
	serial_puts (str);
}

int getDebugChar (void)
{
	return serial_getc ();
}

void kgdb_interruptible (int yes)
{
	return;
}
#endif
