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
 * linux/include/asm-arm/arch-netarm/netarm_registers.h
 *
 * Copyright (C) 2005
 * Art Shipkowski, Videon Central, Inc., <art@videon-central.com>
 *
 * Copyright (C) 2000, 2001 NETsilicon, Inc.
 * Copyright (C) 2000, 2001 WireSpeed Communications Corporation
 *
 * This software is copyrighted by WireSpeed. LICENSEE agrees that
 * it will not delete this copyright notice, trademarks or protective
 * notices from any copy made by LICENSEE.
 *
 * This software is provided "AS-IS" and any express or implied
 * warranties or conditions, including but not limited to any
 * implied warranties of merchantability and fitness for a particular
 * purpose regarding this software. In no event shall WireSpeed
 * be liable for any indirect, consequential, or incidental damages,
 * loss of profits or revenue, loss of use or data, or interruption
 * of business, whether the alleged damages are labeled in contract,
 * tort, or indemnity.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * author(s) : Joe deBlaquiere
 *
 * Modified to support NS7520 by Art Shipkowski.
 */

#ifndef __NET_ARM_REGISTERS_H
#define __NET_ARM_REGISTERS_H

#include <config.h>

/* fundamental constants : */
/* the input crystal/clock frequency ( in Hz ) */
#define	NETARM_XTAL_FREQ_25MHz		(18432000)
#define	NETARM_XTAL_FREQ_33MHz		(23698000)
#define	NETARM_XTAL_FREQ_48MHz		(48000000)
#define	NETARM_XTAL_FREQ_55MHz		(55000000)
#define NETARM_XTAL_FREQ_EMLIN1		(20000000)

/* the frequency of SYS_CLK */
#if defined(CONFIG_NETARM_EMLIN)

/* EMLIN board:  33 MHz (exp.) */
#define	NETARM_PLL_COUNT_VAL		6
#define NETARM_XTAL_FREQ		NETARM_XTAL_FREQ_25MHz

#elif defined(CONFIG_NETARM_NET40_REV2)

/* NET+40 Rev2 boards:  33 MHz (with NETARM_XTAL_FREQ_25MHz) */
#define	NETARM_PLL_COUNT_VAL		6
#define	NETARM_XTAL_FREQ		NETARM_XTAL_FREQ_25MHz

#elif defined(CONFIG_NETARM_NET40_REV4)

/* NET+40 Rev4 boards with EDO must clock slower: 25 MHz (with
   NETARM_XTAL_FREQ_25MHz) 4 */
#define	NETARM_PLL_COUNT_VAL		4
#define	NETARM_XTAL_FREQ		NETARM_XTAL_FREQ_25MHz

#elif defined(CONFIG_NETARM_NET50)

/* NET+50 boards:  40 MHz (with NETARM_XTAL_FREQ_25MHz) */
#define NETARM_PLL_COUNT_VAL		8
#define	NETARM_XTAL_FREQ		NETARM_XTAL_FREQ_25MHz

#else	/* CONFIG_NETARM_NS7520 */

#define	NETARM_PLL_COUNT_VAL		0

#if defined(CONFIG_BOARD_UNC20)
#define	NETARM_XTAL_FREQ		NETARM_XTAL_FREQ_48MHz
#else
#define	NETARM_XTAL_FREQ		NETARM_XTAL_FREQ_55MHz
#endif

#endif

/* #include "arm_registers.h" */
#include <asm/arch/netarm_gen_module.h>
#include <asm/arch/netarm_mem_module.h>
#include <asm/arch/netarm_ser_module.h>
#include <asm/arch/netarm_eni_module.h>
#include <asm/arch/netarm_dma_module.h>
#include <asm/arch/netarm_eth_module.h>

#endif
