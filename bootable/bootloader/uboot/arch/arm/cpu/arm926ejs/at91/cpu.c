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
 * Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#ifdef CONFIG_AT91_LEGACY
#warning Your board is using legacy SoC access. Please update!
#endif

#include <asm/arch/hardware.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/clk.h>
#include <asm/arch/io.h>

#ifndef CONFIG_SYS_AT91_MAIN_CLOCK
#define CONFIG_SYS_AT91_MAIN_CLOCK 0
#endif

/*
 * The at91sam9260 has 4 GPBR (0-3), we'll use the last one, nr 3,
 * to keep track of the bootcount.
 */
#define AT91_GPBR_BOOTCOUNT_REGISTER 3
#define AT91_BOOTCOUNT_ADDRESS (AT91_GPBR + 4*AT91_GPBR_BOOTCOUNT_REGISTER)

int arch_cpu_init(void)
{
	return at91_clock_init(CONFIG_SYS_AT91_MAIN_CLOCK);
}

#if defined(CONFIG_DISPLAY_CPUINFO)
int print_cpuinfo(void)
{
	char buf[32];

	printf("CPU: %s\n", CONFIG_SYS_AT91_CPU_NAME);
	printf("Crystal frequency: %8s MHz\n",
					strmhz(buf, get_main_clk_rate()));
	printf("CPU clock        : %8s MHz\n",
					strmhz(buf, get_cpu_clk_rate()));
	printf("Master clock     : %8s MHz\n",
					strmhz(buf, get_mck_clk_rate()));

	return 0;
}
#endif

#ifdef CONFIG_BOOTCOUNT_LIMIT
/*
 * Just as the mpc5xxx, we combine the BOOTCOUNT_MAGIC and boocount
 * in one 32-bit register. This is done, as the AT91SAM9260 only has
 * 4 GPBR.
 */
void bootcount_store (ulong a)
{
	volatile ulong *save_addr =
		(volatile ulong *)(AT91_BASE_SYS + AT91_BOOTCOUNT_ADDRESS);

	*save_addr = (BOOTCOUNT_MAGIC & 0xffff0000) | (a & 0x0000ffff);
}

ulong bootcount_load (void)
{
	volatile ulong *save_addr =
		(volatile ulong *)(AT91_BASE_SYS + AT91_BOOTCOUNT_ADDRESS);

	if ((*save_addr & 0xffff0000) != (BOOTCOUNT_MAGIC & 0xffff0000))
		return 0;
	else
		return (*save_addr & 0x0000ffff);
}

#endif /* CONFIG_BOOTCOUNT_LIMIT */
