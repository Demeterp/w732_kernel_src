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
 * (C) Copyright 2009 Faraday Technology
 * Po-Yu Chuang <ratbert@faraday-tech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/ftpmu010.h>
#include <asm/arch/fttmr010.h>

static ulong timestamp;
static ulong lastdec;

static struct fttmr010 *tmr = (struct fttmr010 *)CONFIG_FTTMR010_BASE;
static struct ftpmu010 *pmu = (struct ftpmu010 *)CONFIG_FTPMU010_BASE;

#define TIMER_CLOCK	32768
#define TIMER_LOAD_VAL	0xffffffff

int timer_init(void)
{
	unsigned int oscc;
	unsigned int cr;

	debug("%s()\n", __func__);

	/* disable timers */
	writel(0, &tmr->cr);

	/*
	 * use 32768Hz oscillator for RTC, WDT, TIMER
	 */

	/* enable the 32768Hz oscillator */
	oscc = readl(&pmu->OSCC);
	oscc &= ~(FTPMU010_OSCC_OSCL_OFF | FTPMU010_OSCC_OSCL_TRI);
	writel(oscc, &pmu->OSCC);

	/* wait until ready */
	while (!(readl(&pmu->OSCC) & FTPMU010_OSCC_OSCL_STABLE))
		;

	/* select 32768Hz oscillator */
	oscc = readl(&pmu->OSCC);
	oscc |= FTPMU010_OSCC_OSCL_RTCLSEL;
	writel(oscc, &pmu->OSCC);

	/* setup timer */
	writel(TIMER_LOAD_VAL, &tmr->timer3_load);
	writel(TIMER_LOAD_VAL, &tmr->timer3_counter);
	writel(0, &tmr->timer3_match1);
	writel(0, &tmr->timer3_match2);

	/* we don't want timer to issue interrupts */
	writel(FTTMR010_TM3_MATCH1 |
	       FTTMR010_TM3_MATCH2 |
	       FTTMR010_TM3_OVERFLOW,
	       &tmr->interrupt_mask);

	cr = readl(&tmr->cr);
	cr |= FTTMR010_TM3_CLOCK;	/* use external clock */
	cr |= FTTMR010_TM3_ENABLE;
	writel(cr, &tmr->cr);

	/* init the timestamp and lastdec value */
	reset_timer_masked();

	return 0;
}

/*
 * timer without interrupts
 */

/*
 * reset time
 */
void reset_timer_masked(void)
{
	/* capure current decrementer value time */
	lastdec = readl(&tmr->timer3_counter) / (TIMER_CLOCK / CONFIG_SYS_HZ);
	timestamp = 0;		/* start "advancing" time stamp from 0 */

	debug("%s(): lastdec = %lx\n", __func__, lastdec);
}

void reset_timer(void)
{
	debug("%s()\n", __func__);
	reset_timer_masked();
}

/*
 * return timer ticks
 */
ulong get_timer_masked(void)
{
	/* current tick value */
	ulong now = readl(&tmr->timer3_counter) / (TIMER_CLOCK / CONFIG_SYS_HZ);

	debug("%s(): now = %lx, lastdec = %lx\n", __func__, now, lastdec);

	if (lastdec >= now) {
		/*
		 * normal mode (non roll)
		 * move stamp fordward with absoulte diff ticks
		 */
		timestamp += lastdec - now;
	} else {
		/*
		 * we have overflow of the count down timer
		 *
		 * nts = ts + ld + (TLV - now)
		 * ts=old stamp, ld=time that passed before passing through -1
		 * (TLV-now) amount of time after passing though -1
		 * nts = new "advancing time stamp"...it could also roll and
		 * cause problems.
		 */
		timestamp += lastdec + TIMER_LOAD_VAL - now;
	}

	lastdec = now;

	debug("%s() returns %lx\n", __func__, timestamp);

	return timestamp;
}

/*
 * return difference between timer ticks and base
 */
ulong get_timer(ulong base)
{
	debug("%s(%lx)\n", __func__, base);
	return get_timer_masked() - base;
}

void set_timer(ulong t)
{
	debug("%s(%lx)\n", __func__, t);
	timestamp = t;
}

/* delay x useconds AND preserve advance timestamp value */
void __udelay(unsigned long usec)
{
	long tmo = usec * (TIMER_CLOCK / 1000) / 1000;
	unsigned long now, last = readl(&tmr->timer3_counter);

	debug("%s(%lu)\n", __func__, usec);
	while (tmo > 0) {
		now = readl(&tmr->timer3_counter);
		if (now > last) /* count down timer overflow */
			tmo -= TIMER_LOAD_VAL + last - now;
		else
			tmo -= last - now;
		last = now;
	}
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	debug("%s()\n", __func__);
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	debug("%s()\n", __func__);
	return CONFIG_SYS_HZ;
}
