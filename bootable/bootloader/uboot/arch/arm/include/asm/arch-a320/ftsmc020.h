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
 * (C) Copyright 2009 Faraday Technology
 * Po-Yu Chuang <ratbert@faraday-tech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Static Memory Controller
 */
#ifndef __FTSMC020_H
#define __FTSMC020_H

#ifndef __ASSEMBLY__

struct ftsmc020 {
	struct {
		unsigned int	cr;	/* 0x00, 0x08, 0x10, 0x18 */
		unsigned int	tpr;	/* 0x04, 0x0c, 0x14, 0x1c */
	} bank[4];
	unsigned int	pad[8];	/* 0x20 - 0x3c */
	unsigned int	ssr;	/* 0x40 */
};

void ftsmc020_init(void);

#endif /* __ASSEMBLY__ */

/*
 * Memory Bank Configuration Register
 */
#define FTSMC020_BANK_ENABLE	(1 << 28)
#define FTSMC020_BANK_BASE(x)	((x) & 0x0fff1000)

#define FTSMC020_BANK_WPROT	(1 << 11)

#define FTSMC020_BANK_SIZE_32K	(0xb << 4)
#define FTSMC020_BANK_SIZE_64K	(0xc << 4)
#define FTSMC020_BANK_SIZE_128K	(0xd << 4)
#define FTSMC020_BANK_SIZE_256K	(0xe << 4)
#define FTSMC020_BANK_SIZE_512K	(0xf << 4)
#define FTSMC020_BANK_SIZE_1M	(0x0 << 4)
#define FTSMC020_BANK_SIZE_2M	(0x1 << 4)
#define FTSMC020_BANK_SIZE_4M	(0x2 << 4)
#define FTSMC020_BANK_SIZE_8M	(0x3 << 4)
#define FTSMC020_BANK_SIZE_16M	(0x4 << 4)
#define FTSMC020_BANK_SIZE_32M	(0x5 << 4)

#define FTSMC020_BANK_MBW_8	(0x0 << 0)
#define FTSMC020_BANK_MBW_16	(0x1 << 0)
#define FTSMC020_BANK_MBW_32	(0x2 << 0)

/*
 * Memory Bank Timing Parameter Register
 */
#define FTSMC020_TPR_ETRNA(x)	(((x) & 0xf) << 28)
#define FTSMC020_TPR_EATI(x)	(((x) & 0xf) << 24)
#define FTSMC020_TPR_RBE	(1 << 20)
#define FTSMC020_TPR_AST(x)	(((x) & 0x3) << 18)
#define FTSMC020_TPR_CTW(x)	(((x) & 0x3) << 16)
#define FTSMC020_TPR_ATI(x)	(((x) & 0xf) << 12)
#define FTSMC020_TPR_AT2(x)	(((x) & 0x3) << 8)
#define FTSMC020_TPR_WTC(x)	(((x) & 0x3) << 6)
#define FTSMC020_TPR_AHT(x)	(((x) & 0x3) << 4)
#define FTSMC020_TPR_TRNA(x)	(((x) & 0xf) << 0)

#endif	/* __FTSMC020_H */
