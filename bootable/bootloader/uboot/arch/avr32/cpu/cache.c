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
 * Copyright (C) 2004-2006 Atmel Corporation
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

#include <asm/arch/cacheflush.h>

void dcache_clean_range(volatile void *start, size_t size)
{
	unsigned long v, begin, end, linesz;

	linesz = CONFIG_SYS_DCACHE_LINESZ;

	/* You asked for it, you got it */
	begin = (unsigned long)start & ~(linesz - 1);
	end = ((unsigned long)start + size + linesz - 1) & ~(linesz - 1);

	for (v = begin; v < end; v += linesz)
		dcache_clean_line((void *)v);

	sync_write_buffer();
}

void dcache_invalidate_range(volatile void *start, size_t size)
{
	unsigned long v, begin, end, linesz;

	linesz = CONFIG_SYS_DCACHE_LINESZ;

	/* You asked for it, you got it */
	begin = (unsigned long)start & ~(linesz - 1);
	end = ((unsigned long)start + size + linesz - 1) & ~(linesz - 1);

	for (v = begin; v < end; v += linesz)
		dcache_invalidate_line((void *)v);
}

void dcache_flush_range(volatile void *start, size_t size)
{
	unsigned long v, begin, end, linesz;

	linesz = CONFIG_SYS_DCACHE_LINESZ;

	/* You asked for it, you got it */
	begin = (unsigned long)start & ~(linesz - 1);
	end = ((unsigned long)start + size + linesz - 1) & ~(linesz - 1);

	for (v = begin; v < end; v += linesz)
		dcache_flush_line((void *)v);

	sync_write_buffer();
}

void icache_invalidate_range(volatile void *start, size_t size)
{
	unsigned long v, begin, end, linesz;

	linesz = CONFIG_SYS_ICACHE_LINESZ;

	/* You asked for it, you got it */
	begin = (unsigned long)start & ~(linesz - 1);
	end = ((unsigned long)start + size + linesz - 1) & ~(linesz - 1);

	for (v = begin; v < end; v += linesz)
		icache_invalidate_line((void *)v);
}

/*
 * This is called after loading something into memory.  We need to
 * make sure that everything that was loaded is actually written to
 * RAM, and that the icache will look for it. Cleaning the dcache and
 * invalidating the icache will do the trick.
 */
void  flush_cache (unsigned long start_addr, unsigned long size)
{
	dcache_clean_range((void *)start_addr, size);
	icache_invalidate_range((void *)start_addr, size);
}
