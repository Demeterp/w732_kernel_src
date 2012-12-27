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
 *  Copyright (C) 2002 Wolfgang Denk <wd@denx.de>
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

#include <post.h>
#include <ppc_asm.tmpl>
#include <ppc_defs.h>
#include <asm/cache.h>

#if CONFIG_POST & CONFIG_SYS_POST_CPU

/* void cpu_post_exec_02 (ulong *code, ulong op1, ulong op2); */
	.global	cpu_post_exec_02
cpu_post_exec_02:
	isync
	mflr	r0
	stwu	r0, -4(r1)

	subi	r1, r1, 104
	stmw	r6, 0(r1)

	mtlr	r3
	mr	r3, r4
	mr	r4, r5
	blrl

	lmw	r6, 0(r1)
	addi	r1, r1, 104

	lwz	r0, 0(r1)
	addi	r1, r1, 4
	mtlr	r0
	blr

/* void cpu_post_exec_04 (ulong *code, ulong op1, ulong op2, ulong op3, ulong op4); */
	.global	cpu_post_exec_04
cpu_post_exec_04:
	isync
	mflr	r0
	stwu	r0, -4(r1)

	subi	r1, r1, 96
	stmw	r8, 0(r1)

	mtlr	r3
	mr	r3, r4
	mr	r4, r5
	mr	r5, r6
	mtxer	r7
	blrl

	lmw	r8, 0(r1)
	addi	r1, r1, 96

	lwz	r0, 0(r1)
	addi	r1, r1, 4
	mtlr	r0
	blr

/* void cpu_post_exec_12 (ulong *code, ulong *res, ulong op1, ulong op2); */
	.global	cpu_post_exec_12
cpu_post_exec_12:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)

	mtlr	r3
	mr	r3, r5
	mr	r4, r6
	blrl

	lwz	r4, 0(r1)
	stw	r3, 0(r4)

	lwz	r0, 4(r1)
	addi	r1, r1, 8
	mtlr	r0
	blr

/* void cpu_post_exec_11 (ulong *code, ulong *res, ulong op1); */
	.global	cpu_post_exec_11
cpu_post_exec_11:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)

	mtlr	r3
	mr	r3, r5
	blrl

	lwz	r4, 0(r1)
	stw	r3, 0(r4)

	lwz	r0, 4(r1)
	addi	r1, r1, 8
	mtlr	r0
	blr

/* void cpu_post_exec_21 (ulong *code, ulong *cr, ulong *res, ulong op1); */
	.global	cpu_post_exec_21
cpu_post_exec_21:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)
	stwu	r5, -4(r1)

	li	r0, 0
	mtxer	r0
	lwz	r0, 0(r4)
	mtcr	r0

	mtlr	r3
	mr	r3, r6
	blrl

	mfcr	r0
	lwz	r4, 4(r1)
	stw	r0, 0(r4)
	lwz	r4, 0(r1)
	stw	r3, 0(r4)

	lwz	r0, 8(r1)
	addi	r1, r1, 12
	mtlr	r0
	blr

/* void cpu_post_exec_22 (ulong *code, ulong *cr, ulong *res, ulong op1,
    ulong op2); */
	.global	cpu_post_exec_22
cpu_post_exec_22:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)
	stwu	r5, -4(r1)

	li	r0, 0
	mtxer	r0
	lwz	r0, 0(r4)
	mtcr	r0

	mtlr	r3
	mr	r3, r6
	mr	r4, r7
	blrl

	mfcr	r0
	lwz	r4, 4(r1)
	stw	r0, 0(r4)
	lwz	r4, 0(r1)
	stw	r3, 0(r4)

	lwz	r0, 8(r1)
	addi	r1, r1, 12
	mtlr	r0
	blr

/* void cpu_post_exec_12w (ulong *code, ulong *op1, ulong op2, ulong op3); */
	.global	cpu_post_exec_12w
cpu_post_exec_12w:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)

	mtlr	r3
	lwz	r3, 0(r4)
	mr	r4, r5
	mr	r5, r6
	blrl

	lwz	r4, 0(r1)
	stw	r3, 0(r4)

	lwz	r0, 4(r1)
	addi	r1, r1, 8
	mtlr	r0
	blr

