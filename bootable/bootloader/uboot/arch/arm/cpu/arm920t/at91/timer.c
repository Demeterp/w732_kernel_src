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
 * (C) Copyright 2002
 * Lineo, Inc. <www.lineo.com>
 * Bernhard Kuhn <bkuhn@lineo.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
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

#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/arch/at91_tc.h>
#include <asm/arch/at91_pmc.h>

/* the number of clocks per CONFIG_SYS_HZ */
#define TIMER_LOAD_VAL (CONFIG_SYS_HZ_CLOCK/CONFIG_SYS_HZ)

static u32 timestamp;
static u32 lastinc;

int timer_init(void)
{
	at91_tc_t *tc = (at91_tc_t *) AT91_TC_BASE;
	at91_pmc_t *pmc = (at91_pmc_t *) AT91_PMC_BASE;

	/* enables TC1.0 clock */
	writel(1 << AT91_ID_TC0, &pmc->pcer);	/* enable clock */

	writel(0, &tc->bcr);
	writel(AT91_TC_BMR_TC0XC0S_NONE | AT91_TC_BMR_TC1XC1S_NONE |
		AT91_TC_BMR_TC2XC2S_NONE , &tc->bmr);

	writel(AT91_TC_CCR_CLKDIS, &tc->tc[0].ccr);
	/* set to MCLK/2 and restart the timer
	when the value in TC_RC is reached */
	writel(AT91_TC_CMR_TCCLKS_CLOCK1 | AT91_TC_CMR_CPCTRG, &tc->tc[0].cmr);

	writel(0xFFFFFFFF, &tc->tc[0].idr); /* disable interupts */
	writel(TIMER_LOAD_VAL, &tc->tc[0].rc);

	writel(AT91_TC_CCR_SWTRG | AT91_TC_CCR_CLKEN, &tc->tc[0].ccr);
	lastinc = 0;
	timestamp = 0;

	return 0;
}

/*
 * timer without interrupts
 */

void reset_timer(void)
{
	reset_timer_masked();
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

void set_timer(ulong t)
{
	timestamp = t;
}

void __udelay(unsigned long usec)
{
	udelay_masked(usec);
}

void reset_timer_masked(void)
{
	/* reset time */
	at91_tc_t *tc = (at91_tc_t *) AT91_TC_BASE;
	lastinc = readl(&tc->tc[0].cv) & 0x0000ffff;
	timestamp = 0;
}

ulong get_timer_raw(void)
{
	at91_tc_t *tc = (at91_tc_t *) AT91_TC_BASE;
	u32 now;

	now = readl(&tc->tc[0].cv) & 0x0000ffff;

	if (now >= lastinc) {
		/* normal mode */
		timestamp += now - lastinc;
	} else {
		/* we have an overflow ... */
		timestamp += now + TIMER_LOAD_VAL - lastinc;
	}
	lastinc = now;

	return timestamp;
}

ulong get_timer_masked(void)
{
	return get_timer_raw()/TIMER_LOAD_VAL;
}

void udelay_masked(unsigned long usec)
{
	u32 tmo;
	u32 endtime;
	signed long diff;

	tmo = CONFIG_SYS_HZ_CLOCK / 1000;
	tmo *= usec;
	tmo /= 1000;

	endtime = get_timer_raw() + tmo;

	do {
		u32 now = get_timer_raw();
		diff = endtime - now;
	} while (diff >= 0);
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
