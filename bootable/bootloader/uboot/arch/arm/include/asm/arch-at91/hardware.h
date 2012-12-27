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
 * [origin: Linux kernel include/asm-arm/arch-at91/hardware.h]
 *
 *  Copyright (C) 2003 SAN People
 *  Copyright (C) 2003 ATMEL
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <asm/sizes.h>

#if defined(CONFIG_AT91RM9200)
#include <asm/arch-at91/at91rm9200.h>
#elif defined(CONFIG_AT91SAM9260) || defined(CONFIG_AT91SAM9G20)
#include <asm/arch/at91sam9260.h>
#define AT91_BASE_SPI	AT91SAM9260_BASE_SPI0
#define AT91_ID_UHP	AT91SAM9260_ID_UHP
#define AT91_PMC_UHP	AT91SAM926x_PMC_UHP
#elif defined(CONFIG_AT91SAM9261) || defined(CONFIG_AT91SAM9G10)
#include <asm/arch/at91sam9261.h>
#define AT91_BASE_SPI	AT91SAM9261_BASE_SPI0
#define AT91_ID_UHP	AT91SAM9261_ID_UHP
#define AT91_PMC_UHP	AT91SAM926x_PMC_UHP
#elif defined(CONFIG_AT91SAM9263)
#include <asm/arch/at91sam9263.h>
#define AT91_BASE_SPI	AT91SAM9263_BASE_SPI0
#define AT91_ID_UHP	AT91SAM9263_ID_UHP
#define AT91_PMC_UHP	AT91SAM926x_PMC_UHP
#elif defined(CONFIG_AT91SAM9RL)
#include <asm/arch/at91sam9rl.h>
#define AT91_BASE_SPI	AT91SAM9RL_BASE_SPI
#define AT91_ID_UHP	AT91SAM9RL_ID_UHP
#elif defined(CONFIG_AT91SAM9G45) || defined(CONFIG_AT91SAM9M10G45)
#include <asm/arch/at91sam9g45.h>
#define AT91_BASE_EMAC  AT91SAM9G45_BASE_EMAC
#define AT91_BASE_SPI   AT91SAM9G45_BASE_SPI0
#define AT91_ID_UHP     AT91SAM9G45_ID_UHPHS
#define AT91_PMC_UHP    AT91SAM926x_PMC_UHP
#elif defined(CONFIG_AT91CAP9)
#include <asm/arch/at91cap9.h>
#define AT91_BASE_SPI	AT91CAP9_BASE_SPI0
#define AT91_ID_UHP	AT91CAP9_ID_UHP
#define AT91_PMC_UHP	AT91CAP9_PMC_UHP
#elif defined(CONFIG_AT91X40)
#include <asm/arch/at91x40.h>
#else
#error "Unsupported AT91 processor"
#endif

/* External Memory Map */
#define AT91_CHIPSELECT_0	0x10000000
#define AT91_CHIPSELECT_1	0x20000000
#define AT91_CHIPSELECT_2	0x30000000
#define AT91_CHIPSELECT_3	0x40000000
#define AT91_CHIPSELECT_4	0x50000000
#define AT91_CHIPSELECT_5	0x60000000
#define AT91_CHIPSELECT_6	0x70000000
#define AT91_CHIPSELECT_7	0x80000000

/* SDRAM */
#ifdef CONFIG_DRAM_BASE
#define AT91_SDRAM_BASE		CONFIG_DRAM_BASE
#else
#define AT91_SDRAM_BASE		AT91_CHIPSELECT_1
#endif

/* Clocks */
#define AT91_SLOW_CLOCK		32768		/* slow clock */

#endif
