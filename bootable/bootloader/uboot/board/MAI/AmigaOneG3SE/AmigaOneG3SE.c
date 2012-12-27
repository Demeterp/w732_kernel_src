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
 * Hyperion Entertainment, ThomasF@hyperion-entertainment.com
 * (C) Copyright 2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <command.h>
#include <pci.h>
#include <netdev.h>
#include "articiaS.h"
#include "memio.h"
#include "via686.h"

__asm__(" .globl send_kb				\n "
	"send_kb:					\n "
	"	lis	r9, 0xfe00			\n "
	"						\n "
	"	li	r4, 0x10	# retries	\n "
	"	mtctr	r4				\n "
	"						\n "
	"idle:						\n "
	"	lbz	r4, 0x64(r9)			\n "
	"	andi.	r4, r4, 0x02			\n "
	"	bne	idle				\n "

	"ready:						\n "
	"	stb	r3, 0x60(r9)			\n "
	"						\n "
	"check:						\n "
	"	lbz	r4, 0x64(r9)			\n "
	"	andi.	r4, r4, 0x01			\n "
	"	beq	check				\n "
	"						\n "
	"	lbz	r4, 0x60(r9)			\n "
	"	cmpwi	r4, 0xfa			\n "
	"	beq	done				\n "

	"	bdnz	idle				\n "

	"	li	r3, 0				\n "
	"	blr					\n "

	"done:						\n "
	"	li	r3, 1				\n "
	"	blr					\n "

	".globl test_kb					\n "
	"test_kb:					\n "
	"	mflr	r10				\n "
	"	li	r3, 0xed			\n "
	"	bl	send_kb				\n "
	"	li	r3, 0x01			\n "
	"	bl	send_kb				\n "
	"	mtlr	r10				\n "
	"	blr					\n "
);


int checkboard (void)
{
	printf ("Board: AmigaOneG3SE\n");
	return 0;
}

phys_size_t initdram (int board_type)
{
	return articiaS_ram_init ();
}


void after_reloc (ulong dest_addr, gd_t *gd)
{
	board_init_r (gd, dest_addr);
}


int misc_init_r (void)
{
	extern pci_dev_t video_dev;
	extern void drv_video_init (void);

	if (video_dev != ~0)
		drv_video_init ();

	return (0);
}


void pci_init_board (void)
{
#ifndef CONFIG_RAMBOOT
	articiaS_pci_init ();
#endif
}

int board_eth_init(bd_t *bis)
{
#if defined(CONFIG_3COM)
	eth_3com_initialize(bis);
#endif
	return 0;
}
