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

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void *func[8], **pfunc;

typedef struct xxx xxx_t;
struct xxx {
	int dummy;
	void **pfunc;
} q;

#define XF_strcpy 3
#define XF_printf 4

#define LABEL(x)					\
asm volatile (						\

#if defined(__i386__)
#define EXPORT_FUNC(x)					\
asm volatile (						\
"	.globl mon_" #x "\n"				\
"mon_" #x ":\n"						\
"	movl	%0, %%eax\n"				\
"	movl	pfunc, %%ecx\n"				\
"	jmp	*(%%ecx,%%eax)\n"			\
	: : "i"(XF_ ## x * sizeof(void *)) : "eax", "ecx");
#elif defined(__powerpc__)
#define EXPORT_FUNC(x)					\
asm volatile (						\
"	.globl mon_" #x "\n"				\
"mon_" #x ":\n"						\
"	lwz	%%r11, %0(%%r2)\n"			\
"	lwz	%%r11, %1(%%r11)\n"			\
"	mtctr	%%r11\n"				\
"	bctr\n"					\
	: : "i"(offsetof(xxx_t, pfunc)), "i"(XF_ ## x * sizeof(void *)) : "r11", "r2");
#elif defined(__arm__)
#define EXPORT_FUNC(x)					\
asm volatile (						\
"	.globl mon_" #x "\n"				\
"mon_" #x ":\n"						\
"	ldr	ip, [r8, %0]\n"				\
"	ldr	pc, [ip, %1]\n"				\
	: : "i"(offsetof(xxx_t, pfunc)), "i"(XF_ ## x * sizeof(void *)) : "ip");
#elif defined(__mips__)
#define EXPORT_FUNC(x)					\
asm volatile (						\
"	.globl mon_" #x "\n"				\
"mon_" #x ":\n"						\
"	lw	$25, %0($26)\n"				\
"	lw	$25, %1($25)\n"				\
"	jr	$25\n"					\
	: : "i"(offsetof(xxx_t, pfunc)), "i"(XF_ ## x * sizeof(void *)) : "t9");
#else
#error [No stub code for this arch]
#endif

void dummy(void)
{
EXPORT_FUNC(printf)
EXPORT_FUNC(strcpy)
}

int main(void)
{
#if defined(__i386__)
	xxx_t *pq;
#elif defined(__powerpc__)
	register volatile xxx_t *pq asm("r2");
#elif defined(__arm__)
	register volatile xxx_t *pq asm("r8");
#elif defined(__mips__)
	register volatile xxx_t *pq asm("k0");
#endif
	char buf[32];

	func[XF_strcpy] = strcpy;
	func[XF_printf] = printf;
	pq = &q;
	pq->pfunc = pfunc = func;

	mon_strcpy(buf, "test");
	mon_printf("hi %s %d z\n", buf, 444);

	return 0;
}
