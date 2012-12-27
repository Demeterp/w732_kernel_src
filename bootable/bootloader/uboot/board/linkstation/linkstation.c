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
 * linkstation.c
 *
 * Misc LinkStation specific functions
 *
 * Copyright (C) 2006 Mihai Georgian <u-boot@linuxnotincluded.org.uk>
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

#include <version.h>
#include <common.h>
#include <mpc824x.h>
#include <asm/io.h>
#include <ns16550.h>
#include <netdev.h>

#ifdef CONFIG_PCI
#include <pci.h>
#endif

extern void init_AVR_DUART(void);

int checkboard (void)
{
	DECLARE_GLOBAL_DATA_PTR;
	char *p;
	bd_t *bd = gd->bd;

	init_AVR_DUART();

	if ((p = getenv ("console_nr")) != NULL) {
		unsigned long con_nr = simple_strtoul (p, NULL, 10) & 3;

		bd->bi_baudrate &= ~3;
		bd->bi_baudrate |= con_nr & 3;
	}
	return 0;
}

phys_size_t initdram (int board_type)
{
	return (get_ram_size(CONFIG_SYS_SDRAM_BASE, CONFIG_SYS_MAX_RAM_SIZE));
}

/*
 * Initialize PCI Devices
 */
#ifdef CONFIG_PCI

#ifndef CONFIG_PCI_PNP

static struct pci_config_table pci_linkstation_config_table[] = {
	/* vendor, device, class */
	/* bus, dev, func */
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x0b, 0,		/* AN983B or RTL8110S  */
								/* ethernet controller */
	  pci_cfgfunc_config_device, { PCI_ETH_IOADDR,
				       PCI_ETH_MEMADDR,
				       PCI_COMMAND_IO |
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x0c, 0,		/* SII680 or IT8211AF */
								/* ide controller     */
	  pci_cfgfunc_config_device, { PCI_IDE_IOADDR,
				       PCI_IDE_MEMADDR,
				       PCI_COMMAND_IO |
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x0e, 0,		/* D720101 USB controller, 1st USB 1.1 */
	  pci_cfgfunc_config_device, { PCI_USB0_IOADDR,
				       PCI_USB0_MEMADDR,
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x0e, 1,		/* D720101 USB controller, 2nd USB 1.1 */
	  pci_cfgfunc_config_device, { PCI_USB1_IOADDR,
				       PCI_USB1_MEMADDR,
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x0e, 2,		/* D720101 USB controller, USB 2.0 */
	  pci_cfgfunc_config_device, { PCI_USB2_IOADDR,
				       PCI_USB2_MEMADDR,
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ }
};
#endif

struct pci_controller hose = {
#ifndef CONFIG_PCI_PNP
	config_table:pci_linkstation_config_table,
#endif
};

void pci_init_board (void)
{
	pci_mpc824x_init (&hose);

	/* Reset USB 1.1 */
	/* Haven't seen any change without these on a HG, maybe it is
	 * needed on other models */
	out_le32((volatile unsigned*)(PCI_USB0_MEMADDR + 8), 1);
	out_le32((volatile unsigned*)(PCI_USB1_MEMADDR + 8), 1);
}
#endif /* CONFIG_PCI */

#define UART_DCR       0x80004511
int board_early_init_f (void)
{
	/* set DUART mode */
	out_8((volatile u8*)UART_DCR, 1);
	return 0;
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}
