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
 *  linux/include/asm-arm/proc-armv/processor.h
 *
 *  Copyright (C) 1996-1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   20-09-1996	RMK	Created
 *   26-09-1996	RMK	Added 'EXTRA_THREAD_STRUCT*'
 *   28-09-1996	RMK	Moved start_thread into the processor dependencies
 *   09-09-1998	PJB	Delete redundant `wp_works_ok'
 *   30-05-1999	PJB	Save sl across context switches
 *   31-07-1999	RMK	Added 'domain' stuff
 */
#ifndef __ASM_PROC_PROCESSOR_H
#define __ASM_PROC_PROCESSOR_H

#include <asm/proc/domain.h>

#define KERNEL_STACK_SIZE	PAGE_SIZE

struct context_save_struct {
	unsigned long cpsr;
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r9;
	unsigned long sl;
	unsigned long fp;
	unsigned long pc;
};

#define INIT_CSS (struct context_save_struct){ SVC_MODE, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

#define EXTRA_THREAD_STRUCT						\
	unsigned int	domain;

#define EXTRA_THREAD_STRUCT_INIT					\
	domain:	  domain_val(DOMAIN_USER, DOMAIN_CLIENT) |		\
		  domain_val(DOMAIN_KERNEL, DOMAIN_MANAGER) |		\
		  domain_val(DOMAIN_IO, DOMAIN_CLIENT)

#define start_thread(regs,pc,sp)					\
({									\
	unsigned long *stack = (unsigned long *)sp;			\
	set_fs(USER_DS);						\
	memzero(regs->uregs, sizeof(regs->uregs));			\
	if (current->personality & ADDR_LIMIT_32BIT)			\
		regs->ARM_cpsr = USR_MODE;				\
	else								\
		regs->ARM_cpsr = USR26_MODE;				\
	regs->ARM_pc = pc;		/* pc */			\
	regs->ARM_sp = sp;		/* sp */			\
	regs->ARM_r2 = stack[2];	/* r2 (envp) */			\
	regs->ARM_r1 = stack[1];	/* r1 (argv) */			\
	regs->ARM_r0 = stack[0];	/* r0 (argc) */			\
})

#define KSTK_EIP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1019])
#define KSTK_ESP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1017])

/* Allocation and freeing of basic task resources. */
/*
 * NOTE! The task struct and the stack go together
 */
#define ll_alloc_task_struct() ((struct task_struct *) __get_free_pages(GFP_KERNEL,1))
#define ll_free_task_struct(p) free_pages((unsigned long)(p),1)

#endif
