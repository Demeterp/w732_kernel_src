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

/* linux/drivers/hwmon/adxl345.c
 *
 * (C) Copyright 2008 
 * MediaTek <www.mediatek.com>
 *
 * BMA150 driver for MT6516
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  BMA150
 */
#ifndef BMA150_H
#define BMA150_H
	 
#include <linux/ioctl.h>
	 
	#define BMA150_I2C_SLAVE_WRITE_ADDR		0x70
	 
	 /* BMA150 Register Map  (Please refer to BMA150 Specifications) */
	#define BMA150_REG_DEVID			0x00
	#define BMA150_FIXED_DEVID			0x02
	#define BMA150_REG_OFSX				0x16
	#define BMA150_REG_OFSX_HIGH		0x1A
	#define	BMA150_REG_BW_RATE			0x14
	#define BMA150_BW_MASK				0x07
	#define BMA150_BW_200HZ				0x03
	#define BMA150_BW_100HZ				0x02
	#define BMA150_BW_50HZ				0x01
	#define BMA150_BW_25HZ				0x00
	#define BMA150_REG_POWER_CTL		0x0A		
	#define BMA150_REG_DATA_FORMAT		0x14
	#define BMA150_RANGE_MASK			0x18
	#define BMA150_RANGE_2G				0x00
	#define BMA150_RANGE_4G				0x08
	#define BMA150_RANGE_8G				0x10
	#define BMA150_REG_DATAXLOW			0x02
	#define BMA150_REG_DATA_RESOLUTION	0x14
	#define BMA150_MEASURE_MODE			0x01	
	#define BMA150_SELF_TEST           	0x04
	#define BMA150_INT_REG           	0x0B

	
#define BMA150_SUCCESS						0
#define BMA150_ERR_I2C						-1
#define BMA150_ERR_STATUS					-3
#define BMA150_ERR_SETUP_FAILURE			-4
#define BMA150_ERR_GETGSENSORDATA			-5
#define BMA150_ERR_IDENTIFICATION			-6
	 
	 
	 
#define BMA150_BUFSIZE				256
	 
#endif

