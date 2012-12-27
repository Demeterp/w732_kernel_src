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
 * Support for type PCI configuration cycles.
 * based on pci_indirect.c
 *
 * Copyright (C) 2002 Daniel Engström, Omicron Ceti AB, daniel@omicron.se.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <common.h>
#include <asm/io.h>
#include <pci.h>

#define cfg_read(val, addr, op)	*val = op((int)(addr))
#define cfg_write(val, addr, op)	op((val), (int)(addr))

#define TYPE1_PCI_OP(rw, size, type, op, mask)				\
static int								\
type1_##rw##_config_##size(struct pci_controller *hose,			\
			      pci_dev_t dev, int offset, type val)	\
{									\
	outl(dev | (offset & 0xfc) | 0x80000000, (int)hose->cfg_addr);	\
	cfg_##rw(val, hose->cfg_data + (offset & mask), op);		\
	return 0;							\
}


TYPE1_PCI_OP(read, byte, u8 *, inb, 3)
TYPE1_PCI_OP(read, word, u16 *, inw, 2)
TYPE1_PCI_OP(read, dword, u32 *, inl, 0)

TYPE1_PCI_OP(write, byte, u8, outb, 3)
TYPE1_PCI_OP(write, word, u16, outw, 2)
TYPE1_PCI_OP(write, dword, u32, outl, 0)

void pci_setup_type1(struct pci_controller* hose, u32 cfg_addr, u32 cfg_data)
{
	pci_set_ops(hose,
		    type1_read_config_byte,
		    type1_read_config_word,
		    type1_read_config_dword,
		    type1_write_config_byte,
		    type1_write_config_word,
		    type1_write_config_dword);

	hose->cfg_addr = (unsigned int *) cfg_addr;
	hose->cfg_data = (unsigned char *) cfg_data;
}
