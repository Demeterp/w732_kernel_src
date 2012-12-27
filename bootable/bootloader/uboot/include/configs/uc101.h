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
 * (C) Copyright 2003-2009
 * Heiko Schocher, DENX Software Engineering, hs@denx.de.
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

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */

#define CONFIG_UC101		1	/* UC101 board		*/
#define CONFIG_HOSTNAME		uc101

#include "manroland/common.h"
#include "manroland/mpc5200-common.h"

/*
 * Serial console configuration
 */
#define CONFIG_BAUDRATE		115200	/* ... at 115200 bps	*/

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME

/*
 * Flash configuration
 */
#define CONFIG_SYS_MAX_FLASH_SECT	140

/*
 * Environment settings
 */
#define CONFIG_ENV_SECT_SIZE	0x10000

/*
 * Memory map
 */
#define	CONFIG_SYS_IB_MASTER		0xc0510000	/* CS 6 */
#define CONFIG_SYS_IB_EPLD		0xc0500000	/* CS 7 */

/* SRAM */
#define SRAM_BASE		CONFIG_SYS_SRAM_BASE
#define SRAM_LEN		0x1fffff
#define SRAM_END		(SRAM_BASE + SRAM_LEN)

/*
 * GPIO configuration
 */
#define CONFIG_SYS_GPS_PORT_CONFIG	0x4d558044

#define CONFIG_SYS_MEMTEST_START	0x00300000
#define CONFIG_SYS_MEMTEST_END		0x00f00000

#define CONFIG_SYS_LOAD_ADDR		0x300000

#define CONFIG_SYS_BOOTCS_CFG		0x00045D00

/* 8Mbit SRAM @0x80100000 */
#define CONFIG_SYS_CS1_SIZE		0x00200000
#define CONFIG_SYS_CS1_CFG		0x21D00

/* Display H1, Status Inputs, EPLD @0x80600000 8 Bit */
#define CONFIG_SYS_CS3_START		CONFIG_SYS_DISPLAY_BASE
#define CONFIG_SYS_CS3_SIZE		0x00000100
#define CONFIG_SYS_CS3_CFG		0x00081802

/* Interbus Master 16 Bit */
#define CONFIG_SYS_CS6_START		CONFIG_SYS_IB_MASTER
#define CONFIG_SYS_CS6_SIZE		0x00010000
#define CONFIG_SYS_CS6_CFG		0x00FF3500

/* Interbus EPLD 8 Bit */
#define CONFIG_SYS_CS7_START		CONFIG_SYS_IB_EPLD
#define CONFIG_SYS_CS7_SIZE		0x00010000
#define CONFIG_SYS_CS7_CFG		0x00081800

/*-----------------------------------------------------------------------
 * IDE/ATA stuff Supports IDE harddisk
 *-----------------------------------------------------------------------
 */

#define CONFIG_SYS_IDE_MAXDEVICE	1	/* max. 2 drives per IDE bus*/

/*---------------------------------------------------------------------*/
/* Display addresses						       */
/*---------------------------------------------------------------------*/
#define CONFIG_SYS_DISP_CHR_RAM	(CONFIG_SYS_DISPLAY_BASE + 0x38)
#define CONFIG_SYS_DISP_CWORD		(CONFIG_SYS_DISPLAY_BASE + 0x30)

#endif /* __CONFIG_H */
