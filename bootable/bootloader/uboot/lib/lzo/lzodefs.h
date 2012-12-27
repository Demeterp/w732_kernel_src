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
 *  lzodefs.h -- architecture, OS and compiler specific defines
 *
 *  Copyright (C) 1996-2005 Markus F.X.J. Oberhumer <markus@oberhumer.com>
 *
 *  The full LZO package can be found at:
 *  http://www.oberhumer.com/opensource/lzo/
 *
 *  Changed for kernel use by:
 *  Nitin Gupta <nitingupta910@gmail.com>
 *  Richard Purdie <rpurdie@openedhand.com>
 */

#define LZO_VERSION		0x2020
#define LZO_VERSION_STRING	"2.02"
#define LZO_VERSION_DATE	"Oct 17 2005"

#define M1_MAX_OFFSET	0x0400
#define M2_MAX_OFFSET	0x0800
#define M3_MAX_OFFSET	0x4000
#define M4_MAX_OFFSET	0xbfff

#define M1_MIN_LEN	2
#define M1_MAX_LEN	2
#define M2_MIN_LEN	3
#define M2_MAX_LEN	8
#define M3_MIN_LEN	3
#define M3_MAX_LEN	33
#define M4_MIN_LEN	3
#define M4_MAX_LEN	9

#define M1_MARKER	0
#define M2_MARKER	64
#define M3_MARKER	32
#define M4_MARKER	16

#define D_BITS		14
#define D_MASK		((1u << D_BITS) - 1)
#define D_HIGH		((D_MASK >> 1) + 1)

#define DX2(p, s1, s2)	(((((size_t)((p)[2]) << (s2)) ^ (p)[1]) \
							<< (s1)) ^ (p)[0])
#define DX3(p, s1, s2, s3)	((DX2((p)+1, s2, s3) << (s1)) ^ (p)[0])
