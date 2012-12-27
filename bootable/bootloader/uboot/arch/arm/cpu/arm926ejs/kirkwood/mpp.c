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
 * arch/arm/mach-kirkwood/mpp.c
 *
 * MPP functions for Marvell Kirkwood SoCs
 * Referenced from Linux kernel source
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <common.h>
#include <asm/arch/kirkwood.h>
#include <asm/arch/mpp.h>

static u32 kirkwood_variant(void)
{
	switch (readl(KW_REG_DEVICE_ID) & 0x03) {
	case 1:
		return MPP_F6192_MASK;
	case 2:
		return MPP_F6281_MASK;
	default:
		debug("MPP setup: unknown kirkwood variant\n");
		return 0;
	}
}

#define MPP_CTRL(i)	(KW_MPP_BASE + (i* 4))
#define MPP_NR_REGS	(1 + MPP_MAX/8)

void kirkwood_mpp_conf(u32 *mpp_list)
{
	u32 mpp_ctrl[MPP_NR_REGS];
	unsigned int variant_mask;
	int i;

	variant_mask = kirkwood_variant();
	if (!variant_mask)
		return;

	debug( "initial MPP regs:");
	for (i = 0; i < MPP_NR_REGS; i++) {
		mpp_ctrl[i] = readl(MPP_CTRL(i));
		debug(" %08x", mpp_ctrl[i]);
	}
	debug("\n");


	while (*mpp_list) {
		unsigned int num = MPP_NUM(*mpp_list);
		unsigned int sel = MPP_SEL(*mpp_list);
		int shift;

		if (num > MPP_MAX) {
			debug("kirkwood_mpp_conf: invalid MPP "
					"number (%u)\n", num);
			continue;
		}
		if (!(*mpp_list & variant_mask)) {
			debug("kirkwood_mpp_conf: requested MPP%u config "
				"unavailable on this hardware\n", num);
			continue;
		}

		shift = (num & 7) << 2;
		mpp_ctrl[num / 8] &= ~(0xf << shift);
		mpp_ctrl[num / 8] |= sel << shift;

		mpp_list++;
	}

	debug("  final MPP regs:");
	for (i = 0; i < MPP_NR_REGS; i++) {
		writel(mpp_ctrl[i], MPP_CTRL(i));
		debug(" %08x", mpp_ctrl[i]);
	}
	debug("\n");

}
