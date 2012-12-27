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
 * MTD partitioning layer definitions
 *
 * (C) 2000 Nicolas Pitre <nico@cam.org>
 *
 * This code is GPL
 *
 * $Id: partitions.h,v 1.17 2005/11/07 11:14:55 gleixner Exp $
 */

#ifndef MTD_PARTITIONS_H
#define MTD_PARTITIONS_H

#include <linux/types.h>


/*
 * Partition definition structure:
 *
 * An array of struct partition is passed along with a MTD object to
 * add_mtd_partitions() to create them.
 *
 * For each partition, these fields are available:
 * name: string that will be used to label the partition's MTD device.
 * size: the partition size; if defined as MTDPART_SIZ_FULL, the partition
 * 	will extend to the end of the master MTD device.
 * offset: absolute starting position within the master MTD device; if
 * 	defined as MTDPART_OFS_APPEND, the partition will start where the
 * 	previous one ended; if MTDPART_OFS_NXTBLK, at the next erase block.
 * mask_flags: contains flags that have to be masked (removed) from the
 * 	master MTD flag set for the corresponding MTD partition.
 * 	For example, to force a read-only partition, simply adding
 * 	MTD_WRITEABLE to the mask_flags will do the trick.
 *
 * Note: writeable partitions require their size and offset be
 * erasesize aligned (e.g. use MTDPART_OFS_NEXTBLK).
 */

struct mtd_partition {
	char *name;			/* identifier string */
	uint64_t size;			/* partition size */
	uint64_t offset;		/* offset within the master MTD space */
	u_int32_t mask_flags;		/* master MTD flags to mask out for this partition */
	struct nand_ecclayout *ecclayout;	/* out of band layout for this partition (NAND only)*/
	struct mtd_info **mtdp;		/* pointer to store the MTD object */
};

#define MTDPART_OFS_NXTBLK	(-2)
#define MTDPART_OFS_APPEND	(-1)
#define MTDPART_SIZ_FULL	(0)


int add_mtd_partitions(struct mtd_info *, const struct mtd_partition *, int);
int del_mtd_partitions(struct mtd_info *);

#if 0
/*
 * Functions dealing with the various ways of partitioning the space
 */

struct mtd_part_parser {
	struct list_head list;
	struct module *owner;
	const char *name;
	int (*parse_fn)(struct mtd_info *, struct mtd_partition **, unsigned long);
};

extern int register_mtd_parser(struct mtd_part_parser *parser);
extern int deregister_mtd_parser(struct mtd_part_parser *parser);
extern int parse_mtd_partitions(struct mtd_info *master, const char **types,
				struct mtd_partition **pparts, unsigned long origin);

#define put_partition_parser(p) do { module_put((p)->owner); } while(0)

struct device;
struct device_node;

int __devinit of_mtd_parse_partitions(struct device *dev,
				      struct mtd_info *mtd,
				      struct device_node *node,
				      struct mtd_partition **pparts);
#endif

#endif