/* void cpu_post_exec_11w (ulong *code, ulong *op1, ulong op2); */
	.global	cpu_post_exec_11w
cpu_post_exec_11w:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)

	mtlr	r3
	lwz	r3, 0(r4)
	mr	r4, r5
	blrl

	lwz	r4, 0(r1)
	stw	r3, 0(r4)

	lwz	r0, 4(r1)
	addi	r1, r1, 8
	mtlr	r0
	blr

/* void cpu_post_exec_22w (ulong *code, ulong *op1, ulong op2, ulong *op3); */
	.global	cpu_post_exec_22w
cpu_post_exec_22w:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)
	stwu	r6, -4(r1)

	mtlr	r3
	lwz	r3, 0(r4)
	mr	r4, r5
	blrl

	lwz	r4, 4(r1)
	stw	r3, 0(r4)
	lwz	r4, 0(r1)
	stw	r5, 0(r4)

	lwz	r0, 8(r1)
	addi	r1, r1, 12
	mtlr	r0
	blr

/* void cpu_post_exec_21w (ulong *code, ulong *op1, ulong *op2); */
	.global	cpu_post_exec_21w
cpu_post_exec_21w:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)
	stwu	r5, -4(r1)

	mtlr	r3
	lwz	r3, 0(r4)
	blrl

	lwz	r5, 4(r1)
	stw	r3, 0(r5)
	lwz	r5, 0(r1)
	stw	r4, 0(r5)

	lwz	r0, 8(r1)
	addi	r1, r1, 12
	mtlr	r0
	blr

/* void cpu_post_exec_21x (ulong *code, ulong *op1, ulong *op2, ulong op3); */
	.global	cpu_post_exec_21x
cpu_post_exec_21x:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)
	stwu	r5, -4(r1)

	mtlr	r3
	mr	r3, r6
	blrl

	lwz	r5, 4(r1)
	stw	r3, 0(r5)
	lwz	r5, 0(r1)
	stw	r4, 0(r5)

	lwz	r0, 8(r1)
	addi	r1, r1, 12
	mtlr	r0
	blr

/* void cpu_post_exec_31 (ulong *code, ulong *ctr, ulong *lr, ulong *jump,
    ulong cr); */
	.global	cpu_post_exec_31
cpu_post_exec_31:
	isync
	mflr	r0
	stwu	r0, -4(r1)
	stwu	r4, -4(r1)
	stwu	r5, -4(r1)
	stwu	r6, -4(r1)

	mtlr	r3
	lwz	r3, 0(r4)
	lwz	r4, 0(r5)
	mr	r6, r7

	mfcr	r7
	blrl
	mtcr	r7

	lwz	r7, 8(r1)
	stw	r3, 0(r7)
	lwz	r7, 4(r1)
	stw	r4, 0(r7)
	lwz	r7, 0(r1)
	stw	r5, 0(r7)

	lwz	r0, 12(r1)
	addi	r1, r1, 16
	mtlr	r0
	blr

/* int cpu_post_complex_1_asm (int a1, int a2, int a3, int a4, int n); */
	.global	cpu_post_complex_1_asm
cpu_post_complex_1_asm:
	li	r9,0
	cmpw	r9,r7
	bge	cpu_post_complex_1_done
	mtctr	r7
cpu_post_complex_1_loop:
	mullw	r0,r3,r4
	subf	r0,r5,r0
	divw	r0,r0,r6
	add	r9,r9,r0
	bdnz	cpu_post_complex_1_loop
cpu_post_complex_1_done:
	mr	r3,r9
	blr

/* int cpu_post_complex_2_asm (int x, int n); */
	.global	cpu_post_complex_2_asm
cpu_post_complex_2_asm:
	mr.	r0,r4
	mtctr	r0
	mr	r0,r3
	li	r3,1
	li	r4,1
	blelr
cpu_post_complex_2_loop:
	mullw	r3,r3,r0
	add	r3,r3,r4
	bdnz	cpu_post_complex_2_loop
blr

#endif
