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
 * $Id: inftl-user.h,v 1.2 2005/11/07 11:14:56 gleixner Exp $
 *
 * Parts of INFTL headers shared with userspace
 *
 */

#ifndef __MTD_INFTL_USER_H__
#define __MTD_INFTL_USER_H__

#define	OSAK_VERSION	0x5120
#define	PERCENTUSED	98

#define	SECTORSIZE	512

/* Block Control Information */

struct inftl_bci {
	uint8_t ECCsig[6];
	uint8_t Status;
	uint8_t Status1;
} __attribute__((packed));

struct inftl_unithead1 {
	uint16_t virtualUnitNo;
	uint16_t prevUnitNo;
	uint8_t ANAC;
	uint8_t NACs;
	uint8_t parityPerField;
	uint8_t discarded;
} __attribute__((packed));

struct inftl_unithead2 {
	uint8_t parityPerField;
	uint8_t ANAC;
	uint16_t prevUnitNo;
	uint16_t virtualUnitNo;
	uint8_t NACs;
	uint8_t discarded;
} __attribute__((packed));

struct inftl_unittail {
	uint8_t Reserved[4];
	uint16_t EraseMark;
	uint16_t EraseMark1;
} __attribute__((packed));

union inftl_uci {
	struct inftl_unithead1 a;
	struct inftl_unithead2 b;
	struct inftl_unittail c;
};

struct inftl_oob {
	struct inftl_bci b;
	union inftl_uci u;
};


/* INFTL Media Header */

struct INFTLPartition {
	__u32 virtualUnits;
	__u32 firstUnit;
	__u32 lastUnit;
	__u32 flags;
	__u32 spareUnits;
	__u32 Reserved0;
	__u32 Reserved1;
} __attribute__((packed));

struct INFTLMediaHeader {
	char bootRecordID[8];
	__u32 NoOfBootImageBlocks;
	__u32 NoOfBinaryPartitions;
	__u32 NoOfBDTLPartitions;
	__u32 BlockMultiplierBits;
	__u32 FormatFlags;
	__u32 OsakVersion;
	__u32 PercentUsed;
	struct INFTLPartition Partitions[4];
} __attribute__((packed));

/* Partition flag types */
#define	INFTL_BINARY	0x20000000
#define	INFTL_BDTL	0x40000000
#define	INFTL_LAST	0x80000000

#endif /* __MTD_INFTL_USER_H__ */
