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
 * Copyright (C) 2007 Sascha Hauer, Pengutronix
 * Copyright (C) 2008,2009 Eric Jarrige <jorasse@users.sourceforge.net>
 * Copyright (C) 2009 Ilya Yanok <yanok@emcraft.com>
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
 *
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/imx-regs.h>

DECLARE_GLOBAL_DATA_PTR;

int board_init (void)
{
	struct gpio_regs *regs = (struct gpio_regs *)IMX_GPIO_BASE;
#if defined(CONFIG_SYS_NAND_LARGEPAGE)
	struct system_control_regs *sc_regs =
		(struct system_control_regs *)IMX_SYSTEM_CTL_BASE;
#endif

	gd->bd->bi_arch_number = MACH_TYPE_IMX27LITE;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

#ifdef CONFIG_MXC_UART
	mx27_uart_init_pins();
#endif
#ifdef CONFIG_FEC_MXC
	mx27_fec_init_pins();
	imx_gpio_mode((GPIO_PORTC | GPIO_OUT | GPIO_PUEN | GPIO_GPIO | 31));
	writel(readl(&regs->port[PORTC].dr) | (1 << 31),
				&regs->port[PORTC].dr);
#endif
#ifdef CONFIG_MXC_MMC
#if defined(CONFIG_MAGNESIUM)
	mx27_sd1_init_pins();
#else
	mx27_sd2_init_pins();
#endif
#endif

#if defined(CONFIG_SYS_NAND_LARGEPAGE)
	/*
	 * set in FMCR NF_FMS Bit(5) to 1
	 * (NAND Flash with 2 Kbyte page size)
	 */
	writel(readl(&sc_regs->fmcr) | (1 << 5), &sc_regs->fmcr);
#endif
	return 0;
}

int dram_init (void)
{

#if CONFIG_NR_DRAM_BANKS > 0
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = get_ram_size((volatile void *)PHYS_SDRAM_1,
			PHYS_SDRAM_1_SIZE);
#endif
#if CONFIG_NR_DRAM_BANKS > 1
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = get_ram_size((volatile void *)PHYS_SDRAM_2,
			PHYS_SDRAM_2_SIZE);
#endif

	return 0;
}

int checkboard(void)
{
	puts ("Board: ");
	puts(CONFIG_BOARDNAME);
	return 0;
}
