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
 * Vipin Kumar, STMicroelectronics, <vipin.kumar@st.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#if defined(CONFIG_MK_spear300)
#define CONFIG_SPEAR3XX				1
#define CONFIG_SPEAR300				1
#elif defined(CONFIG_MK_spear310)
#define CONFIG_SPEAR3XX				1
#define CONFIG_SPEAR310				1
#elif defined(CONFIG_MK_spear320)
#define CONFIG_SPEAR3XX				1
#define CONFIG_SPEAR320				1
#endif

#include <configs/spear-common.h>

/* Serial Configuration (PL011) */
#define CONFIG_SYS_SERIAL0			0xD0000000

#if defined(CONFIG_SPEAR300)
#define CONFIG_PL01x_PORTS			{(void *)CONFIG_SYS_SERIAL0}

#elif defined(CONFIG_SPEAR310)

#if (CONFIG_CONS_INDEX)
#undef  CONFIG_PL011_CLOCK
#define CONFIG_PL011_CLOCK			(83 * 1000 * 1000)
#endif

#define CONFIG_SYS_SERIAL1			0xB2000000
#define CONFIG_SYS_SERIAL2			0xB2080000
#define CONFIG_SYS_SERIAL3			0xB2100000
#define CONFIG_SYS_SERIAL4			0xB2180000
#define CONFIG_SYS_SERIAL5			0xB2200000
#define CONFIG_PL01x_PORTS			{(void *)CONFIG_SYS_SERIAL0, \
						(void *)CONFIG_SYS_SERIAL1, \
						(void *)CONFIG_SYS_SERIAL2, \
						(void *)CONFIG_SYS_SERIAL3, \
						(void *)CONFIG_SYS_SERIAL4, \
						(void *)CONFIG_SYS_SERIAL5 }
#elif defined(CONFIG_SPEAR320)

#if (CONFIG_CONS_INDEX)
#undef  CONFIG_PL011_CLOCK
#define CONFIG_PL011_CLOCK			(83 * 1000 * 1000)
#endif

#define CONFIG_SYS_SERIAL1			0xA3000000
#define CONFIG_SYS_SERIAL2			0xA4000000
#define CONFIG_PL01x_PORTS			{(void *)CONFIG_SYS_SERIAL0, \
						(void *)CONFIG_SYS_SERIAL1, \
						(void *)CONFIG_SYS_SERIAL2 }
#endif

#if defined(CONFIG_SPEAR_EMI)

#define CONFIG_SYS_FLASH_CFI
#define CONFIG_FLASH_CFI_DRIVER

#if defined(CONFIG_SPEAR310)
#define CONFIG_SYS_FLASH_BASE			0x50000000
#define CONFIG_SYS_CS1_FLASH_BASE		0x60000000
#define CONFIG_SYS_CS2_FLASH_BASE		0x70000000
#define CONFIG_SYS_CS3_FLASH_BASE		0x80000000
#define CONFIG_SYS_CS4_FLASH_BASE		0x90000000
#define CONFIG_SYS_CS5_FLASH_BASE		0xA0000000
#define CONFIG_SYS_FLASH_BANKS_LIST		{ CONFIG_SYS_FLASH_BASE,   \
						CONFIG_SYS_CS1_FLASH_BASE, \
						CONFIG_SYS_CS2_FLASH_BASE, \
						CONFIG_SYS_CS3_FLASH_BASE, \
						CONFIG_SYS_CS4_FLASH_BASE, \
						CONFIG_SYS_CS5_FLASH_BASE }
#define CONFIG_SYS_MAX_FLASH_BANKS		6

#elif defined(CONFIG_SPEAR320)
#define CONFIG_SYS_FLASH_BASE			0x44000000
#define CONFIG_SYS_CS1_FLASH_BASE		0x45000000
#define CONFIG_SYS_CS2_FLASH_BASE		0x46000000
#define CONFIG_SYS_CS3_FLASH_BASE		0x47000000
#define CONFIG_SYS_FLASH_BANKS_LIST		{ CONFIG_SYS_FLASH_BASE,   \
						CONFIG_SYS_CS1_FLASH_BASE, \
						CONFIG_SYS_CS2_FLASH_BASE, \
						CONFIG_SYS_CS3_FLASH_BASE }
#define CONFIG_SYS_MAX_FLASH_BANKS		4

#endif

#define CONFIG_SYS_MAX_FLASH_SECT		(127 + 8)
#define CONFIG_SYS_FLASH_QUIET_TEST		1

#endif

#if defined(CONFIG_SPEAR300)
#define CONFIG_SYS_NAND_BASE			(0x80000000)

#elif defined(CONFIG_SPEAR310)
#define CONFIG_SYS_NAND_BASE			(0x40000000)

#elif defined(CONFIG_SPEAR320)
#define CONFIG_SYS_NAND_BASE			(0x50000000)

#endif

#endif  /* __CONFIG_H */
