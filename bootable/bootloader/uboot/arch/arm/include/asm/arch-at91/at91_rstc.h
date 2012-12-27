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
 * [origin: Linux kernel include/asm-arm/arch-at91/at91_rstc.h]
 *
 * Copyright (C) 2007 Andrew Victor
 * Copyright (C) 2007 Atmel Corporation.
 *
 * Reset Controller (RSTC) - System peripherals regsters.
 * Based on AT91SAM9261 datasheet revision D.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_RSTC_H
#define AT91_RSTC_H

#define AT91_ASM_RSTC_MR	(AT91_RSTC_BASE + 0x08)

#ifndef __ASSEMBLY__

typedef struct at91_rstc {
	u32	cr;	/* Reset Controller Control Register */
	u32	sr;	/* Reset Controller Status Register */
	u32	mr;	/* Reset Controller Mode Register */
} at91_rstc_t;

#endif /* __ASSEMBLY__ */

#define AT91_RSTC_KEY		0xA5000000

#define AT91_RSTC_CR_PROCRST	0x00000001
#define AT91_RSTC_CR_PERRST	0x00000004
#define AT91_RSTC_CR_EXTRST	0x00000008

#define AT91_RSTC_MR_URSTEN	0x00000001
#define AT91_RSTC_MR_URSTIEN	0x00000010
#define AT91_RSTC_MR_ERSTL(x)	((x & 0xf) << 8)
#define AT91_RSTC_MR_ERSTL_MASK	0x0000FF00

#define AT91_RSTC_SR_NRSTL	0x00010000

#ifdef CONFIG_AT91_LEGACY

#define AT91_RSTC_CR		(AT91_RSTC + 0x00)	/* Reset Controller Control Register */
#define		AT91_RSTC_PROCRST	(1 << 0)		/* Processor Reset */
#define		AT91_RSTC_PERRST	(1 << 2)		/* Peripheral Reset */
#define		AT91_RSTC_EXTRST	(1 << 3)		/* External Reset */

#define AT91_RSTC_SR		(AT91_RSTC + 0x04)	/* Reset Controller Status Register */
#define		AT91_RSTC_URSTS		(1 << 0)		/* User Reset Status */
#define		AT91_RSTC_RSTTYP	(7 << 8)		/* Reset Type */
#define			AT91_RSTC_RSTTYP_GENERAL	(0 << 8)
#define			AT91_RSTC_RSTTYP_WAKEUP		(1 << 8)
#define			AT91_RSTC_RSTTYP_WATCHDOG	(2 << 8)
#define			AT91_RSTC_RSTTYP_SOFTWARE	(3 << 8)
#define			AT91_RSTC_RSTTYP_USER	(4 << 8)
#define		AT91_RSTC_NRSTL		(1 << 16)		/* NRST Pin Level */
#define		AT91_RSTC_SRCMP		(1 << 17)		/* Software Reset Command in Progress */

#define AT91_RSTC_MR		(AT91_RSTC + 0x08)	/* Reset Controller Mode Register */
#define		AT91_RSTC_URSTEN	(1 << 0)		/* User Reset Enable */
#define		AT91_RSTC_URSTIEN	(1 << 4)		/* User Reset Interrupt Enable */
#define		AT91_RSTC_ERSTL		(0xf << 8)		/* External Reset Length */

#endif /* CONFIG_AT91_LEGACY */

#endif
