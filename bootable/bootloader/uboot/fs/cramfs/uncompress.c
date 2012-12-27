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
 * uncompress.c
 *
 * Copyright (C) 1999 Linus Torvalds
 * Copyright (C) 2000-2002 Transmeta Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 *
 * cramfs interfaces to the uncompression library. There's really just
 * three entrypoints:
 *
 *  - cramfs_uncompress_init() - called to initialize the thing.
 *  - cramfs_uncompress_exit() - tell me when you're done
 *  - cramfs_uncompress_block() - uncompress a block.
 *
 * NOTE NOTE NOTE! The uncompression is entirely single-threaded. We
 * only have one stream, and we'll initialize it only once even if it
 * then is used by multiple filesystems.
 */

#include <common.h>
#include <malloc.h>
#include <watchdog.h>
#include <u-boot/zlib.h>

static z_stream stream;

void *zalloc(void *, unsigned, unsigned);
void zfree(void *, void *, unsigned);

/* Returns length of decompressed data. */
int cramfs_uncompress_block (void *dst, void *src, int srclen)
{
	int err;

	inflateReset (&stream);

	stream.next_in = src;
	stream.avail_in = srclen;

	stream.next_out = dst;
	stream.avail_out = 4096 * 2;

	err = inflate (&stream, Z_FINISH);

	if (err != Z_STREAM_END)
		goto err;
	return stream.total_out;

      err:
	/*printf ("Error %d while decompressing!\n", err); */
	/*printf ("%p(%d)->%p\n", src, srclen, dst); */
	return -1;
}

int cramfs_uncompress_init (void)
{
	int err;

	stream.zalloc = zalloc;
	stream.zfree = zfree;
	stream.next_in = 0;
	stream.avail_in = 0;

#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	stream.outcb = (cb_func) WATCHDOG_RESET;
#else
	stream.outcb = Z_NULL;
#endif /* CONFIG_HW_WATCHDOG */

	err = inflateInit (&stream);
	if (err != Z_OK) {
		printf ("Error: inflateInit2() returned %d\n", err);
		return -1;
	}

	return 0;
}

int cramfs_uncompress_exit (void)
{
	inflateEnd (&stream);
	return 0;
}
