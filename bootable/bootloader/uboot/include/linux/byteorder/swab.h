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

#ifndef _LINUX_BYTEORDER_SWAB_H
#define _LINUX_BYTEORDER_SWAB_H

/*
 * linux/byteorder/swab.h
 * Byte-swapping, independently from CPU endianness
 *	swabXX[ps]?(foo)
 *
 * Francois-Rene Rideau <fare@tunes.org> 19971205
 *    separated swab functions from cpu_to_XX,
 *    to clean up support for bizarre-endian architectures.
 *
 * See asm-i386/byteorder.h and suches for examples of how to provide
 * architecture-dependent optimized versions
 *
 */

/* casts are necessary for constants, because we never know how for sure
 * how U/UL/ULL map to __u16, __u32, __u64. At least not in a portable way.
 */
#define ___swab16(x) \
	((__u16)( \
		(((__u16)(x) & (__u16)0x00ffU) << 8) | \
		(((__u16)(x) & (__u16)0xff00U) >> 8) ))
#define ___swab32(x) \
	((__u32)( \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24) ))
#define ___swab64(x) \
	((__u64)( \
		(__u64)(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) | \
		(__u64)(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) | \
		(__u64)(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) | \
		(__u64)(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) | \
		(__u64)(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) | \
		(__u64)(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) | \
		(__u64)(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) | \
		(__u64)(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56) ))

/*
 * provide defaults when no architecture-specific optimization is detected
 */
#ifndef __arch__swab16
#  define __arch__swab16(x) ___swab16(x)
#endif
#ifndef __arch__swab32
#  define __arch__swab32(x) ___swab32(x)
#endif
#ifndef __arch__swab64
#  define __arch__swab64(x) ___swab64(x)
#endif

#ifndef __arch__swab16p
#  define __arch__swab16p(x) __swab16(*(x))
#endif
#ifndef __arch__swab32p
#  define __arch__swab32p(x) __swab32(*(x))
#endif
#ifndef __arch__swab64p
#  define __arch__swab64p(x) __swab64(*(x))
#endif

#ifndef __arch__swab16s
#  define __arch__swab16s(x) do { *(x) = __swab16p((x)); } while (0)
#endif
#ifndef __arch__swab32s
#  define __arch__swab32s(x) do { *(x) = __swab32p((x)); } while (0)
#endif
#ifndef __arch__swab64s
#  define __arch__swab64s(x) do { *(x) = __swab64p((x)); } while (0)
#endif


/*
 * Allow constant folding
 */
#if defined(__GNUC__) && (__GNUC__ >= 2) && defined(__OPTIMIZE__)
#  define __swab16(x) \
(__builtin_constant_p((__u16)(x)) ? \
 ___swab16((x)) : \
 __fswab16((x)))
#  define __swab32(x) \
(__builtin_constant_p((__u32)(x)) ? \
 ___swab32((x)) : \
 __fswab32((x)))
#  define __swab64(x) \
(__builtin_constant_p((__u64)(x)) ? \
 ___swab64((x)) : \
 __fswab64((x)))
#else
#  define __swab16(x) __fswab16(x)
#  define __swab32(x) __fswab32(x)
#  define __swab64(x) __fswab64(x)
#endif /* OPTIMIZE */


static __inline__ __attribute__((const)) __u16 __fswab16(__u16 x)
{
	return __arch__swab16(x);
}
static __inline__ __u16 __swab16p(__u16 *x)
{
	return __arch__swab16p(x);
}
static __inline__ void __swab16s(__u16 *addr)
{
	__arch__swab16s(addr);
}

static __inline__ __attribute__((const)) __u32 __fswab32(__u32 x)
{
	return __arch__swab32(x);
}
static __inline__ __u32 __swab32p(__u32 *x)
{
	return __arch__swab32p(x);
}
static __inline__ void __swab32s(__u32 *addr)
{
	__arch__swab32s(addr);
}

#ifdef __BYTEORDER_HAS_U64__
static __inline__ __attribute__((const)) __u64 __fswab64(__u64 x)
{
#  ifdef __SWAB_64_THRU_32__
	__u32 h = x >> 32;
	__u32 l = x & ((1ULL<<32)-1);
	return (((__u64)__swab32(l)) << 32) | ((__u64)(__swab32(h)));
#  else
	return __arch__swab64(x);
#  endif
}
static __inline__ __u64 __swab64p(__u64 *x)
{
	return __arch__swab64p(x);
}
static __inline__ void __swab64s(__u64 *addr)
{
	__arch__swab64s(addr);
}
#endif /* __BYTEORDER_HAS_U64__ */

#if defined(__KERNEL__)
#define swab16 __swab16
#define swab32 __swab32
#define swab64 __swab64
#define swab16p __swab16p
#define swab32p __swab32p
#define swab64p __swab64p
#define swab16s __swab16s
#define swab32s __swab32s
#define swab64s __swab64s
#endif

#endif /* _LINUX_BYTEORDER_SWAB_H */
