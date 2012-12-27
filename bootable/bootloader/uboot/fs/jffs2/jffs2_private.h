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

#ifndef jffs2_private_h
#define jffs2_private_h

#include <jffs2/jffs2.h>


struct b_node {
	u32 offset;
	struct b_node *next;
	enum { CRC_UNKNOWN = 0, CRC_OK, CRC_BAD } datacrc;
};

struct b_list {
	struct b_node *listTail;
	struct b_node *listHead;
#ifdef CONFIG_SYS_JFFS2_SORT_FRAGMENTS
	struct b_node *listLast;
	int (*listCompare)(struct b_node *new, struct b_node *node);
	u32 listLoops;
#endif
	u32 listCount;
	struct mem_block *listMemBase;
};

struct b_lists {
	struct b_list dir;
	struct b_list frag;
	void *readbuf;
};

struct b_compr_info {
	u32 num_frags;
	u32 compr_sum;
	u32 decompr_sum;
};

struct b_jffs2_info {
	struct b_compr_info compr_info[JFFS2_NUM_COMPR];
};

static inline int
hdr_crc(struct jffs2_unknown_node *node)
{
#if 1
	u32 crc = crc32_no_comp(0, (unsigned char *)node, sizeof(struct jffs2_unknown_node) - 4);
#else
	/* what's the semantics of this? why is this here? */
	u32 crc = crc32_no_comp(~0, (unsigned char *)node, sizeof(struct jffs2_unknown_node) - 4);

	crc ^= ~0;
#endif
	if (node->hdr_crc != crc) {
		return 0;
	} else {
		return 1;
	}
}

static inline int
dirent_crc(struct jffs2_raw_dirent *node)
{
	if (node->node_crc != crc32_no_comp(0, (unsigned char *)node, sizeof(struct jffs2_raw_dirent) - 8)) {
		return 0;
	} else {
		return 1;
	}
}

static inline int
dirent_name_crc(struct jffs2_raw_dirent *node)
{
	if (node->name_crc != crc32_no_comp(0, (unsigned char *)&(node->name), node->nsize)) {
		return 0;
	} else {
		return 1;
	}
}

static inline int
inode_crc(struct jffs2_raw_inode *node)
{
	if (node->node_crc != crc32_no_comp(0, (unsigned char *)node, sizeof(struct jffs2_raw_inode) - 8)) {
		return 0;
	} else {
		return 1;
	}
}

static inline int
data_crc(struct jffs2_raw_inode *node)
{
	if (node->data_crc != crc32_no_comp(0, (unsigned char *)
					    ((int) &node->node_crc + sizeof (node->node_crc)),
					     node->csize)) {
		return 0;
	} else {
		return 1;
	}
}

#endif /* jffs2_private.h */
