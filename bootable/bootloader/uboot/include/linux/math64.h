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

#ifndef _LINUX_MATH64_H
#define _LINUX_MATH64_H

#include <linux/types.h>

#if BITS_PER_LONG == 64

/**
 * div_u64_rem - unsigned 64bit divide with 32bit divisor with remainder
 *
 * This is commonly provided by 32bit archs to provide an optimized 64bit
 * divide.
 */
static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

/**
 * div_s64_rem - signed 64bit divide with 32bit divisor with remainder
 */
static inline s64 div_s64_rem(s64 dividend, s32 divisor, s32 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

/**
 * div64_u64 - unsigned 64bit divide with 64bit divisor
 */
static inline u64 div64_u64(u64 dividend, u64 divisor)
{
	return dividend / divisor;
}

#elif BITS_PER_LONG == 32

#ifndef div_u64_rem
static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = do_div(dividend, divisor);
	return dividend;
}
#endif

#ifndef div_s64_rem
extern s64 div_s64_rem(s64 dividend, s32 divisor, s32 *remainder);
#endif

#ifndef div64_u64
extern u64 div64_u64(u64 dividend, u64 divisor);
#endif

#endif /* BITS_PER_LONG */

/**
 * div_u64 - unsigned 64bit divide with 32bit divisor
 *
 * This is the most common 64bit divide and should be used if possible,
 * as many 32bit archs can optimize this variant better than a full 64bit
 * divide.
 */
#ifndef div_u64
static inline u64 div_u64(u64 dividend, u32 divisor)
{
	u32 remainder;
	return div_u64_rem(dividend, divisor, &remainder);
}
#endif

/**
 * div_s64 - signed 64bit divide with 32bit divisor
 */
#ifndef div_s64
static inline s64 div_s64(s64 dividend, s32 divisor)
{
	s32 remainder;
	return div_s64_rem(dividend, divisor, &remainder);
}
#endif

u32 iter_div_u64_rem(u64 dividend, u32 divisor, u64 *remainder);

#endif /* _LINUX_MATH64_H */
