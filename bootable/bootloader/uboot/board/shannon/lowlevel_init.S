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
 * Memory Setup stuff - taken from blob memsetup.S
 *
 * Copyright (C) 1999 2000 2001 Erik Mouw (J.A.K.Mouw@its.tudelft.nl) and
 *                     Jan-Derk Bakker (J.D.Bakker@its.tudelft.nl)
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
#include <version.h>


/* some parameters for the board */

MEM_BASE:	.long	0xa0000000
MEM_START:	.long	0xc0000000

#define	MDCNFG	0x00
#define MDCAS0	0x04
#define MDCAS1	0x08
#define MDCAS2	0x0c
#define MSC0	0x10
#define MSC1	0x14
#define MECR	0x18

mdcas0:		.long	0xc71c703f @ cccccccf
mdcas1:		.long	0xffc71c71 @ fffffffc
mdcas2:		.long	0xffffffff @ ffffffff
mdcnfg:		.long	0x0334b21f @ 9326991f
msc0:		.long   0xfff84458 @ 42304230
msc1:		.long	0xffffffff @ 20182018
mecr:		.long	0x7fff7fff @ 01000000

/* setting up the memory */

.globl lowlevel_init
lowlevel_init:
	ldr	r0, MEM_BASE

	/* Setup the flash memory */
	ldr	r1, msc0
	str	r1, [r0, #MSC0]

	/* Set up the DRAM */

	/* MDCAS0 */
	ldr	r1, mdcas0
	str	r1, [r0, #MDCAS0]

	/* MDCAS1 */
	ldr	r1, mdcas1
	str	r1, [r0, #MDCAS1]

	/* MDCAS2 */
	ldr	r1, mdcas2
	str	r1, [r0, #MDCAS2]

	/* MDCNFG */
	ldr	r1, mdcnfg
	str	r1, [r0, #MDCNFG]

	/* Set up PCMCIA space */
	ldr	r1, mecr
	str	r1, [r0, #MECR]

	/* Load something to activate bank */
	ldr	r1, MEM_START

.rept	8
	ldr	r0, [r1]
.endr

	/* everything is fine now */
	mov	pc, lr
