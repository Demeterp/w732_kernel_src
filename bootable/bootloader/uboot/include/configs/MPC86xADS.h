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
  * A collection of structures, addresses, and values associated with
  * the Motorola MPC8xxADS board.  Copied from the FADS config.
  *
  * Copyright (c) 1998 Dan Malek (dmalek@jlc.net)
  *
  * Modified by, Yuli Barcohen, Arabella Software Ltd., yuli@arabellasw.com
  *
  * Values common to all FADS family boards are in board/fads/fads.h
  */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */

/* board type */
#define CONFIG_MPC86xADS        1       /* new ADS */
#define CONFIG_FADS		1       /* We are FADS compatible (more or less) */

/* CPU type - pick one of these */
#define CONFIG_MPC866T		1
#undef CONFIG_MPC866P
#undef CONFIG_MPC859T
#undef CONFIG_MPC859DSL
#undef CONFIG_MPC852T

#define	CONFIG_8xx_CONS_SMC1	1	/* Console is on SMC1		*/
#undef	CONFIG_8xx_CONS_SMC2
#undef	CONFIG_8xx_CONS_NONE
#define CONFIG_BAUDRATE		38400

#define CONFIG_8xx_OSCLK		10000000 /* 10MHz oscillator on EXTCLK  */
#define CONFIG_8xx_CPUCLK_DEFAULT	50000000
#define CONFIG_SYS_8xx_CPUCLK_MIN		40000000
#define CONFIG_SYS_8xx_CPUCLK_MAX		80000000

#define CONFIG_DRAM_50MHZ       1
#define CONFIG_SDRAM_50MHZ      1

#include "../../board/fads/fads.h"

#define CONFIG_SYS_OR5_PRELIM		0xFFFF8110	/* 64Kbyte address space */
#define CONFIG_SYS_BR5_PRELIM		(CONFIG_SYS_PHYDEV_ADDR | BR_PS_8 | BR_V)

#endif	/* __CONFIG_H */
