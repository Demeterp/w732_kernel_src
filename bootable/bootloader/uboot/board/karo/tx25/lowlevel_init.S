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
 * (C) Copyright 2009 DENX Software Engineering
 * Author: John Rigby <jrigby@gmail.com>
 *
 * Based on U-Boot and RedBoot sources for several different i.mx
 * platforms.
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

#include <asm/macro.h>

.macro init_aips
	write32	0x43f00000, 0x77777777
	write32	0x43f00004, 0x77777777
	write32	0x43f00000, 0x77777777
	write32	0x53f00004, 0x77777777
.endm

.macro init_max
	write32	0x43f04000, 0x43210
	write32	0x43f04100, 0x43210
	write32	0x43f04200, 0x43210
	write32	0x43f04300, 0x43210
	write32	0x43f04400, 0x43210

	write32	0x43f04010, 0x10
	write32	0x43f04110, 0x10
	write32	0x43f04210, 0x10
	write32	0x43f04310, 0x10
	write32	0x43f04410, 0x10

	write32	0x43f04800, 0x0
	write32	0x43f04900, 0x0
	write32	0x43f04a00, 0x0
	write32	0x43f04b00, 0x0
	write32	0x43f04c00, 0x0
.endm

.macro init_m3if
	write32 0xb8003000, 0x1
.endm

.macro init_clocks
	/*
	 * clocks
	 *
	 * first enable CLKO debug output
	 * 0x40000000 enables the debug CLKO signal
	 * 0x05000000 sets CLKO divider to 6
	 * 0x00600000 makes CLKO parent clk the USB clk
	 */
	write32	0x53f80064, 0x45600000
	write32	0x53f80008, 0x20034000

	/*
	 * enable all implemented clocks in all three
	 * clock control registers
	 */
	write32	0x53f8000c, 0x1fffffff
	write32	0x53f80010, 0xffffffff
	write32	0x53f80014, 0xfdfff
.endm

.macro init_ddrtype
	/*
	 * ddr_type is 3.3v SDRAM
	 */
	write32	0x43fac454, 0x800
.endm

/*
 * sdram controller init
 */
.macro init_sdram_bank bankaddr, ctl, cfg
	ldr	r0, =0xb8001000
	ldr	r2, =\bankaddr
	/*
	 * reset SDRAM controller
	 * then wait for initialization to complete
	 */
	ldr	r1, =(1 << 1)
	str	r1, [r0, #0x10]
1:	ldr	r3, [r0, #0x10]
	tst	r3, #(1 << 31)
	beq	1b

	ldr	r1, =0x95728
	str	r1, [r0, #\cfg]		/* config */

	ldr	r1, =0x92116480		/* control | precharge */
	str	r1, [r0, #\ctl]		/* write command to controller */
	str	r1, [r2, #0x400]	/* command encoded in address */

	ldr	r1, =0xa2116480		/* auto refresh */
	str	r1, [r0, #\ctl]
	ldrb	r3, [r2]		/* read dram twice to auto refresh */
	ldrb	r3, [r2]

	ldr	r1, =0xb2116480		/* control | load mode */
	str	r1, [r0, #\ctl]		/* write command to controller */
	strb	r1, [r2, #0x33]		/* command encoded in address */

	ldr	r1, =0x82116480		/* control  | normal (0)*/
	str	r1, [r0, #\ctl]		/* write command to controller */
.endm

.globl lowlevel_init
lowlevel_init:
	init_aips
	init_max
	init_m3if
	init_clocks

	init_sdram_bank 0x80000000, 0x0, 0x4

	init_sdram_bank 0x90000000, 0x8, 0xc
	mov	pc, lr
