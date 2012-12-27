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
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright © 2004  Ferenc Havasi <havasi@inf.u-szeged.hu>,
 *		     Zoltan Sogor <weth@inf.u-szeged.hu>,
 *		     Patrik Kluba <pajko@halom.u-szeged.hu>,
 *		     University of Szeged, Hungary
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#ifndef JFFS2_SUMMARY_H
#define JFFS2_SUMMARY_H

#define BLK_STATE_ALLFF		0
#define BLK_STATE_CLEAN		1
#define BLK_STATE_PARTDIRTY	2
#define BLK_STATE_CLEANMARKER	3
#define BLK_STATE_ALLDIRTY	4
#define BLK_STATE_BADBLOCK	5

#define JFFS2_SUMMARY_NOSUM_SIZE 0xffffffff
#define JFFS2_SUMMARY_INODE_SIZE (sizeof(struct jffs2_sum_inode_flash))
#define JFFS2_SUMMARY_DIRENT_SIZE(x) (sizeof(struct jffs2_sum_dirent_flash) + (x))
#define JFFS2_SUMMARY_XATTR_SIZE (sizeof(struct jffs2_sum_xattr_flash))
#define JFFS2_SUMMARY_XREF_SIZE (sizeof(struct jffs2_sum_xref_flash))

/* Summary structures used on flash */

struct jffs2_sum_unknown_flash
{
	__u16 nodetype;	/* node type */
};

struct jffs2_sum_inode_flash
{
	__u16 nodetype;	/* node type */
	__u32 inode;		/* inode number */
	__u32 version;	/* inode version */
	__u32 offset;	/* offset on jeb */
	__u32 totlen; 	/* record length */
} __attribute__((packed));

struct jffs2_sum_dirent_flash
{
	__u16 nodetype;	/* == JFFS_NODETYPE_DIRENT */
	__u32 totlen;	/* record length */
	__u32 offset;	/* offset on jeb */
	__u32 pino;		/* parent inode */
	__u32 version;	/* dirent version */
	__u32 ino; 		/* == zero for unlink */
	uint8_t nsize;		/* dirent name size */
	uint8_t type;		/* dirent type */
	uint8_t name[0];	/* dirent name */
} __attribute__((packed));

struct jffs2_sum_xattr_flash
{
	__u16 nodetype;	/* == JFFS2_NODETYPE_XATR */
	__u32 xid;		/* xattr identifier */
	__u32 version;	/* version number */
	__u32 offset;	/* offset on jeb */
	__u32 totlen;	/* node length */
} __attribute__((packed));

struct jffs2_sum_xref_flash
{
	__u16 nodetype;	/* == JFFS2_NODETYPE_XREF */
	__u32 offset;	/* offset on jeb */
} __attribute__((packed));

union jffs2_sum_flash
{
	struct jffs2_sum_unknown_flash u;
	struct jffs2_sum_inode_flash i;
	struct jffs2_sum_dirent_flash d;
	struct jffs2_sum_xattr_flash x;
	struct jffs2_sum_xref_flash r;
};

/* Summary structures used in the memory */

struct jffs2_sum_unknown_mem
{
	union jffs2_sum_mem *next;
	__u16 nodetype;	/* node type */
};

struct jffs2_sum_inode_mem
{
	union jffs2_sum_mem *next;
	__u16 nodetype;	/* node type */
	__u32 inode;		/* inode number */
	__u32 version;	/* inode version */
	__u32 offset;	/* offset on jeb */
	__u32 totlen; 	/* record length */
} __attribute__((packed));

struct jffs2_sum_dirent_mem
{
	union jffs2_sum_mem *next;
	__u16 nodetype;	/* == JFFS_NODETYPE_DIRENT */
	__u32 totlen;	/* record length */
	__u32 offset;	/* ofset on jeb */
	__u32 pino;		/* parent inode */
	__u32 version;	/* dirent version */
	__u32 ino; 		/* == zero for unlink */
	uint8_t nsize;		/* dirent name size */
	uint8_t type;		/* dirent type */
	uint8_t name[0];	/* dirent name */
} __attribute__((packed));

struct jffs2_sum_xattr_mem
{
	union jffs2_sum_mem *next;
	__u16 nodetype;
	__u32 xid;
	__u32 version;
	__u32 offset;
	__u32 totlen;
} __attribute__((packed));

struct jffs2_sum_xref_mem
{
	union jffs2_sum_mem *next;
	__u16 nodetype;
	__u32 offset;
} __attribute__((packed));

union jffs2_sum_mem
{
	struct jffs2_sum_unknown_mem u;
	struct jffs2_sum_inode_mem i;
	struct jffs2_sum_dirent_mem d;
	struct jffs2_sum_xattr_mem x;
	struct jffs2_sum_xref_mem r;
};

/* Summary related information stored in superblock */

struct jffs2_summary
{
	uint32_t sum_size;      /* collected summary information for nextblock */
	uint32_t sum_num;
	uint32_t sum_padded;
	union jffs2_sum_mem *sum_list_head;
	union jffs2_sum_mem *sum_list_tail;

	__u32 *sum_buf;	/* buffer for writing out summary */
};

/* Summary marker is stored at the end of every sumarized erase block */

struct jffs2_sum_marker
{
	__u32 offset;	/* offset of the summary node in the jeb */
	__u32 magic; 	/* == JFFS2_SUM_MAGIC */
};

#define JFFS2_SUMMARY_FRAME_SIZE (sizeof(struct jffs2_raw_summary) + sizeof(struct jffs2_sum_marker))

#endif /* JFFS2_SUMMARY_H */
