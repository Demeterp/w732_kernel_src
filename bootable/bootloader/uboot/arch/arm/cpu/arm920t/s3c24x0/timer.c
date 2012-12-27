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
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
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
#ifdef CONFIG_S3C24X0

#include <asm/io.h>
#include <asm/arch/s3c24x0_cpu.h>

int timer_load_val = 0;
static ulong timer_clk;

/* macro to read the 16 bit timer */
static inline ulong READ_TIMER(void)
{
	struct s3c24x0_timers *timers = s3c24x0_get_base_timers();

	return readl(&timers->TCNTO4) & 0xffff;
}

static ulong timestamp;
static ulong lastdec;

int timer_init(void)
{
	struct s3c24x0_timers *timers = s3c24x0_get_base_timers();
	ulong tmr;

	/* use PWM Timer 4 because it has no output */
	/* prescaler for Timer 4 is 16 */
	writel(0x0f00, &timers->TCFG0);
	if (timer_load_val == 0) {
		/*
		 * for 10 ms clock period @ PCLK with 4 bit divider = 1/2
		 * (default) and prescaler = 16. Should be 10390
		 * @33.25MHz and 15625 @ 50 MHz
		 */
		timer_load_val = get_PCLK() / (2 * 16 * 100);
		timer_clk = get_PCLK() / (2 * 16);
	}
	/* load value for 10 ms timeout */
	lastdec = timer_load_val;
	writel(timer_load_val, &timers->TCNTB4);
	/* auto load, manual update of Timer 4 */
	tmr = (readl(&timers->TCON) & ~0x0700000) | 0x0600000;
	writel(tmr, &timers->TCON);
	/* auto load, start Timer 4 */
	tmr = (tmr & ~0x0700000) | 0x0500000;
	writel(tmr, &timers->TCON);
	timestamp = 0;

	return (0);
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

void __udelay (unsigned long usec)
{
	ulong tmo;
	ulong start = get_ticks();

	tmo = usec / 1000;
	tmo *= (timer_load_val * 100);
	tmo /= 1000;

	while ((ulong) (get_ticks() - start) < tmo)
		/*NOP*/;
}

void reset_timer_masked(void)
{
	/* reset time */
	lastdec = READ_TIMER();
	timestamp = 0;
}

ulong get_timer_masked(void)
{
	ulong tmr = get_ticks();

	return tmr / (timer_clk / CONFIG_SYS_HZ);
}

void udelay_masked(unsigned long usec)
{
	ulong tmo;
	ulong endtime;
	signed long diff;

	if (usec >= 1000) {
		tmo = usec / 1000;
		tmo *= (timer_load_val * 100);
		tmo /= 1000;
	} else {
		tmo = usec * (timer_load_val * 100);
		tmo /= (1000 * 1000);
	}

	endtime = get_ticks() + tmo;

	do {
		ulong now = get_ticks();
		diff = endtime - now;
	} while (diff >= 0);
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	ulong now = READ_TIMER();

	if (lastdec >= now) {
		/* normal mode */
		timestamp += lastdec - now;
	} else {
		/* we have an overflow ... */
		timestamp += lastdec + timer_load_val - now;
	}
	lastdec = now;

	return timestamp;
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	ulong tbclk;

#if defined(CONFIG_SMDK2400) || defined(CONFIG_TRAB)
	tbclk = timer_load_val * 100;
#elif defined(CONFIG_SBC2410X) || \
      defined(CONFIG_SMDK2410) || \
      defined(CONFIG_VCMA9)
	tbclk = CONFIG_SYS_HZ;
#else
#	error "tbclk not configured"
#endif

	return tbclk;
}

/*
 * reset the cpu by setting up the watchdog timer and let him time out
 */
void reset_cpu(ulong ignored)
{
	struct s3c24x0_watchdog *watchdog;

#ifdef CONFIG_TRAB
	extern void disable_vfd(void);

	disable_vfd();
#endif

	watchdog = s3c24x0_get_base_watchdog();

	/* Disable watchdog */
	writel(0x0000, &watchdog->WTCON);

	/* Initialize watchdog timer count register */
	writel(0x0001, &watchdog->WTCNT);

	/* Enable watchdog timer; assert reset at timer timeout */
	writel(0x0021, &watchdog->WTCON);

	while (1)
		/* loop forever and wait for reset to happen */;

	/*NOTREACHED*/
}

#endif /* CONFIG_S3C24X0 */
