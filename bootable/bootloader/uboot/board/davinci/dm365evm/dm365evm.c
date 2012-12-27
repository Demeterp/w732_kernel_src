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
 * Copyright (C) 2009 Texas Instruments Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/emif_defs.h>
#include <asm/arch/nand_defs.h>
#include <asm/arch/gpio_defs.h>
#include <netdev.h>
#include "../common/misc.h"

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_DAVINCI_DM365_EVM;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

#ifdef CONFIG_DRIVER_TI_EMAC
int board_eth_init(bd_t *bis)
{
	uint8_t eeprom_enetaddr[6];
	int i;
	struct davinci_gpio *gpio1_base =
			(struct davinci_gpio *)DAVINCI_GPIO_BANK01;

	/* Configure PINMUX 3 to enable EMAC pins */
	writel((readl(PINMUX3) | 0x1affff), PINMUX3);

	/* Configure GPIO20 as output */
	writel((readl(&gpio1_base->dir) & ~(1 << 20)), &gpio1_base->dir);

	/* Toggle GPIO 20 */
	for (i = 0; i < 20; i++) {
		/* GPIO 20 low */
		writel((readl(&gpio1_base->out_data) & ~(1 << 20)),
						&gpio1_base->out_data);

		udelay(1000);

		/* GPIO 20 high */
		writel((readl(&gpio1_base->out_data) | (1 << 20)),
						&gpio1_base->out_data);
	}

	/* Configure I2C pins so that EEPROM can be read */
	writel((readl(PINMUX3) | 0x01400000), PINMUX3);

	/* Read Ethernet MAC address from EEPROM */
	if (dvevm_read_mac_address(eeprom_enetaddr))
		dv_configure_mac_address(eeprom_enetaddr);

	davinci_emac_initialize();

	return 0;
}
#endif

#ifdef CONFIG_NAND_DAVINCI
static void nand_dm365evm_select_chip(struct mtd_info *mtd, int chip)
{
	struct nand_chip	*this = mtd->priv;
	unsigned long		wbase = (unsigned long) this->IO_ADDR_W;
	unsigned long		rbase = (unsigned long) this->IO_ADDR_R;

	if (chip == 1) {
		__set_bit(14, &wbase);
		__set_bit(14, &rbase);
	} else {
		__clear_bit(14, &wbase);
		__clear_bit(14, &rbase);
	}
	this->IO_ADDR_W = (void *)wbase;
	this->IO_ADDR_R = (void *)rbase;
}

int board_nand_init(struct nand_chip *nand)
{
	davinci_nand_init(nand);
	nand->select_chip = nand_dm365evm_select_chip;
	return 0;
}
#endif
