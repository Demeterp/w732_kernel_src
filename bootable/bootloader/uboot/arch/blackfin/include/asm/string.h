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
 * U-boot - string.h String functions
 *
 * Copyright (c) 2005-2007 Analog Devices Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

/* Changed by Lineo Inc. May 2001 */

#ifndef _BLACKFINNOMMU_STRING_H_
#define _BLACKFINNOMMU_STRING_H_

#ifdef __KERNEL__		/* only set these up for kernel code */

#define __HAVE_ARCH_STRCPY
#define __HAVE_ARCH_STRNCPY
#define __HAVE_ARCH_STRCMP
#define __HAVE_ARCH_STRNCMP
#define __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMCMP
#define __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_MEMMOVE

extern char *strcpy(char *dest, const char *src);
extern char *strncpy(char *dest, const char *src, size_t n);
extern int strcmp(const char *cs, const char *ct);
extern int strncmp(const char *cs, const char *ct, size_t count);
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *s, int c, size_t count);
extern int memcmp(const void *, const void *, size_t);
extern void *memmove(void *dest, const void *src, size_t count);

#else				/* KERNEL */

/*
 * let user libraries deal with these,
 * IMHO the kernel has no place defining these functions for user apps
 */

#define __HAVE_ARCH_STRCPY	1
#define __HAVE_ARCH_STRNCPY	1
#define __HAVE_ARCH_STRCAT	1
#define __HAVE_ARCH_STRNCAT	1
#define __HAVE_ARCH_STRCMP	1
#define __HAVE_ARCH_STRNCMP	1
#define __HAVE_ARCH_STRNICMP	1
#define __HAVE_ARCH_STRCHR	1
#define __HAVE_ARCH_STRRCHR	1
#define __HAVE_ARCH_STRSTR	1
#define __HAVE_ARCH_STRLEN	1
#define __HAVE_ARCH_STRNLEN	1
#define __HAVE_ARCH_MEMSET	1
#define __HAVE_ARCH_MEMCPY	1
#define __HAVE_ARCH_MEMMOVE	1
#define __HAVE_ARCH_MEMSCAN	1
#define __HAVE_ARCH_MEMCMP	1
#define __HAVE_ARCH_MEMCHR	1
#define __HAVE_ARCH_STRTOK	1

#endif				/* KERNEL */

#endif				/* _BLACKFIN_STRING_H_ */
