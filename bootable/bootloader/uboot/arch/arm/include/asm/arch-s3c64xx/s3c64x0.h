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
 * (C) Copyright 2003
 * David MÃŒller ELSOFT AG Switzerland. d.mueller@elsoft.ch
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/************************************************
 * NAME	    : S3C64XX.h
 * Version  : 31.3.2003
 *
 * common stuff for SAMSUNG S3C64XX SoC
 ************************************************/

#ifndef __S3C64XX_H__
#define __S3C64XX_H__

#if defined(CONFIG_SYNC_MODE) && defined(CONFIG_S3C6400)
#error CONFIG_SYNC_MODE unavailable on S3C6400, please, fix your configuration!
#endif

#include <asm/types.h>

/* UART (see manual chapter 11) */
typedef struct {
	volatile u32	ULCON;
	volatile u32	UCON;
	volatile u32	UFCON;
	volatile u32	UMCON;
	volatile u32	UTRSTAT;
	volatile u32	UERSTAT;
	volatile u32	UFSTAT;
	volatile u32	UMSTAT;
#ifdef __BIG_ENDIAN
	volatile u8	res1[3];
	volatile u8	UTXH;
	volatile u8	res2[3];
	volatile u8	URXH;
#else /* Little Endian */
	volatile u8	UTXH;
	volatile u8	res1[3];
	volatile u8	URXH;
	volatile u8	res2[3];
#endif
	volatile u32	UBRDIV;
#ifdef __BIG_ENDIAN
	volatile u8	res3[2];
	volatile u16	UDIVSLOT;
#else
	volatile u16	UDIVSLOT;
	volatile u8	res3[2];
#endif
} s3c64xx_uart;

/* PWM TIMER (see manual chapter 10) */
typedef struct {
	volatile u32	TCNTB;
	volatile u32	TCMPB;
	volatile u32	TCNTO;
} s3c64xx_timer;

typedef struct {
	volatile u32	TCFG0;
	volatile u32	TCFG1;
	volatile u32	TCON;
	s3c64xx_timer	ch[4];
	volatile u32	TCNTB4;
	volatile u32	TCNTO4;
} s3c64xx_timers;

#endif /*__S3C64XX_H__*/
