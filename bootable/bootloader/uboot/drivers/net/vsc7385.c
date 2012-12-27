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
 * Vitesse 7385 Switch Firmware Upload
 *
 * Author: Timur Tabi <timur@freescale.com>
 *
 * Copyright 2008 Freescale Semiconductor, Inc.  This file is licensed
 * under the terms of the GNU General Public License version 2.  This
 * program is licensed "as is" without any warranty of any kind, whether
 * express or implied.
 *
 * This module uploads proprietary firmware for the Vitesse VSC7385 5-port
 * switch.
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>

/*
 * Upload a Vitesse VSC7385 firmware image to the hardware
 *
 * This function takes a pointer to a VSC7385 firmware image and a size, and
 * uploads that firmware to the VSC7385.
 *
 * This firmware is typically located at a board-specific flash address,
 * and the size is typically 8KB.
 *
 * The firmware is Vitesse proprietary.
 *
 * Further details on the register information can be obtained from Vitesse.
 */
int vsc7385_upload_firmware(void *firmware, unsigned int size)
{
	u8 *fw = firmware;
	unsigned int i;

	u32 *gloreset = (u32 *) (CONFIG_SYS_VSC7385_BASE + 0x1c050);
	u32 *icpu_ctrl = (u32 *) (CONFIG_SYS_VSC7385_BASE + 0x1c040);
	u32 *icpu_addr = (u32 *) (CONFIG_SYS_VSC7385_BASE + 0x1c044);
	u32 *icpu_data = (u32 *) (CONFIG_SYS_VSC7385_BASE + 0x1c048);
	u32 *icpu_rom_map = (u32 *) (CONFIG_SYS_VSC7385_BASE + 0x1c070);
#ifdef DEBUG
	u32 *chipid = (u32 *) (CONFIG_SYS_VSC7385_BASE + 0x1c060);
#endif

	out_be32(gloreset, 3);
	udelay(200);

	out_be32(icpu_ctrl, 0x8E);
	udelay(20);

	out_be32(icpu_rom_map, 1);
	udelay(20);

	/* Write the firmware to I-RAM */
	out_be32(icpu_addr, 0);
	udelay(20);

	for (i = 0; i < size; i++) {
		out_be32(icpu_data, fw[i]);
		udelay(20);
		if (ctrlc())
			return -EINTR;
	}

	/* Read back and compare */
	out_be32(icpu_addr, 0);
	udelay(20);

	for (i = 0; i < size; i++) {
		u8 value;

		value = (u8) in_be32(icpu_data);
		udelay(20);
		if (value != fw[i]) {
			debug("VSC7385: Upload mismatch: address 0x%x, "
			      "read value 0x%x, image value 0x%x\n",
			      i, value, fw[i]);

			return -EIO;
		}
		if (ctrlc())
			break;
	}

	out_be32(icpu_ctrl, 0x0B);
	udelay(20);

#ifdef DEBUG
	printf("VSC7385: Chip ID is %08x\n", in_be32(chipid));
	udelay(20);
#endif

	return 0;
}
