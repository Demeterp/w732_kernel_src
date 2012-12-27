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
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2003
 * Gleb Natapov <gnatapov@mrv.com>
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

#include <common.h>
#include <asm/processor.h>
#include <watchdog.h>
#ifdef CONFIG_STATUS_LED
#include <status_led.h>
#endif

#ifdef CONFIG_SHOW_ACTIVITY
void board_show_activity (ulong) __attribute__((weak, alias("__board_show_activity")));

void __board_show_activity (ulong dummy)
{
	return;
}
#endif /* CONFIG_SHOW_ACTIVITY */

#ifndef CONFIG_SYS_WATCHDOG_FREQ
#define CONFIG_SYS_WATCHDOG_FREQ (CONFIG_SYS_HZ / 2)
#endif

extern int interrupt_init_cpu (unsigned *);
extern void timer_interrupt_cpu (struct pt_regs *);

static unsigned decrementer_count; /* count value for 1e6/HZ microseconds */

static __inline__ unsigned long get_msr (void)
{
	unsigned long msr;

	asm volatile ("mfmsr %0":"=r" (msr):);

	return msr;
}

static __inline__ void set_msr (unsigned long msr)
{
	asm volatile ("mtmsr %0"::"r" (msr));
}

static __inline__ unsigned long get_dec (void)
{
	unsigned long val;

	asm volatile ("mfdec %0":"=r" (val):);

	return val;
}


static __inline__ void set_dec (unsigned long val)
{
	if (val)
		asm volatile ("mtdec %0"::"r" (val));
}


void enable_interrupts (void)
{
	set_msr (get_msr () | MSR_EE);
}

/* returns flag if MSR_EE was set before */
int disable_interrupts (void)
{
	ulong msr = get_msr ();

	set_msr (msr & ~MSR_EE);
	return ((msr & MSR_EE) != 0);
}

int interrupt_init (void)
{
	int ret;

	/* call cpu specific function from $(CPU)/interrupts.c */
	ret = interrupt_init_cpu (&decrementer_count);

	if (ret)
		return ret;

	set_dec (decrementer_count);

	set_msr (get_msr () | MSR_EE);

	return (0);
}

static volatile ulong timestamp = 0;

void timer_interrupt (struct pt_regs *regs)
{
	/* call cpu specific function from $(CPU)/interrupts.c */
	timer_interrupt_cpu (regs);

	/* Restore Decrementer Count */
	set_dec (decrementer_count);

	timestamp++;

#if defined(CONFIG_WATCHDOG) || defined (CONFIG_HW_WATCHDOG)
	if ((timestamp % (CONFIG_SYS_WATCHDOG_FREQ)) == 0)
		WATCHDOG_RESET ();
#endif    /* CONFIG_WATCHDOG || CONFIG_HW_WATCHDOG */

#ifdef CONFIG_STATUS_LED
	status_led_tick (timestamp);
#endif /* CONFIG_STATUS_LED */

#ifdef CONFIG_SHOW_ACTIVITY
	board_show_activity (timestamp);
#endif /* CONFIG_SHOW_ACTIVITY */
}

void reset_timer (void)
{
	timestamp = 0;
}

ulong get_timer (ulong base)
{
	return (timestamp - base);
}

void set_timer (ulong t)
{
	timestamp = t;
}
