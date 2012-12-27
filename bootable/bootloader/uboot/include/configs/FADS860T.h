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
  * the Motorola 860T FADS board.  Copied from the MBX stuff.
  * Magnus Damm added defines for 8xxrom and extended bd_info.
  * Helmut Buchsbaum added bitvalues for BCSRx
  *
  * Copyright (c) 1998 Dan Malek (dmalek@jlc.net)
  *
  * Modified by, Yuli Barcohen, Arabella Software Ltd., yuli@arabellasw.com
  *
  * Values common to all FADS family boards are in board/fads/fads.h
  */

#ifndef __CONFIG_H
#define __CONFIG_H

/* board type */
#define CONFIG_FADS		1       /* old/new FADS + new ADS */

/* processor type */
#define CONFIG_MPC860T		1       /* 860T */

#define	CONFIG_8xx_CONS_SMC1	1	/* Console is on SMC1		*/
#undef	CONFIG_8xx_CONS_SMC2
#undef	CONFIG_8xx_CONS_NONE
#define CONFIG_BAUDRATE		38400
#define CONFIG_LOADS_ECHO	1	/* echo on for serial download	*/

#if 0 /* old FADS */
# define CONFIG_SYS_8XX_FACT		12	/* 4 MHz oscillator on EXTCLK */
#else /* new FADS */
# define CONFIG_SYS_8XX_FACT		10	/* 5 MHz oscillator on EXTCLK */
#endif

#define CONFIG_SYS_PLPRCR  (((CONFIG_SYS_8XX_FACT-1) << PLPRCR_MF_SHIFT) |	\
		PLPRCR_SPLSS | PLPRCR_TEXPS | PLPRCR_TMIST)

#define CONFIG_DRAM_50MHZ		1
#define CONFIG_SDRAM_50MHZ              1

#include "../../board/fads/fads.h"

#ifdef USE_REAL_FLASH_VALUES
/*
 * These values fit our FADS860T ...
 * The "default" behaviour with 1Mbyte initial doesn't work for us!
 */
#undef CONFIG_SYS_OR0_PRELIM
#undef CONFIG_SYS_BR0_PRELIM
#define CONFIG_SYS_OR0_PRELIM	0x0FFC00D34 /* Real values for the board */
#define CONFIG_SYS_BR0_PRELIM	0x02800001  /* Real values for the board */
#endif

#define CONFIG_SYS_DAUGHTERBOARD /* FADS has processor-specific daughterboard */

#endif	/* __CONFIG_H */
