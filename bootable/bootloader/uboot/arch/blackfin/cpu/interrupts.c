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
 * U-boot - interrupts.c Interrupt related routines
 *
 * Copyright (c) 2005-2008 Analog Devices Inc.
 *
 * This file is based on interrupts.c
 * Copyright 1996 Roman Zippel
 * Copyright 1999 D. Jeff Dionne <jeff@uclinux.org>
 * Copyright 2000-2001 Lineo, Inc. D. Jefff Dionne <jeff@lineo.ca>
 * Copyright 2002 Arcturus Networks Inc. MaTed <mated@sympatico.ca>
 * Copyright 2003 Metrowerks/Motorola
 * Copyright 2003 Bas Vermeulen <bas@buyways.nl>,
 *			BuyWays B.V. (www.buyways.nl)
 *
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <config.h>
#include <watchdog.h>
#include <asm/blackfin.h>
#include "cpu.h"

static ulong timestamp;
static ulong last_time;
static int int_flag;

int irq_flags;			/* needed by asm-blackfin/system.h */

/* Functions just to satisfy the linker */

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On Blackfin it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On Blackfin it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	ulong tbclk;

	tbclk = CONFIG_SYS_HZ;
	return tbclk;
}

void enable_interrupts(void)
{
	local_irq_restore(int_flag);
}

int disable_interrupts(void)
{
	local_irq_save(int_flag);
	return 1;
}

void __udelay(unsigned long usec)
{
	unsigned long delay, start, stop;
	unsigned long cclk;
	cclk = (CONFIG_CCLK_HZ);

	while (usec > 1) {
		WATCHDOG_RESET();

		/*
		 * how many clock ticks to delay?
		 *  - request(in useconds) * clock_ticks(Hz) / useconds/second
		 */
		if (usec < 1000) {
			delay = (usec * (cclk / 244)) >> 12;
			usec = 0;
		} else {
			delay = (1000 * (cclk / 244)) >> 12;
			usec -= 1000;
		}

		asm volatile (" %0 = CYCLES;" : "=r" (start));
		do {
			asm volatile (" %0 = CYCLES; " : "=r" (stop));
		} while (stop - start < delay);
	}

	return;
}

#define MAX_TIM_LOAD	0xFFFFFFFF
int timer_init(void)
{
	bfin_write_TCNTL(0x1);
	CSYNC();
	bfin_write_TSCALE(0x0);
	bfin_write_TCOUNT(MAX_TIM_LOAD);
	bfin_write_TPERIOD(MAX_TIM_LOAD);
	bfin_write_TCNTL(0x7);
	CSYNC();

	timestamp = 0;
	last_time = 0;

	return 0;
}

/*
 * Any network command or flash
 * command is started get_timer shall
 * be called before TCOUNT gets reset,
 * to implement the accurate timeouts.
 *
 * How ever milliconds doesn't return
 * the number that has been elapsed from
 * the last reset.
 *
 * As get_timer is used in the u-boot
 * only for timeouts this should be
 * sufficient
 */
ulong get_timer(ulong base)
{
	ulong milisec;

	/* Number of clocks elapsed */
	ulong clocks = (MAX_TIM_LOAD - bfin_read_TCOUNT());

	/*
	 * Find if the TCOUNT is reset
	 * timestamp gives the number of times
	 * TCOUNT got reset
	 */
	if (clocks < last_time)
		timestamp++;
	last_time = clocks;

	/* Get the number of milliseconds */
	milisec = clocks / (CONFIG_CCLK_HZ / 1000);

	/*
	 * Find the number of millisonds that
	 * got elapsed before this TCOUNT cycle
	 */
	milisec += timestamp * (MAX_TIM_LOAD / (CONFIG_CCLK_HZ / 1000));

	return (milisec - base);
}

void reset_timer(void)
{
	timer_init();
}
