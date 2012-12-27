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
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

#include <common.h>

/*
 * I2C test
 *
 * For verifying the I2C bus, a full I2C bus scanning is performed.
 *
 * #ifdef I2C_ADDR_LIST
 *   The test is considered as passed if all the devices and
 *   only the devices in the list are found.
 * #else [ ! I2C_ADDR_LIST ]
 *   The test is considered as passed if any I2C device is found.
 * #endif
 */

#include <post.h>
#include <i2c.h>

#if CONFIG_POST & CONFIG_SYS_POST_I2C

int i2c_post_test (int flags)
{
	unsigned int i;
	unsigned int good = 0;
#ifdef I2C_ADDR_LIST
	unsigned int bad  = 0;
	int j;
	unsigned char i2c_addr_list[] = I2C_ADDR_LIST;
	unsigned char i2c_miss_list[] = I2C_ADDR_LIST;
#endif

	for (i = 0; i < 128; i++) {
		if (i2c_probe (i) == 0) {
#ifndef	I2C_ADDR_LIST
			good++;
#else	/* I2C_ADDR_LIST */
			for (j=0; j<sizeof(i2c_addr_list); ++j) {
				if (i == i2c_addr_list[j]) {
					good++;
					i2c_miss_list[j] = 0xFF;
					break;
				}
			}
			if (j == sizeof(i2c_addr_list)) {
				bad++;
				post_log ("I2C: addr %02X not expected\n",
						i);
			}
#endif	/* I2C_ADDR_LIST */
		}
	}

#ifndef	I2C_ADDR_LIST
	return good > 0 ? 0 : -1;
#else	/* I2C_ADDR_LIST */
	if (good != sizeof(i2c_addr_list)) {
		for (j=0; j<sizeof(i2c_miss_list); ++j) {
			if (i2c_miss_list[j] != 0xFF) {
				post_log ("I2C: addr %02X did not respond\n",
						i2c_miss_list[j]);
			}
		}
	}
	return ((good == sizeof(i2c_addr_list)) && (bad == 0)) ? 0 : -1;
#endif
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_I2C */
