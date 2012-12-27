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
 * Feng Kan, Applied Micro Circuits Corp., fkan@amcc.com.
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
	 * BOOT_CS (FLASH) must be first. Before relocation SA_I can be off to use the
	 * speed up boot process. It is patched after relocation to enable SA_I
	 */
	tlbentry(0xff000000, SZ_16M, 0xff000000, 4, AC_RWX | SA_G)

	/*
	 * TLB entries for SDRAM are not needed on this platform.
	 * They are dynamically generated in the SPD DDR(2) detection
	 * routine.
	 */

	/* Although 512 KB, map 256k at a time */
	tlbentry(CONFIG_SYS_ISRAM_BASE, SZ_256K, 0x00000000, 4, AC_RWX | SA_I)
	tlbentry(CONFIG_SYS_ISRAM_BASE + 0x40000, SZ_256K, 0x00040000, 4, AC_RWX | SA_I)

	tlbentry(CONFIG_SYS_OPER_FLASH, SZ_16M, 0xE7000000, 4,AC_RWX | SA_IG)

	/*
	 * Peripheral base
	 */
	tlbentry(CONFIG_SYS_PERIPHERAL_BASE, SZ_16K, 0xEF600000, 4, AC_RW | SA_IG)

	tlbentry(CONFIG_SYS_PCIE0_XCFGBASE,SZ_16M, 0x00000000, 0xC, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE1_XCFGBASE,SZ_16M, 0x10000000, 0xC, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE2_XCFGBASE,SZ_16M, 0x20000000, 0xC, AC_RW | SA_IG)

	tlbentry(CONFIG_SYS_PCIE0_MEMBASE, SZ_256M, 0x00000000, 0xD, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE1_MEMBASE, SZ_256M, 0x00000000, 0xE, AC_RW | SA_IG)

	tlbentry(CONFIG_SYS_PCIE0_REGBASE, SZ_64K, 0x30000000, 0xC, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE1_REGBASE, SZ_64K, 0x30010000, 0xC, AC_RW | SA_IG)
	tlbentry(CONFIG_SYS_PCIE2_REGBASE, SZ_64K, 0x30020000, 0xC, AC_RW | SA_IG)
	tlbtab_end
