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
 * (C) Copyright 2005
 * 2N Telekomunikace, a.s. <www.2n.cz>
 * Ladislav Michl <michl@2n.cz>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
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
#include <nand.h>

#ifndef CONFIG_SYS_NAND_BASE_LIST
#define CONFIG_SYS_NAND_BASE_LIST { CONFIG_SYS_NAND_BASE }
#endif

DECLARE_GLOBAL_DATA_PTR;

int nand_curr_device = -1;
nand_info_t nand_info[CONFIG_SYS_MAX_NAND_DEVICE];

static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];
static ulong base_address[CONFIG_SYS_MAX_NAND_DEVICE] = CONFIG_SYS_NAND_BASE_LIST;

static const char default_nand_name[] = "nand";
static __attribute__((unused)) char dev_name[CONFIG_SYS_MAX_NAND_DEVICE][8];

static void nand_init_chip(struct mtd_info *mtd, struct nand_chip *nand,
			   ulong base_addr)
{
	int maxchips = CONFIG_SYS_NAND_MAX_CHIPS;
	int __attribute__((unused)) i = 0;

	if (maxchips < 1)
		maxchips = 1;
	mtd->priv = nand;

	nand->IO_ADDR_R = nand->IO_ADDR_W = (void  __iomem *)base_addr;
	if (board_nand_init(nand) == 0) {
		if (nand_scan(mtd, maxchips) == 0) {
			if (!mtd->name)
				mtd->name = (char *)default_nand_name;
#ifndef CONFIG_RELOC_FIXUP_WORKS
			else
				mtd->name += gd->reloc_off;
#endif

#ifdef CONFIG_MTD_DEVICE
			/*
			 * Add MTD device so that we can reference it later
			 * via the mtdcore infrastructure (e.g. ubi).
			 */
			sprintf(dev_name[i], "nand%d", i);
			mtd->name = dev_name[i++];
			add_mtd_device(mtd);
#endif
		} else
			mtd->name = NULL;
	} else {
		mtd->name = NULL;
		mtd->size = 0;
	}

}

void nand_init(void)
{
	int i;
	unsigned int size = 0;
	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++) {
		nand_init_chip(&nand_info[i], &nand_chip[i], base_address[i]);
		size += nand_info[i].size / 1024;
		if (nand_curr_device == -1)
			nand_curr_device = i;
	}
	printf("%u MiB\n", size / 1024);

#ifdef CONFIG_SYS_NAND_SELECT_DEVICE
	/*
	 * Select the chip in the board/cpu specific driver
	 */
	board_nand_select_device(nand_info[nand_curr_device].priv, nand_curr_device);
#endif
}
