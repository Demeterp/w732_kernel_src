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
 * SH7780 PCI Controller (PCIC) for U-Boot.
 * (C) Dustin McIntire (dustin@sensoria.com)
 * (C) 2007,2008 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 * (C) 2008 Yusuke Goda <goda.yusuke@renesas.com>
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

#include <pci.h>
#include <asm/processor.h>
#include <asm/pci.h>
#include <asm/io.h>

#define SH7780_VENDOR_ID	0x1912
#define SH7780_DEVICE_ID	0x0002
#define SH7780_PCICR_PREFIX	0xA5000000
#define SH7780_PCICR_PFCS	0x00000800
#define SH7780_PCICR_FTO	0x00000400
#define SH7780_PCICR_PFE	0x00000200
#define SH7780_PCICR_TBS	0x00000100
#define SH7780_PCICR_ARBM	0x00000040
#define SH7780_PCICR_IOCS	0x00000004
#define SH7780_PCICR_PRST	0x00000002
#define SH7780_PCICR_CFIN	0x00000001

#define p4_in(addr)			(*(vu_long *)addr)
#define p4_out(data, addr)	(*(vu_long *)addr) = (data)
#define p4_inw(addr)		(*(vu_short *)addr)
#define p4_outw(data, addr)	(*(vu_short *)addr) = (data)

int pci_sh4_read_config_dword(struct pci_controller *hose,
				    pci_dev_t dev, int offset, u32 *value)
{
	u32 par_data = 0x80000000 | dev;

	p4_out(par_data | (offset & 0xfc), SH7780_PCIPAR);
	*value = p4_in(SH7780_PCIPDR);

	return 0;
}

int pci_sh4_write_config_dword(struct pci_controller *hose,
				     pci_dev_t dev, int offset, u32 value)
{
	u32 par_data = 0x80000000 | dev;

	p4_out(par_data | (offset & 0xfc), SH7780_PCIPAR);
	p4_out(value, SH7780_PCIPDR);
	return 0;
}

int pci_sh7780_init(struct pci_controller *hose)
{
	p4_out(0x01, SH7780_PCIECR);

	if (p4_inw(SH7780_PCIVID) != SH7780_VENDOR_ID
	    && p4_inw(SH7780_PCIDID) != SH7780_DEVICE_ID) {
		printf("PCI: Unknown PCI host bridge.\n");
		return -1;
	}
	printf("PCI: SH7780 PCI host bridge found.\n");

	/* Toggle PCI reset pin */
	p4_out((SH7780_PCICR_PREFIX | SH7780_PCICR_PRST), SH7780_PCICR);
	udelay(100000);
	p4_out(SH7780_PCICR_PREFIX, SH7780_PCICR);
	p4_outw(0x0047, SH7780_PCICMD);

	p4_out(CONFIG_SH7780_PCI_LSR, SH7780_PCILSR0);
	p4_out(CONFIG_SH7780_PCI_LAR, SH7780_PCILAR0);
	p4_out(0x00000000, SH7780_PCILSR1);
	p4_out(0, SH7780_PCILAR1);
	p4_out(CONFIG_SH7780_PCI_BAR, SH7780_PCIMBAR0);
	p4_out(0x00000000, SH7780_PCIMBAR1);

	p4_out(0xFD000000, SH7780_PCIMBR0);
	p4_out(0x00FC0000, SH7780_PCIMBMR0);

	/* if use Operand Cache then enable PCICSCR Soonp bits. */
	p4_out(0x08000000, SH7780_PCICSAR0);
	p4_out(0x0000001B, SH7780_PCICSCR0);	/* Snoop bit :On */

	p4_out((SH7780_PCICR_PREFIX | SH7780_PCICR_CFIN | SH7780_PCICR_ARBM
	      | SH7780_PCICR_FTO | SH7780_PCICR_PFCS | SH7780_PCICR_PFE),
	     SH7780_PCICR);

	pci_sh4_init(hose);
	return 0;
}
