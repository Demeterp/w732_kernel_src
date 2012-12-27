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
 * (C) Copyright 2008
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
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

#include <ppc_asm.tmpl>
#include <config.h>
#include <asm/mmu.h>

/**************************************************************************
 * TLB TABLE
 *
 * This table is used by the cpu boot code to setup the initial tlb
 * entries. Rather than make broad assumptions in the cpu source tree,
 * this table lets each board set things up however they like.
 *
 *  Pointer to the table is returned in r1
 *
 *************************************************************************/
	.section .bootpg,"ax"
	.globl tlbtab

tlbtab:
	tlbtab_start

	/*
	 * BOOT_CS (FLASH) must be first. Before relocation SA_I can be off to
	 * use the speed up boot process. It is patched after relocation to
	 * enable SA_I
	 */
#ifndef CONFIG_NAND_SPL
	tlbentry(CONFIG_SYS_BOOT_BASE_ADDR, SZ_16M, CONFIG_SYS_BOOT_BASE_ADDR, 4, AC_RWX | SA_G) /* TLB 0 */
#else
	tlbentry(CONFIG_SYS_NAND_BOOT_SPL_SRC, SZ_4K, CONFIG_SYS_NAND_BOOT_SPL_SRC, 4, AC_RWX | SA_G)
	tlbentry(CONFIG_SYS_SDRAM_BASE, SZ_256M, CONFIG_SYS_SDRAM_BASE, 0, AC_RWX | SA_IG)
	tlbentry(256 << 20, SZ_256M, 256 << 20, 0, AC_RWX | SA_IG)
#endif

	/*
	 * TLB entries for SDRAM are not needed on this platform.
	 * They are dynamically generated in the SPD DDR(2) detection
	 * routine.
	 */

#ifdef CONFIG_SYS_INIT_RAM_DCACHE
	/* TLB-entry for init-ram in dcache (SA_I must be turned off!) */
	tlbentry(CONFIG_SYS_INIT_RAM_ADDR, SZ_4K, CONFIG_SYS_INIT_RAM_ADDR, 0, AC_RWX | SA_G)
#endif

	tlbentry(CONFIG_SYS_PCI_BASE, SZ_256M, 0x00000000, 0xC, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCI_MEMBASE, SZ_256M, 0x20000000, 0xC, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE_MEMBASE, SZ_256M, 0xB0000000, 0xD, AC_RW | SA_IG)

	tlbentry(CONFIG_SYS_PCIE0_CFGBASE, SZ_16M, 0x00000000, 0xD, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE1_CFGBASE, SZ_16M, 0x20000000, 0xD, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE0_XCFGBASE, SZ_1K, 0x10000000, 0xD, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE1_XCFGBASE, SZ_1K, 0x30000000, 0xD, AC_RW | SA_IG)

	/* PCIe UTL register */
	tlbentry(CONFIG_SYS_PCIE_BASE, SZ_16K, 0x08010000, 0xC, AC_RW | SA_IG)

#if !defined(CONFIG_ARCHES)
	/* TLB-entry for NAND */
	tlbentry(CONFIG_SYS_NAND_ADDR, SZ_16M, CONFIG_SYS_NAND_ADDR, 4, AC_RWX | SA_IG)

	/* TLB-entry for CPLD */
	tlbentry(CONFIG_SYS_BCSR_BASE, SZ_1K, CONFIG_SYS_BCSR_BASE, 4, AC_RW | SA_IG)
#else
	/* TLB-entry for FPGA */
	tlbentry(CONFIG_SYS_FPGA_BASE, SZ_16M, CONFIG_SYS_FPGA_BASE, 4, AC_RW | SA_IG)
#endif

	/* TLB-entry for OCM */
	tlbentry(CONFIG_SYS_OCM_BASE, SZ_1M, 0x00000000, 4, AC_RWX | SA_I)

	/* TLB-entry for Local Configuration registers => peripherals */
	tlbentry(CONFIG_SYS_LOCAL_CONF_REGS, SZ_16M, CONFIG_SYS_LOCAL_CONF_REGS, 4, AC_RWX | SA_IG)

	/* AHB: Internal USB Peripherals (USB, SATA) */
	tlbentry(CONFIG_SYS_AHB_BASE, SZ_1M, 0xbff00000, 4, AC_RWX | SA_IG)

#if defined(CONFIG_RAPIDIO)
	/* TLB-entries for RapidIO (SRIO) */
	tlbentry(CONFIG_SYS_SRGPL0_REG_BAR, SZ_16M, CONFIG_SYS_SRGPL0_REG_BAR,
					0xD, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_SRGPL0_CFG_BAR, SZ_16M, CONFIG_SYS_SRGPL0_CFG_BAR,
					0xD, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_SRGPL0_MNT_BAR, SZ_16M, CONFIG_SYS_SRGPL0_MNT_BAR,
					0xD, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_I2ODMA_BASE, SZ_1K,  0x00100000,
					0x4, AC_RW | SA_IG)
#endif

	tlbtab_end

#if defined(CONFIG_NAND_U_BOOT) && !defined(CONFIG_NAND_SPL)
	/*
	 * For NAND booting the first TLB has to be reconfigured to full size
	 * and with caching disabled after running from RAM!
	 */
#define TLB00	TLB0(CONFIG_SYS_BOOT_BASE_ADDR, SZ_256M)
#define TLB01	TLB1(CONFIG_SYS_BOOT_BASE_ADDR, 1)
#define TLB02	TLB2(AC_RWX | SA_IG)

	.globl	reconfig_tlb0
reconfig_tlb0:
	sync
	isync
	addi	r4,r0,0x0000		/* TLB entry #0 */
	lis	r5,TLB00@h
	ori	r5,r5,TLB00@l
	tlbwe	r5,r4,0x0000		/* Save it out */
	lis	r5,TLB01@h
	ori	r5,r5,TLB01@l
	tlbwe	r5,r4,0x0001		/* Save it out */
	lis	r5,TLB02@h
	ori	r5,r5,TLB02@l
	tlbwe	r5,r4,0x0002		/* Save it out */
	sync
	isync
	blr
#endif
