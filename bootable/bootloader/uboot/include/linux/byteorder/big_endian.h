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

#ifndef _LINUX_BYTEORDER_BIG_ENDIAN_H
#define _LINUX_BYTEORDER_BIG_ENDIAN_H

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 4321
#endif
#ifndef __BIG_ENDIAN_BITFIELD
#define __BIG_ENDIAN_BITFIELD
#endif
#define	__BYTE_ORDER	__BIG_ENDIAN

#include <linux/byteorder/swab.h>

#define __constant_htonl(x) ((__u32)(x))
#define __constant_ntohl(x) ((__u32)(x))
#define __constant_htons(x) ((__u16)(x))
#define __constant_ntohs(x) ((__u16)(x))
#define __constant_cpu_to_le64(x) ___swab64((x))
#define __constant_le64_to_cpu(x) ___swab64((x))
#define __constant_cpu_to_le32(x) ___swab32((x))
#define __constant_le32_to_cpu(x) ___swab32((x))
#define __constant_cpu_to_le16(x) ___swab16((x))
#define __constant_le16_to_cpu(x) ___swab16((x))
#define __constant_cpu_to_be64(x) ((__u64)(x))
#define __constant_be64_to_cpu(x) ((__u64)(x))
#define __constant_cpu_to_be32(x) ((__u32)(x))
#define __constant_be32_to_cpu(x) ((__u32)(x))
#define __constant_cpu_to_be16(x) ((__u16)(x))
#define __constant_be16_to_cpu(x) ((__u16)(x))
#define __cpu_to_le64(x) __swab64((x))
#define __le64_to_cpu(x) __swab64((x))
#define __cpu_to_le32(x) __swab32((x))
#define __le32_to_cpu(x) __swab32((x))
#define __cpu_to_le16(x) __swab16((x))
#define __le16_to_cpu(x) __swab16((x))
#define __cpu_to_be64(x) ((__u64)(x))
#define __be64_to_cpu(x) ((__u64)(x))
#define __cpu_to_be32(x) ((__u32)(x))
#define __be32_to_cpu(x) ((__u32)(x))
#define __cpu_to_be16(x) ((__u16)(x))
#define __be16_to_cpu(x) ((__u16)(x))
#define __cpu_to_le64p(x) __swab64p((x))
#define __le64_to_cpup(x) __swab64p((x))
#define __cpu_to_le32p(x) __swab32p((x))
#define __le32_to_cpup(x) __swab32p((x))
#define __cpu_to_le16p(x) __swab16p((x))
#define __le16_to_cpup(x) __swab16p((x))
#define __cpu_to_be64p(x) (*(__u64*)(x))
#define __be64_to_cpup(x) (*(__u64*)(x))
#define __cpu_to_be32p(x) (*(__u32*)(x))
#define __be32_to_cpup(x) (*(__u32*)(x))
#define __cpu_to_be16p(x) (*(__u16*)(x))
#define __be16_to_cpup(x) (*(__u16*)(x))
#define __cpu_to_le64s(x) __swab64s((x))
#define __le64_to_cpus(x) __swab64s((x))
#define __cpu_to_le32s(x) __swab32s((x))
#define __le32_to_cpus(x) __swab32s((x))
#define __cpu_to_le16s(x) __swab16s((x))
#define __le16_to_cpus(x) __swab16s((x))
#define __cpu_to_be64s(x) do {} while (0)
#define __be64_to_cpus(x) do {} while (0)
#define __cpu_to_be32s(x) do {} while (0)
#define __be32_to_cpus(x) do {} while (0)
#define __cpu_to_be16s(x) do {} while (0)
#define __be16_to_cpus(x) do {} while (0)

#include <linux/byteorder/generic.h>

#endif /* _LINUX_BYTEORDER_BIG_ENDIAN_H */
