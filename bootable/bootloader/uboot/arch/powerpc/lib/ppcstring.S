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
 * String handling functions for PowerPC.
 *
 * Copyright (C) 1996 Paul Mackerras.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <ppc_asm.tmpl>
#include <asm/errno.h>

	.globl	strcpy
strcpy:
	addi	r5,r3,-1
	addi	r4,r4,-1
1:	lbzu	r0,1(r4)
	cmpwi	0,r0,0
	stbu	r0,1(r5)
	bne	1b
	blr

	.globl	strncpy
strncpy:
	cmpwi	0,r5,0
	beqlr
	mtctr	r5
	addi	r6,r3,-1
	addi	r4,r4,-1
1:	lbzu	r0,1(r4)
	cmpwi	0,r0,0
	stbu	r0,1(r6)
	bdnzf	2,1b		/* dec ctr, branch if ctr != 0 && !cr0.eq */
	blr

	.globl	strcat
strcat:
	addi	r5,r3,-1
	addi	r4,r4,-1
1:	lbzu	r0,1(r5)
	cmpwi	0,r0,0
	bne	1b
	addi	r5,r5,-1
1:	lbzu	r0,1(r4)
	cmpwi	0,r0,0
	stbu	r0,1(r5)
	bne	1b
	blr

	.globl	strcmp
strcmp:
	addi	r5,r3,-1
	addi	r4,r4,-1
1:	lbzu	r3,1(r5)
	cmpwi	1,r3,0
	lbzu	r0,1(r4)
	subf.	r3,r0,r3
	beqlr	1
	beq	1b
	blr

	.globl	strlen
strlen:
	addi	r4,r3,-1
1:	lbzu	r0,1(r4)
	cmpwi	0,r0,0
	bne	1b
	subf	r3,r3,r4
	blr

	.globl	memset
memset:
	rlwimi	r4,r4,8,16,23
	rlwimi	r4,r4,16,0,15
	addi	r6,r3,-4
	cmplwi	0,r5,4
	blt	7f
	stwu	r4,4(r6)
	beqlr
	andi.	r0,r6,3
	add	r5,r0,r5
	subf	r6,r0,r6
	rlwinm	r0,r5,32-2,2,31
	mtctr	r0
	bdz	6f
1:	stwu	r4,4(r6)
	bdnz	1b
6:	andi.	r5,r5,3
7:	cmpwi	0,r5,0
	beqlr
	mtctr	r5
	addi	r6,r6,3
8:	stbu	r4,1(r6)
	bdnz	8b
	blr

	.globl	bcopy
bcopy:
	mr	r6,r3
	mr	r3,r4
	mr	r4,r6
	b	memcpy

	.globl	memmove
memmove:
	cmplw	0,r3,r4
	bgt	backwards_memcpy
	/* fall through */

	.globl	memcpy
memcpy:
	rlwinm.	r7,r5,32-3,3,31		/* r0 = r5 >> 3 */
	addi	r6,r3,-4
	addi	r4,r4,-4
	beq	2f			/* if less than 8 bytes to do */
	andi.	r0,r6,3			/* get dest word aligned */
	mtctr	r7
	bne	5f
1:	lwz	r7,4(r4)
	lwzu	r8,8(r4)
	stw	r7,4(r6)
	stwu	r8,8(r6)
	bdnz	1b
	andi.	r5,r5,7
2:	cmplwi	0,r5,4
	blt	3f
	lwzu	r0,4(r4)
	addi	r5,r5,-4
	stwu	r0,4(r6)
3:	cmpwi	0,r5,0
	beqlr
	mtctr	r5
	addi	r4,r4,3
	addi	r6,r6,3
4:	lbzu	r0,1(r4)
	stbu	r0,1(r6)
	bdnz	4b
	blr
5:	subfic	r0,r0,4
	mtctr	r0
6:	lbz	r7,4(r4)
	addi	r4,r4,1
	stb	r7,4(r6)
	addi	r6,r6,1
	bdnz	6b
	subf	r5,r0,r5
	rlwinm.	r7,r5,32-3,3,31
	beq	2b
	mtctr	r7
	b	1b

	.globl	backwards_memcpy
backwards_memcpy:
	rlwinm.	r7,r5,32-3,3,31		/* r0 = r5 >> 3 */
	add	r6,r3,r5
	add	r4,r4,r5
	beq	2f
	andi.	r0,r6,3
	mtctr	r7
	bne	5f
1:	lwz	r7,-4(r4)
	lwzu	r8,-8(r4)
	stw	r7,-4(r6)
	stwu	r8,-8(r6)
	bdnz	1b
	andi.	r5,r5,7
2:	cmplwi	0,r5,4
	blt	3f
	lwzu	r0,-4(r4)
	subi	r5,r5,4
	stwu	r0,-4(r6)
3:	cmpwi	0,r5,0
	beqlr
	mtctr	r5
4:	lbzu	r0,-1(r4)
	stbu	r0,-1(r6)
	bdnz	4b
	blr
5:	mtctr	r0
6:	lbzu	r7,-1(r4)
	stbu	r7,-1(r6)
	bdnz	6b
	subf	r5,r0,r5
	rlwinm.	r7,r5,32-3,3,31
	beq	2b
	mtctr	r7
	b	1b

	.globl	memcmp
memcmp:
	cmpwi	0,r5,0
	ble-	2f
	mtctr	r5
	addi	r6,r3,-1
	addi	r4,r4,-1
1:	lbzu	r3,1(r6)
	lbzu	r0,1(r4)
	subf.	r3,r0,r3
	bdnzt	2,1b
	blr
2:	li	r3,0
	blr

	.global	memchr
memchr:
	cmpwi	0,r5,0
	ble-	2f
	mtctr	r5
	addi	r3,r3,-1
1:	lbzu	r0,1(r3)
	cmpw	0,r0,r4
	bdnzf	2,1b
	beqlr
2:	li	r3,0
	blr
