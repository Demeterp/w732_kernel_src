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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 Waldorf GMBH
 * Copyright (C) 1995, 1996, 1997, 1998, 1999, 2001, 2002, 2003 Ralf Baechle
 * Copyright (C) 1996 Paul M. Antoine
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_PROCESSOR_H
#define _ASM_PROCESSOR_H

#include <linux/config.h>

#include <asm/isadep.h>

#include <asm/cachectl.h>
#include <asm/mipsregs.h>
#include <asm/reg.h>
#include <asm/system.h>

/*
 * Return current * instruction pointer ("program counter").
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l;})

/*
 * System setup and hardware flags..
 */
extern void (*cpu_wait)(void);

extern unsigned int vced_count, vcei_count;

#define NUM_FPU_REGS	32

typedef __u64 fpureg_t;

/*
 * It would be nice to add some more fields for emulator statistics, but there
 * are a number of fixed offsets in offset.h and elsewhere that would have to
 * be recalculated by hand.  So the additional information will be private to
 * the FPU emulator for now.  See asm-mips/fpu_emulator.h.
 */

struct mips_fpu_struct {
	fpureg_t	fpr[NUM_FPU_REGS];
	unsigned int	fcr31;
};

#define NUM_DSP_REGS   6

typedef __u32 dspreg_t;

struct mips_dsp_state {
	dspreg_t        dspr[NUM_DSP_REGS];
	unsigned int    dspcontrol;
};

typedef struct {
	unsigned long seg;
} mm_segment_t;

#define ARCH_MIN_TASKALIGN	8

struct mips_abi;

/*
 * If you change thread_struct remember to change the #defines below too!
 */
struct thread_struct {
	/* Saved main processor registers. */
	unsigned long reg16;
	unsigned long reg17, reg18, reg19, reg20, reg21, reg22, reg23;
	unsigned long reg29, reg30, reg31;

	/* Saved cp0 stuff. */
	unsigned long cp0_status;

	/* Saved fpu/fpu emulator stuff. */
	struct mips_fpu_struct fpu;
#ifdef CONFIG_MIPS_MT_FPAFF
	/* Emulated instruction count */
	unsigned long emulated_fp;
	/* Saved per-thread scheduler affinity mask */
	cpumask_t user_cpus_allowed;
#endif /* CONFIG_MIPS_MT_FPAFF */

	/* Saved state of the DSP ASE, if available. */
	struct mips_dsp_state dsp;

	/* Other stuff associated with the thread. */
	unsigned long cp0_badvaddr;	/* Last user fault */
	unsigned long cp0_baduaddr;	/* Last kernel fault accessing USEG */
	unsigned long error_code;
	unsigned long trap_no;
	unsigned long irix_trampoline;  /* Wheee... */
	unsigned long irix_oldctx;
	struct mips_abi *abi;
};

struct task_struct;

/* Free all resources held by a thread. */
#define release_thread(thread) do { } while(0)

/* Prepare to copy thread state - unlazy all lazy status */
#define prepare_to_copy(tsk)	do { } while (0)

#define cpu_relax()	barrier()

/*
 * Return_address is a replacement for __builtin_return_address(count)
 * which on certain architectures cannot reasonably be implemented in GCC
 * (MIPS, Alpha) or is unuseable with -fomit-frame-pointer (i386).
 * Note that __builtin_return_address(x>=1) is forbidden because GCC
 * aborts compilation on some CPUs.  It's simply not possible to unwind
 * some CPU's stackframes.
 *
 * __builtin_return_address works only for non-leaf functions.  We avoid the
 * overhead of a function call by forcing the compiler to save the return
 * address register on the stack.
 */
#define return_address() ({__asm__ __volatile__("":::"$31");__builtin_return_address(0);})

#ifdef CONFIG_CPU_HAS_PREFETCH

#define ARCH_HAS_PREFETCH

static inline void prefetch(const void *addr)
{
	__asm__ __volatile__(
	"	.set	mips4		\n"
	"	pref	%0, (%1)	\n"
	"	.set	mips0		\n"
	:
	: "i" (Pref_Load), "r" (addr));
}

#endif

#endif /* _ASM_PROCESSOR_H */
