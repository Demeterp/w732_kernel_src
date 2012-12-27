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
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
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
#include <netdev.h>
#include <asm/arch/s3c6400.h>

/* ------------------------------------------------------------------------- */
#define CS8900_Tacs	0x0	/* 0clk		address set-up		*/
#define CS8900_Tcos	0x4	/* 4clk		chip selection set-up	*/
#define CS8900_Tacc	0xE	/* 14clk	access cycle		*/
#define CS8900_Tcoh	0x1	/* 1clk		chip selection hold	*/
#define CS8900_Tah	0x4	/* 4clk		address holding time	*/
#define CS8900_Tacp	0x6	/* 6clk		page mode access cycle	*/
#define CS8900_PMC	0x0	/* normal(1data)page mode configuration	*/

static inline void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n"
			  "bne 1b"
			  : "=r" (loops) : "0" (loops));
}

/*
 * Miscellaneous platform dependent initialisations
 */

static void cs8900_pre_init(void)
{
	SROM_BW_REG &= ~(0xf << 4);
	SROM_BW_REG |= (1 << 7) | (1 << 6) | (1 << 4);
	SROM_BC1_REG = ((CS8900_Tacs << 28) + (CS8900_Tcos << 24) +
			(CS8900_Tacc << 16) + (CS8900_Tcoh << 12) +
			(CS8900_Tah << 8) + (CS8900_Tacp << 4) + CS8900_PMC);
}

int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	cs8900_pre_init();

	/* NOR-flash in SROM0 */

	/* Enable WAIT */
	SROM_BW_REG |= 4 | 8 | 1;

	gd->bd->bi_arch_number = MACH_TYPE;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:   SMDK6400\n");
	return 0;
}
#endif

#ifdef CONFIG_ENABLE_MMU
ulong virt_to_phy_smdk6400(ulong addr)
{
	if ((0xc0000000 <= addr) && (addr < 0xc8000000))
		return addr - 0xc0000000 + 0x50000000;
	else
		printf("do not support this address : %08lx\n", addr);

	return addr;
}
#endif

ulong board_flash_get_legacy (ulong base, int banknum, flash_info_t *info)
{
	if (banknum == 0) {	/* non-CFI boot flash */
		info->portwidth = FLASH_CFI_16BIT;
		info->chipwidth = FLASH_CFI_BY16;
		info->interface = FLASH_CFI_X16;
		return 1;
	} else
		return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_CS8900
	rc = cs8900_initialize(0, CONFIG_CS8900_BASE);
#endif
	return rc;
}
#endif
