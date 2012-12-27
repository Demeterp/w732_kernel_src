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
 * (C) Copyright 2003
 * Gerry Hamel, geh@ti.com, Texas Instruments
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include <common.h>
#include <malloc.h>

#include <circbuf.h>


int buf_init (circbuf_t * buf, unsigned int size)
{
	assert (buf != NULL);

	buf->size = 0;
	buf->totalsize = size;
	buf->data = (char *) malloc (sizeof (char) * size);
	assert (buf->data != NULL);

	buf->top = buf->data;
	buf->tail = buf->data;
	buf->end = &(buf->data[size]);

	return 1;
}

int buf_free (circbuf_t * buf)
{
	assert (buf != NULL);
	assert (buf->data != NULL);

	free (buf->data);
	memset (buf, 0, sizeof (circbuf_t));

	return 1;
}

int buf_pop (circbuf_t * buf, char *dest, unsigned int len)
{
	unsigned int i;
	char *p = buf->top;

	assert (buf != NULL);
	assert (dest != NULL);

	/* Cap to number of bytes in buffer */
	if (len > buf->size)
		len = buf->size;

	for (i = 0; i < len; i++) {
		dest[i] = *p++;
		/* Bounds check. */
		if (p == buf->end) {
			p = buf->data;
		}
	}

	/* Update 'top' pointer */
	buf->top = p;
	buf->size -= len;

	return len;
}

int buf_push (circbuf_t * buf, const char *src, unsigned int len)
{
	/* NOTE:  this function allows push to overwrite old data. */
	unsigned int i;
	char *p = buf->tail;

	assert (buf != NULL);
	assert (src != NULL);

	for (i = 0; i < len; i++) {
		*p++ = src[i];
		if (p == buf->end) {
			p = buf->data;
		}
		/* Make sure pushing too much data just replaces old data */
		if (buf->size < buf->totalsize) {
			buf->size++;
		} else {
			buf->top++;
			if (buf->top == buf->end) {
				buf->top = buf->data;
			}
		}
	}

	/* Update 'tail' pointer */
	buf->tail = p;

	return len;
}
