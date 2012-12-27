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
 * (C) Copyright 2006
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * (C) Copyright 2002
 * Kyle Harris, Nexus Technologies, Inc. kharris@nexus-tech.net
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
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
#include <command.h>
#include <malloc.h>
#include <netdev.h>
#include <asm/arch/ixp425.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Miscelaneous platform dependent initialisations
 */
int board_init (void)
{
	/* arch number of IXDP */
	gd->bd->bi_arch_number = MACH_TYPE_IXDP425;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x00000100;

#ifdef CONFIG_IXDPG425
	/* arch number of IXDP */
	gd->bd->bi_arch_number = MACH_TYPE_IXDPG425;

	/*
	 * Get realtek RTL8305 switch and SLIC out of reset
	 */
	GPIO_OUTPUT_SET(CONFIG_SYS_GPIO_SWITCH_RESET_N);
	GPIO_OUTPUT_ENABLE(CONFIG_SYS_GPIO_SWITCH_RESET_N);
	GPIO_OUTPUT_SET(CONFIG_SYS_GPIO_SLIC_RESET_N);
	GPIO_OUTPUT_ENABLE(CONFIG_SYS_GPIO_SLIC_RESET_N);

	/*
	 * Setup GPIO's for PCI INTA & INTB
	 */
	GPIO_OUTPUT_DISABLE(CONFIG_SYS_GPIO_PCI_INTA_N);
	GPIO_INT_ACT_LOW_SET(CONFIG_SYS_GPIO_PCI_INTA_N);
	GPIO_OUTPUT_DISABLE(CONFIG_SYS_GPIO_PCI_INTB_N);
	GPIO_INT_ACT_LOW_SET(CONFIG_SYS_GPIO_PCI_INTB_N);

	/*
	 * Setup GPIO's for 33MHz clock output
	 */
	*IXP425_GPIO_GPCLKR = 0x01FF01FF;
	GPIO_OUTPUT_ENABLE(CONFIG_SYS_GPIO_PCI_CLK);
	GPIO_OUTPUT_ENABLE(CONFIG_SYS_GPIO_EXTBUS_CLK);
#endif

	return 0;
}

/*
 * Check Board Identity
 */
int checkboard(void)
{
	char *s = getenv("serial#");

#ifdef CONFIG_IXDPG425
	puts("Board: IXDPG425 - Intel Network Gateway Reference Platform");
#else
	puts("Board: IXDP425 - Intel Development Platform");
#endif

	if (s != NULL) {
		puts(", serial# ");
		puts(s);
	}
	putc('\n');

	return (0);
}

int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size  = PHYS_SDRAM_1_SIZE;

	return (0);
}

#if defined(CONFIG_CMD_PCI) || defined(CONFIG_PCI)
extern struct pci_controller hose;
extern void pci_ixp_init(struct pci_controller * hose);

void pci_init_board(void)
{
	extern void pci_ixp_init (struct pci_controller *hose);

	pci_ixp_init(&hose);
}
#endif

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}
