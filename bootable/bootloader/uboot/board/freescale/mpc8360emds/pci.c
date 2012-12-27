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
 * Copyright (C) 2006-2009 Freescale Semiconductor, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

/*
 * PCI Configuration space access support for MPC83xx PCI Bridge
 */

#include <asm/mmu.h>
#include <asm/io.h>
#include <common.h>
#include <mpc83xx.h>
#include <pci.h>
#include <i2c.h>
#include <asm/fsl_i2c.h>
#include "../common/pq-mds-pib.h"

DECLARE_GLOBAL_DATA_PTR;

static struct pci_region pci1_regions[] = {
	{
		bus_start: CONFIG_SYS_PCI1_MEM_BASE,
		phys_start: CONFIG_SYS_PCI1_MEM_PHYS,
		size: CONFIG_SYS_PCI1_MEM_SIZE,
		flags: PCI_REGION_MEM | PCI_REGION_PREFETCH
	},
	{
		bus_start: CONFIG_SYS_PCI1_IO_BASE,
		phys_start: CONFIG_SYS_PCI1_IO_PHYS,
		size: CONFIG_SYS_PCI1_IO_SIZE,
		flags: PCI_REGION_IO
	},
	{
		bus_start: CONFIG_SYS_PCI1_MMIO_BASE,
		phys_start: CONFIG_SYS_PCI1_MMIO_PHYS,
		size: CONFIG_SYS_PCI1_MMIO_SIZE,
		flags: PCI_REGION_MEM
	},
};

#ifdef CONFIG_MPC83XX_PCI2
static struct pci_region pci2_regions[] = {
	{
		bus_start: CONFIG_SYS_PCI2_MEM_BASE,
		phys_start: CONFIG_SYS_PCI2_MEM_PHYS,
		size: CONFIG_SYS_PCI2_MEM_SIZE,
		flags: PCI_REGION_MEM | PCI_REGION_PREFETCH
	},
	{
		bus_start: CONFIG_SYS_PCI2_IO_BASE,
		phys_start: CONFIG_SYS_PCI2_IO_PHYS,
		size: CONFIG_SYS_PCI2_IO_SIZE,
		flags: PCI_REGION_IO
	},
	{
		bus_start: CONFIG_SYS_PCI2_MMIO_BASE,
		phys_start: CONFIG_SYS_PCI2_MMIO_PHYS,
		size: CONFIG_SYS_PCI2_MMIO_SIZE,
		flags: PCI_REGION_MEM
	},
};
#endif

void pci_init_board(void)
#ifdef CONFIG_PCISLAVE
{
	volatile immap_t *immr = (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile law83xx_t *pci_law = immr->sysconf.pcilaw;
	volatile pcictrl83xx_t *pci_ctrl = &immr->pci_ctrl[0];
	struct pci_region *reg[] = { pci1_regions };

	/* Configure PCI Local Access Windows */
	pci_law[0].bar = CONFIG_SYS_PCI1_MEM_PHYS & LAWBAR_BAR;
	pci_law[0].ar = LAWAR_EN | LAWAR_SIZE_512M;

	pci_law[1].bar = CONFIG_SYS_PCI1_IO_PHYS & LAWBAR_BAR;
	pci_law[1].ar = LAWAR_EN | LAWAR_SIZE_1M;

	mpc83xx_pci_init(1, reg, 0);

	/*
	 * Configure PCI Inbound Translation Windows
	 */
	pci_ctrl[0].pitar0 = 0x0;
	pci_ctrl[0].pibar0 = 0x0;
	pci_ctrl[0].piwar0 = PIWAR_EN | PIWAR_RTT_SNOOP |
	    PIWAR_WTT_SNOOP | PIWAR_IWS_4K;

	pci_ctrl[0].pitar1 = 0x0;
	pci_ctrl[0].pibar1 = 0x0;
	pci_ctrl[0].piebar1 = 0x0;
	pci_ctrl[0].piwar1 &= ~PIWAR_EN;

	pci_ctrl[0].pitar2 = 0x0;
	pci_ctrl[0].pibar2 = 0x0;
	pci_ctrl[0].piebar2 = 0x0;
	pci_ctrl[0].piwar2 &= ~PIWAR_EN;

	/* Unlock the configuration bit */
	mpc83xx_pcislave_unlock(0);
	printf("PCI:   Agent mode enabled\n");
}
#else
{
	volatile immap_t *immr = (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile clk83xx_t *clk = (volatile clk83xx_t *)&immr->clk;
	volatile law83xx_t *pci_law = immr->sysconf.pcilaw;
#ifndef CONFIG_MPC83XX_PCI2
	struct pci_region *reg[] = { pci1_regions };
#else
	struct pci_region *reg[] = { pci1_regions, pci2_regions };
#endif

	/* initialize the PCA9555PW IO expander on the PIB board */
	pib_init();

#if defined(PCI_66M)
	clk->occr = OCCR_PCICOE0 | OCCR_PCICOE1 | OCCR_PCICOE2;
	printf("PCI clock is 66MHz\n");
#elif defined(PCI_33M)
	clk->occr = OCCR_PCICOE0 | OCCR_PCICOE1 | OCCR_PCICOE2 |
	    OCCR_PCICD0 | OCCR_PCICD1 | OCCR_PCICD2 | OCCR_PCICR;
	printf("PCI clock is 33MHz\n");
#else
	clk->occr = OCCR_PCICOE0 | OCCR_PCICOE1 | OCCR_PCICOE2;
	printf("PCI clock is 66MHz\n");
#endif
	udelay(2000);

	/* Configure PCI Local Access Windows */
	pci_law[0].bar = CONFIG_SYS_PCI1_MEM_PHYS & LAWBAR_BAR;
	pci_law[0].ar = LAWAR_EN | LAWAR_SIZE_512M;

	pci_law[1].bar = CONFIG_SYS_PCI1_IO_PHYS & LAWBAR_BAR;
	pci_law[1].ar = LAWAR_EN | LAWAR_SIZE_1M;

	udelay(2000);

#ifndef CONFIG_MPC83XX_PCI2
	mpc83xx_pci_init(1, reg, 0);
#else
	mpc83xx_pci_init(2, reg, 0);
#endif
}
#endif				/* CONFIG_PCISLAVE */
