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
 * Pulse Width Modulation Memory Map
 *
 * Copyright (C) 2004-2008 Freescale Semiconductor, Inc.
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

#ifndef __ATA_H__
#define __ATA_H__

/* Pulse Width Modulation (PWM) */
typedef struct pwm_ctrl {
#ifdef CONFIG_M5272
	u8 cr0;
	u8 res1[3];
	u8 cr1;
	u8 res2[3];
	u8 cr2;
	u8 res3[7];
	u8 pwr0;
	u8 res4[3];
	u8 pwr1;
	u8 res5[3];
	u8 pwr2;
	u8 res6[7];
#else
	u8 en;			/* 0x00 PWM Enable */
	u8 pol;			/* 0x01 Polarity */
	u8 clk;			/* 0x02 Clock Select */
	u8 prclk;		/* 0x03 Prescale Clock Select */
	u8 cae;			/* 0x04 Center Align Enable */
	u8 ctl;			/* 0x05 Control */
	u16 res1;		/* 0x06 - 0x07 */
	u8 scla;		/* 0x08 Scale A */
	u8 sclb;		/* 0x09 Scale B */
	u16 res2;		/* 0x0A - 0x0B */
#ifdef CONFIG_M5275
	u8 cnt[4];		/* 0x0C Channel n Counter */
	u16 res3;		/* 0x10 - 0x11 */
	u8 per[4];		/* 0x14 Channel n Period */
	u16 res4;		/* 0x16 - 0x17 */
	u8 dty[4];		/* 0x18 Channel n Duty */
#else
	u8 cnt[8];		/* 0x0C Channel n Counter */
	u8 per[8];		/* 0x14 Channel n Period */
	u8 dty[8];		/* 0x1C Channel n Duty */
	u8 sdn;			/* 0x24 Shutdown */
	u8 res3[3];		/* 0x25 - 0x27 */
#endif				/* CONFIG_M5275 */
#endif				/* CONFIG_M5272 */
} pwm_t;

#ifdef CONFIG_M5272

#define PWM_CR_EN			(0x80)
#define PWM_CR_FRC1			(0x40)
#define PWM_CR_LVL			(0x20)
#define PWM_CR_CLKSEL(x)		((x) & 0x0F)
#define PWM_CR_CLKSEL_MASK		(0xF0)

#else

#define PWM_EN_PWMEn(x)			(1 << ((x) & 0x07))
#define PWM_EN_PWMEn_MASK		(0xF0)

#define PWM_POL_PPOLn(x)		(1 << ((x) & 0x07))
#define PWM_POL_PPOLn_MASK		(0xF0)

#define PWM_CLK_PCLKn(x)		(1 << ((x) & 0x07))
#define PWM_CLK_PCLKn_MASK		(0xF0)

#define PWM_PRCLK_PCKB(x)		(((x) & 0x07) << 4)
#define PWM_PRCLK_PCKB_MASK		(0x8F)
#define PWM_PRCLK_PCKA(x)		((x) & 0x07)
#define PWM_PRCLK_PCKA_MASK		(0xF8)

#define PWM_CLK_PCLKn(x)		(1 << ((x) & 0x07))
#define PWM_CLK_PCLKn_MASK		(0xF0)

#define PWM_CTL_CON67			(0x80)
#define PWM_CTL_CON45			(0x40)
#define PWM_CTL_CON23			(0x20)
#define PWM_CTL_CON01			(0x10)
#define PWM_CTL_PSWAR			(0x08)
#define PWM_CTL_PFRZ			(0x04)

#define PWM_SDN_IF			(0x80)
#define PWM_SDN_IE			(0x40)
#define PWM_SDN_RESTART			(0x20)
#define PWM_SDN_LVL			(0x10)
#define PWM_SDN_PWM7IN			(0x04)
#define PWM_SDN_PWM7IL			(0x02)
#define PWM_SDN_SDNEN			(0x01)

#endif				/* CONFIG_M5272 */

#endif				/* __ATA_H__ */
