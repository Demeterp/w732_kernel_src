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
 * (C) Copyright 2009
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
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
#include <asm/arch/hardware.h>
#include <asm/arch/spr_gpt.h>
#include <asm/arch/spr_misc.h>

#define GPT_RESOLUTION	(CONFIG_SPEAR_HZ_CLOCK / CONFIG_SPEAR_HZ)
#define READ_TIMER()	(readl(&gpt_regs_p->count) & GPT_FREE_RUNNING)

static struct gpt_regs *const gpt_regs_p =
    (struct gpt_regs *)CONFIG_SPEAR_TIMERBASE;

static struct misc_regs *const misc_regs_p =
    (struct misc_regs *)CONFIG_SPEAR_MISCBASE;

static ulong timestamp;
static ulong lastdec;

int timer_init(void)
{
	u32 synth;

	/* Prescaler setting */
#if defined(CONFIG_SPEAR3XX)
	writel(MISC_PRSC_CFG, &misc_regs_p->prsc2_clk_cfg);
	synth = MISC_GPT4SYNTH;
#elif defined(CONFIG_SPEAR600)
	writel(MISC_PRSC_CFG, &misc_regs_p->prsc1_clk_cfg);
	synth = MISC_GPT3SYNTH;
#else
# error Incorrect config. Can only be spear{600|300|310|320}
#endif

	writel(readl(&misc_regs_p->periph_clk_cfg) | synth,
	       &misc_regs_p->periph_clk_cfg);

	/* disable timers */
	writel(GPT_PRESCALER_1 | GPT_MODE_AUTO_RELOAD, &gpt_regs_p->control);

	/* load value for free running */
	writel(GPT_FREE_RUNNING, &gpt_regs_p->compare);

	/* auto reload, start timer */
	writel(readl(&gpt_regs_p->control) | GPT_ENABLE, &gpt_regs_p->control);

	reset_timer_masked();

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
	return (get_timer_masked() / GPT_RESOLUTION) - base;
}

void set_timer(ulong t)
{
	timestamp = t;
}

void __udelay(unsigned long usec)
{
	ulong tmo;
	ulong start = get_timer_masked();
	ulong tenudelcnt = CONFIG_SPEAR_HZ_CLOCK / (1000 * 100);
	ulong rndoff;

	rndoff = (usec % 10) ? 1 : 0;

	/* tenudelcnt timer tick gives 10 microsecconds delay */
	tmo = ((usec / 10) + rndoff) * tenudelcnt;

	while ((ulong) (get_timer_masked() - start) < tmo)
		;
}

void reset_timer_masked(void)
{
	/* reset time */
	lastdec = READ_TIMER();
	timestamp = 0;
}

ulong get_timer_masked(void)
{
	ulong now = READ_TIMER();

	if (now >= lastdec) {
		/* normal mode */
		timestamp += now - lastdec;
	} else {
		/* we have an overflow ... */
		timestamp += now + GPT_FREE_RUNNING - lastdec;
	}
	lastdec = now;

	return timestamp;
}

void udelay_masked(unsigned long usec)
{
	return udelay(usec);
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
	return CONFIG_SPEAR_HZ;
}
