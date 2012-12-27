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
 * Copyright 2009 Freescale Semiconductor, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <pci.h>
#include <asm/immap_85xx.h>
#include <asm/io.h>
#include <asm/fsl_pci.h>
#include <libfdt.h>
#include <fdt_support.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_PCIE1
static struct pci_controller pcie1_hose;
#endif

#ifdef CONFIG_PCIE2
static struct pci_controller pcie2_hose;
#endif

void pci_init_board(void)
{
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	struct fsl_pci_info pci_info[2];
	u32 devdisr, pordevsr, io_sel;
	int first_free_busno = 0;
	int num = 0;

	int pcie_ep, pcie_configured;

	devdisr = in_be32(&gur->devdisr);
	pordevsr = in_be32(&gur->pordevsr);
	io_sel = (pordevsr & MPC85xx_PORDEVSR_IO_SEL) >> 19;

	debug ("   pci_init_board: devdisr=%x, io_sel=%x\n", devdisr, io_sel);

	if (!(pordevsr & MPC85xx_PORDEVSR_SGMII2_DIS))
		printf ("    eTSEC2 is in sgmii mode.\n");

	puts("\n");
#ifdef CONFIG_PCIE2
	pcie_configured = is_fsl_pci_cfg(LAW_TRGT_IF_PCIE_2, io_sel);

	if (pcie_configured && !(devdisr & MPC85xx_DEVDISR_PCIE)){
		SET_STD_PCIE_INFO(pci_info[num], 2);
		pcie_ep = fsl_setup_hose(&pcie2_hose, pci_info[num].regs);
		printf("    PCIE2 connected to Slot 1 as %s (base addr %lx)\n",
				pcie_ep ? "Endpoint" : "Root Complex",
				pci_info[num].regs);
		first_free_busno = fsl_pci_init_port(&pci_info[num++],
					&pcie2_hose, first_free_busno);
	} else {
		printf ("    PCIE2: disabled\n");
	}
	puts("\n");
#else
	setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE2); /* disable */
#endif

#ifdef CONFIG_PCIE1
	pcie_configured = is_fsl_pci_cfg(LAW_TRGT_IF_PCIE_1, io_sel);

	if (pcie_configured && !(devdisr & MPC85xx_DEVDISR_PCIE)){
		SET_STD_PCIE_INFO(pci_info[num], 1);
		pcie_ep = fsl_setup_hose(&pcie1_hose, pci_info[num].regs);
		printf("    PCIE1 connected to Slot 2 as %s (base addr %lx)\n",
				pcie_ep ? "Endpoint" : "Root Complex",
				pci_info[num].regs);
		first_free_busno = fsl_pci_init_port(&pci_info[num++],
					&pcie1_hose, first_free_busno);
	} else {
		printf ("    PCIE1: disabled\n");
	}
	puts("\n");
#else
	setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE); /* disable */
#endif
}

void ft_pci_board_setup(void *blob)
{
/* According to h/w manual, PCIE2 is at lower address(0x9000)
 * than PCIE1(0xa000).
 * Hence PCIE2 is made to occupy the pci1 position in dts to
 * keep the addresses sorted there.
 * Generally the case with all FSL SOCs.
 */
#ifdef CONFIG_PCIE2
	ft_fsl_pci_setup(blob, "pci1", &pcie2_hose);
#endif
#ifdef CONFIG_PCIE1
	ft_fsl_pci_setup(blob, "pci2", &pcie1_hose);
#endif
}
