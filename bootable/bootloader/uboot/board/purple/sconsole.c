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
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>

#include "sconsole.h"

void	(*sconsole_putc) (char) = 0;
void	(*sconsole_puts) (const char *) = 0;
int	(*sconsole_getc) (void) = 0;
int	(*sconsole_tstc) (void) = 0;
void	(*sconsole_setbrg) (void) = 0;

int serial_init (void)
{
	sconsole_buffer_t *sb = SCONSOLE_BUFFER;

	sb->pos  = 0;
	sb->size = 0;
	sb->max_size = CONFIG_SYS_SCONSOLE_SIZE - sizeof (sconsole_buffer_t);

	return (0);
}

void serial_putc (char c)
{
	if (sconsole_putc) {
		(*sconsole_putc) (c);
	} else {
		sconsole_buffer_t *sb = SCONSOLE_BUFFER;

		if (c) {
			sb->data[sb->pos++] = c;
			if (sb->pos == sb->max_size) {
				sb->pos = 0;
			}
			if (sb->size < sb->max_size) {
				sb->size++;
			}
		}
	}
}

void serial_puts (const char *s)
{
	if (sconsole_puts) {
		(*sconsole_puts) (s);
	} else {
		sconsole_buffer_t *sb = SCONSOLE_BUFFER;

		while (*s) {
			sb->data[sb->pos++] = *s++;
			if (sb->pos == sb->max_size) {
				sb->pos = 0;
			}
			if (sb->size < sb->max_size) {
				sb->size++;
			}
		}
	}
}

int serial_getc (void)
{
	if (sconsole_getc) {
		return (*sconsole_getc) ();
	} else {
		return 0;
	}
}

int serial_tstc (void)
{
	if (sconsole_tstc) {
		return (*sconsole_tstc) ();
	} else {
		return 0;
	}
}

void serial_setbrg (void)
{
	if (sconsole_setbrg) {
		(*sconsole_setbrg) ();
	}
}

void sconsole_flush (void)
{
	if (sconsole_putc) {
		sconsole_buffer_t *sb = SCONSOLE_BUFFER;
		unsigned int end = sb->pos < sb->size
				? sb->pos + sb->max_size - sb->size
				: sb->pos - sb->size;

		while (sb->size) {
			(*sconsole_putc) (sb->data[end++]);
			if (end == sb->max_size) {
				end = 0;
			}
			sb->size--;
		}
	}
}
