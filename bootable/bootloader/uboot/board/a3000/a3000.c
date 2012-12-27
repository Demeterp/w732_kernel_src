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
 * (C) Copyright 2001
 * Rob Taylor, Flying Pig Systems. robt@flyingpig.com.
 *
 * Modified during 2003 by
 * Ken Chou, kchou@ieee.org
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
#include <mpc824x.h>
#include <pci.h>
#include <netdev.h>

int checkboard (void)
{
	ulong busfreq  = get_bus_freq(0);
	char  buf[32];

	printf("Board: A3000 Local Bus at %s MHz\n", strmhz(buf, busfreq));
	return 0;

}

phys_size_t initdram (int board_type)
{
	long size;
	long new_bank0_end;
	long mear1;
	long emear1;

	size = get_ram_size(CONFIG_SYS_SDRAM_BASE, CONFIG_SYS_MAX_RAM_SIZE);

	new_bank0_end = size - 1;
	mear1 = mpc824x_mpc107_getreg(MEAR1);
	emear1 = mpc824x_mpc107_getreg(EMEAR1);
	mear1 = (mear1  & 0xFFFFFF00) |
		((new_bank0_end & MICR_ADDR_MASK) >> MICR_ADDR_SHIFT);
	emear1 = (emear1 & 0xFFFFFF00) |
		((new_bank0_end & MICR_ADDR_MASK) >> MICR_EADDR_SHIFT);
	mpc824x_mpc107_setreg(MEAR1, mear1);
	mpc824x_mpc107_setreg(EMEAR1, emear1);

	return (size);
}

/*
 * Initialize PCI Devices
 */
#ifndef CONFIG_PCI_PNP
static struct pci_config_table pci_a3000_config_table[] = {
	/* vendor, device, class */
	/* bus, dev, func */
	{ PCI_VENDOR_ID_NS, PCI_DEVICE_ID_NS_83815, PCI_ANY_ID,
	  PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,	/* dp83815 eth0 divice */
	  pci_cfgfunc_config_device, { PCI_ENET0_IOADDR,
				       PCI_ENET0_MEMADDR,
				       PCI_COMMAND_IO |
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x14, PCI_ANY_ID,		/* PCI slot1 */
	  pci_cfgfunc_config_device, { PCI_ENET1_IOADDR,
				       PCI_ENET1_MEMADDR,
				       PCI_COMMAND_IO |
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x15, PCI_ANY_ID,		/* PCI slot2 */
	  pci_cfgfunc_config_device, { PCI_ENET2_IOADDR,
				       PCI_ENET2_MEMADDR,
				       PCI_COMMAND_IO |
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_ANY_ID, 0x16, PCI_ANY_ID,		/* PCI slot3 */
	  pci_cfgfunc_config_device, { PCI_ENET3_IOADDR,
				       PCI_ENET3_MEMADDR,
				       PCI_COMMAND_IO |
				       PCI_COMMAND_MEMORY |
				       PCI_COMMAND_MASTER }},
	{ }
};
#endif

struct pci_controller hose = {
#ifndef CONFIG_PCI_PNP
	config_table: pci_a3000_config_table,
#endif
};

void pci_init_board(void)
{
	pci_mpc824x_init(&hose);
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}
