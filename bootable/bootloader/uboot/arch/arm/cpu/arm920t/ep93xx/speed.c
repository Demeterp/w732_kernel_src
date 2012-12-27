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
 * Cirrus Logic EP93xx PLL support.
 *
 * Copyright (C) 2009 Matthias Kaehlcke <matthias@kaehlcke.net>
 *
 * See file CREDITS for list of people who contributed to this project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <common.h>
#include <asm/arch/ep93xx.h>
#include <asm/io.h>
#include <div64.h>

/*
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_FCLK(), get_HCLK(), get_PCLK() and get_UCLK() return the clock of
 * the specified bus in HZ.
 */

/*
 * return the PLL output frequency
 *
 * PLL rate = CONFIG_SYS_CLK_FREQ * (X1FBD + 1) * (X2FBD + 1)
 * / (X2IPD + 1) / 2^PS
 */
static ulong get_PLLCLK(uint32_t *pllreg)
{
	uint8_t i;
	const uint32_t clkset = readl(pllreg);
	uint64_t rate = CONFIG_SYS_CLK_FREQ;
	rate *= ((clkset >> SYSCON_CLKSET_PLL_X1FBD1_SHIFT) & 0x1f) + 1;
	rate *= ((clkset >> SYSCON_CLKSET_PLL_X2FBD2_SHIFT) & 0x3f) + 1;
	do_div(rate, (clkset  & 0x1f) + 1);			/* X2IPD */
	for (i = 0; i < ((clkset >> SYSCON_CLKSET_PLL_PS_SHIFT) & 3); i++)
		rate >>= 1;

	return (ulong)rate;
}

/* return FCLK frequency */
ulong get_FCLK()
{
	const uint8_t fclk_divisors[] = { 1, 2, 4, 8, 16, 1, 1, 1 };
	struct syscon_regs *syscon = (struct syscon_regs *)SYSCON_BASE;

	const uint32_t clkset1 = readl(&syscon->clkset1);
	const uint8_t fclk_div =
		fclk_divisors[(clkset1 >> SYSCON_CLKSET1_FCLK_DIV_SHIFT) & 7];
	const ulong fclk_rate = get_PLLCLK(&syscon->clkset1) / fclk_div;

	return fclk_rate;
}

/* return HCLK frequency */
ulong get_HCLK(void)
{
	const uint8_t hclk_divisors[] = { 1, 2, 4, 5, 6, 8, 16, 32 };
	struct syscon_regs *syscon = (struct syscon_regs *)SYSCON_BASE;

	const uint32_t clkset1 = readl(&syscon->clkset1);
	const uint8_t hclk_div =
		hclk_divisors[(clkset1 >> SYSCON_CLKSET1_HCLK_DIV_SHIFT) & 7];
	const ulong hclk_rate = get_PLLCLK(&syscon->clkset1) / hclk_div;

	return hclk_rate;
}

/* return PCLK frequency */
ulong get_PCLK(void)
{
	const uint8_t pclk_divisors[] = { 1, 2, 4, 8 };
	struct syscon_regs *syscon = (struct syscon_regs *)SYSCON_BASE;

	const uint32_t clkset1 = readl(&syscon->clkset1);
	const uint8_t pclk_div =
		pclk_divisors[(clkset1 >> SYSCON_CLKSET1_PCLK_DIV_SHIFT) & 3];
	const ulong pclk_rate = get_HCLK() / pclk_div;

	return pclk_rate;
}

/* return UCLK frequency */
ulong get_UCLK(void)
{
	struct syscon_regs *syscon = (struct syscon_regs *)SYSCON_BASE;
	ulong uclk_rate;

	const uint32_t value = readl(&syscon->pwrcnt);
	if (value & SYSCON_PWRCNT_UART_BAUD)
		uclk_rate = CONFIG_SYS_CLK_FREQ;
	else
		uclk_rate = CONFIG_SYS_CLK_FREQ / 2;

	return uclk_rate;
}
