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
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2007 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* XXX U-BOOT XXX */
#include <common.h>

const char *yaffs_nand_c_version =
    "$Id: yaffs_nand.c,v 1.7 2007/02/14 01:09:06 wookey Exp $";

#include "yaffs_nand.h"
#include "yaffs_tagscompat.h"
#include "yaffs_tagsvalidity.h"


int yaffs_ReadChunkWithTagsFromNAND(yaffs_Device * dev, int chunkInNAND,
					   __u8 * buffer,
					   yaffs_ExtendedTags * tags)
{
	int result;
	yaffs_ExtendedTags localTags;

	int realignedChunkInNAND = chunkInNAND - dev->chunkOffset;

	/* If there are no tags provided, use local tags to get prioritised gc working */
	if(!tags)
		tags = &localTags;

	if (dev->readChunkWithTagsFromNAND)
		result = dev->readChunkWithTagsFromNAND(dev, realignedChunkInNAND, buffer,
						      tags);
	else
		result = yaffs_TagsCompatabilityReadChunkWithTagsFromNAND(dev,
									realignedChunkInNAND,
									buffer,
									tags);
	if(tags &&
	   tags->eccResult > YAFFS_ECC_RESULT_NO_ERROR){

		yaffs_BlockInfo *bi = yaffs_GetBlockInfo(dev, chunkInNAND/dev->nChunksPerBlock);
		yaffs_HandleChunkError(dev,bi);
	}

	return result;
}

int yaffs_WriteChunkWithTagsToNAND(yaffs_Device * dev,
						   int chunkInNAND,
						   const __u8 * buffer,
						   yaffs_ExtendedTags * tags)
{
	chunkInNAND -= dev->chunkOffset;


	if (tags) {
		tags->sequenceNumber = dev->sequenceNumber;
		tags->chunkUsed = 1;
		if (!yaffs_ValidateTags(tags)) {
			T(YAFFS_TRACE_ERROR,
			  (TSTR("Writing uninitialised tags" TENDSTR)));
			YBUG();
		}
		T(YAFFS_TRACE_WRITE,
		  (TSTR("Writing chunk %d tags %d %d" TENDSTR), chunkInNAND,
		   tags->objectId, tags->chunkId));
	} else {
		T(YAFFS_TRACE_ERROR, (TSTR("Writing with no tags" TENDSTR)));
		YBUG();
	}

	if (dev->writeChunkWithTagsToNAND)
		return dev->writeChunkWithTagsToNAND(dev, chunkInNAND, buffer,
						     tags);
	else
		return yaffs_TagsCompatabilityWriteChunkWithTagsToNAND(dev,
								       chunkInNAND,
								       buffer,
								       tags);
}

int yaffs_MarkBlockBad(yaffs_Device * dev, int blockNo)
{
	blockNo -= dev->blockOffset;

;
	if (dev->markNANDBlockBad)
		return dev->markNANDBlockBad(dev, blockNo);
	else
		return yaffs_TagsCompatabilityMarkNANDBlockBad(dev, blockNo);
}

int yaffs_QueryInitialBlockState(yaffs_Device * dev,
						 int blockNo,
						 yaffs_BlockState * state,
						 unsigned *sequenceNumber)
{
	blockNo -= dev->blockOffset;

	if (dev->queryNANDBlock)
		return dev->queryNANDBlock(dev, blockNo, state, sequenceNumber);
	else
		return yaffs_TagsCompatabilityQueryNANDBlock(dev, blockNo,
							     state,
							     sequenceNumber);
}


int yaffs_EraseBlockInNAND(struct yaffs_DeviceStruct *dev,
				  int blockInNAND)
{
	int result;

	blockInNAND -= dev->blockOffset;


	dev->nBlockErasures++;
	result = dev->eraseBlockInNAND(dev, blockInNAND);

	return result;
}

int yaffs_InitialiseNAND(struct yaffs_DeviceStruct *dev)
{
	return dev->initialiseNAND(dev);
}
