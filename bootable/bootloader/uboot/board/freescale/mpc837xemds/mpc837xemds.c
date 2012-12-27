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
 * Copyright (C) 2007 Freescale Semiconductor, Inc.
 * Dave Liu <daveliu@freescale.com>
 *
 * CREDITS: Kim Phillips contribute to LIBFDT code
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <common.h>
#include <hwconfig.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/fsl_mpc83xx_serdes.h>
#include <spd_sdram.h>
#include <tsec.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <fsl_esdhc.h>
#include "pci.h"
#include "../common/pq-mds-pib.h"

int board_early_init_f(void)
{
	u8 *bcsr = (u8 *)CONFIG_SYS_BCSR;

	/* Enable flash write */
	bcsr[0x9] &= ~0x04;
	/* Clear all of the interrupt of BCSR */
	bcsr[0xe] = 0xff;

#ifdef CONFIG_FSL_SERDES
	immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
	u32 spridr = in_be32(&immr->sysconf.spridr);

	/* we check only part num, and don't look for CPU revisions */
	switch (PARTID_NO_E(spridr)) {
	case SPR_8377:
		fsl_setup_serdes(CONFIG_FSL_SERDES1, FSL_SERDES_PROTO_SATA,
				FSL_SERDES_CLK_100, FSL_SERDES_VDD_1V);
		break;
	case SPR_8378:
		fsl_setup_serdes(CONFIG_FSL_SERDES1, FSL_SERDES_PROTO_SGMII,
				FSL_SERDES_CLK_125, FSL_SERDES_VDD_1V);
		break;
	case SPR_8379:
		fsl_setup_serdes(CONFIG_FSL_SERDES1, FSL_SERDES_PROTO_SATA,
				FSL_SERDES_CLK_100, FSL_SERDES_VDD_1V);
		fsl_setup_serdes(CONFIG_FSL_SERDES2, FSL_SERDES_PROTO_SATA,
				FSL_SERDES_CLK_100, FSL_SERDES_VDD_1V);
		break;
	default:
		printf("serdes not configured: unknown CPU part number: "
				"%04x\n", spridr >> 16);
		break;
	}
#endif /* CONFIG_FSL_SERDES */
	return 0;
}

#ifdef CONFIG_FSL_ESDHC
int board_mmc_init(bd_t *bd)
{
	struct immap __iomem *im = (struct immap __iomem *)CONFIG_SYS_IMMR;
	u8 *bcsr = (u8 *)CONFIG_SYS_BCSR;

	if (!hwconfig("esdhc"))
		return 0;

	/* Set SPI_SD, SER_SD, and IRQ4_WP so that SD signals go through */
	bcsr[0xc] |= 0x4c;

	/* Set proper bits in SICR to allow SD signals through */
	clrsetbits_be32(&im->sysconf.sicrl, SICRL_USB_B, SICRL_USB_B_SD);
	clrsetbits_be32(&im->sysconf.sicrh, SICRH_GPIO2_E | SICRH_SPI,
			SICRH_GPIO2_E_SD | SICRH_SPI_SD);

	return fsl_esdhc_mmc_init(bd);
}
#endif

#if defined(CONFIG_TSEC1) || defined(CONFIG_TSEC2)
int board_eth_init(bd_t *bd)
{
	struct tsec_info_struct tsec_info[2];
	struct immap __iomem *im = (struct immap __iomem *)CONFIG_SYS_IMMR;
	u32 rcwh = in_be32(&im->reset.rcwh);
	u32 tsec_mode;
	int num = 0;

	/* New line after Net: */
	printf("\n");

#ifdef CONFIG_TSEC1
	SET_STD_TSEC_INFO(tsec_info[num], 1);

	printf(CONFIG_TSEC1_NAME ": ");

	tsec_mode = rcwh & HRCWH_TSEC1M_MASK;
	if (tsec_mode == HRCWH_TSEC1M_IN_RGMII) {
		printf("RGMII\n");
		/* this is default, no need to fixup */
	} else if (tsec_mode == HRCWH_TSEC1M_IN_SGMII) {
		printf("SGMII\n");
		tsec_info[num].phyaddr = TSEC1_PHY_ADDR_SGMII;
		tsec_info[num].flags = TSEC_GIGABIT;
	} else {
		printf("unsupported PHY type\n");
	}
	num++;
#endif
#ifdef CONFIG_TSEC2
	SET_STD_TSEC_INFO(tsec_info[num], 2);

	printf(CONFIG_TSEC2_NAME ": ");

	tsec_mode = rcwh & HRCWH_TSEC2M_MASK;
	if (tsec_mode == HRCWH_TSEC2M_IN_RGMII) {
		printf("RGMII\n");
		/* this is default, no need to fixup */
	} else if (tsec_mode == HRCWH_TSEC2M_IN_SGMII) {
		printf("SGMII\n");
		tsec_info[num].phyaddr = TSEC2_PHY_ADDR_SGMII;
		tsec_info[num].flags = TSEC_GIGABIT;
	} else {
		printf("unsupported PHY type\n");
	}
	num++;
#endif
	return tsec_eth_init(bd, tsec_info, num);
}

