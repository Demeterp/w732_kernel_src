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
 * Copyright (C) 2005-2006 Atmel Corporation
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
#include <command.h>

#include <asm/io.h>
#include <asm/sections.h>
#include <asm/sysreg.h>

#include <asm/arch/clk.h>
#include <asm/arch/memory-map.h>

#include "hsmc3.h"

/* Sanity checks */
#if (CONFIG_SYS_CLKDIV_CPU > CONFIG_SYS_CLKDIV_HSB)		\
	|| (CONFIG_SYS_CLKDIV_HSB > CONFIG_SYS_CLKDIV_PBA)	\
	|| (CONFIG_SYS_CLKDIV_HSB > CONFIG_SYS_CLKDIV_PBB)
# error Constraint fCPU >= fHSB >= fPB{A,B} violated
#endif
#if defined(CONFIG_PLL) && ((CONFIG_SYS_PLL0_MUL < 1) || (CONFIG_SYS_PLL0_DIV < 1))
# error Invalid PLL multiplier and/or divider
#endif

DECLARE_GLOBAL_DATA_PTR;

int cpu_init(void)
{
	extern void _evba(void);

	gd->cpu_hz = CONFIG_SYS_OSC0_HZ;

	/* TODO: Move somewhere else, but needs to be run before we
	 * increase the clock frequency. */
	hsmc3_writel(MODE0, 0x00031103);
	hsmc3_writel(CYCLE0, 0x000c000d);
	hsmc3_writel(PULSE0, 0x0b0a0906);
	hsmc3_writel(SETUP0, 0x00010002);

	clk_init();

	/* Update the CPU speed according to the PLL configuration */
	gd->cpu_hz = get_cpu_clk_rate();

	/* Set up the exception handler table and enable exceptions */
	sysreg_write(EVBA, (unsigned long)&_evba);
	asm volatile("csrf	%0" : : "i"(SYSREG_EM_OFFSET));

	return 0;
}

void prepare_to_boot(void)
{
	/* Flush both caches and the write buffer */
	asm volatile("cache  %0[4], 010\n\t"
		     "cache  %0[0], 000\n\t"
		     "sync   0" : : "r"(0) : "memory");
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	/* This will reset the CPU core, caches, MMU and all internal busses */
	__builtin_mtdr(8, 1 << 13);	/* set DC:DBE */
	__builtin_mtdr(8, 1 << 30);	/* set DC:RES */

	/* Flush the pipeline before we declare it a failure */
	asm volatile("sub   pc, pc, -4");

	return -1;
}
