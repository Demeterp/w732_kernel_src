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
 * (C) Copyright 2008 Dmitry Rakhchev, EmCraft Systems, rda@emcraft.com
 *
 * Developed for DENX Software Engineering GmbH
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

/* This test attempts to verify board GDC. A scratch register tested, then
 * simple memory test (get_ram_size()) run over GDC memory.
 */

#include <post.h>

#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

#define GDC_SCRATCH_REG 0xC1FF8044
#define GDC_VERSION_REG 0xC1FF8084
#define GDC_RAM_START   0xC0000000
#define GDC_RAM_END     0xC2000000

#if CONFIG_POST & CONFIG_SYS_POST_BSPEC4

static int gdc_test_reg_one(uint value)
{
	int ret;
	uint read_value;

	/* write test pattern */
	out_be32((void *)GDC_SCRATCH_REG, value);
	/* read other location (protect against data lines capacity) */
	ret = in_be32((void *)GDC_RAM_START);
	/* verify test pattern */
	read_value = in_be32((void *)GDC_SCRATCH_REG);
	if (read_value != value) {
		post_log("GDC SCRATCH test failed write %08X, read %08X\n",
			value, read_value);
	}

	return (read_value != value);
}

/* Verify GDC, get memory size */
int gdc_post_test(int flags)
{
	uint   old_value;
	int    ret = 0;

	post_log("\n");
	old_value = in_be32((void *)GDC_SCRATCH_REG);

	/*
	 * GPIOC2 register behaviour: the LIME graphics processor has a
	 * maximum of 5 GPIO ports that can be used in this hardware
	 * configuration. Thus only the  bits  for these 5 GPIOs can be
	 * activated in the GPIOC2 register. All other bits will always be
	 * read as zero.
	 */
	if (gdc_test_reg_one(0x00150015))
		ret = 1;
	if (gdc_test_reg_one(0x000A000A))
		ret = 1;

	out_be32((void *)GDC_SCRATCH_REG, old_value);

	old_value = in_be32((void *)GDC_VERSION_REG);
	post_log("GDC chip version %u.%u, year %04X\n",
		(old_value >> 8) & 0xFF, old_value & 0xFF,
		(old_value >> 16) & 0xFFFF);

	old_value = get_ram_size((void *)GDC_RAM_START,
				 GDC_RAM_END - GDC_RAM_START);
	post_log("GDC RAM size: %d bytes\n", old_value);

	return ret;
}
#endif /* CONFIG_POST & CONFIG_SYS_POST_BSPEC4 */
