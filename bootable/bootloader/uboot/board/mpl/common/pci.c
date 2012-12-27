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

/*-----------------------------------------------------------------------------+
|       This source code is dual-licensed.  You may use it under the terms of
|       the GNU General Public License version 2, or under the license below.
|
|       This source code has been made available to you by IBM on an AS-IS
|       basis.  Anyone receiving this source is licensed under IBM
|       copyrights to use it in any way he or she deems fit, including
|       copying it, modifying it, compiling it, and redistributing it either
|       with or without modifications.  No license under IBM patents or
|       patent applications is to be implied by the copyright license.
|
|       Any user of this software should understand that IBM cannot provide
|       technical support for this software and will not be responsible for
|       any consequences resulting from the use of this software.
|
|       Any person who transfers this source code or any derivative work
|       must include the IBM copyright notice, this paragraph, and the
|       preceding two paragraphs in the transferred software.
|
|       COPYRIGHT   I B M   CORPORATION 1995
|       LICENSED MATERIAL  -  PROGRAM PROPERTY OF I B M
+-----------------------------------------------------------------------------*/
/*
 * Adapted for PIP405 03.07.01
 * Denis Peter, MPL AG Switzerland, d.peter@mpl.ch
 *
 * TODO: Clean-up
 */

#include <common.h>
#include <pci.h>
#include "isa.h"

#ifdef CONFIG_405GP
#ifdef CONFIG_PCI

DECLARE_GLOBAL_DATA_PTR;

#include "piix4_pci.h"
#include "pci_parts.h"

void pci_pip405_write_regs(struct pci_controller *hose, pci_dev_t dev,
			   struct pci_config_table *entry)
{
	struct pci_pip405_config_entry *table;
	int i;

	table = (struct pci_pip405_config_entry*) entry->priv[0];

	for (i=0; table[i].width; i++)
	{
#ifdef DEBUG
		printf("Reg 0x%02X Value 0x%08lX Width %02d written\n",
		       table[i].index, table[i].val, table[i].width);
#endif

		switch(table[i].width)
		{
		case 1: pci_hose_write_config_byte(hose, dev, table[i].index, table[i].val); break;
		case 2: pci_hose_write_config_word(hose, dev, table[i].index, table[i].val); break;
		case 4: pci_hose_write_config_dword(hose, dev, table[i].index, table[i].val); break;
		}
	}
}


static void pci_pip405_fixup_irq(struct pci_controller *hose, pci_dev_t dev)
{
	unsigned char int_line = 0xff;
	unsigned char pin;
	/*
	 * Write pci interrupt line register
	 */
	if(PCI_DEV(dev)==0) /* Device0 = PPC405 -> skip */
		return;
	pci_hose_read_config_byte(hose, dev, PCI_INTERRUPT_PIN, &pin);
	if ((pin == 0) || (pin > 4))
	    return;

	int_line = ((PCI_DEV(dev) + (pin-1) + 10) % 4) + 28;
	pci_hose_write_config_byte(hose, dev, PCI_INTERRUPT_LINE, int_line);
#ifdef DEBUG
	printf("Fixup IRQ: dev %d (%x) int line %d 0x%x\n",
	       PCI_DEV(dev),dev,int_line,int_line);
#endif
}

extern void pci_405gp_init(struct pci_controller *hose);


static struct pci_controller hose = {
  config_table: pci_pip405_config_table,
  fixup_irq: pci_pip405_fixup_irq,
};


void pci_init_board(void)
{
	/*we want the ptrs to RAM not flash (ie don't use init list)*/
	hose.fixup_irq    = pci_pip405_fixup_irq;
	hose.config_table = pci_pip405_config_table;
#ifdef DEBUG
	printf("Init PCI: fixup_irq=%p config_table=%p hose=%p\n",pci_pip405_fixup_irq,pci_pip405_config_table,hose);
#endif
	pci_405gp_init(&hose);
}

#endif /* CONFIG_PCI */
#endif /* CONFIG_405GP */
