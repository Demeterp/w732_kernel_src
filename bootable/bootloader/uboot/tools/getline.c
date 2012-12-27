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

/* getline.c -- Replacement for GNU C library function getline

Copyright (C) 1993, 1996, 2001, 2002 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/* Written by Jan Brittenson, bson@gnu.ai.mit.edu.  */

#include <assert.h>
#include <stdio.h>

/* Always add at least this many bytes when extending the buffer.  */
#define MIN_CHUNK 64

/* Read up to (and including) a TERMINATOR from STREAM into *LINEPTR
   + OFFSET (and null-terminate it). *LINEPTR is a pointer returned from
   malloc (or NULL), pointing to *N characters of space.  It is realloc'd
   as necessary.  Return the number of characters read (not including the
   null terminator), or -1 on error or EOF.
   NOTE: There is another getstr() function declared in <curses.h>.  */
static int getstr(char **lineptr, size_t *n, FILE *stream,
		  char terminator, size_t offset)
{
	int nchars_avail;	/* Allocated but unused chars in *LINEPTR.  */
	char *read_pos;		/* Where we're reading into *LINEPTR. */
	int ret;

	if (!lineptr || !n || !stream)
		return -1;

	if (!*lineptr) {
		*n = MIN_CHUNK;
		*lineptr = malloc(*n);
		if (!*lineptr)
			return -1;
	}

	nchars_avail = *n - offset;
	read_pos = *lineptr + offset;

	for (;;) {
		register int c = getc(stream);

		/* We always want at least one char left in the buffer, since we
		   always (unless we get an error while reading the first char)
		   NUL-terminate the line buffer.  */

		assert(*n - nchars_avail == read_pos - *lineptr);
		if (nchars_avail < 2) {
			if (*n > MIN_CHUNK)
				*n *= 2;
			else
				*n += MIN_CHUNK;

			nchars_avail = *n + *lineptr - read_pos;
			*lineptr = realloc(*lineptr, *n);
			if (!*lineptr)
				return -1;
			read_pos = *n - nchars_avail + *lineptr;
			assert(*n - nchars_avail == read_pos - *lineptr);
		}

		if (c == EOF || ferror (stream)) {
			/* Return partial line, if any.  */
			if (read_pos == *lineptr)
				return -1;
			else
				break;
		}

		*read_pos++ = c;
		nchars_avail--;

		if (c == terminator)
			/* Return the line.  */
			break;
	}

	/* Done - NUL terminate and return the number of chars read.  */
	*read_pos = '\0';

	ret = read_pos - (*lineptr + offset);
	return ret;
}

int getline (char **lineptr, size_t *n, FILE *stream)
{
	return getstr(lineptr, n, stream, '\n', 0);
}
