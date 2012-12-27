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
 * $Id: nftl-user.h,v 1.2 2005/11/07 11:14:56 gleixner Exp $
 *
 * Parts of NFTL headers shared with userspace
 *
 */

#ifndef __MTD_NFTL_USER_H__
#define __MTD_NFTL_USER_H__

/* Block Control Information */

struct nftl_bci {
	unsigned char ECCSig[6];
	uint8_t Status;
	uint8_t Status1;
}__attribute__((packed));

/* Unit Control Information */

struct nftl_uci0 {
	uint16_t VirtUnitNum;
	uint16_t ReplUnitNum;
	uint16_t SpareVirtUnitNum;
	uint16_t SpareReplUnitNum;
} __attribute__((packed));

struct nftl_uci1 {
	uint32_t WearInfo;
	uint16_t EraseMark;
	uint16_t EraseMark1;
} __attribute__((packed));

struct nftl_uci2 {
	uint16_t FoldMark;
	uint16_t FoldMark1;
	uint32_t unused;
} __attribute__((packed));

union nftl_uci {
	struct nftl_uci0 a;
	struct nftl_uci1 b;
	struct nftl_uci2 c;
};

struct nftl_oob {
	struct nftl_bci b;
	union nftl_uci u;
};

/* NFTL Media Header */

struct NFTLMediaHeader {
	char DataOrgID[6];
	uint16_t NumEraseUnits;
	uint16_t FirstPhysicalEUN;
	uint32_t FormattedSize;
	unsigned char UnitSizeFactor;
} __attribute__((packed));

#define MAX_ERASE_ZONES (8192 - 512)

#define ERASE_MARK 0x3c69
#define SECTOR_FREE 0xff
#define SECTOR_USED 0x55
#define SECTOR_IGNORE 0x11
#define SECTOR_DELETED 0x00

#define FOLD_MARK_IN_PROGRESS 0x5555

#define ZONE_GOOD 0xff
#define ZONE_BAD_ORIGINAL 0
#define ZONE_BAD_MARKED 7


#endif /* __MTD_NFTL_USER_H__ */
