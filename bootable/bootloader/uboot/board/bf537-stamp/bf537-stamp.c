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
 * U-boot - main board file
 *
 * Copyright (c) 2005-2008 Analog Devices Inc.
 *
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <asm/blackfin.h>
#include <asm/net.h>
#include <net.h>
#include <asm/mach-common/bits/bootrom.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	printf("Board: ADI BF537 stamp board\n");
	printf("       Support: http://blackfin.uclinux.org/\n");
	return 0;
}

void board_reset(void)
{
	/* workaround for weak pull ups on ssel */
	if (CONFIG_BFIN_BOOT_MODE == BFIN_BOOT_SPI_MASTER) {
		bfin_write_PORTF_FER(bfin_read_PORTF_FER() & ~PF10);
		bfin_write_PORTFIO_SET(PF10);
		udelay(1);
	}
}

#ifdef CONFIG_BFIN_MAC
static void board_init_enetaddr(uchar *mac_addr)
{
#ifdef CONFIG_SYS_NO_FLASH
# define USE_MAC_IN_FLASH 0
#else
# define USE_MAC_IN_FLASH 1
#endif
	bool valid_mac = false;

	if (USE_MAC_IN_FLASH) {
		/* we cram the MAC in the last flash sector */
		uchar *board_mac_addr = (uchar *)0x203F0000;
		if (is_valid_ether_addr(board_mac_addr)) {
			memcpy(mac_addr, board_mac_addr, 6);
			valid_mac = true;
		}
	}

	if (!valid_mac) {
		puts("Warning: Generating 'random' MAC address\n");
		bfin_gen_rand_mac(mac_addr);
	}

	eth_setenv_enetaddr("ethaddr", mac_addr);
}

int board_eth_init(bd_t *bis)
{
	return bfin_EMAC_initialize(bis);
}
#endif

/* miscellaneous platform dependent initialisations */
int misc_init_r(void)
{
#ifdef CONFIG_BFIN_MAC
	uchar enetaddr[6];
	if (!eth_getenv_enetaddr("ethaddr", enetaddr))
		board_init_enetaddr(enetaddr);
#endif

#ifndef CONFIG_SYS_NO_FLASH
	/* we use the last sector for the MAC address / POST LDR */
	extern flash_info_t flash_info[];
	flash_protect(FLAG_PROTECT_SET, 0x203F0000, 0x203FFFFF, &flash_info[0]);
#endif

#ifdef CONFIG_BFIN_IDE
	cf_ide_init();
#endif

	return 0;
}
