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
 * Copyright 2008 Extreme Engineering Solutions, Inc.
 *
 * mmap/munmap implementation derived from:
 * Clamav Native Windows Port : mmap win32 compatibility layer
 * Copyright (c) 2005-2006 Gianluigi Tiesi <sherpya@netfarm.it>
 * Parts by Kees Zeelenberg <kzlg@users.sourceforge.net> (LibGW32C)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this software; if not, write to the
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "mingw_support.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <io.h>

int fsync(int fd)
{
	return _commit(fd);
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, int offset)
{
	void *map = NULL;
	HANDLE handle = INVALID_HANDLE_VALUE;
	DWORD cfm_flags = 0, mvf_flags = 0;

	switch (prot) {
	case PROT_READ | PROT_WRITE:
		cfm_flags = PAGE_READWRITE;
		mvf_flags = FILE_MAP_ALL_ACCESS;
		break;
	case PROT_WRITE:
		cfm_flags = PAGE_READWRITE;
		mvf_flags = FILE_MAP_WRITE;
		break;
	case PROT_READ:
		cfm_flags = PAGE_READONLY;
		mvf_flags = FILE_MAP_READ;
		break;
	default:
		return MAP_FAILED;
	}

	handle = CreateFileMappingA((HANDLE) _get_osfhandle(fd), NULL,
				cfm_flags, HIDWORD(len), LODWORD(len), NULL);
	if (!handle)
		return MAP_FAILED;

	map = MapViewOfFile(handle, mvf_flags, HIDWORD(offset),
			LODWORD(offset), len);
	CloseHandle(handle);

	if (!map)
		return MAP_FAILED;

	return map;
}

int munmap(void *addr, size_t len)
{
	if (!UnmapViewOfFile(addr))
		return -1;

	return 0;
}

/* Reentrant string tokenizer.  Generic version.
   Copyright (C) 1991,1996-1999,2001,2004,2007 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

/* Parse S into tokens separated by characters in DELIM.
   If S is NULL, the saved pointer in SAVE_PTR is used as
   the next starting point.  For example:
	char s[] = "-abc-=-def";
	char *sp;
	x = strtok_r(s, "-", &sp);	// x = "abc", sp = "=-def"
	x = strtok_r(NULL, "-=", &sp);	// x = "def", sp = NULL
	x = strtok_r(NULL, "=", &sp);	// x = NULL
		// s = "abc\0-def\0"
*/
char *strtok_r(char *s, const char *delim, char **save_ptr)
{
	char *token;

	if (s == NULL)
		s = *save_ptr;

	/* Scan leading delimiters.  */
	s += strspn(s, delim);
	if (*s == '\0') {
		*save_ptr = s;
		return NULL;
	}

	/* Find the end of the token.  */
	token = s;
	s = strpbrk (token, delim);
	if (s == NULL) {
		/* This token finishes the string.  */
		*save_ptr = memchr(token, '\0', strlen(token));
	} else {
		/* Terminate the token and make *SAVE_PTR point past it.  */
		*s = '\0';
		*save_ptr = s + 1;
	}
	return token;
}

#include "getline.c"
