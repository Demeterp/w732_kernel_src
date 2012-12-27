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
 * (C) 1999-2003 David Woodhouse <dwmw2@infradead.org>
 */

#ifndef __MTD_NFTL_H__
#define __MTD_NFTL_H__

#include <linux/mtd/mtd.h>
#include <linux/mtd/blktrans.h>

#include <linux/mtd/nftl-user.h>

/* these info are used in ReplUnitTable */
#define BLOCK_NIL	   0xffff /* last block of a chain */
#define BLOCK_FREE	   0xfffe /* free block */
#define BLOCK_NOTEXPLORED  0xfffd /* non explored block, only used during mounting */
#define BLOCK_RESERVED	   0xfffc /* bios block or bad block */

struct NFTLrecord {
	struct mtd_blktrans_dev mbd;
	__u16 MediaUnit, SpareMediaUnit;
	__u32 EraseSize;
	struct NFTLMediaHeader MediaHdr;
	int usecount;
	unsigned char heads;
	unsigned char sectors;
	unsigned short cylinders;
	__u16 numvunits;
	__u16 lastEUN;			/* should be suppressed */
	__u16 numfreeEUNs;
	__u16 LastFreeEUN;		/* To speed up finding a free EUN */
	int head,sect,cyl;
	__u16 *EUNtable;		/* [numvunits]: First EUN for each virtual unit  */
	__u16 *ReplUnitTable;		/* [numEUNs]: ReplUnitNumber for each */
	unsigned int nb_blocks;		/* number of physical blocks */
	unsigned int nb_boot_blocks;	/* number of blocks used by the bios */
	struct erase_info instr;
	struct nand_ecclayout oobinfo;
};

int NFTL_mount(struct NFTLrecord *s);
int NFTL_formatblock(struct NFTLrecord *s, int block);

int nftl_read_oob(struct mtd_info *mtd, loff_t offs, size_t len,
		  size_t *retlen, uint8_t *buf);
int nftl_write_oob(struct mtd_info *mtd, loff_t offs, size_t len,
		   size_t *retlen, uint8_t *buf);

#ifndef NFTL_MAJOR
#define NFTL_MAJOR 93
#endif

#define MAX_NFTLS 16
#define MAX_SECTORS_PER_UNIT 64
#define NFTL_PARTN_BITS 4

#endif /* __MTD_NFTL_H__ */
