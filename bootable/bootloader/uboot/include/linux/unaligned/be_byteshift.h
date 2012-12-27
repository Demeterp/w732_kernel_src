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

#ifndef _LINUX_UNALIGNED_BE_BYTESHIFT_H
#define _LINUX_UNALIGNED_BE_BYTESHIFT_H

#include <linux/types.h>

static inline u16 __get_unaligned_be16(const u8 *p)
{
	return p[0] << 8 | p[1];
}

static inline u32 __get_unaligned_be32(const u8 *p)
{
	return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

static inline u64 __get_unaligned_be64(const u8 *p)
{
	return (u64)__get_unaligned_be32(p) << 32 |
	       __get_unaligned_be32(p + 4);
}

static inline void __put_unaligned_be16(u16 val, u8 *p)
{
	*p++ = val >> 8;
	*p++ = val;
}

static inline void __put_unaligned_be32(u32 val, u8 *p)
{
	__put_unaligned_be16(val >> 16, p);
	__put_unaligned_be16(val, p + 2);
}

static inline void __put_unaligned_be64(u64 val, u8 *p)
{
	__put_unaligned_be32(val >> 32, p);
	__put_unaligned_be32(val, p + 4);
}

static inline u16 get_unaligned_be16(const void *p)
{
	return __get_unaligned_be16((const u8 *)p);
}

static inline u32 get_unaligned_be32(const void *p)
{
	return __get_unaligned_be32((const u8 *)p);
}

static inline u64 get_unaligned_be64(const void *p)
{
	return __get_unaligned_be64((const u8 *)p);
}

static inline void put_unaligned_be16(u16 val, void *p)
{
	__put_unaligned_be16(val, p);
}

static inline void put_unaligned_be32(u32 val, void *p)
{
	__put_unaligned_be32(val, p);
}

static inline void put_unaligned_be64(u64 val, void *p)
{
	__put_unaligned_be64(val, p);
}

#endif /* _LINUX_UNALIGNED_BE_BYTESHIFT_H */
