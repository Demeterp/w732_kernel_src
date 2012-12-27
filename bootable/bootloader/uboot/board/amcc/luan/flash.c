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
 * (C) Copyright 2002 Jun Gu <jung@artesyncp.com>
 * Add support for Am29F016D and dynamic switch setting.
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

#undef DEBUG
#ifdef DEBUG
#define DEBUGF(x...) printf(x)
#else
#define DEBUGF(x...)
#endif				/* DEBUG */

static unsigned long flash_addr_table[1][CONFIG_SYS_MAX_FLASH_BANKS] = {
	{0xff900000, 0xff980000, 0xffc00000},	/* 0:000: configuraton 3 */
};

/*
 * include common flash code (for amcc boards)
 */
#include "../common/flash.c"

/*-----------------------------------------------------------------------
 * Functions
 */
static ulong flash_get_size(vu_long * addr, flash_info_t * info);

unsigned long flash_init(void)
{
	unsigned long total_b = 0;
	unsigned long size_b[CONFIG_SYS_MAX_FLASH_BANKS];
	unsigned short index = 0;
	int i;

	/* read FPGA base register FPGA_REG0 */

	DEBUGF("\n");
	DEBUGF("FLASH: Index: %d\n", index);

	/* Init: no FLASHes known */
	for (i = 0; i < CONFIG_SYS_MAX_FLASH_BANKS; ++i) {
		flash_info[i].flash_id = FLASH_UNKNOWN;
		flash_info[i].sector_count = -1;
		flash_info[i].size = 0;

		/* check whether the address is 0 */
		if (flash_addr_table[index][i] == 0) {
			continue;
		}

		/* call flash_get_size() to initialize sector address */
		size_b[i] = flash_get_size((vu_long *)
					   flash_addr_table[index][i],
					   &flash_info[i]);
		flash_info[i].size = size_b[i];
		if (flash_info[i].flash_id == FLASH_UNKNOWN) {
			printf("## Unknown FLASH on Bank %d - Size = 0x%08lx = %ld MB\n",
			       i, size_b[i], size_b[i] << 20);
			flash_info[i].sector_count = -1;
			flash_info[i].size = 0;
		}

		/* Monitor protection ON by default */
		(void)flash_protect(FLAG_PROTECT_SET, CONFIG_SYS_MONITOR_BASE,
				    CONFIG_SYS_MONITOR_BASE + CONFIG_SYS_MONITOR_LEN - 1,
				    &flash_info[2]);
#ifdef CONFIG_ENV_IS_IN_FLASH
		(void)flash_protect(FLAG_PROTECT_SET, CONFIG_ENV_ADDR,
				    CONFIG_ENV_ADDR + CONFIG_ENV_SECT_SIZE - 1,
				    &flash_info[2]);
		(void)flash_protect(FLAG_PROTECT_SET, CONFIG_ENV_ADDR_REDUND,
				    CONFIG_ENV_ADDR_REDUND + CONFIG_ENV_SECT_SIZE - 1,
				    &flash_info[2]);
#endif

		total_b += flash_info[i].size;
	}

	return total_b;
}
