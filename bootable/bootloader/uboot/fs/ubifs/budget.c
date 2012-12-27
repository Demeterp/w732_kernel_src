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
 * This file is part of UBIFS.
 *
 * Copyright (C) 2006-2008 Nokia Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Authors: Adrian Hunter
 *          Artem Bityutskiy (Битюцкий Артём)
 */

/*
 * This file implements the budgeting sub-system which is responsible for UBIFS
 * space management.
 *
 * Factors such as compression, wasted space at the ends of LEBs, space in other
 * journal heads, the effect of updates on the index, and so on, make it
 * impossible to accurately predict the amount of space needed. Consequently
 * approximations are used.
 */

#include "ubifs.h"
#include <linux/math64.h>

/**
 * ubifs_calc_min_idx_lebs - calculate amount of eraseblocks for the index.
 * @c: UBIFS file-system description object
 *
 * This function calculates and returns the number of eraseblocks which should
 * be kept for index usage.
 */
int ubifs_calc_min_idx_lebs(struct ubifs_info *c)
{
	int idx_lebs, eff_leb_size = c->leb_size - c->max_idx_node_sz;
	long long idx_size;

	idx_size = c->old_idx_sz + c->budg_idx_growth + c->budg_uncommitted_idx;

	/* And make sure we have thrice the index size of space reserved */
	idx_size = idx_size + (idx_size << 1);

	/*
	 * We do not maintain 'old_idx_size' as 'old_idx_lebs'/'old_idx_bytes'
	 * pair, nor similarly the two variables for the new index size, so we
	 * have to do this costly 64-bit division on fast-path.
	 */
	idx_size += eff_leb_size - 1;
	idx_lebs = div_u64(idx_size, eff_leb_size);
	/*
	 * The index head is not available for the in-the-gaps method, so add an
	 * extra LEB to compensate.
	 */
	idx_lebs += 1;
	if (idx_lebs < MIN_INDEX_LEBS)
		idx_lebs = MIN_INDEX_LEBS;
	return idx_lebs;
}

/**
 * ubifs_reported_space - calculate reported free space.
 * @c: the UBIFS file-system description object
 * @free: amount of free space
 *
 * This function calculates amount of free space which will be reported to
 * user-space. User-space application tend to expect that if the file-system
 * (e.g., via the 'statfs()' call) reports that it has N bytes available, they
 * are able to write a file of size N. UBIFS attaches node headers to each data
 * node and it has to write indexing nodes as well. This introduces additional
 * overhead, and UBIFS has to report slightly less free space to meet the above
 * expectations.
 *
 * This function assumes free space is made up of uncompressed data nodes and
 * full index nodes (one per data node, tripled because we always allow enough
 * space to write the index thrice).
 *
 * Note, the calculation is pessimistic, which means that most of the time
 * UBIFS reports less space than it actually has.
 */
long long ubifs_reported_space(const struct ubifs_info *c, long long free)
{
	int divisor, factor, f;

	/*
	 * Reported space size is @free * X, where X is UBIFS block size
	 * divided by UBIFS block size + all overhead one data block
	 * introduces. The overhead is the node header + indexing overhead.
	 *
	 * Indexing overhead calculations are based on the following formula:
	 * I = N/(f - 1) + 1, where I - number of indexing nodes, N - number
	 * of data nodes, f - fanout. Because effective UBIFS fanout is twice
	 * as less than maximum fanout, we assume that each data node
	 * introduces 3 * @c->max_idx_node_sz / (@c->fanout/2 - 1) bytes.
	 * Note, the multiplier 3 is because UBIFS reserves thrice as more space
	 * for the index.
	 */
	f = c->fanout > 3 ? c->fanout >> 1 : 2;
	factor = UBIFS_BLOCK_SIZE;
	divisor = UBIFS_MAX_DATA_NODE_SZ;
	divisor += (c->max_idx_node_sz * 3) / (f - 1);
	free *= factor;
	return div_u64(free, divisor);
}
