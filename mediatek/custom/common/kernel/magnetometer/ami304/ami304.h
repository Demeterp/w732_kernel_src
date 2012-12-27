/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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

/* include/linux/ami304.h - AMI304 compass driver
 *
 * Copyright (C) 2009 AMIT Technology Inc.
 * Author: Kyle Chen <sw-support@amit-inc.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*
 * Definitions for ami304 compass chip.
 */
#ifndef AMI304_H
#define AMI304_H

#include <linux/ioctl.h>
//#include <asm-arm/arch/regs-gpio.h>

#define AMI304_I2C_ADDRESS 			0x1E  //new Addr=0x0E(Low), old Addr=0x0F(High)

/* AMI304 Internal Register Address  (Please refer to AMI304 Specifications) */
#define AMI304_REG_CTRL1			0x1B
#define AMI304_REG_CTRL2			0x1C
#define AMI304_REG_CTRL3			0x1D
#define AMI304_REG_DATAXH			0x10
#define AMI304_REG_DATAXL			0x11
#define AMI304_REG_DATAYH			0x12
#define AMI304_REG_DATAYL			0x13
#define AMI304_REG_DATAZH			0x14
#define AMI304_REG_DATAZL			0x15
#define AMI304_REG_WIA				0x0F

/* AMI304 Control Bit  (Please refer to AMI304 Specifications) */
#define AMI304_CTRL1_PC1			0x80
#define AMI304_CTRL1_FS1_NORMAL			0x00 //Normal
#define AMI304_CTRL1_FS1_FORCE			0x02 //Force
#define AMI304_CTRL1_ODR1			0x10 //20SPS(20HZ)
#define AMI304_CTRL2_DREN			0x08
#define AMI304_CTRL2_DRP			0x04
#define AMI304_CTRL3_NOFORCE_BIT		0x00
#define AMI304_CTRL3_FORCE_BIT			0x40
#define AMI304_CTRL3_B0_LO_CLR			0x00

#define AMI304_BUFSIZE				256
#define AMI304_NORMAL_MODE			0
#define AMI304_FORCE_MODE			1
#define AMI304_IRQ				IRQ_EINT9

// conversion of magnetic data to nT units
#define CONVERT_M                       25
#define ORIENTATION_ACCURACY_RATE                   10


#endif
