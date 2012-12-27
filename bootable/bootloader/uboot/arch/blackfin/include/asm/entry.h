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
 * entry.h - routines for context saving and restoring (for interrupts/exceptions)
 *
 * Copyright (c) 2005-2007 Analog Devices Inc.
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#ifndef __BLACKFIN_ENTRY_H
#define __BLACKFIN_ENTRY_H
#ifdef __ASSEMBLY__

#define SAVE_ALL_INT		save_context_no_interrupts
#define SAVE_ALL_SYS		save_context_no_interrupts
#define SAVE_CONTEXT		save_context_with_interrupts

#define RESTORE_ALL		restore_context_no_interrupts
#define RESTORE_ALL_SYS		restore_context_no_interrupts
#define RESTORE_CONTEXT		restore_context_with_interrupts

/*
 * Code to save processor context.
 * We even save the register which are preserved by a function call
 * - r4, r5, r6, r7, p3, p4, p5
 */
.macro save_context_with_interrupts
	[--sp] = R0;
	[--sp] = ( R7:0, P5:0 );
	[--sp] = fp;
	[--sp] = usp;

	[--sp] = i0;
	[--sp] = i1;
	[--sp] = i2;
	[--sp] = i3;

	[--sp] = m0;
	[--sp] = m1;
	[--sp] = m2;
	[--sp] = m3;

	[--sp] = l0;
	[--sp] = l1;
	[--sp] = l2;
	[--sp] = l3;

	[--sp] = b0;
	[--sp] = b1;
	[--sp] = b2;
	[--sp] = b3;
	[--sp] = a0.x;
	[--sp] = a0.w;
	[--sp] = a1.x;
	[--sp] = a1.w;

	[--sp] = LC0;
	[--sp] = LC1;
	[--sp] = LT0;
	[--sp] = LT1;
	[--sp] = LB0;
	[--sp] = LB1;

	[--sp] = ASTAT;

	[--sp] = r0;	/* Skip reserved */
	[--sp] = RETS;
	[--sp] = RETI;
	[--sp] = RETX;
	[--sp] = RETN;
	[--sp] = RETE;
	[--sp] = SEQSTAT;
	[--sp] = SYSCFG;
#ifdef CONFIG_CMD_KGDB
	p0.l = lo(IPEND)
	p0.h = hi(IPEND)
	r0 = [p0];
#endif
	[--sp] = r0;	/* Skip IPEND as well. */
.endm

.macro save_context_no_interrupts
	[--sp] = R0;
	[--sp] = ( R7:0, P5:0 );
	[--sp] = fp;
	[--sp] = usp;

	[--sp] = i0;
	[--sp] = i1;
	[--sp] = i2;
	[--sp] = i3;

	[--sp] = m0;
	[--sp] = m1;
	[--sp] = m2;
	[--sp] = m3;

	[--sp] = l0;
	[--sp] = l1;
	[--sp] = l2;
	[--sp] = l3;

	[--sp] = b0;
	[--sp] = b1;
	[--sp] = b2;
	[--sp] = b3;
	[--sp] = a0.x;
	[--sp] = a0.w;
	[--sp] = a1.x;
	[--sp] = a1.w;

	[--sp] = LC0;
	[--sp] = LC1;
	[--sp] = LT0;
	[--sp] = LT1;
	[--sp] = LB0;
	[--sp] = LB1;

	[--sp] = ASTAT;

	[--sp] = r0;	/* Skip reserved */
	[--sp] = RETS;
	r0 = RETI;
	[--sp] = r0;
	[--sp] = RETX;
	[--sp] = RETN;
	[--sp] = RETE;
	[--sp] = SEQSTAT;
	[--sp] = SYSCFG;
#ifdef CONFIG_CMD_KGDB
	p0.l = lo(IPEND)
	p0.h = hi(IPEND)
	r0 = [p0];
#endif
	[--sp] = r0;	/* Skip IPEND as well. */
.endm

.macro restore_context_no_interrupts
	sp += 4;
	SYSCFG = [sp++];
	SEQSTAT = [sp++];
	RETE = [sp++];
	RETN = [sp++];
	RETX = [sp++];
	r0 = [sp++];
	RETI = r0;
	RETS = [sp++];

	sp += 4;

	ASTAT = [sp++];

	LB1 = [sp++];
	LB0 = [sp++];
	LT1 = [sp++];
	LT0 = [sp++];
	LC1 = [sp++];
	LC0 = [sp++];

	a1.w = [sp++];
	a1.x = [sp++];
	a0.w = [sp++];
	a0.x = [sp++];
	b3 = [sp++];
	b2 = [sp++];
	b1 = [sp++];
	b0 = [sp++];

	l3 = [sp++];
	l2 = [sp++];
	l1 = [sp++];
	l0 = [sp++];

	m3 = [sp++];
	m2 = [sp++];
	m1 = [sp++];
	m0 = [sp++];

	i3 = [sp++];
	i2 = [sp++];
	i1 = [sp++];
	i0 = [sp++];

	sp += 4;
	fp = [sp++];

	( R7 : 0, P5 : 0) = [ SP ++ ];
	sp += 4;
.endm

.macro restore_context_with_interrupts
	sp += 4;
	SYSCFG = [sp++];
	SEQSTAT = [sp++];
	RETE = [sp++];
	RETN = [sp++];
	RETX = [sp++];
	RETI = [sp++];
	RETS = [sp++];

	sp += 4;

	ASTAT = [sp++];

	LB1 = [sp++];
	LB0 = [sp++];
	LT1 = [sp++];
	LT0 = [sp++];
	LC1 = [sp++];
	LC0 = [sp++];

	a1.w = [sp++];
	a1.x = [sp++];
	a0.w = [sp++];
	a0.x = [sp++];
	b3 = [sp++];
	b2 = [sp++];
	b1 = [sp++];
	b0 = [sp++];

	l3 = [sp++];
	l2 = [sp++];
	l1 = [sp++];
	l0 = [sp++];

	m3 = [sp++];
	m2 = [sp++];
	m1 = [sp++];
	m0 = [sp++];

	i3 = [sp++];
	i2 = [sp++];
	i1 = [sp++];
	i0 = [sp++];

	sp += 4;
	fp = [sp++];

	( R7 : 0, P5 : 0) = [ SP ++ ];
	sp += 4;
.endm

#endif
#endif
