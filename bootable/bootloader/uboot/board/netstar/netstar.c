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
 * (C) Copyright 2005 2N TELEKOMUNIKACE, Ladislav Michl
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
#include <i2c.h>
#include <flash.h>
#include <nand.h>

#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* arch number of NetStar board */
	gd->bd->bi_arch_number = MACH_TYPE_NETSTAR;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x10000100;

	return 0;
}

int dram_init(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	/* Take the Ethernet controller out of reset and wait
	 * for the EEPROM load to complete. */
	*((volatile unsigned short *) GPIO_DATA_OUTPUT_REG) |= 0x80;
	udelay(10);	/* doesn't work before timer_init call */
	*((volatile unsigned short *) GPIO_DATA_OUTPUT_REG) &= ~0x80;
	udelay(500);

	return 0;
}

int misc_init_r(void)
{
#if defined(CONFIG_RTC_DS1307)
	/* enable trickle charge */
	i2c_reg_write(CONFIG_SYS_I2C_RTC_ADDR, 0x10, 0xaa);
#endif
	return 0;
}

int board_late_init(void)
{
	return 0;
}

#if defined(CONFIG_CMD_FLASH)
ulong board_flash_get_legacy(ulong base, int banknum, flash_info_t * info)
{
	if (banknum == 0) {	/* AM29LV800 boot flash */
		info->portwidth = FLASH_CFI_16BIT;
		info->chipwidth = FLASH_CFI_BY16;
		info->interface = FLASH_CFI_X16;
		return 1;
	}

	return 0;
}
#endif

#if defined(CONFIG_CMD_NAND)
/*
 *	hardware specific access to control-lines
 *
 *	NAND_NCE: bit 0 - don't care
 *	NAND_CLE: bit 1 -> bit 1  (0x0002)
 *	NAND_ALE: bit 2 -> bit 2  (0x0004)
 */
static void netstar_nand_hwcontrol(struct mtd_info *mtd, int cmd,
	unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;
	unsigned long mask;

	if (cmd == NAND_CMD_NONE)
		return;

	mask = (ctrl & NAND_CLE) ? 0x02 : 0;
	if (ctrl & NAND_ALE)
		mask |= 0x04;
	writeb(cmd, (unsigned long)chip->IO_ADDR_W | mask);
}

int board_nand_init(struct nand_chip *nand)
{
	nand->options = NAND_SAMSUNG_LP_OPTIONS;
	nand->ecc.mode = NAND_ECC_SOFT;
	nand->cmd_ctrl = netstar_nand_hwcontrol;
	nand->chip_delay = 400;
	return 0;
}
#endif

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC91111
	rc = smc91111_initialize(0, CONFIG_SMC91111_BASE);
#endif
	return rc;
}
#endif
