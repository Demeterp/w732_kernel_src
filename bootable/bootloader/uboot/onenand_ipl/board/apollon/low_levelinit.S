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
 * Board specific setup info
 *
 * (C) Copyright 2005-2008 Samsung Electronics
 * Kyungmin Park <kyungmin.park@samsung.com>
 *
 * Derived from board/omap2420h4/platform.S
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
#include <asm/arch/omap2420.h>
#include <asm/arch/mem.h>
#include <asm/arch/clocks.h>

#define APOLLON_CS0_BASE	0x00000000

#ifdef PRCM_CONFIG_I
#define SDRC_ACTIM_CTRLA_0_VAL	0x7BA35907
#define SDRC_ACTIM_CTRLB_0_VAL	0x00000013
#define SDRC_RFR_CTRL_0_VAL	0x00044C01

/* GPMC */
#define APOLLON_GPMC_CONFIG1_0	0xe30d1201
#define APOLLON_GPMC_CONFIG2_0	0x000c1000
#define APOLLON_GPMC_CONFIG3_0	0x00030400
#define APOLLON_GPMC_CONFIG4_0	0x0B841006
#define APOLLON_GPMC_CONFIG5_0	0x020F0C11
#define APOLLON_GPMC_CONFIG6_0	0x00000000
#define APOLLON_GPMC_CONFIG7_0	(0x00000e40 | (APOLLON_CS0_BASE >> 24))

#elif defined(PRCM_CONFIG_II)
#define SDRC_ACTIM_CTRLA_0_VAL	0x4A59B485
#define SDRC_ACTIM_CTRLB_0_VAL	0x0000000C
#define SDRC_RFR_CTRL_0_VAL	0x00030001

/* GPMC */
#define APOLLON_GPMC_CONFIG1_0	0xe30d1201
#define APOLLON_GPMC_CONFIG2_0	0x00080E81
#define APOLLON_GPMC_CONFIG3_0	0x00030400
#define APOLLON_GPMC_CONFIG4_0	0x08041586
#define APOLLON_GPMC_CONFIG5_0	0x020C090E
#define APOLLON_GPMC_CONFIG6_0	0x00000000
#define APOLLON_GPMC_CONFIG7_0	(0x00000e40 | (APOLLON_CS0_BASE >> 24))

#else
#error "Please configure PRCM schecm"
#endif

_TEXT_BASE:
	.word	TEXT_BASE	/* sdram load addr from config.mk */

.globl lowlevel_init
lowlevel_init:
	mov r3, r0     /* save skip information */

	/* Disable watchdog */
	ldr	r0, =WD2_BASE
	ldr	r1, =WD_UNLOCK1
	str	r1, [r0, #WSPR]

	ldr	r1, =WD_UNLOCK2
	str	r1, [r0, #WSPR]

#ifdef DEBUG_LED
	/* LED0 OFF */
	ldr	r0, =0x480000E5		/* ball AA10, mode 3 */
	mov	r1, #0x0b
	strb	r1, [r0]
#endif

	/* Pin muxing for SDRC */
	mov	r1, #0x00
	ldr	r0, =0x480000A1		/* ball C12, mode 0 */
	strb	r1, [r0]

	ldr	r0, =0x48000032		/* ball D11, mode 0 */
	strb	r1, [r0]

	ldr	r0, =0x480000A3		/* ball B13, mode 0 */
	strb	r1, [r0]

	/* SDRC setting */
	ldr	r0, =OMAP2420_SDRC_BASE
	ldr	r1, =0x00000010
	str	r1, [r0, #0x10]

	ldr	r1, =0x00000100
	str	r1, [r0, #0x44]

	/* SDRC CS0 configuration */
#ifdef CONFIG_APOLLON_PLUS
	ldr	r1, =0x01702011
#else
	ldr	r1, =0x00d04011
#endif
	str	r1, [r0, #0x80]

	ldr	r1, =SDRC_ACTIM_CTRLA_0_VAL
	str	r1, [r0, #0x9C]

	ldr	r1, =SDRC_ACTIM_CTRLB_0_VAL
	str	r1, [r0, #0xA0]

	ldr	r1, =SDRC_RFR_CTRL_0_VAL
	str	r1, [r0, #0xA4]

	ldr	r1, =0x00000041
	str	r1, [r0, #0x70]

	/* Manual command sequence */
	ldr	r1, =0x00000007
	str	r1, [r0, #0xA8]

	ldr	r1, =0x00000000
	str	r1, [r0, #0xA8]

	ldr	r1, =0x00000001
	str	r1, [r0, #0xA8]

	ldr	r1, =0x00000002
	str	r1, [r0, #0xA8]
	str	r1, [r0, #0xA8]

	/*
	 * CS0 SDRC Mode register
	 *   Burst length = 4 - DDR memory
	 *   Serial mode
	 *   CAS latency = 3
	 */
	ldr	r1, =0x00000032
	str	r1, [r0, #0x84]

	/* Note: You MUST set EMR values */
	/* EMR1 & EMR2 */
	ldr	r1, =0x00000000
	str	r1, [r0, #0x88]
	str	r1, [r0, #0x8C]

#ifdef OLD_SDRC_DLLA_CTRL
	/* SDRC_DLLA_CTRL */
	ldr	r1, =0x00007306
	str	r1, [r0, #0x60]

	ldr	r1, =0x00007303
	str	r1, [r0, #0x60]
#else
	/* SDRC_DLLA_CTRL */
	ldr	r1, =0x00000506
	str	r1, [r0, #0x60]

	ldr	r1, =0x00000503
	str	r1, [r0, #0x60]
#endif

#ifdef __BROKEN_FEATURE__
	/* SDRC_DLLB_CTRL */
	ldr	r1, =0x00000506
	str	r1, [r0, #0x68]

	ldr	r1, =0x00000503
	str	r1, [r0, #0x68]
#endif

	/* little delay after init */
	mov	r2, #0x1800
1:
	subs	r2, r2, #0x1
	bne	1b

	ldr	sp, SRAM_STACK
	str	ip, [sp]	/* stash old link register */
	mov	ip, lr		/* save link reg across call */
	mov	r0, r3		/* pass skip info to s_init */

	bl	s_init		/* go setup pll,mux,memory */

	ldr	ip, [sp]	/* restore save ip */
	mov	lr, ip		/* restore link reg */

	/* back to arch calling code */
	mov	pc,	lr

	/* the literal pools origin */
	.ltorg

SRAM_STACK:
	.word LOW_LEVEL_SRAM_STACK
