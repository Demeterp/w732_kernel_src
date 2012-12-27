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
 * Copyright 2008 Extreme Engineering Solutions, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <asm/immap_85xx.h>
#include <asm/fsl_pci.h>
#include <asm/io.h>
#include <asm/cache.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <pca953x.h>

DECLARE_GLOBAL_DATA_PTR;

extern void ft_board_pci_setup(void *blob, bd_t *bd);

int checkboard(void)
{
	char *s;

	printf("Board: X-ES %s 3U VPX SBC\n", CONFIG_SYS_BOARD_NAME);
	printf("       ");
	s = getenv("board_rev");
	if (s)
		printf("Rev %s, ", s);
	s = getenv("serial#");
	if (s)
		printf("Serial# %s, ", s);
	s = getenv("board_cfg");
	if (s)
		printf("Cfg %s", s);
	printf("\n");

	return 0;
}

static void flash_cs_fixup(void)
{
	volatile ccsr_lbc_t *lbc = (void *)(CONFIG_SYS_MPC85xx_LBC_ADDR);
	int flash_sel;

	/*
	 * Print boot dev and swap flash flash chip selects if booted from 2nd
	 * flash.  Swapping chip selects presents user with a common memory
	 * map regardless of which flash was booted from.
	 */
	flash_sel = !((pca953x_get_val(CONFIG_SYS_I2C_PCA953X_ADDR0) &
			CONFIG_SYS_PCA953X_C0_FLASH_PASS_CS));
	printf("FLASH: Executed from FLASH%d\n", flash_sel ? 2 : 1);

	if (flash_sel) {
		out_be32(&lbc->br0, CONFIG_SYS_BR1_PRELIM);
		out_be32(&lbc->or0, CONFIG_SYS_OR1_PRELIM);

		out_be32(&lbc->br1, CONFIG_SYS_BR0_PRELIM);
		out_be32(&lbc->or1, CONFIG_SYS_OR0_PRELIM);
	}
}

int board_early_init_r(void)
{
	/* Initialize PCA9557 devices */
	pca953x_set_pol(CONFIG_SYS_I2C_PCA953X_ADDR0, 0xff, 0);
	pca953x_set_pol(CONFIG_SYS_I2C_PCA953X_ADDR1, 0xff, 0);
	pca953x_set_pol(CONFIG_SYS_I2C_PCA953X_ADDR2, 0xff, 0);
	pca953x_set_pol(CONFIG_SYS_I2C_PCA953X_ADDR3, 0xff, 0);

	/*
	 * Remap NOR flash region to caching-inhibited
	 * so that flash can be erased/programmed properly.
	 */

	/* Flush d-cache and invalidate i-cache of any FLASH data */
	flush_dcache();
	invalidate_icache();

	/* Invalidate existing TLB entry for NOR flash */
	disable_tlb(0);
	set_tlb(1, (CONFIG_SYS_FLASH_BASE2 & 0xf0000000),
		(CONFIG_SYS_FLASH_BASE2 & 0xf0000000),
		MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		0, 0, BOOKE_PAGESZ_256M, 1);

	flash_cs_fixup();

	return 0;
}

#if defined(CONFIG_OF_BOARD_SETUP)
void ft_board_setup(void *blob, bd_t *bd)
{
#ifdef CONFIG_PCI
	ft_board_pci_setup(blob, bd);
#endif
	ft_cpu_setup(blob, bd);
}
#endif

#ifdef CONFIG_MP
extern void cpu_mp_lmb_reserve(struct lmb *lmb);

void board_lmb_reserve(struct lmb *lmb)
{
	cpu_mp_lmb_reserve(lmb);
}
#endif
