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

#ifndef __LINUX_COMPILER_H
#error "Please don't include <linux/compiler-gcc4.h> directly, include <linux/compiler.h> instead."
#endif

/* GCC 4.1.[01] miscompiles __weak */
#ifdef __KERNEL__
# if __GNUC_MINOR__ == 1 && __GNUC_PATCHLEVEL__ <= 1
#  error Your version of gcc miscompiles the __weak directive
# endif
#endif

#define __used			__attribute__((__used__))
#define __must_check 		__attribute__((warn_unused_result))
#define __compiler_offsetof(a,b) __builtin_offsetof(a,b)
#define __always_inline		inline __attribute__((always_inline))

/*
 * A trick to suppress uninitialized variable warning without generating any
 * code
 */
#define uninitialized_var(x) x = x

#if __GNUC_MINOR__ >= 3
/* Mark functions as cold. gcc will assume any path leading to a call
   to them will be unlikely.  This means a lot of manual unlikely()s
   are unnecessary now for any paths leading to the usual suspects
   like BUG(), printk(), panic() etc. [but let's keep them for now for
   older compilers]

   Early snapshots of gcc 4.3 don't support this and we can't detect this
   in the preprocessor, but we can live with this because they're unreleased.
   Maketime probing would be overkill here.

   gcc also has a __attribute__((__hot__)) to move hot functions into
   a special section, but I don't see any sense in this right now in
   the kernel context */
#define __cold			__attribute__((__cold__))


#if __GNUC_MINOR__ >= 5
/*
 * Mark a position in code as unreachable.  This can be used to
 * suppress control flow warnings after asm blocks that transfer
 * control elsewhere.
 *
 * Early snapshots of gcc 4.5 don't support this and we can't detect
 * this in the preprocessor, but we can live with this because they're
 * unreleased.  Really, we need to have autoconf for the kernel.
 */
#define unreachable() __builtin_unreachable()
#endif

#endif

#if __GNUC_MINOR__ > 0
#define __compiletime_object_size(obj) __builtin_object_size(obj, 0)
#endif
#if __GNUC_MINOR__ >= 4
#define __compiletime_warning(message) __attribute__((warning(message)))
#define __compiletime_error(message) __attribute__((error(message)))
#endif
