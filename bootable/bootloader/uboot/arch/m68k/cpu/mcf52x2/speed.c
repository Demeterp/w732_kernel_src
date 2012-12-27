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
 * Josef Baumgartner <josef.baumgartner@telex.de>
 *
 * Copyright (C) 2004-2007 Freescale Semiconductor, Inc.
 * Hayden Fraser (Hayden.Fraser@freescale.com)
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
#include <asm/immap.h>

DECLARE_GLOBAL_DATA_PTR;

/* get_clocks() fills in gd->cpu_clock and gd->bus_clk */
int get_clocks (void)
{
#if defined(CONFIG_M5208)
	volatile pll_t *pll = (pll_t *) MMAP_PLL;

	pll->odr = CONFIG_SYS_PLL_ODR;
	pll->fdr = CONFIG_SYS_PLL_FDR;
#endif

#if defined(CONFIG_M5249) || defined(CONFIG_M5253)
	volatile unsigned long cpll = mbar2_readLong(MCFSIM_PLLCR);
	unsigned long pllcr;

#ifndef CONFIG_SYS_PLL_BYPASS

#ifdef CONFIG_M5249
	/* Setup the PLL to run at the specified speed */
#ifdef CONFIG_SYS_FAST_CLK
	pllcr = 0x925a3100;	/* ~140MHz clock (PLL bypass = 0) */
#else
	pllcr = 0x135a4140;	/* ~72MHz clock (PLL bypass = 0) */
#endif
#endif				/* CONFIG_M5249 */

#ifdef CONFIG_M5253
	pllcr = CONFIG_SYS_PLLCR;
#endif				/* CONFIG_M5253 */

	cpll = cpll & 0xfffffffe;	/* Set PLL bypass mode = 0 (PSTCLK = crystal) */
	mbar2_writeLong(MCFSIM_PLLCR, cpll);	/* Set the PLL to bypass mode (PSTCLK = crystal) */
	mbar2_writeLong(MCFSIM_PLLCR, pllcr);	/* set the clock speed */
	pllcr ^= 0x00000001;	/* Set pll bypass to 1 */
	mbar2_writeLong(MCFSIM_PLLCR, pllcr);	/* Start locking (pll bypass = 1) */
	udelay(0x20);		/* Wait for a lock ... */
#endif				/* #ifndef CONFIG_SYS_PLL_BYPASS */

#endif				/* CONFIG_M5249 || CONFIG_M5253 */

#if defined(CONFIG_M5275)
	volatile pll_t *pll = (volatile pll_t *)(MMAP_PLL);

	/* Setup PLL */
	pll->syncr = 0x01080000;
	while (!(pll->synsr & FMPLL_SYNSR_LOCK))
		;
	pll->syncr = 0x01000000;
	while (!(pll->synsr & FMPLL_SYNSR_LOCK))
		;
#endif

	gd->cpu_clk = CONFIG_SYS_CLK;
#if defined(CONFIG_M5208) || defined(CONFIG_M5249) || defined(CONFIG_M5253) || \
    defined(CONFIG_M5271) || defined(CONFIG_M5275)
	gd->bus_clk = gd->cpu_clk / 2;
#else
	gd->bus_clk = gd->cpu_clk;
#endif

#ifdef CONFIG_FSL_I2C
	gd->i2c1_clk = gd->bus_clk;
#ifdef CONFIG_SYS_I2C2_OFFSET
	gd->i2c2_clk = gd->bus_clk;
#endif
#endif

	return (0);
}
