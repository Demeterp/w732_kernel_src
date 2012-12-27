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
 * (C) 2003 David Woodhouse <dwmw2@infradead.org>
 *
 * Interface to Linux block layer for MTD 'translation layers'.
 *
 */

#ifndef __MTD_TRANS_H__
#define __MTD_TRANS_H__

/* XXX U-BOOT XXX */
#if 0
#include <linux/mutex.h>
#else
#include <linux/list.h>
#endif

struct hd_geometry;
struct mtd_info;
struct mtd_blktrans_ops;
struct file;
struct inode;

struct mtd_blktrans_dev {
	struct mtd_blktrans_ops *tr;
	struct list_head list;
	struct mtd_info *mtd;
/* XXX U-BOOT XXX */
#if 0
	struct mutex lock;
#endif
	int devnum;
	unsigned long size;
	int readonly;
	void *blkcore_priv; /* gendisk in 2.5, devfs_handle in 2.4 */
};

struct blkcore_priv; /* Differs for 2.4 and 2.5 kernels; private */

struct mtd_blktrans_ops {
	char *name;
	int major;
	int part_bits;
	int blksize;
	int blkshift;

	/* Access functions */
	int (*readsect)(struct mtd_blktrans_dev *dev,
		    unsigned long block, char *buffer);
	int (*writesect)(struct mtd_blktrans_dev *dev,
		     unsigned long block, char *buffer);

	/* Block layer ioctls */
	int (*getgeo)(struct mtd_blktrans_dev *dev, struct hd_geometry *geo);
	int (*flush)(struct mtd_blktrans_dev *dev);

	/* Called with mtd_table_mutex held; no race with add/remove */
	int (*open)(struct mtd_blktrans_dev *dev);
	int (*release)(struct mtd_blktrans_dev *dev);

	/* Called on {de,}registration and on subsequent addition/removal
	   of devices, with mtd_table_mutex held. */
	void (*add_mtd)(struct mtd_blktrans_ops *tr, struct mtd_info *mtd);
	void (*remove_dev)(struct mtd_blktrans_dev *dev);

	struct list_head devs;
	struct list_head list;
	struct module *owner;

	struct mtd_blkcore_priv *blkcore_priv;
};

extern int register_mtd_blktrans(struct mtd_blktrans_ops *tr);
extern int deregister_mtd_blktrans(struct mtd_blktrans_ops *tr);
extern int add_mtd_blktrans_dev(struct mtd_blktrans_dev *dev);
extern int del_mtd_blktrans_dev(struct mtd_blktrans_dev *dev);


#endif /* __MTD_TRANS_H__ */