static void __ft_tsec_fixup(void *blob, bd_t *bd, const char *alias,
			    int phy_addr)
{
	const char *phy_type = "sgmii";
	const u32 *ph;
	int off;
	int err;

	off = fdt_path_offset(blob, alias);
	if (off < 0) {
		printf("WARNING: could not find %s alias: %s.\n", alias,
			fdt_strerror(off));
		return;
	}

	err = fdt_setprop(blob, off, "phy-connection-type", phy_type,
			  strlen(phy_type) + 1);
	if (err) {
		printf("WARNING: could not set phy-connection-type for %s: "
			"%s.\n", alias, fdt_strerror(err));
		return;
	}

	ph = (u32 *)fdt_getprop(blob, off, "phy-handle", 0);
	if (!ph) {
		printf("WARNING: could not get phy-handle for %s.\n",
			alias);
		return;
	}

	off = fdt_node_offset_by_phandle(blob, *ph);
	if (off < 0) {
		printf("WARNING: could not get phy node for %s: %s\n", alias,
			fdt_strerror(off));
		return;
	}

	phy_addr = cpu_to_fdt32(phy_addr);
	err = fdt_setprop(blob, off, "reg", &phy_addr, sizeof(phy_addr));
	if (err < 0) {
		printf("WARNING: could not set phy node's reg for %s: "
			"%s.\n", alias, fdt_strerror(err));
		return;
	}
}

static void ft_tsec_fixup(void *blob, bd_t *bd)
{
	struct immap __iomem *im = (struct immap __iomem *)CONFIG_SYS_IMMR;
	u32 rcwh = in_be32(&im->reset.rcwh);
	u32 tsec_mode;

#ifdef CONFIG_TSEC1
	tsec_mode = rcwh & HRCWH_TSEC1M_MASK;
	if (tsec_mode == HRCWH_TSEC1M_IN_SGMII)
		__ft_tsec_fixup(blob, bd, "ethernet0", TSEC1_PHY_ADDR_SGMII);
#endif

#ifdef CONFIG_TSEC2
	tsec_mode = rcwh & HRCWH_TSEC2M_MASK;
	if (tsec_mode == HRCWH_TSEC2M_IN_SGMII)
		__ft_tsec_fixup(blob, bd, "ethernet1", TSEC2_PHY_ADDR_SGMII);
#endif
}
#else
static inline void ft_tsec_fixup(void *blob, bd_t *bd) {}
#endif /* defined(CONFIG_TSEC1) || defined(CONFIG_TSEC2) */

int board_early_init_r(void)
{
#ifdef CONFIG_PQ_MDS_PIB
	pib_init();
#endif
	return 0;
}

#if defined(CONFIG_DDR_ECC) && !defined(CONFIG_ECC_INIT_VIA_DDRCONTROLLER)
extern void ddr_enable_ecc(unsigned int dram_size);
#endif
int fixed_sdram(void);

phys_size_t initdram(int board_type)
{
	volatile immap_t *im = (immap_t *) CONFIG_SYS_IMMR;
	u32 msize = 0;

	if ((im->sysconf.immrbar & IMMRBAR_BASE_ADDR) != (u32) im)
		return -1;

#if defined(CONFIG_SPD_EEPROM)
	msize = spd_sdram();
#else
	msize = fixed_sdram();
#endif

#if defined(CONFIG_DDR_ECC) && !defined(CONFIG_ECC_INIT_VIA_DDRCONTROLLER)
	/* Initialize DDR ECC byte */
	ddr_enable_ecc(msize * 1024 * 1024);
#endif

	/* return total bus DDR size(bytes) */
	return (msize * 1024 * 1024);
}

