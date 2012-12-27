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

#ifndef _405GP_PCI_H
#define _405GP_PCI_H

#include <pci.h>

/*----------------------------------------------------------------------------+
| 405GP PCI core memory map defines.
+----------------------------------------------------------------------------*/
#define MIN_PCI_MEMADDR1    0x80000000
#define MIN_PCI_MEMADDR2    0x00000000
#define MIN_PLB_PCI_IOADDR  0xE8000000  /* PLB side of PCI I/O address space */
#define MIN_PCI_PCI_IOADDR  0x00000000  /* PCI side of PCI I/O address space */
#define MAX_PCI_DEVICES     32

/*----------------------------------------------------------------------------+
| Defines for the 405GP PCI Config address and data registers followed by
| defines for the standard PCI device configuration header.
+----------------------------------------------------------------------------*/
#define PCICFGADR       0xEEC00000
#define PCICFGDATA      0xEEC00004

#define PCIBUSNUM       0x40        /* 405GP specific parameters */
#define PCISUBBUSNUM    0x41
#define PCIDISCOUNT     0x42
#define PCIBRDGOPT1     0x4A
#define PCIBRDGOPT2     0x60

/*----------------------------------------------------------------------------+
| Defines for 405GP PCI Master local configuration regs.
+----------------------------------------------------------------------------*/
#define PMM0LA          0xEF400000
#define PMM0MA          0xEF400004
#define PMM0PCILA       0xEF400008
#define PMM0PCIHA       0xEF40000C
#define PMM1LA          0xEF400010
#define PMM1MA          0xEF400014
#define PMM1PCILA       0xEF400018
#define PMM1PCIHA       0xEF40001C
#define PMM2LA          0xEF400020
#define PMM2MA          0xEF400024
#define PMM2PCILA       0xEF400028
#define PMM2PCIHA       0xEF40002C

/*----------------------------------------------------------------------------+
| Defines for 405GP PCI Target local configuration regs.
+----------------------------------------------------------------------------*/
#define PTM1MS          0xEF400030
#define PTM1LA          0xEF400034
#define PTM2MS          0xEF400038
#define PTM2LA          0xEF40003C

#define PCIDEVID_405GP	0x0

void board_pci_fixup_irq(struct pci_controller *hose, pci_dev_t dev);
int pci_arbiter_enabled(void);
int __pci_pre_init(struct pci_controller *hose);
void __pci_target_init(struct pci_controller *hose);
void __pci_master_init(struct pci_controller *hose);

#endif
