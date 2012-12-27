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

#ifndef __KGDB_H__
#define __KGDB_H__

#include <asm/ptrace.h>

#define KGDBERR_BADPARAMS	1
#define KGDBERR_NOTHEXDIG	2
#define KGDBERR_MEMFAULT	3
#define KGDBERR_NOSPACE		4
#define KGDBERR_ALIGNFAULT	5

#define KGDBDATA_MAXREGS	8
#define KGDBDATA_MAXPRIV	8

#define KGDBEXIT_TYPEMASK	0xff

#define KGDBEXIT_KILL		0
#define KGDBEXIT_CONTINUE	1
#define KGDBEXIT_SINGLE		2

#define KGDBEXIT_WITHADDR	0x100

typedef
	struct {
		int num;
		unsigned long val;
	}
kgdb_reg;

typedef
	struct {
		int sigval;
		int extype;
		unsigned long exaddr;
		int nregs;
		kgdb_reg regs[KGDBDATA_MAXREGS];
		unsigned long private[KGDBDATA_MAXPRIV];
	}
kgdb_data;

/* these functions are provided by the generic kgdb support */
extern void kgdb_init(void);
extern void kgdb_error(int);
extern int kgdb_output_string(const char *, unsigned int);
extern void breakpoint(void);

/* these functions are provided by the platform specific kgdb support */
extern void kgdb_flush_cache_range(void *, void *);
extern void kgdb_flush_cache_all(void);
extern int kgdb_setjmp(long *);
extern void kgdb_longjmp(long *, int);
extern void kgdb_enter(struct pt_regs *, kgdb_data *);
extern void kgdb_exit(struct pt_regs *, kgdb_data *);
extern int kgdb_getregs(struct pt_regs *, char *, int);
extern void kgdb_putreg(struct pt_regs *, int, char *, int);
extern void kgdb_putregs(struct pt_regs *, char *, int);
extern int kgdb_trap(struct pt_regs *);
extern void kgdb_breakpoint(int argc, char *argv[]);

/* these functions are provided by the platform serial driver */
extern void kgdb_serial_init(void);
extern int getDebugChar(void);
extern void putDebugChar(int);
extern void putDebugStr(const char *);
extern void kgdb_interruptible(int);

/* this is referenced in the trap handler for the platform */
extern int (*debugger_exception_handler)(struct pt_regs *);

#endif /* __KGDB_H__ */
