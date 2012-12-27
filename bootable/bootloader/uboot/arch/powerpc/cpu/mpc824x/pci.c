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
 * arch/powerpc/kernel/mpc10x_common.c
 *
 * Common routines for the Motorola SPS MPC106, MPC107 and MPC8240 Host bridge,
 * Mem ctlr, EPIC, etc.
 *
 * Author: Mark A. Greer
 *         mgreer@mvista.com
 *
 * Copyright 2001 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <common.h>

#ifdef CONFIG_PCI

#include <asm/processor.h>
#include <asm/io.h>
#include <pci.h>
#include <mpc824x.h>

void pci_mpc824x_init (struct pci_controller *hose)
{
	hose->first_busno = 0;
	hose->last_busno = 0xff;

	/* System memory space */
	pci_set_region(hose->regions + 0,
		       CHRP_PCI_MEMORY_BUS,
		       CHRP_PCI_MEMORY_PHYS,
		       CHRP_PCI_MEMORY_SIZE,
		       PCI_REGION_MEM | PCI_REGION_SYS_MEMORY);

	/* PCI memory space */
	pci_set_region(hose->regions + 1,
		       CHRP_PCI_MEM_BUS,
		       CHRP_PCI_MEM_PHYS,
		       CHRP_PCI_MEM_SIZE,
		       PCI_REGION_MEM);

	/* ISA/PCI memory space */
	pci_set_region(hose->regions + 2,
		       CHRP_ISA_MEM_BUS,
		       CHRP_ISA_MEM_PHYS,
		       CHRP_ISA_MEM_SIZE,
		       PCI_REGION_MEM);

	/* PCI I/O space */
	pci_set_region(hose->regions + 3,
		       CHRP_PCI_IO_BUS,
		       CHRP_PCI_IO_PHYS,
		       CHRP_PCI_IO_SIZE,
		       PCI_REGION_IO);

	/* ISA/PCI I/O space */
	pci_set_region(hose->regions + 4,
		       CHRP_ISA_IO_BUS,
		       CHRP_ISA_IO_PHYS,
		       CHRP_ISA_IO_SIZE,
		       PCI_REGION_IO);

	hose->region_count = 5;

	pci_setup_indirect(hose,
			   CHRP_REG_ADDR,
			   CHRP_REG_DATA);

	pci_register_hose(hose);

	hose->last_busno = pci_hose_scan(hose);
}

#endif
