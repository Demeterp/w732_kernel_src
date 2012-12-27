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
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
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

/* This code should work for both the S3C2400 and the S3C2410
 * as they seem to have the same PLL and clock machinery inside.
 * The different address mapping is handled by the s3c24xx.h files below.
 */

#include <common.h>
#ifdef CONFIG_S3C24X0

#include <asm/io.h>
#include <asm/arch/s3c24x0_cpu.h>

#define MPLL 0
#define UPLL 1

/* ------------------------------------------------------------------------- */
/* NOTE: This describes the proper use of this file.
 *
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_FCLK(), get_HCLK(), get_PCLK() and get_UCLK() return the clock of
 * the specified bus in HZ.
 */
/* ------------------------------------------------------------------------- */

static ulong get_PLLCLK(int pllreg)
{
	struct s3c24x0_clock_power *clk_power = s3c24x0_get_base_clock_power();
	ulong r, m, p, s;

	if (pllreg == MPLL)
		r = readl(&clk_power->MPLLCON);
	else if (pllreg == UPLL)
		r = readl(&clk_power->UPLLCON);
	else
		hang();

	m = ((r & 0xFF000) >> 12) + 8;
	p = ((r & 0x003F0) >> 4) + 2;
	s = r & 0x3;

	return (CONFIG_SYS_CLK_FREQ * m) / (p << s);
}

/* return FCLK frequency */
ulong get_FCLK(void)
{
	return get_PLLCLK(MPLL);
}

/* return HCLK frequency */
ulong get_HCLK(void)
{
	struct s3c24x0_clock_power *clk_power = s3c24x0_get_base_clock_power();

	return (readl(&clk_power->CLKDIVN) & 2) ? get_FCLK() / 2 : get_FCLK();
}

/* return PCLK frequency */
ulong get_PCLK(void)
{
	struct s3c24x0_clock_power *clk_power = s3c24x0_get_base_clock_power();

	return (readl(&clk_power->CLKDIVN) & 1) ? get_HCLK() / 2 : get_HCLK();
}

/* return UCLK frequency */
ulong get_UCLK(void)
{
	return get_PLLCLK(UPLL);
}

#endif /* CONFIG_S3C24X0 */
