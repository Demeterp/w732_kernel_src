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
 * U-boot - system.h
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

#ifndef _BLACKFIN_SYSTEM_H
#define _BLACKFIN_SYSTEM_H

/*
 * Interrupt configuring macros.
 */

extern int irq_flags;

#define local_irq_enable() \
	__asm__ __volatile__ ( \
		"sti %0;" \
		: \
		: "d" (irq_flags) \
	)

#define local_irq_disable() \
	do { \
		int __tmp_dummy; \
		__asm__ __volatile__ ( \
			"cli %0;" \
			: "=d" (__tmp_dummy) \
		); \
	} while (0)

# define local_irq_save(x) \
	__asm__ __volatile__ ( \
		"cli %0;" \
		: "=&d" (x) \
	)

#define local_save_flags(x) \
	__asm__ __volatile__ ( \
		"cli %0;" \
		"sti %0;" \
		: "=d" (x) \
	)

#define irqs_enabled_from_flags(x) ((x) != 0x1f)

#define local_irq_restore(x) \
	do { \
		if (irqs_enabled_from_flags(x)) \
			local_irq_enable(); \
	} while (0)

/*
 * Force strict CPU ordering.
 */
#define nop()			asm volatile ("nop;\n\t"::)
#define mb()			asm volatile (""   : : :"memory")
#define rmb()			asm volatile (""   : : :"memory")
#define wmb()			asm volatile (""   : : :"memory")
#define set_rmb(var, value)	do { xchg(&var, value); } while (0)
#define set_mb(var, value)	set_rmb(var, value)
#define set_wmb(var, value)	do { var = value; wmb(); } while (0)

#define xchg(ptr,x) ((__typeof__(*(ptr)))__xchg((unsigned long)(x),(ptr),sizeof(*(ptr))))

struct __xchg_dummy {
	unsigned long a[100];
};
#define __xg(x) ((volatile struct __xchg_dummy *)(x))

static inline unsigned long __xchg(unsigned long x, volatile void *ptr,
				   int size)
{
	unsigned long tmp = 0;
	unsigned long flags = 0;

	local_irq_save(flags);

	switch (size) {
	case 1:
		__asm__ __volatile__
			("%0 = b%2 (z);\n\t"
			 "b%2 = %1;\n\t"
			 : "=&d" (tmp) : "d" (x), "m" (*__xg(ptr)) : "memory");
		break;
	case 2:
		__asm__ __volatile__
			("%0 = w%2 (z);\n\t"
			 "w%2 = %1;\n\t"
			 : "=&d" (tmp) : "d" (x), "m" (*__xg(ptr)) : "memory");
		break;
	case 4:
		__asm__ __volatile__
			("%0 = %2;\n\t"
			 "%2 = %1;\n\t"
			 : "=&d" (tmp) : "d" (x), "m" (*__xg(ptr)) : "memory");
		break;
	}
	local_irq_restore(flags);
	return tmp;
}

#endif	/* _BLACKFIN_SYSTEM_H */
