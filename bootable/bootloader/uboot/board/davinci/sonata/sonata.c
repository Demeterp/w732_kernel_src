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
 * Copyright (C) 2007 Sergey Kubushyn <ksi@koi8.net>
 *
 * Parts are shamelessly stolen from various TI sources, original copyright
 * follows:
 * -----------------------------------------------------------------
 *
 * Copyright (C) 2004 Texas Instruments.
 *
 * ----------------------------------------------------------------------------
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------
 */

#include <common.h>
#include <nand.h>
#include <asm/arch/nand_defs.h>
#include <asm/arch/hardware.h>
#include "../common/misc.h"

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* arch number of the board */
	gd->bd->bi_arch_number = MACH_TYPE_SONATA;

	/* address of boot parameters */
	gd->bd->bi_boot_params = LINUX_BOOT_PARAM_ADDR;

	/* Configure AEMIF pins (although this should be configured at boot time
	 * with pull-up/pull-down resistors) */
	REG(PINMUX0) = 0x00000c1f;

	davinci_errata_workarounds();

	/* Power on required peripherals */
	lpsc_on(DAVINCI_LPSC_GPIO);

#if !defined(CONFIG_SYS_USE_DSPLINK)
	/* Powerup the DSP */
	dsp_on();
#endif /* CONFIG_SYS_USE_DSPLINK */

	davinci_enable_uart0();
	davinci_enable_emac();
	davinci_enable_i2c();

	lpsc_on(DAVINCI_LPSC_TIMER1);
	timer_init();

	return(0);
}

int misc_init_r(void)
{
	uint8_t eeprom_enetaddr[6];

	/* Read Ethernet MAC address from EEPROM if available. */
	if (dvevm_read_mac_address(eeprom_enetaddr))
		dv_configure_mac_address(eeprom_enetaddr);

	return(0);
}

#ifdef CONFIG_NAND_DAVINCI

/* Set WP on deselect, write enable on select */
static void nand_sonata_select_chip(struct mtd_info *mtd, int chip)
{
#define GPIO_SET_DATA01	0x01c67018
#define GPIO_CLR_DATA01	0x01c6701c
#define GPIO_NAND_WP	(1 << 4)
#ifdef SONATA_BOARD_GPIOWP
	if (chip < 0) {
		REG(GPIO_CLR_DATA01) |= GPIO_NAND_WP;
	} else {
		REG(GPIO_SET_DATA01) |= GPIO_NAND_WP;
	}
#endif
}

int board_nand_init(struct nand_chip *nand)
{
	davinci_nand_init(nand);
	nand->select_chip = nand_sonata_select_chip;
	return 0;
}

#endif /* CONFIG_NAND_DAVINCI */
