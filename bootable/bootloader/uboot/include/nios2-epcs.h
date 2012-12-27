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
 * (C) Copyright 2004, Psyent Corporation <www.psyent.com>
 * Scott McNutt <smcnutt@psyent.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*************************************************************************
 * Altera Nios-II EPCS Controller Core interfaces
 ************************************************************************/

#ifndef __NIOS2_EPCS_H__
#define __NIOS2_EPCS_H__

typedef struct epcs_devinfo_t {
	const char	*name;		/* Device name */
	unsigned char	id;		/* Device silicon id */
	unsigned char	size;		/* Total size log2(bytes)*/
	unsigned char	num_sects;	/* Number of sectors */
	unsigned char	sz_sect;	/* Sector size log2(bytes) */
	unsigned char	sz_page;	/* Page size log2(bytes) */
	unsigned char   prot_mask;	/* Protection mask */
}epcs_devinfo_t;

/* Resets the epcs controller -- to prevent (potential) soft-reset
 * problems when booting from the epcs controller
 */
extern int epcs_reset (void);

/* Returns the devinfo struct if EPCS device is found;
 * NULL otherwise.
 */
extern epcs_devinfo_t *epcs_dev_find (void);

/* Returns the number of bytes used by config data.
 * Negative on error.
 */
extern int epcs_cfgsz (void);

/* Erase sectors 'start' to 'end' - return zero on success
 */
extern int epcs_erase (unsigned start, unsigned end);

/* Read 'cnt' bytes from device offset 'off' into buf at 'addr'
 * Zero return on success
 */
extern int epcs_read (ulong addr, ulong off, ulong cnt);

/* Write 'cnt' bytes to device offset 'off' from buf at 'addr'.
 * Zero return on success
 */
extern int epcs_write (ulong addr, ulong off, ulong cnt);

/* Verify 'cnt' bytes at device offset 'off' comparing with buf
 * at 'addr'. On failure, write first invalid offset to *err.
 * Zero return on success
 */
extern int epcs_verify (ulong addr, ulong off, ulong cnt, ulong *err);

#endif /* __NIOS2_EPCS_H__ */
