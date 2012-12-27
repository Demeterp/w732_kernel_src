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
static ulong flash_get_size (vu_long * addr, flash_info_t * info);
static void flash_get_offsets (ulong base, flash_info_t * info);

/*-----------------------------------------------------------------------
 */

unsigned long flash_init (void)
{
#ifdef __DEBUG_START_FROM_SRAM__
	return CONFIG_SYS_DUMMY_FLASH_SIZE;
#else
	unsigned long size;
	int i;
	uint pbcr;
	unsigned long base;
	int size_val = 0;

	debug("[%s, %d] Entering ...\n", __FUNCTION__, __LINE__);
	debug("[%s, %d] flash_info = 0x%08X ...\n", __FUNCTION__, __LINE__, flash_info);

	/* Init: no FLASHes known */
	for (i=0; i<CONFIG_SYS_MAX_FLASH_BANKS; ++i) {
		flash_info[i].flash_id = FLASH_UNKNOWN;
	}

	/* Static FLASH Bank configuration here - FIXME XXX */

	debug("[%s, %d] Calling flash_get_size ...\n", __FUNCTION__, __LINE__);
	size = flash_get_size((vu_long *)FLASH_BASE0_PRELIM, &flash_info[0]);

	if (flash_info[0].flash_id == FLASH_UNKNOWN) {
		printf ("## Unknown FLASH on Bank 0 - Size = 0x%08lx = %ld MB\n",
			size, size<<20);
	}

	debug("[%s, %d] Test point ...\n", __FUNCTION__, __LINE__);

	/* Setup offsets */
	flash_get_offsets (-size, &flash_info[0]);
	debug("[%s, %d] Test point ...\n", __FUNCTION__, __LINE__);

	/* Re-do sizing to get full correct info */
	mtdcr(EBC0_CFGADDR, PB0CR);
	pbcr = mfdcr(EBC0_CFGDATA);
	mtdcr(EBC0_CFGADDR, PB0CR);
	base = -size;
	switch (size) {
	case 1 << 20:
		size_val = 0;
		break;
	case 2 << 20:
		size_val = 1;
		break;
	case 4 << 20:
		size_val = 2;
		break;
	case 8 << 20:
		size_val = 3;
		break;
	case 16 << 20:
		size_val = 4;
		break;
	}
	pbcr = (pbcr & 0x0001ffff) | base | (size_val << 17);
	mtdcr(EBC0_CFGDATA, pbcr);
	debug("[%s, %d] Test point ...\n", __FUNCTION__, __LINE__);

	/* Monitor protection ON by default */
	(void)flash_protect(FLAG_PROTECT_SET,
			    -CONFIG_SYS_MONITOR_LEN,
			    0xffffffff,
			    &flash_info[0]);

	debug("[%s, %d] Test point ...\n", __FUNCTION__, __LINE__);
	flash_info[0].size  = size;

	return (size);
#endif
}
