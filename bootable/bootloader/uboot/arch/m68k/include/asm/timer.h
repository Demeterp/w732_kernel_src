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
 * timer.h -- ColdFire internal TIMER support defines.
 *
 * Copyright (C) 2004-2007 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
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

/****************************************************************************/
#ifndef	timer_h
#define	timer_h
/****************************************************************************/

/****************************************************************************/
/* Timer structure */
/****************************************************************************/
/* DMA Timer module registers */
typedef struct dtimer_ctrl {
#if defined(CONFIG_M5249) || defined(CONFIG_M5253) || defined(CONFIG_M5272)
	u16 tmr;		/* 0x00 Mode register */
	u16 res1;		/* 0x02 */
	u16 trr;		/* 0x04 Reference register */
	u16 res2;		/* 0x06 */
	u16 tcr;		/* 0x08 Capture register */
	u16 res3;		/* 0x0A */
	u16 tcn;		/* 0x0C Counter register */
	u16 res4;		/* 0x0E */
	u8 res6;		/* 0x10 */
	u8 ter;			/* 0x11 Event register */
	u16 res7;		/* 0x12 */
#else
	u16 tmr;		/* 0x00 Mode register */
	u8 txmr;		/* 0x02 Extended Mode register */
	u8 ter;			/* 0x03 Event register */
	u32 trr;		/* 0x04 Reference register */
	u32 tcr;		/* 0x08 Capture register */
	u32 tcn;		/* 0x0C Counter register */
#endif
} dtmr_t;

/*Programmable Interrupt Timer */
typedef struct pit_ctrl {
	u16 pcsr;		/* 0x00 Control and Status Register */
	u16 pmr;		/* 0x02 Modulus Register */
	u16 pcntr;		/* 0x04 Count Register */
} pit_t;

/*********************************************************************
* DMA Timers (DTIM)
*********************************************************************/
/* Bit definitions and macros for DTMR */
#define DTIM_DTMR_RST		(0x0001)	/* Reset */
#define DTIM_DTMR_CLK(x)	(((x)&0x0003)<<1)	/* Input clock source */
#define DTIM_DTMR_FRR		(0x0008)	/* Free run/restart */
#define DTIM_DTMR_ORRI		(0x0010)	/* Output reference request/interrupt enable */
#define DTIM_DTMR_OM		(0x0020)	/* Output Mode */
#define DTIM_DTMR_CE(x)		(((x)&0x0003)<<6)	/* Capture Edge */
#define DTIM_DTMR_PS(x)		(((x)&0x00FF)<<8)	/* Prescaler value */
#define DTIM_DTMR_RST_EN	(0x0001)
#define DTIM_DTMR_RST_RST	(0x0000)
#define DTIM_DTMR_CE_ANY	(0x00C0)
#define DTIM_DTMR_CE_FALL	(0x0080)
#define DTIM_DTMR_CE_RISE	(0x0040)
#define DTIM_DTMR_CE_NONE	(0x0000)
#define DTIM_DTMR_CLK_DTIN	(0x0006)
#define DTIM_DTMR_CLK_DIV16	(0x0004)
#define DTIM_DTMR_CLK_DIV1	(0x0002)
#define DTIM_DTMR_CLK_STOP	(0x0000)

/* Bit definitions and macros for DTXMR */
#define DTIM_DTXMR_MODE16	(0x01)	/* Increment Mode */
#define DTIM_DTXMR_DMAEN	(0x80)	/* DMA request */

/* Bit definitions and macros for DTER */
#define DTIM_DTER_CAP		(0x01)	/* Capture event */
#define DTIM_DTER_REF		(0x02)	/* Output reference event */

/*********************************************************************
*
* Programmable Interrupt Timer Modules (PIT)
*
*********************************************************************/

/* Bit definitions and macros for PCSR */
#define PIT_PCSR_EN		(0x0001)
#define PIT_PCSR_RLD		(0x0002)
#define PIT_PCSR_PIF		(0x0004)
#define PIT_PCSR_PIE		(0x0008)
#define PIT_PCSR_OVW		(0x0010)
#define PIT_PCSR_HALTED		(0x0020)
#define PIT_PCSR_DOZE		(0x0040)
#define PIT_PCSR_PRE(x)		(((x)&0x000F)<<8)

/* Bit definitions and macros for PMR */
#define PIT_PMR_PM(x)		(x)

/* Bit definitions and macros for PCNTR */
#define PIT_PCNTR_PC(x)		(x)

/****************************************************************************/
#endif				/* timer_h */
