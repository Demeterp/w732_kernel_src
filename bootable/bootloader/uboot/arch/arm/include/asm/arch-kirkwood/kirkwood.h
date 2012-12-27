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
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Prafulla Wadaskar <prafulla@marvell.com>
 *
 * Header file for the Marvell's Feroceon CPU core.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#ifndef _ASM_ARCH_KIRKWOOD_H
#define _ASM_ARCH_KIRKWOOD_H

#ifndef __ASSEMBLY__
#include <asm/types.h>
#include <asm/io.h>
#endif /* __ASSEMBLY__ */

#if defined (CONFIG_FEROCEON_88FR131) || defined (CONFIG_SHEEVA_88SV131)
#include <asm/arch/cpu.h>

/* SOC specific definations */
#define INTREG_BASE			0xd0000000
#define KW_REGISTER(x)			(KW_REGS_PHY_BASE + x)
#define KW_OFFSET_REG			(INTREG_BASE + 0x20080)

/* undocumented registers */
#define KW_REG_UNDOC_0x1470		(KW_REGISTER(0x1470))
#define KW_REG_UNDOC_0x1478		(KW_REGISTER(0x1478))

#define KW_TWSI_BASE			(KW_REGISTER(0x11000))
#define KW_UART0_BASE			(KW_REGISTER(0x12000))
#define KW_UART1_BASE			(KW_REGISTER(0x12100))
#define KW_MPP_BASE			(KW_REGISTER(0x10000))
#define KW_GPIO0_BASE			(KW_REGISTER(0x10100))
#define KW_GPIO1_BASE			(KW_REGISTER(0x10140))
#define KW_NANDF_BASE			(KW_REGISTER(0x10418))
#define KW_SPI_BASE			(KW_REGISTER(0x10600))
#define KW_CPU_WIN_BASE			(KW_REGISTER(0x20000))
#define KW_CPU_REG_BASE			(KW_REGISTER(0x20100))
#define KW_TIMER_BASE			(KW_REGISTER(0x20300))
#define KW_REG_PCIE_BASE		(KW_REGISTER(0x40000))
#define KW_USB20_BASE			(KW_REGISTER(0x50000))
#define KW_EGIGA0_BASE			(KW_REGISTER(0x72000))
#define KW_EGIGA1_BASE			(KW_REGISTER(0x76000))

#if defined (CONFIG_KW88F6281)
#include <asm/arch/kw88f6281.h>
#elif defined (CONFIG_KW88F6192)
#include <asm/arch/kw88f6192.h>
#else
#error "SOC Name not defined"
#endif /* CONFIG_KW88F6281 */
#endif /* CONFIG_FEROCEON_88FR131 */
#endif /* _ASM_ARCH_KIRKWOOD_H */
