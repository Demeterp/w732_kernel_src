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
 *  (C) Copyright 2003 - 2004
 *  Sysgo AG, <www.elinos.com>, Pavel Bartusek <pba@sysgo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <common.h>
#include <config.h>
#include <reiserfs.h>

#include "reiserfs_private.h"

static block_dev_desc_t *reiserfs_block_dev_desc;
static disk_partition_t part_info;


int reiserfs_set_blk_dev(block_dev_desc_t *rbdd, int part)
{
	reiserfs_block_dev_desc = rbdd;

	if (part == 0) {
		/* disk doesn't use partition table */
		part_info.start = 0;
		part_info.size = rbdd->lba;
		part_info.blksz = rbdd->blksz;
	} else {
		if (get_partition_info (reiserfs_block_dev_desc, part, &part_info)) {
			return 0;
		}
	}
	return (part_info.size);
}


int reiserfs_devread (int sector, int byte_offset, int byte_len, char *buf)
{
	char sec_buf[SECTOR_SIZE];
	unsigned block_len;
/*
	unsigned len = byte_len;
	u8 *start = buf;
*/
	/*
	*  Check partition boundaries
	*/
	if (sector < 0
	    || ((sector + ((byte_offset + byte_len - 1) >> SECTOR_BITS))
	    >= part_info.size)) {
/*		errnum = ERR_OUTSIDE_PART; */
		printf (" ** reiserfs_devread() read outside partition\n");
		return 0;
	}

	/*
	 *  Get the read to the beginning of a partition.
	 */
	sector += byte_offset >> SECTOR_BITS;
	byte_offset &= SECTOR_SIZE - 1;

#if defined(DEBUG)
	printf (" <%d, %d, %d> ", sector, byte_offset, byte_len);
#endif


	if (reiserfs_block_dev_desc == NULL)
		return 0;


	if (byte_offset != 0) {
		/* read first part which isn't aligned with start of sector */
		if (reiserfs_block_dev_desc->block_read(reiserfs_block_dev_desc->dev,
		    part_info.start+sector, 1, (unsigned long *)sec_buf) != 1) {
			printf (" ** reiserfs_devread() read error\n");
			return 0;
		}
		memcpy(buf, sec_buf+byte_offset, min(SECTOR_SIZE-byte_offset, byte_len));
		buf+=min(SECTOR_SIZE-byte_offset, byte_len);
		byte_len-=min(SECTOR_SIZE-byte_offset, byte_len);
		sector++;
	}

	/* read sector aligned part */
	block_len = byte_len & ~(SECTOR_SIZE-1);
	if (reiserfs_block_dev_desc->block_read(reiserfs_block_dev_desc->dev,
	    part_info.start+sector, block_len/SECTOR_SIZE, (unsigned long *)buf) !=
	    block_len/SECTOR_SIZE) {
		printf (" ** reiserfs_devread() read error - block\n");
		return 0;
	}
	buf+=block_len;
	byte_len-=block_len;
	sector+= block_len/SECTOR_SIZE;

	if ( byte_len != 0 ) {
		/* read rest of data which are not in whole sector */
		if (reiserfs_block_dev_desc->block_read(reiserfs_block_dev_desc->dev,
		    part_info.start+sector, 1, (unsigned long *)sec_buf) != 1) {
			printf (" ** reiserfs_devread() read error - last part\n");
			return 0;
		}
		memcpy(buf, sec_buf, byte_len);
	}

	return 1;
}
