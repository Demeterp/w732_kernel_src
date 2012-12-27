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
 * Copyright 2008 Freescale Semiconductor, Inc.
 *
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <asm/fsl_law.h>
#include <asm/mmu.h>

/*
 * LAW(Local Access Window) configuration:
 *
 * 0x0000_0000     0x7fff_ffff     DDR                     2G
 * 0x8000_0000     0x9fff_ffff     PCI1 MEM                512M
 * 0xc000_0000     0xdfff_ffff     RapidIO                 512M
 * 0xe000_0000     0xe000_ffff     CCSR                    1M
 * 0xe200_0000     0xe2ff_ffff     PCI1 IO                 16M
 * 0xf000_0000     0xf7ff_ffff     SDRAM                   128M
 * 0xfc00_0000     0xfc00_ffff     Config Latch            64K
 * 0xff00_0000     0xffff_ffff     FLASH (boot bank)       16M
 *
 * Notes:
 *    CCSRBAR and L2-as-SRAM don't need a configured Local Access Window.
 *    If flash is 8M at default position (last 8M), no LAW needed.
 */

struct law_entry law_table[] = {
#ifndef CONFIG_SPD_EEPROM
	SET_LAW(CONFIG_SYS_DDR_SDRAM_BASE, LAW_SIZE_128M, LAW_TRGT_IF_DDR),
#endif
	SET_LAW(CONFIG_SYS_PCI1_MEM_PHYS, LAW_SIZE_512M, LAW_TRGT_IF_PCI),
	/* This is not so much the SDRAM map as it is the whole localbus map. */
	SET_LAW(CONFIG_SYS_LBC_SDRAM_BASE, LAW_SIZE_256M, LAW_TRGT_IF_LBC),
	SET_LAW(CONFIG_SYS_PCI1_IO_PHYS, LAW_SIZE_16M, LAW_TRGT_IF_PCI),
	SET_LAW(CONFIG_SYS_RIO_MEM_BASE, LAW_SIZE_512M, LAW_TRGT_IF_RIO),
};

int num_law_entries = ARRAY_SIZE(law_table);