#if !defined(CONFIG_SPD_EEPROM)
/*************************************************************************
 *  fixed sdram init -- doesn't use serial presence detect.
 ************************************************************************/
int fixed_sdram(void)
{
	volatile immap_t *im = (immap_t *) CONFIG_SYS_IMMR;
	u32 msize = CONFIG_SYS_DDR_SIZE * 1024 * 1024;
	u32 msize_log2 = __ilog2(msize);

	im->sysconf.ddrlaw[0].bar = CONFIG_SYS_DDR_SDRAM_BASE & 0xfffff000;
	im->sysconf.ddrlaw[0].ar = LBLAWAR_EN | (msize_log2 - 1);

#if (CONFIG_SYS_DDR_SIZE != 512)
#warning Currenly any ddr size other than 512 is not supported
#endif
	im->sysconf.ddrcdr = CONFIG_SYS_DDRCDR_VALUE;
	udelay(50000);

	im->ddr.sdram_clk_cntl = CONFIG_SYS_DDR_SDRAM_CLK_CNTL;
	udelay(1000);

	im->ddr.csbnds[0].csbnds = CONFIG_SYS_DDR_CS0_BNDS;
	im->ddr.cs_config[0] = CONFIG_SYS_DDR_CS0_CONFIG;
	udelay(1000);

	im->ddr.timing_cfg_0 = CONFIG_SYS_DDR_TIMING_0;
	im->ddr.timing_cfg_1 = CONFIG_SYS_DDR_TIMING_1;
	im->ddr.timing_cfg_2 = CONFIG_SYS_DDR_TIMING_2;
	im->ddr.timing_cfg_3 = CONFIG_SYS_DDR_TIMING_3;
	im->ddr.sdram_cfg = CONFIG_SYS_DDR_SDRAM_CFG;
	im->ddr.sdram_cfg2 = CONFIG_SYS_DDR_SDRAM_CFG2;
	im->ddr.sdram_mode = CONFIG_SYS_DDR_MODE;
	im->ddr.sdram_mode2 = CONFIG_SYS_DDR_MODE2;
	im->ddr.sdram_interval = CONFIG_SYS_DDR_INTERVAL;
	__asm__ __volatile__("sync");
	udelay(1000);

	im->ddr.sdram_cfg |= SDRAM_CFG_MEM_EN;
	udelay(2000);
	return CONFIG_SYS_DDR_SIZE;
}
#endif /*!CONFIG_SYS_SPD_EEPROM */

int checkboard(void)
{
	puts("Board: Freescale MPC837xEMDS\n");
	return 0;
}

#ifdef CONFIG_PCI
int board_pci_host_broken(void)
{
	struct immap __iomem *im = (struct immap __iomem *)CONFIG_SYS_IMMR;
	const u32 rcw_mask = HRCWH_PCI1_ARBITER_ENABLE | HRCWH_PCI_HOST;

	/* It's always OK in case of external arbiter. */
	if (hwconfig_subarg_cmp("pci", "arbiter", "external"))
		return 0;

	if ((in_be32(&im->reset.rcwh) & rcw_mask) != rcw_mask)
		return 1;

	return 0;
}

static void ft_pci_fixup(void *blob, bd_t *bd)
{
	const char *status = "broken (no arbiter)";
	int off;
	int err;

	off = fdt_path_offset(blob, "pci0");
	if (off < 0) {
		printf("WARNING: could not find pci0 alias: %s.\n",
			fdt_strerror(off));
		return;
	}

	err = fdt_setprop(blob, off, "status", status, strlen(status) + 1);
	if (err) {
		printf("WARNING: could not set status for pci0: %s.\n",
			fdt_strerror(err));
		return;
	}
}
#endif

#if defined(CONFIG_OF_BOARD_SETUP)
void ft_board_setup(void *blob, bd_t *bd)
{
	ft_cpu_setup(blob, bd);
	ft_tsec_fixup(blob, bd);
	fdt_fixup_dr_usb(blob, bd);
	fdt_fixup_esdhc(blob, bd);
#ifdef CONFIG_PCI
	ft_pci_setup(blob, bd);
	if (board_pci_host_broken())
		ft_pci_fixup(blob, bd);
	ft_pcie_fixup(blob, bd);
#endif
}
#endif /* CONFIG_OF_BOARD_SETUP */
