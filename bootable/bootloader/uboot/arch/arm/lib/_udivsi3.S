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

/* # 1 "libgcc1.S" */
@ libgcc1 routines for ARM cpu.
@ Division routines, written by Richard Earnshaw, (rearnsha@armltd.co.uk)
dividend	.req	r0
divisor		.req	r1
result		.req	r2
curbit		.req	r3
/* ip		.req	r12	*/
/* sp		.req	r13	*/
/* lr		.req	r14	*/
/* pc		.req	r15	*/
	.text
	.globl	 __udivsi3
	.type	__udivsi3 ,function
	.globl	__aeabi_uidiv
	.type	__aeabi_uidiv ,function
	.align	0
 __udivsi3:
 __aeabi_uidiv:
	cmp	divisor, #0
	beq	Ldiv0
	mov	curbit, #1
	mov	result, #0
	cmp	dividend, divisor
	bcc	Lgot_result
Loop1:
	@ Unless the divisor is very big, shift it up in multiples of
	@ four bits, since this is the amount of unwinding in the main
	@ division loop.  Continue shifting until the divisor is
	@ larger than the dividend.
	cmp	divisor, #0x10000000
	cmpcc	divisor, dividend
	movcc	divisor, divisor, lsl #4
	movcc	curbit, curbit, lsl #4
	bcc	Loop1
Lbignum:
	@ For very big divisors, we must shift it a bit at a time, or
	@ we will be in danger of overflowing.
	cmp	divisor, #0x80000000
	cmpcc	divisor, dividend
	movcc	divisor, divisor, lsl #1
	movcc	curbit, curbit, lsl #1
	bcc	Lbignum
Loop3:
	@ Test for possible subtractions, and note which bits
	@ are done in the result.  On the final pass, this may subtract
	@ too much from the dividend, but the result will be ok, since the
	@ "bit" will have been shifted out at the bottom.
	cmp	dividend, divisor
	subcs	dividend, dividend, divisor
	orrcs	result, result, curbit
	cmp	dividend, divisor, lsr #1
	subcs	dividend, dividend, divisor, lsr #1
	orrcs	result, result, curbit, lsr #1
	cmp	dividend, divisor, lsr #2
	subcs	dividend, dividend, divisor, lsr #2
	orrcs	result, result, curbit, lsr #2
	cmp	dividend, divisor, lsr #3
	subcs	dividend, dividend, divisor, lsr #3
	orrcs	result, result, curbit, lsr #3
	cmp	dividend, #0			@ Early termination?
	movnes	curbit, curbit, lsr #4		@ No, any more bits to do?
	movne	divisor, divisor, lsr #4
	bne	Loop3
Lgot_result:
	mov	r0, result
	mov	pc, lr
Ldiv0:
	str	lr, [sp, #-4]!
	bl	 __div0       (PLT)
	mov	r0, #0			@ about as wrong as it could be
	ldmia	sp!, {pc}
	.size  __udivsi3       , . -  __udivsi3

.globl __aeabi_uidivmod
__aeabi_uidivmod:

	stmfd	sp!, {r0, r1, ip, lr}
	bl	__aeabi_uidiv
	ldmfd	sp!, {r1, r2, ip, lr}
	mul	r3, r0, r2
	sub	r1, r1, r3
	mov	pc, lr

.globl __aeabi_idivmod
__aeabi_idivmod:

	stmfd	sp!, {r0, r1, ip, lr}
	bl	__aeabi_idiv
	ldmfd	sp!, {r1, r2, ip, lr}
	mul	r3, r0, r2
	sub	r1, r1, r3
	mov	pc, lr
