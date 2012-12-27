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
 * (C) Copyright 2001
 * Stefan Roese, esd gmbh germany, stefan.roese@esd-electronics.com
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
#include <ppc4xx.h>
#include <asm/processor.h>

/*
 * include common flash code (for esd boards)
 */
#include "../common/flash.c"

/*-----------------------------------------------------------------------
 * Functions
 */
static ulong flash_get_size (vu_long *addr, flash_info_t *info);
static void flash_get_offsets (ulong base, flash_info_t *info);

/*-----------------------------------------------------------------------
 */

unsigned long flash_init (void)
{
	unsigned long size_b0, size_b1;
	int i;

	/* Init: no FLASHes known */
	for (i=0; i<CONFIG_SYS_MAX_FLASH_BANKS; ++i) {
		flash_info[i].flash_id = FLASH_UNKNOWN;
	}

	size_b0 = flash_get_size((vu_long *)FLASH_BASE0_PRELIM, &flash_info[0]);

	if (flash_info[0].flash_id == FLASH_UNKNOWN) {
		printf ("## Unknown FLASH on Bank 0 - Size = 0x%08lx = %ld MB\n",
			size_b0, size_b0<<20);
	}

	size_b1 = flash_get_size((vu_long *)FLASH_BASE1_PRELIM, &flash_info[1]);

	if (size_b1 > size_b0) {
		printf ("## ERROR: "
			"Bank 1 (0x%08lx = %ld MB) > Bank 0 (0x%08lx = %ld MB)\n",
			size_b1, size_b1<<20,
			size_b0, size_b0<<20
		);
		flash_info[0].flash_id	= FLASH_UNKNOWN;
		flash_info[1].flash_id	= FLASH_UNKNOWN;
		flash_info[0].sector_count	= -1;
		flash_info[1].sector_count	= -1;
		flash_info[0].size		= 0;
		flash_info[1].size		= 0;
		return (0);
	}

	/* Re-do sizing to get full correct info */
	size_b0 = flash_get_size((vu_long *)FLASH_BASE0_PRELIM, &flash_info[0]);

	flash_get_offsets (FLASH_BASE0_PRELIM, &flash_info[0]);

	/* monitor protection ON by default */
	flash_protect(FLAG_PROTECT_SET,
		      FLASH_BASE0_PRELIM+size_b0-monitor_flash_len,
		      FLASH_BASE0_PRELIM+size_b0-1,
		      &flash_info[0]);

	if (size_b1) {
		/* Re-do sizing to get full correct info */
		size_b1 = flash_get_size((vu_long *)(FLASH_BASE0_PRELIM + size_b0),
					  &flash_info[1]);

		flash_get_offsets (FLASH_BASE0_PRELIM + size_b0, &flash_info[1]);

		/* monitor protection ON by default */
		flash_protect(FLAG_PROTECT_SET,
			      FLASH_BASE0_PRELIM+size_b0+size_b1-monitor_flash_len,
			      FLASH_BASE0_PRELIM+size_b0+size_b1-1,
			      &flash_info[1]);
		/* monitor protection OFF by default (one is enough) */
		flash_protect(FLAG_PROTECT_CLEAR,
			      FLASH_BASE0_PRELIM+size_b0-monitor_flash_len,
			      FLASH_BASE0_PRELIM+size_b0-1,
			      &flash_info[0]);
	} else {
		flash_info[1].flash_id = FLASH_UNKNOWN;
		flash_info[1].sector_count = -1;
	}

	flash_info[0].size = size_b0;
	flash_info[1].size = size_b1;

	return (size_b0 + size_b1);
}
