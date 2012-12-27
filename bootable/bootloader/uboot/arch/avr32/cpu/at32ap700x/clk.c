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
 * Copyright (C) 2005-2008 Atmel Corporation
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

#include <asm/arch/clk.h>
#include <asm/arch/memory-map.h>
#include <asm/arch/portmux.h>

#include "sm.h"

void clk_init(void)
{
	uint32_t cksel;

	/* in case of soft resets, disable watchdog */
	sm_writel(WDT_CTRL, SM_BF(KEY, 0x55));
	sm_writel(WDT_CTRL, SM_BF(KEY, 0xaa));

#ifdef CONFIG_PLL
	/* Initialize the PLL */
	sm_writel(PM_PLL0, (SM_BF(PLLCOUNT, CONFIG_SYS_PLL0_SUPPRESS_CYCLES)
			    | SM_BF(PLLMUL, CONFIG_SYS_PLL0_MUL - 1)
			    | SM_BF(PLLDIV, CONFIG_SYS_PLL0_DIV - 1)
			    | SM_BF(PLLOPT, CONFIG_SYS_PLL0_OPT)
			    | SM_BF(PLLOSC, 0)
			    | SM_BIT(PLLEN)));

	/* Wait for lock */
	while (!(sm_readl(PM_ISR) & SM_BIT(LOCK0))) ;
#endif

	/* Set up clocks for the CPU and all peripheral buses */
	cksel = 0;
	if (CONFIG_SYS_CLKDIV_CPU)
		cksel |= SM_BIT(CPUDIV) | SM_BF(CPUSEL, CONFIG_SYS_CLKDIV_CPU - 1);
	if (CONFIG_SYS_CLKDIV_HSB)
		cksel |= SM_BIT(HSBDIV) | SM_BF(HSBSEL, CONFIG_SYS_CLKDIV_HSB - 1);
	if (CONFIG_SYS_CLKDIV_PBA)
		cksel |= SM_BIT(PBADIV) | SM_BF(PBASEL, CONFIG_SYS_CLKDIV_PBA - 1);
	if (CONFIG_SYS_CLKDIV_PBB)
		cksel |= SM_BIT(PBBDIV) | SM_BF(PBBSEL, CONFIG_SYS_CLKDIV_PBB - 1);
	sm_writel(PM_CKSEL, cksel);

#ifdef CONFIG_PLL
	/* Use PLL0 as main clock */
	sm_writel(PM_MCCTRL, SM_BIT(PLLSEL));

#ifdef CONFIG_LCD
	/* Set up pixel clock for the LCDC */
	sm_writel(PM_GCCTRL(7), SM_BIT(PLLSEL) | SM_BIT(CEN));
#endif
#endif
}

unsigned long __gclk_set_rate(unsigned int id, enum gclk_parent parent,
		unsigned long rate, unsigned long parent_rate)
{
	unsigned long divider;

	if (rate == 0 || parent_rate == 0) {
		sm_writel(PM_GCCTRL(id), 0);
		return 0;
	}

	divider = (parent_rate + rate / 2) / rate;
	if (divider <= 1) {
		sm_writel(PM_GCCTRL(id), parent | SM_BIT(CEN));
		rate = parent_rate;
	} else {
		divider = min(255, divider / 2 - 1);
		sm_writel(PM_GCCTRL(id), parent | SM_BIT(CEN) | SM_BIT(DIVEN)
				| SM_BF(DIV, divider));
		rate = parent_rate / (2 * (divider + 1));
	}

	return rate;
}
