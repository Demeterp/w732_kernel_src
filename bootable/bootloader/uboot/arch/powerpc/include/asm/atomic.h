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
 * PowerPC atomic operations
 */

#ifndef _ASM_PPC_ATOMIC_H_
#define _ASM_PPC_ATOMIC_H_

#include <linux/config.h>

#ifdef CONFIG_SMP
typedef struct { volatile int counter; } atomic_t;
#else
typedef struct { int counter; } atomic_t;
#endif

#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		((v)->counter)
#define atomic_set(v,i)		(((v)->counter) = (i))

extern void atomic_clear_mask(unsigned long mask, unsigned long *addr);
extern void atomic_set_mask(unsigned long mask, unsigned long *addr);

extern __inline__ int atomic_add_return(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__("\n\
1:	lwarx	%0,0,%3\n\
	add	%0,%2,%0\n\
	stwcx.	%0,0,%3\n\
	bne-	1b"
	: "=&r" (t), "=m" (*v)
	: "r" (a), "r" (v), "m" (*v)
	: "cc");

	return t;
}

extern __inline__ int atomic_sub_return(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__("\n\
1:	lwarx	%0,0,%3\n\
	subf	%0,%2,%0\n\
	stwcx.	%0,0,%3\n\
	bne-	1b"
	: "=&r" (t), "=m" (*v)
	: "r" (a), "r" (v), "m" (*v)
	: "cc");

	return t;
}

extern __inline__ int atomic_inc_return(atomic_t *v)
{
	int t;

	__asm__ __volatile__("\n\
1:	lwarx	%0,0,%2\n\
	addic	%0,%0,1\n\
	stwcx.	%0,0,%2\n\
	bne-	1b"
	: "=&r" (t), "=m" (*v)
	: "r" (v), "m" (*v)
	: "cc");

	return t;
}

extern __inline__ int atomic_dec_return(atomic_t *v)
{
	int t;

	__asm__ __volatile__("\n\
1:	lwarx	%0,0,%2\n\
	addic	%0,%0,-1\n\
	stwcx.	%0,0,%2\n\
	bne	1b"
	: "=&r" (t), "=m" (*v)
	: "r" (v), "m" (*v)
	: "cc");

	return t;
}

#define atomic_add(a, v)		((void) atomic_add_return((a), (v)))
#define atomic_sub(a, v)		((void) atomic_sub_return((a), (v)))
#define atomic_sub_and_test(a, v)	(atomic_sub_return((a), (v)) == 0)
#define atomic_inc(v)			((void) atomic_inc_return((v)))
#define atomic_dec(v)			((void) atomic_dec_return((v)))
#define atomic_dec_and_test(v)		(atomic_dec_return((v)) == 0)

#endif /* _ASM_PPC_ATOMIC_H_ */
