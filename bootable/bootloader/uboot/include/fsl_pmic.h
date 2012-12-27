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
 * (C) Copyright 2010
 * Stefano Babic, DENX Software Engineering, sbabic@denx.de.
 *
 * (C) Copyright 2009 Freescale Semiconductor, Inc.
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

#ifndef __FSL_PMIC_H__
#define __FSL_PMIC_H__

/*
 * The registers of different PMIC has the same meaning
 * but the bit positions of the fields can differ or
 * some fields has a meaning only on some devices.
 * You have to check with the internal SPI bitmap
 * (see Freescale Documentation) to set the registers
 * for the device you are using
 */
enum {
	REG_INT_STATUS0 = 0,
	REG_INT_MASK0,
	REG_INT_SENSE0,
	REG_INT_STATUS1,
	REG_INT_MASK1,
	REG_INT_SENSE1,
	REG_PU_MODE_S,
	REG_IDENTIFICATION,
	REG_UNUSED0,
	REG_ACC0,
	REG_ACC1,		/*10 */
	REG_UNUSED1,
	REG_UNUSED2,
	REG_POWER_CTL0,
	REG_POWER_CTL1,
	REG_POWER_CTL2,
	REG_REGEN_ASSIGN,
	REG_UNUSED3,
	REG_MEM_A,
	REG_MEM_B,
	REG_RTC_TIME,		/*20 */
	REG_RTC_ALARM,
	REG_RTC_DAY,
	REG_RTC_DAY_ALARM,
	REG_SW_0,
	REG_SW_1,
	REG_SW_2,
	REG_SW_3,
	REG_SW_4,
	REG_SW_5,
	REG_SETTING_0,		/*30 */
	REG_SETTING_1,
	REG_MODE_0,
	REG_MODE_1,
	REG_POWER_MISC,
	REG_UNUSED4,
	REG_UNUSED5,
	REG_UNUSED6,
	REG_UNUSED7,
	REG_UNUSED8,
	REG_UNUSED9,		/*40 */
	REG_UNUSED10,
	REG_UNUSED11,
	REG_ADC0,
	REG_ADC1,
	REG_ADC2,
	REG_ADC3,
	REG_ADC4,
	REG_CHARGE,
	REG_USB0,
	REG_USB1,		/*50 */
	REG_LED_CTL0,
	REG_LED_CTL1,
	REG_LED_CTL2,
	REG_LED_CTL3,
	REG_UNUSED12,
	REG_UNUSED13,
	REG_TRIM0,
	REG_TRIM1,
	REG_TEST0,
	REG_TEST1,		/*60 */
	REG_TEST2,
	REG_TEST3,
	REG_TEST4,
};

/* REG_POWER_MISC */
#define GPO1EN		(1 << 6)
#define GPO1STBY	(1 << 7)
#define GPO2EN		(1 << 8)
#define GPO2STBY	(1 << 9)
#define GPO3EN		(1 << 10)
#define GPO3STBY	(1 << 11)
#define GPO4EN		(1 << 12)
#define GPO4STBY	(1 << 13)
#define PWGT1SPIEN	(1 << 15)
#define PWGT2SPIEN	(1 << 16)
#define PWUP		(1 << 21)

/* Power Control 0 */
#define COINCHEN	(1 << 23)
#define BATTDETEN	(1 << 19)

/* Interrupt status 1 */
#define RTCRSTI		(1 << 7)

void pmic_show_pmic_info(void);
void pmic_reg_write(u32 reg, u32 value);
u32 pmic_reg_read(u32 reg);

#endif