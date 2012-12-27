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
 * (C) Copyright 2007
 * Michael Schwingen, michael@schwingen.org
 *
 * hardware register definitions for the AcTux-3 board.
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

#ifndef _ACTUX3_HW_H
#define _ACTUX3_HW_H

/* 0 = LED off,1 = ON */
#define ACTUX3_LED1_RT(a)	writeb((a), IXP425_EXP_BUS_CS7_BASE_PHYS + 0)
#define ACTUX3_LED1_GN(a)	writeb((a), IXP425_EXP_BUS_CS7_BASE_PHYS + 1)
#define ACTUX3_LED2_RT(a)	writeb((a), IXP425_EXP_BUS_CS7_BASE_PHYS + 2)
#define ACTUX3_LED2_GN(a)	writeb((a), IXP425_EXP_BUS_CS7_BASE_PHYS + 3)
#define ACTUX3_LED3_RT(a)	writeb((a), IXP425_EXP_BUS_CS7_BASE_PHYS + 4)
#define ACTUX3_LED3_GN(a)	writeb((a), IXP425_EXP_BUS_CS7_BASE_PHYS + 5)
#define ACTUX3_LED4_GN(a)	writeb((a)^1, IXP425_EXP_BUS_CS7_BASE_PHYS + 6)
#define ACTUX3_LED5_RT(a)	writeb((a), IXP425_EXP_BUS_CS7_BASE_PHYS + 7)

#define ACTUX3_DBG_PORT		IXP425_EXP_BUS_CS5_BASE_PHYS
#define ACTUX3_BOARDREL		(readb(IXP425_EXP_BUS_CS6_BASE_PHYS) & 0x0F)
#define ACTUX3_OPTION		(readb(IXP425_EXP_BUS_CS6_BASE_PHYS) & 0xF0)

/* GPIO settings */
#define CONFIG_SYS_GPIO_DBGINT			0
#define CONFIG_SYS_GPIO_ETHINT			1
#define CONFIG_SYS_GPIO_ETHRST			2	/* Out */
#define CONFIG_SYS_GPIO_LED5_GN		3	/* Out */
#define CONFIG_SYS_GPIO_LED6_RT		4	/* Out */
#define CONFIG_SYS_GPIO_LED6_GN		5	/* Out */
#define CONFIG_SYS_GPIO_DSR			6	/* Out */
#define CONFIG_SYS_GPIO_DCD			7	/* Out */
#define CONFIG_SYS_GPIO_DBGJUMPER		9
#define CONFIG_SYS_GPIO_BUTTON1		10
#define CONFIG_SYS_GPIO_DBGSENSE		11
#define CONFIG_SYS_GPIO_DTR			12
#define CONFIG_SYS_GPIO_IORST			13	/* Out */
#define CONFIG_SYS_GPIO_PCI_CLK		14	/* Out */
#define CONFIG_SYS_GPIO_EXTBUS_CLK		15	/* Out */

#endif
