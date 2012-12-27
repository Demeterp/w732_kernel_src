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

#ifndef _FDOS_H_
#define _FDOS_H_


#undef	FDOS_DEBUG

#ifdef	FDOS_DEBUG
#define	PRINTF(fmt,args...)	printf (fmt ,##args)
#else
#define PRINTF(fmt,args...)
#endif

/* Data structure describing media                                           */
typedef struct fs
{
    unsigned long       tot_sectors;

    int                 cluster_size;
    int                 num_clus;

    int                 fat_start;
    int                 fat_len;
    int                 nb_fat;
    int                 num_fat;

    int                 dir_start;
    int                 dir_len;

    unsigned char       *fat_buf;

} Fs_t;

/* Data structure describing one file system slot                            */
typedef struct slot {
    int (*map) (struct fs *fs,
		struct slot *file,
		int where,
		int *len);
    unsigned long FileSize;

    unsigned short int FirstAbsCluNr;
    unsigned short int PreviousAbsCluNr;
    unsigned short int PreviousRelCluNr;

    Directory_t dir;
} Slot_t;

typedef struct file {
    char                *name;
    int                 Case;
    Fs_t                *fs;
    Slot_t              subdir;
    Slot_t              file;
} File_t;


/* dev.c                                                                     */
int dev_read (void *buffer, int where, int len);
int dev_open (void);
int check_dev (BootSector_t *boot, Fs_t *fs);

/* fat.c                                                                     */
unsigned int fat_decode (Fs_t *fs, unsigned int num);
int read_fat (BootSector_t *boot, Fs_t *fs);

/* vfat.c                                                                    */
int vfat_lookup (Slot_t *dir,
		 Fs_t *fs,
		 Directory_t *dirent,
		 int *entry,
		 int *vfat_start,
		 char *filename,
		 int flags,
		 char *outname,
		 Slot_t *file);

/* subdir.c                                                                  */
char *basename (char *name);
int open_subdir (File_t *desc);
int open_file (Slot_t *file, Directory_t *dir);
int read_file (Fs_t *fs,
	       Slot_t *file,
	       char *buf,
	       int where,
	       int len);
void init_subdir (void);

/* fs.c                                                                      */
int fs_init (Fs_t *fs);


#endif
