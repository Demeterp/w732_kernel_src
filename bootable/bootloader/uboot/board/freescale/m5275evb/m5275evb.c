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
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2005-2008 Arthur Shipkowski (art@videon-central.com)
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
#include <asm/immap.h>

#define PERIOD		13	/* system bus period in ns */
#define SDRAM_TREFI	7800	/* in ns */

int checkboard(void)
{
	puts("Board: ");
	puts("Freescale MCF5275 EVB\n");
	return 0;
};

phys_size_t initdram(int board_type)
{
	volatile sdramctrl_t *sdp = (sdramctrl_t *)(MMAP_SDRAM);
	volatile gpio_t *gpio_reg = (gpio_t *)(MMAP_GPIO);

	gpio_reg->par_sdram = 0x3FF; /* Enable SDRAM */

	/* Set up chip select */
	sdp->sdbar0 = CONFIG_SYS_SDRAM_BASE;
	sdp->sdbmr0 = MCF_SDRAMC_SDMRn_BAM_32M | MCF_SDRAMC_SDMRn_V;

	/* Set up timing */
	sdp->sdcfg1 = 0x83711630;
	sdp->sdcfg2 = 0x46770000;

	/* Enable clock */
	sdp->sdcr = MCF_SDRAMC_SDCR_MODE_EN | MCF_SDRAMC_SDCR_CKE;

	/* Set precharge */
	sdp->sdcr |= MCF_SDRAMC_SDCR_IPALL;

	/* Dummy write to start SDRAM */
	*((volatile unsigned long *)CONFIG_SYS_SDRAM_BASE) = 0xa5a59696;

	/* Send LEMR */
	sdp->sdmr = MCF_SDRAMC_SDMR_BNKAD_LEMR
			| MCF_SDRAMC_SDMR_AD(0x0)
			| MCF_SDRAMC_SDMR_CMD;
	*((volatile unsigned long *)CONFIG_SYS_SDRAM_BASE) = 0xa5a59696;

	/* Send LMR */
	sdp->sdmr = 0x058d0000;
	*((volatile unsigned long *)CONFIG_SYS_SDRAM_BASE) = 0xa5a59696;

	/* Stop sending commands */
	sdp->sdmr &= ~(MCF_SDRAMC_SDMR_CMD);

	/* Set precharge */
	sdp->sdcr |= MCF_SDRAMC_SDCR_IPALL;
	*((volatile unsigned long *)CONFIG_SYS_SDRAM_BASE) = 0xa5a59696;

	/* Stop manual precharge, send 2 IREF */
	sdp->sdcr &= ~(MCF_SDRAMC_SDCR_IPALL);
	sdp->sdcr |= MCF_SDRAMC_SDCR_IREF;
	*((volatile unsigned long *)CONFIG_SYS_SDRAM_BASE) = 0xa5a59696;
	*((volatile unsigned long *)CONFIG_SYS_SDRAM_BASE) = 0xa5a59696;

	/* Write mode register, clear reset DLL */
	sdp->sdmr = 0x018d0000;
	*((volatile unsigned long *)CONFIG_SYS_SDRAM_BASE) = 0xa5a59696;

	/* Stop sending commands */
	sdp->sdmr &= ~(MCF_SDRAMC_SDMR_CMD);
	sdp->sdcr &= ~(MCF_SDRAMC_SDCR_MODE_EN);

	/* Turn on auto refresh, lock SDMR */
	sdp->sdcr =
		MCF_SDRAMC_SDCR_CKE
		| MCF_SDRAMC_SDCR_REF
		| MCF_SDRAMC_SDCR_MUX(1)
		/* 1 added to round up */
		| MCF_SDRAMC_SDCR_RCNT((SDRAM_TREFI/(PERIOD*64)) - 1 + 1)
		| MCF_SDRAMC_SDCR_DQS_OE(0x3);

	return CONFIG_SYS_SDRAM_SIZE * 1024 * 1024;
};

int testdram(void)
{
	/* TODO: XXX XXX XXX */
	printf("DRAM test not implemented!\n");

	return (0);
}
