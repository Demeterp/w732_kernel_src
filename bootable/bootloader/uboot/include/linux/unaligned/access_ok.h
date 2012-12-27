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

#ifndef _LINUX_UNALIGNED_ACCESS_OK_H
#define _LINUX_UNALIGNED_ACCESS_OK_H

#include <asm/byteorder.h>

static inline u16 get_unaligned_le16(const void *p)
{
	return le16_to_cpup((__le16 *)p);
}

static inline u32 get_unaligned_le32(const void *p)
{
	return le32_to_cpup((__le32 *)p);
}

static inline u64 get_unaligned_le64(const void *p)
{
	return le64_to_cpup((__le64 *)p);
}

static inline u16 get_unaligned_be16(const void *p)
{
	return be16_to_cpup((__be16 *)p);
}

static inline u32 get_unaligned_be32(const void *p)
{
	return be32_to_cpup((__be32 *)p);
}

static inline u64 get_unaligned_be64(const void *p)
{
	return be64_to_cpup((__be64 *)p);
}

static inline void put_unaligned_le16(u16 val, void *p)
{
	*((__le16 *)p) = cpu_to_le16(val);
}

static inline void put_unaligned_le32(u32 val, void *p)
{
	*((__le32 *)p) = cpu_to_le32(val);
}

static inline void put_unaligned_le64(u64 val, void *p)
{
	*((__le64 *)p) = cpu_to_le64(val);
}

static inline void put_unaligned_be16(u16 val, void *p)
{
	*((__be16 *)p) = cpu_to_be16(val);
}

static inline void put_unaligned_be32(u32 val, void *p)
{
	*((__be32 *)p) = cpu_to_be32(val);
}

static inline void put_unaligned_be64(u64 val, void *p)
{
	*((__be64 *)p) = cpu_to_be64(val);
}

#endif /* _LINUX_UNALIGNED_ACCESS_OK_H */
