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
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright (C) 2001  Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <asm/zimage.h>

/*cmd_boot.c*/
int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
{
	void		*base_ptr;
	ulong		os_data, os_len;
	image_header_t	*hdr;

#if defined(CONFIG_FIT)
	const void	*data;
	size_t		len;
#endif

	if ((flag != 0) && (flag != BOOTM_STATE_OS_GO))
		return 1;

	if (images->legacy_hdr_valid) {
		hdr = images->legacy_hdr_os;
		if (image_check_type (hdr, IH_TYPE_MULTI)) {
			/* if multi-part image, we need to get first subimage */
			image_multi_getimg (hdr, 0, &os_data, &os_len);
		} else {
			/* otherwise get image data */
			os_data = image_get_data (hdr);
			os_len = image_get_data_size (hdr);
		}
#if defined(CONFIG_FIT)
	} else if (images->fit_uname_os) {
		ret = fit_image_get_data (images->fit_hdr_os,
					images->fit_noffset_os, &data, &len);
		if (ret) {
			puts ("Can't get image data/size!\n");
			goto error;
		}
		os_data = (ulong)data;
		os_len = (ulong)len;
#endif
	} else {
		puts ("Could not find kernel image!\n");
		goto error;
	}

	base_ptr = load_zimage ((void*)os_data, os_len,
			images->rd_start, images->rd_end - images->rd_start, 0);

	if (NULL == base_ptr) {
		printf ("## Kernel loading failed ...\n");
		goto error;

	}

#ifdef DEBUG
	printf ("## Transferring control to Linux (at address %08x) ...\n",
		(u32)base_ptr);
#endif

	/* we assume that the kernel is in place */
	printf("\nStarting kernel ...\n\n");

	boot_zimage(base_ptr);
	/* does not return */

error:
	return 1;
}
