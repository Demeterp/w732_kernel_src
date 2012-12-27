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
 * Daniel Engström, Omicron Ceti AB, daniel@omicron.se
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
#include <asm/ptrace.h>
#include <asm/realmode.h>


#define REALMODE_BASE    ((char*)0x7c0)
#define REALMODE_MAILBOX ((char*)0xe00)


extern ulong _i386boot_realmode;
extern ulong _i386boot_realmode_size;
extern char realmode_enter;

int realmode_setup(void)
{
	ulong i386boot_realmode      = (ulong)&_i386boot_realmode + gd->reloc_off;
	ulong i386boot_realmode_size = (ulong)&_i386boot_realmode_size;

	/* copy the realmode switch code */
	if (i386boot_realmode_size > (REALMODE_MAILBOX-REALMODE_BASE)) {
		printf("realmode switch too large (%ld bytes, max is %d)\n",
		       i386boot_realmode_size, (REALMODE_MAILBOX-REALMODE_BASE));
		return -1;
	}

	memcpy(REALMODE_BASE, (void*)i386boot_realmode, i386boot_realmode_size);
	asm("wbinvd\n");

	return 0;
}

int enter_realmode(u16 seg, u16 off, struct pt_regs *in, struct pt_regs *out)
{

	/* setup out thin bios emulation */
	if (bios_setup()) {
		return -1;
	}

	if (realmode_setup()) {
		return -1;
	}

	in->eip = off;
	in->xcs = seg;
	if (3>(in->esp & 0xffff)) {
		printf("Warning: entering realmode with sp < 4 will fail\n");
	}

	memcpy(REALMODE_MAILBOX, in, sizeof(struct pt_regs));
	asm("wbinvd\n");

	__asm__ volatile (
		 "lcall $0x20,%0\n"  : :  "i" (&realmode_enter) );

	asm("wbinvd\n");
	memcpy(out, REALMODE_MAILBOX, sizeof(struct pt_regs));

	return out->eax;
}


/* This code is supposed to access a realmode interrupt
 * it does currently not work for me */
int enter_realmode_int(u8 lvl, struct pt_regs *in, struct pt_regs *out)
{
	/* place two instructions at 0x700 */
	writeb(0xcd, 0x700);  /* int $lvl */
	writeb(lvl, 0x701);
	writeb(0xcb, 0x702);  /* lret */
	asm("wbinvd\n");

	enter_realmode(0x00, 0x700, in, out);

	return out->eflags&1;
}
