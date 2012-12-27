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

#ifndef __ASM_SH_IRQFLAGS_H
#define __ASM_SH_IRQFLAGS_H

static inline void raw_local_irq_enable(void)
{
	unsigned long __dummy0, __dummy1;

	__asm__ __volatile__ (
		"stc	sr, %0\n\t"
		"and	%1, %0\n\t"
#ifdef CONFIG_CPU_HAS_SR_RB
		"stc	r6_bank, %1\n\t"
		"or	%1, %0\n\t"
#endif
		"ldc	%0, sr\n\t"
		: "=&r" (__dummy0), "=r" (__dummy1)
		: "1" (~0x000000f0)
		: "memory"
	);
}

static inline void raw_local_irq_disable(void)
{
	unsigned long flags;

	__asm__ __volatile__ (
		"stc	sr, %0\n\t"
		"or	#0xf0, %0\n\t"
		"ldc	%0, sr\n\t"
		: "=&z" (flags)
		: /* no inputs */
		: "memory"
	);
}

static inline void set_bl_bit(void)
{
	unsigned long __dummy0, __dummy1;

	__asm__ __volatile__ (
		"stc	sr, %0\n\t"
		"or	%2, %0\n\t"
		"and	%3, %0\n\t"
		"ldc	%0, sr\n\t"
		: "=&r" (__dummy0), "=r" (__dummy1)
		: "r" (0x10000000), "r" (0xffffff0f)
		: "memory"
	);
}

static inline void clear_bl_bit(void)
{
	unsigned long __dummy0, __dummy1;

	__asm__ __volatile__ (
		"stc	sr, %0\n\t"
		"and	%2, %0\n\t"
		"ldc	%0, sr\n\t"
		: "=&r" (__dummy0), "=r" (__dummy1)
		: "1" (~0x10000000)
		: "memory"
	);
}

static inline unsigned long __raw_local_save_flags(void)
{
	unsigned long flags;

	__asm__ __volatile__ (
		"stc	sr, %0\n\t"
		"and	#0xf0, %0\n\t"
		: "=&z" (flags)
		: /* no inputs */
		: "memory"
	);

	return flags;
}

#define raw_local_save_flags(flags) \
		do { (flags) = __raw_local_save_flags(); } while (0)

static inline int raw_irqs_disabled_flags(unsigned long flags)
{
	return (flags != 0);
}

static inline int raw_irqs_disabled(void)
{
	unsigned long flags = __raw_local_save_flags();

	return raw_irqs_disabled_flags(flags);
}

static inline unsigned long __raw_local_irq_save(void)
{
	unsigned long flags, __dummy;

	__asm__ __volatile__ (
		"stc	sr, %1\n\t"
		"mov	%1, %0\n\t"
		"or	#0xf0, %0\n\t"
		"ldc	%0, sr\n\t"
		"mov	%1, %0\n\t"
		"and	#0xf0, %0\n\t"
		: "=&z" (flags), "=&r" (__dummy)
		: /* no inputs */
		: "memory"
	);

	return flags;
}

#define raw_local_irq_save(flags) \
		do { (flags) = __raw_local_irq_save(); } while (0)

#define local_irq_save raw_local_irq_save

static inline void raw_local_irq_restore(unsigned long flags)
{
	if ((flags & 0xf0) != 0xf0)
		raw_local_irq_enable();
}
#define local_irq_restore raw_local_irq_restore

#endif /* __ASM_SH_IRQFLAGS_H */
