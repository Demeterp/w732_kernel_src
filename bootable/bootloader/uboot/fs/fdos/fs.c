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
 * Stäubli Faverges - <www.staubli.com>
 * Pierre AUBERT  p.aubert@staubli.com
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

#include <common.h>
#include <config.h>
#include <malloc.h>

#include "dos.h"
#include "fdos.h"


/*-----------------------------------------------------------------------------
 * fill_fs -- Read info on file system
 *-----------------------------------------------------------------------------
 */
static int fill_fs (BootSector_t *boot, Fs_t *fs)
{

    fs -> fat_start = __le16_to_cpu (boot -> nrsvsect);
    fs -> fat_len = __le16_to_cpu (boot -> fatlen);
    fs -> nb_fat = boot -> nfat;

    fs -> dir_start = fs -> fat_start + fs -> nb_fat * fs -> fat_len;
    fs -> dir_len = __le16_to_cpu (boot -> dirents) * MDIR_SIZE / SZ_STD_SECTOR;
    fs -> cluster_size = boot -> clsiz;
    fs -> num_clus = (fs -> tot_sectors - fs -> dir_start - fs -> dir_len) / fs -> cluster_size;

    return (0);
}

/*-----------------------------------------------------------------------------
 * fs_init --
 *-----------------------------------------------------------------------------
 */
int fs_init (Fs_t *fs)
{
    BootSector_t *boot;

    /* Initialize physical device                                            */
    if (dev_open () < 0) {
	PRINTF ("Unable to initialize the fdc\n");
	return (-1);
    }
    init_subdir ();

    /* Allocate space for read the boot sector                               */
    if ((boot = (BootSector_t *)malloc (sizeof (BootSector_t))) == NULL) {
	PRINTF ("Unable to allocate space for boot sector\n");
	return (-1);
    }

    /* read boot sector                                                      */
    if (dev_read (boot, 0, 1)){
	PRINTF ("Error during boot sector read\n");
	free (boot);
	return (-1);
    }

    /* we verify it'a a DOS diskette                                         */
    if (boot -> jump [0] !=  JUMP_0_1 && boot -> jump [0] !=  JUMP_0_2) {
	PRINTF ("Not a DOS diskette\n");
	free (boot);
	return (-1);
    }

    if (boot -> descr < MEDIA_STD) {
	/* We handle only recent medias (type F0)                            */
	PRINTF ("unrecognized diskette type\n");
	free (boot);
	return (-1);
    }

    if (check_dev (boot, fs) < 0) {
	PRINTF ("Bad diskette\n");
	free (boot);
	return (-1);
    }

    if (fill_fs (boot, fs) < 0) {
	free (boot);

	return (-1);
    }

    /* Read FAT                                                              */
    if (read_fat (boot, fs) < 0) {
	free (boot);
	return (-1);
    }

    free (boot);
    return (0);
}
