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
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004-2008 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
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
#include <spi.h>
#include <asm/immap.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	/*
	 * need to to:
	 * Check serial flash size. if 2mb evb, else 8mb demo
	 */
	puts("Board: ");
	puts("Freescale M54451 EVB\n");
	return 0;
};

phys_size_t initdram(int board_type)
{
	u32 dramsize;
#ifdef CONFIG_CF_SBF
	/*
	 * Serial Boot: The dram is already initialized in start.S
	 * only require to return DRAM size
	 */
	dramsize = CONFIG_SYS_SDRAM_SIZE * 0x100000;
#else
	volatile sdramc_t *sdram = (volatile sdramc_t *)(MMAP_SDRAM);
	volatile gpio_t *gpio = (volatile gpio_t *)(MMAP_GPIO);
	u32 i;

	dramsize = CONFIG_SYS_SDRAM_SIZE * 0x100000;

	if ((sdram->sdcfg1 == CONFIG_SYS_SDRAM_CFG1) &&
	    (sdram->sdcfg2 == CONFIG_SYS_SDRAM_CFG2))
		return dramsize;

	for (i = 0x13; i < 0x20; i++) {
		if (dramsize == (1 << i))
			break;
	}
	i--;

	gpio->mscr_sdram = CONFIG_SYS_SDRAM_DRV_STRENGTH;

	sdram->sdcs0 = (CONFIG_SYS_SDRAM_BASE | i);

	sdram->sdcfg1 = CONFIG_SYS_SDRAM_CFG1;
	sdram->sdcfg2 = CONFIG_SYS_SDRAM_CFG2;

	udelay(200);

	/* Issue PALL */
	sdram->sdcr = CONFIG_SYS_SDRAM_CTRL | 2;
	__asm__("nop");

	/* Perform two refresh cycles */
	sdram->sdcr = CONFIG_SYS_SDRAM_CTRL | 4;
	__asm__("nop");
	sdram->sdcr = CONFIG_SYS_SDRAM_CTRL | 4;
	__asm__("nop");

	/* Issue LEMR */
	sdram->sdmr = CONFIG_SYS_SDRAM_MODE;
	__asm__("nop");
	sdram->sdmr = CONFIG_SYS_SDRAM_EMOD;
	__asm__("nop");

	sdram->sdcr = (CONFIG_SYS_SDRAM_CTRL & ~0x80000000) | 0x10000000;

	udelay(100);
#endif
	return (dramsize);
};

int testdram(void)
{
	/* TODO: XXX XXX XXX */
	printf("DRAM test not implemented!\n");

	return (0);
}
