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
 * (C) Copyright 2000
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

/*
 * Modified 4/5/2001
 * Wait for completion of each sector erase command issued
 * 4/5/2001
 * Chris Hallinan - DS4.COM, Inc. - clh@net1plus.com
 */

#include <common.h>
#include <ppc4xx.h>
#include <asm/processor.h>

flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];	/* info for FLASH chips        */

#undef DEBUG
#ifdef DEBUG
#define DEBUGF(x...) printf(x)
#else
#define DEBUGF(x...)
#endif				/* DEBUG */

/*
 * include common flash code (for amcc boards)
 */
#include "../common/flash.c"

/*-----------------------------------------------------------------------
 * Functions
 */
static ulong flash_get_size(vu_long * addr, flash_info_t * info);
static void flash_get_offsets(ulong base, flash_info_t * info);

unsigned long flash_init(void)
{
	unsigned long size_b0, size_b1;
	int i;
	uint pbcr;
	unsigned long base_b0, base_b1;

	/* Init: no FLASHes known */
	for (i = 0; i < CONFIG_SYS_MAX_FLASH_BANKS; ++i) {
		flash_info[i].flash_id = FLASH_UNKNOWN;
	}

	/* Static FLASH Bank configuration here - FIXME XXX */

	size_b0 =
	    flash_get_size((vu_long *) FLASH_BASE0_PRELIM, &flash_info[0]);

	if (flash_info[0].flash_id == FLASH_UNKNOWN) {
		printf("## Unknown FLASH on Bank 0 - Size = 0x%08lx = %ld MB\n",
		       size_b0, size_b0 << 20);
	}

	/* Only one bank */
	if (CONFIG_SYS_MAX_FLASH_BANKS == 1) {
		/* Setup offsets */
		flash_get_offsets(FLASH_BASE0_PRELIM, &flash_info[0]);

		/* Monitor protection ON by default */
		(void)flash_protect(FLAG_PROTECT_SET,
				    CONFIG_SYS_MONITOR_BASE,
				    CONFIG_SYS_MONITOR_BASE + CONFIG_SYS_MONITOR_LEN - 1,
				    &flash_info[0]);
#ifdef CONFIG_ENV_IS_IN_FLASH
		(void)flash_protect(FLAG_PROTECT_SET, CONFIG_ENV_ADDR,
				    CONFIG_ENV_ADDR + CONFIG_ENV_SECT_SIZE - 1,
				    &flash_info[0]);
		(void)flash_protect(FLAG_PROTECT_SET, CONFIG_ENV_ADDR_REDUND,
				    CONFIG_ENV_ADDR_REDUND + CONFIG_ENV_SECT_SIZE - 1,
				    &flash_info[0]);
#endif

		size_b1 = 0;
		flash_info[0].size = size_b0;
	}

	/* 2 banks */
	else {
		size_b1 =
		    flash_get_size((vu_long *) FLASH_BASE1_PRELIM,
				   &flash_info[1]);

		/* Re-do sizing to get full correct info */

		if (size_b1) {
			mtdcr(EBC0_CFGADDR, PB0CR);
			pbcr = mfdcr(EBC0_CFGDATA);
			mtdcr(EBC0_CFGADDR, PB0CR);
			base_b1 = -size_b1;
			pbcr = (pbcr & 0x0001ffff) | base_b1 |
			    (((size_b1 / 1024 / 1024) - 1) << 17);
			mtdcr(EBC0_CFGDATA, pbcr);
			/*          printf("PB1CR = %x\n", pbcr); */
		}

		if (size_b0) {
			mtdcr(EBC0_CFGADDR, PB1CR);
			pbcr = mfdcr(EBC0_CFGDATA);
			mtdcr(EBC0_CFGADDR, PB1CR);
			base_b0 = base_b1 - size_b0;
			pbcr = (pbcr & 0x0001ffff) | base_b0 |
			    (((size_b0 / 1024 / 1024) - 1) << 17);
			mtdcr(EBC0_CFGDATA, pbcr);
			/*            printf("PB0CR = %x\n", pbcr); */
		}

		size_b0 = flash_get_size((vu_long *) base_b0, &flash_info[0]);

		flash_get_offsets(base_b0, &flash_info[0]);

		/* monitor protection ON by default */
		(void)flash_protect(FLAG_PROTECT_SET,
				    base_b0 + size_b0 - CONFIG_SYS_MONITOR_LEN,
				    base_b0 + size_b0 - 1, &flash_info[0]);
		/* Also protect sector containing initial power-up instruction */
		/* (flash_protect() checks address range - other call ignored) */
		(void)flash_protect(FLAG_PROTECT_SET,
				    0xFFFFFFFC, 0xFFFFFFFF, &flash_info[0]);
		(void)flash_protect(FLAG_PROTECT_SET,
				    0xFFFFFFFC, 0xFFFFFFFF, &flash_info[1]);

		if (size_b1) {
			/* Re-do sizing to get full correct info */
			size_b1 =
			    flash_get_size((vu_long *) base_b1, &flash_info[1]);

			flash_get_offsets(base_b1, &flash_info[1]);

			/* monitor protection ON by default */
			(void)flash_protect(FLAG_PROTECT_SET,
					    base_b1 + size_b1 - CONFIG_SYS_MONITOR_LEN,
					    base_b1 + size_b1 - 1,
					    &flash_info[1]);
			/* monitor protection OFF by default (one is enough) */
			(void)flash_protect(FLAG_PROTECT_CLEAR,
					    base_b0 + size_b0 - CONFIG_SYS_MONITOR_LEN,
					    base_b0 + size_b0 - 1,
					    &flash_info[0]);
		} else {
			flash_info[1].flash_id = FLASH_UNKNOWN;
			flash_info[1].sector_count = -1;
		}

		flash_info[0].size = size_b0;
		flash_info[1].size = size_b1;
	}			/* else 2 banks */
	return (size_b0 + size_b1);
}

static void flash_get_offsets(ulong base, flash_info_t * info)
{
	int i;

	/* set up sector start address table */
	if (((info->flash_id & FLASH_VENDMASK) == FLASH_MAN_SST) ||
	    (info->flash_id == FLASH_AM040)) {
		for (i = 0; i < info->sector_count; i++)
			info->start[i] = base + (i * 0x00010000);
	} else {
		if (info->flash_id & FLASH_BTYPE) {
			/* set sector offsets for bottom boot block type        */
			info->start[0] = base + 0x00000000;
			info->start[1] = base + 0x00004000;
			info->start[2] = base + 0x00006000;
			info->start[3] = base + 0x00008000;
			for (i = 4; i < info->sector_count; i++) {
				info->start[i] =
				    base + (i * 0x00010000) - 0x00030000;
			}
		} else {
			/* set sector offsets for top boot block type           */
			i = info->sector_count - 1;
			info->start[i--] = base + info->size - 0x00004000;
			info->start[i--] = base + info->size - 0x00006000;
			info->start[i--] = base + info->size - 0x00008000;
			for (; i >= 0; i--) {
				info->start[i] = base + i * 0x00010000;
			}
		}
	}
}
