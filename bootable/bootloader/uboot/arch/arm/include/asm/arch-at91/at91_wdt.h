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
 * [origin: Linux kernel arch/arm/mach-at91/include/mach/at91_wdt.h]
 *
 * Copyright (C) 2008 Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 * Copyright (C) 2007 Andrew Victor
 * Copyright (C) 2007 Atmel Corporation.
 *
 * Watchdog Timer (WDT) - System peripherals regsters.
 * Based on AT91SAM9261 datasheet revision D.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_WDT_H
#define AT91_WDT_H

#ifdef __ASSEMBLY__

#define AT91_ASM_WDT_MR	(AT91_WDT_BASE +  0x04)

#else

typedef struct at91_wdt {
	u32	cr;
	u32	mr;
	u32	sr;
} at91_wdt_t;

#endif

#define AT91_WDT_CR_WDRSTT		1
#define AT91_WDT_CR_KEY			0xa5000000	/* KEY Password */

#define AT91_WDT_MR_WDV(x)		(x & 0xfff)
#define AT91_WDT_MR_WDFIEN		0x00001000
#define AT91_WDT_MR_WDRSTEN		0x00002000
#define AT91_WDT_MR_WDRPROC		0x00004000
#define AT91_WDT_MR_WDDIS		0x00008000
#define AT91_WDT_MR_WDD(x)		((x & 0xfff) << 16)
#define AT91_WDT_MR_WDDBGHLT		0x10000000
#define AT91_WDT_MR_WDIDLEHLT		0x20000000

#ifdef CONFIG_AT91_LEGACY

#define AT91_WDT_CR		(AT91_WDT + 0x00)	/* Watchdog Control Register */
#define		AT91_WDT_WDRSTT		(1    << 0)		/* Restart */
#define		AT91_WDT_KEY		(0xa5 << 24)		/* KEY Password */

#define AT91_WDT_MR		(AT91_WDT + 0x04)	/* Watchdog Mode Register */
#define		AT91_WDT_WDV		(0xfff << 0)		/* Counter Value */
#define		AT91_WDT_WDFIEN		(1     << 12)		/* Fault Interrupt Enable */
#define		AT91_WDT_WDRSTEN	(1     << 13)		/* Reset Processor */
#define		AT91_WDT_WDRPROC	(1     << 14)		/* Timer Restart */
#define		AT91_WDT_WDDIS		(1     << 15)		/* Watchdog Disable */
#define		AT91_WDT_WDD		(0xfff << 16)		/* Delta Value */
#define		AT91_WDT_WDDBGHLT	(1     << 28)		/* Debug Halt */
#define		AT91_WDT_WDIDLEHLT	(1     << 29)		/* Idle Halt */

#define AT91_WDT_SR		(AT91_WDT + 0x08)	/* Watchdog Status Register */
#define		AT91_WDT_WDUNF		(1 << 0)		/* Watchdog Underflow */
#define		AT91_WDT_WDERR		(1 << 1)		/* Watchdog Error */

#endif /* CONFIG_AT91_LEGACY */
#endif
