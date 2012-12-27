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
 * (C) Copyright 2003
 * Martin Winistoerfer, martinwinistoerfer@gmx.ch.
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
 * Foundation,
 */

/*
 * File:		cpu.c
 *
 * Discription:		Some cpu specific function for watchdog,
 *                      cpu version test, clock setting ...
 *
 */


#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <mpc5xx.h>

DECLARE_GLOBAL_DATA_PTR;

#if (defined(CONFIG_MPC555))
#  define	ID_STR	"MPC555/556"

/*
 * Check version of cpu with Processor Version Register (PVR)
 */
static int check_cpu_version (long clock, uint pvr, uint immr)
{
    char buf[32];
	/* The highest 16 bits should be 0x0002 for a MPC555/556 */
	if ((pvr >> 16) == 0x0002) {
		printf (" " ID_STR " Version %x", (pvr >> 16));
		printf (" at %s MHz:", strmhz (buf, clock));
	} else {
		printf ("Not supported cpu version");
		return -1;
	}
	return 0;
}
#endif /* CONFIG_MPC555 */


/*
 * Check version of mpc5xx
 */
int checkcpu (void)
{
	ulong clock = gd->cpu_clk;
	uint immr = get_immr (0);	/* Return full IMMR contents */
	uint pvr = get_pvr ();		/* Retrieve PVR register */

	puts ("CPU:   ");

	return check_cpu_version (clock, pvr, immr);
}

/*
 * Called by macro WATCHDOG_RESET
 */
#if defined(CONFIG_WATCHDOG)
void watchdog_reset (void)
{
	int re_enable = disable_interrupts ();

	reset_5xx_watchdog ((immap_t *) CONFIG_SYS_IMMR);
	if (re_enable)
		enable_interrupts ();
}

/*
 * Will clear software reset
 */
void reset_5xx_watchdog (volatile immap_t * immr)
{
	/* Use the MPC5xx Internal Watchdog */
	immr->im_siu_conf.sc_swsr = 0x556c;	/* Prevent SW time-out */
	immr->im_siu_conf.sc_swsr = 0xaa39;
}

#endif /* CONFIG_WATCHDOG */


/*
 * Get timebase clock frequency
 */
unsigned long get_tbclk (void)
{
	volatile immap_t *immr = (volatile immap_t *) CONFIG_SYS_IMMR;
	ulong oscclk, factor;

	if (immr->im_clkrst.car_sccr & SCCR_TBS) {
		return (gd->cpu_clk / 16);
	}

	factor = (((CONFIG_SYS_PLPRCR) & PLPRCR_MF_MSK) >> PLPRCR_MF_SHIFT) + 1;

	oscclk = gd->cpu_clk / factor;

	if ((immr->im_clkrst.car_sccr & SCCR_RTSEL) == 0 || factor > 2) {
		return (oscclk / 4);
	}
	return (oscclk / 16);
}

void dcache_enable (void)
{
	return;
}

void dcache_disable (void)
{
	return;
}

int dcache_status (void)
{
	return 0;	/* always off */
}

/*
 * Reset board
 */
int do_reset (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_PATI)
	volatile ulong *addr = (ulong *) CONFIG_SYS_RESET_ADDRESS;
	*addr = 1;
#else
	ulong addr;

	/* Interrupts off, enable reset */
	__asm__ volatile	("  mtspr	81, %r0		\n\t"
				 "  mfmsr	%r3		\n\t"
				 "  rlwinm	%r31,%r3,0,25,23\n\t"
				 "  mtmsr	%r31		\n\t");
	/*
	 * Trying to execute the next instruction at a non-existing address
	 * should cause a machine check, resulting in reset
	 */
#ifdef CONFIG_SYS_RESET_ADDRESS
	addr = CONFIG_SYS_RESET_ADDRESS;
#else
	/*
	 * note: when CONFIG_SYS_MONITOR_BASE points to a RAM address, CONFIG_SYS_MONITOR_BASE         * - sizeof (ulong) is usually a valid address. Better pick an address
	 * known to be invalid on your system and assign it to CONFIG_SYS_RESET_ADDRESS.
	 * "(ulong)-1" used to be a good choice for many systems...
	 */
	addr = CONFIG_SYS_MONITOR_BASE - sizeof (ulong);
#endif
	((void (*) (void)) addr) ();
#endif  /* #if defined(CONFIG_PATI) */
	return 1;
}
