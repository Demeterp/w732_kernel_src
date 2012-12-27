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


const char *month [] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

Fs_t    fs;
File_t  file;

/*-----------------------------------------------------------------------------
 * dos_open --
 *-----------------------------------------------------------------------------
 */
int dos_open(char *name)
{
    int lg;
    int entry;
    char *fname;

    /* We need to suppress the " char around the name                        */
    if (name [0] == '"') {
	name ++;
    }
    lg = strlen (name);
    if (name [lg - 1] == '"') {
	name [lg - 1] = '\0';
    }

    /* Open file system                                                      */
    if (fs_init (&fs) < 0) {
	return -1;
    }

    /* Init the file descriptor                                              */
    file.name = name;
    file.fs = &fs;

    /* find the subdirectory containing the file                             */
    if (open_subdir (&file) < 0) {
	return (-1);
    }

    fname = basename (name);

    /* if we try to open root directory                                      */
    if (*fname == '\0') {
	file.file = file.subdir;
	return (0);
    }

    /* find the file in the subdir                                           */
    entry = 0;
    if (vfat_lookup (&file.subdir,
		     file.fs,
		     &file.file.dir,
		     &entry,
		     0,
		     fname,
		     ACCEPT_DIR | ACCEPT_PLAIN | SINGLE | DO_OPEN,
		     0,
		     &file.file) != 0) {
	/* File not found                                                    */
	printf ("File not found\n");
	return (-1);
    }

    return 0;
}

/*-----------------------------------------------------------------------------
 * dos_read --
 *-----------------------------------------------------------------------------
 */
int dos_read (ulong addr)
{
    int read = 0, nb;

    /* Try to boot a directory ?                                             */
    if (file.file.dir.attr & (ATTR_DIRECTORY | ATTR_VOLUME)) {
	printf ("Unable to boot %s !!\n", file.name);
	return (-1);
    }
    while (read < file.file.FileSize) {
	PRINTF ("read_file (%ld)\n", (file.file.FileSize - read));
	nb = read_file (&fs,
			&file.file,
			(char *)addr + read,
			read,
			(file.file.FileSize - read));
	PRINTF ("read_file -> %d\n", nb);
	if (nb < 0) {
	    printf ("read error\n");
	    return (-1);
	}
	read += nb;
    }
    return (read);
}
/*-----------------------------------------------------------------------------
 * dos_dir --
 *-----------------------------------------------------------------------------
 */
int dos_dir (void)
{
    int entry;
    Directory_t dir;
    char *name;


    if ((file.file.dir.attr & ATTR_DIRECTORY) == 0) {
	printf ("%s: not a directory !!\n", file.name);
	return (1);
    }
    entry = 0;
    if ((name = malloc (MAX_VNAMELEN + 1)) == NULL) {
	PRINTF ("Allcation error\n");
	return (1);
    }

    while (vfat_lookup (&file.file,
			file.fs,
			&dir,
			&entry,
			0,
			NULL,
			ACCEPT_DIR | ACCEPT_PLAIN | MATCH_ANY,
			name,
			NULL) == 0) {
	/* Display file info                                                 */
	printf ("%3.3s %9d %s %02d %04d %02d:%02d:%02d %s\n",
		(dir.attr & ATTR_DIRECTORY) ? "dir" : "   ",
		__le32_to_cpu (dir.size),
		month [DOS_MONTH (&dir) - 1],
		DOS_DAY (&dir),
		DOS_YEAR (&dir),
		DOS_HOUR (&dir),
		DOS_MINUTE (&dir),
		DOS_SEC (&dir),
		name);

    }
    free (name);
    return (0);
}
