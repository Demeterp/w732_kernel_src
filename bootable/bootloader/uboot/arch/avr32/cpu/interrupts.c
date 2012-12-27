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
 * Copyright (C) 2004-2006 Atmel Corporation
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
#include <div64.h>

#include <asm/errno.h>
#include <asm/io.h>
#include <asm/processor.h>
#include <asm/sysreg.h>

#include <asm/arch/memory-map.h>

#define HANDLER_MASK	0x00ffffff
#define INTLEV_SHIFT	30
#define INTLEV_MASK	0x00000003

DECLARE_GLOBAL_DATA_PTR;

/* Incremented whenever COUNT reaches 0xffffffff by timer_interrupt_handler */
volatile unsigned long timer_overflow;

/*
 * Instead of dividing by get_tbclk(), multiply by this constant and
 * right-shift the result by 32 bits.
 */
static unsigned long tb_factor;

unsigned long get_tbclk(void)
{
	return gd->cpu_hz;
}

unsigned long long get_ticks(void)
{
	unsigned long lo, hi_now, hi_prev;

	do {
		hi_prev = timer_overflow;
		lo = sysreg_read(COUNT);
		hi_now = timer_overflow;
	} while (hi_prev != hi_now);

	return ((unsigned long long)hi_now << 32) | lo;
}

void reset_timer(void)
{
	sysreg_write(COUNT, 0);
	cpu_sync_pipeline();	/* process any pending interrupts */
	timer_overflow = 0;
}

unsigned long get_timer(unsigned long base)
{
	u64 now = get_ticks();

	now *= tb_factor;
	return (unsigned long)(now >> 32) - base;
}

void set_timer(unsigned long t)
{
	unsigned long long ticks = t;
	unsigned long lo, hi, hi_new;

	ticks = (ticks * get_tbclk()) / CONFIG_SYS_HZ;
	hi = ticks >> 32;
	lo = ticks & 0xffffffffUL;

	do {
		timer_overflow = hi;
		sysreg_write(COUNT, lo);
		hi_new = timer_overflow;
	} while (hi_new != hi);
}

/*
 * For short delays only. It will overflow after a few seconds.
 */
void __udelay(unsigned long usec)
{
	unsigned long cycles;
	unsigned long base;
	unsigned long now;

	base = sysreg_read(COUNT);
	cycles = ((usec * (get_tbclk() / 10000)) + 50) / 100;

	do {
		now = sysreg_read(COUNT);
	} while ((now - base) < cycles);
}

static int set_interrupt_handler(unsigned int nr, void (*handler)(void),
				 unsigned int priority)
{
	extern void _evba(void);
	unsigned long intpr;
	unsigned long handler_addr = (unsigned long)handler;

	handler_addr -= (unsigned long)&_evba;

	if ((handler_addr & HANDLER_MASK) != handler_addr
	    || (priority & INTLEV_MASK) != priority)
		return -EINVAL;

	intpr = (handler_addr & HANDLER_MASK);
	intpr |= (priority & INTLEV_MASK) << INTLEV_SHIFT;
	writel(intpr, (void *)INTC_BASE + 4 * nr);

	return 0;
}

void timer_init(void)
{
	extern void timer_interrupt_handler(void);
	u64 tmp;

	sysreg_write(COUNT, 0);

	tmp = (u64)CONFIG_SYS_HZ << 32;
	tmp += gd->cpu_hz / 2;
	do_div(tmp, gd->cpu_hz);
	tb_factor = (u32)tmp;

	if (set_interrupt_handler(0, &timer_interrupt_handler, 3))
		return;

	/* For all practical purposes, this gives us an overflow interrupt */
	sysreg_write(COMPARE, 0xffffffff);
}
