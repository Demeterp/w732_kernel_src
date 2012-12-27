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

#include <asm/mmu.h>
#include <asm/io.h>
#include <common.h>
#include <mpc83xx.h>
#include <pci.h>
#include <i2c.h>
#include <fdt_support.h>
#include <asm/fsl_i2c.h>
#include <asm/fsl_mpc83xx_serdes.h>

static struct pci_region pci_regions[] = {
	{
		bus_start: CONFIG_SYS_PCI_MEM_BASE,
		phys_start: CONFIG_SYS_PCI_MEM_PHYS,
		size: CONFIG_SYS_PCI_MEM_SIZE,
		flags: PCI_REGION_MEM | PCI_REGION_PREFETCH
	},
	{
		bus_start: CONFIG_SYS_PCI_MMIO_BASE,
		phys_start: CONFIG_SYS_PCI_MMIO_PHYS,
		size: CONFIG_SYS_PCI_MMIO_SIZE,
		flags: PCI_REGION_MEM
	},
	{
		bus_start: CONFIG_SYS_PCI_IO_BASE,
		phys_start: CONFIG_SYS_PCI_IO_PHYS,
		size: CONFIG_SYS_PCI_IO_SIZE,
		flags: PCI_REGION_IO
	}
};

static struct pci_region pcie_regions_0[] = {
	{
		.bus_start = CONFIG_SYS_PCIE1_MEM_BASE,
		.phys_start = CONFIG_SYS_PCIE1_MEM_PHYS,
		.size = CONFIG_SYS_PCIE1_MEM_SIZE,
		.flags = PCI_REGION_MEM,
	},
	{
		.bus_start = CONFIG_SYS_PCIE1_IO_BASE,
		.phys_start = CONFIG_SYS_PCIE1_IO_PHYS,
		.size = CONFIG_SYS_PCIE1_IO_SIZE,
		.flags = PCI_REGION_IO,
	},
};

static struct pci_region pcie_regions_1[] = {
	{
		.bus_start = CONFIG_SYS_PCIE2_MEM_BASE,
		.phys_start = CONFIG_SYS_PCIE2_MEM_PHYS,
		.size = CONFIG_SYS_PCIE2_MEM_SIZE,
		.flags = PCI_REGION_MEM,
	},
	{
		.bus_start = CONFIG_SYS_PCIE2_IO_BASE,
		.phys_start = CONFIG_SYS_PCIE2_IO_PHYS,
		.size = CONFIG_SYS_PCIE2_IO_SIZE,
		.flags = PCI_REGION_IO,
	},
};

static int is_pex_x2(void)
{
	const char *pex_x2 = getenv("pex_x2");

	if (pex_x2 && !strcmp(pex_x2, "yes"))
		return 1;
	return 0;
}

void pci_init_board(void)
{
	volatile immap_t *immr = (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile sysconf83xx_t *sysconf = &immr->sysconf;
	volatile clk83xx_t *clk = (volatile clk83xx_t *)&immr->clk;
	volatile law83xx_t *pci_law = immr->sysconf.pcilaw;
	volatile law83xx_t *pcie_law = sysconf->pcielaw;
	struct pci_region *reg[] = { pci_regions };
	struct pci_region *pcie_reg[] = { pcie_regions_0, pcie_regions_1, };
	u32 spridr = in_be32(&immr->sysconf.spridr);
	int pex2 = is_pex_x2();

	if (board_pci_host_broken())
		goto skip_pci;

	/* Enable all 5 PCI_CLK_OUTPUTS */
	clk->occr |= 0xf8000000;
	udelay(2000);

	/* Configure PCI Local Access Windows */
	pci_law[0].bar = CONFIG_SYS_PCI_MEM_PHYS & LAWBAR_BAR;
	pci_law[0].ar = LBLAWAR_EN | LBLAWAR_512MB;

	pci_law[1].bar = CONFIG_SYS_PCI_IO_PHYS & LAWBAR_BAR;
	pci_law[1].ar = LBLAWAR_EN | LBLAWAR_1MB;

	udelay(2000);

	mpc83xx_pci_init(1, reg, 0);
skip_pci:
	/* There is no PEX in MPC8379 parts. */
	if (PARTID_NO_E(spridr) == SPR_8379)
		return;

	if (pex2)
		fsl_setup_serdes(CONFIG_FSL_SERDES2, FSL_SERDES_PROTO_PEX_X2,
				 FSL_SERDES_CLK_100, FSL_SERDES_VDD_1V);
	else
		fsl_setup_serdes(CONFIG_FSL_SERDES2, FSL_SERDES_PROTO_PEX,
				 FSL_SERDES_CLK_100, FSL_SERDES_VDD_1V);

	/* Configure the clock for PCIE controller */
	clrsetbits_be32(&clk->sccr, SCCR_PCIEXP1CM | SCCR_PCIEXP2CM,
				    SCCR_PCIEXP1CM_1 | SCCR_PCIEXP2CM_1);

	/* Deassert the resets in the control register */
	out_be32(&sysconf->pecr1, 0xE0008000);
	if (!pex2)
		out_be32(&sysconf->pecr2, 0xE0008000);
	udelay(2000);

	/* Configure PCI Express Local Access Windows */
	out_be32(&pcie_law[0].bar, CONFIG_SYS_PCIE1_BASE & LAWBAR_BAR);
	out_be32(&pcie_law[0].ar, LBLAWAR_EN | LBLAWAR_512MB);

	out_be32(&pcie_law[1].bar, CONFIG_SYS_PCIE2_BASE & LAWBAR_BAR);
	out_be32(&pcie_law[1].ar, LBLAWAR_EN | LBLAWAR_512MB);

	mpc83xx_pcie_init(pex2 ? 1 : 2, pcie_reg, 0);
}

void ft_pcie_fixup(void *blob, bd_t *bd)
{
	const char *status = "disabled (PCIE1 is x2)";

	if (!is_pex_x2())
		return;

	do_fixup_by_path(blob, "pci2", "status", status,
			 strlen(status) + 1, 1);
}
