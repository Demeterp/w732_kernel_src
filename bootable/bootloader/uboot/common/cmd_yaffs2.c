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

#include <common.h>

#include <config.h>
#include <command.h>

#ifdef  YAFFS2_DEBUG
#define PRINTF(fmt,args...) printf (fmt ,##args)
#else
#define PRINTF(fmt,args...)
#endif

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern void cmd_yaffs_read_file(char *fn);
extern void cmd_yaffs_write_file(char *fn,char bval,int sizeOfFile);
extern void cmd_yaffs_ls(const char *mountpt, int longlist);
extern void cmd_yaffs_mwrite_file(char *fn, char *addr, int size);
extern void cmd_yaffs_mread_file(char *fn, char *addr);
extern void cmd_yaffs_mkdir(const char *dir);
extern void cmd_yaffs_rmdir(const char *dir);
extern void cmd_yaffs_rm(const char *path);
extern void cmd_yaffs_mv(const char *oldPath, const char *newPath);

extern int yaffs_DumpDevStruct(const char *path);


int do_ymount (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *mtpoint = argv[1];
    cmd_yaffs_mount(mtpoint);

    return(0);
}

int do_yumount (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *mtpoint = argv[1];
    cmd_yaffs_umount(mtpoint);

    return(0);
}

int do_yls (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *dirname = argv[argc-1];

    cmd_yaffs_ls(dirname, (argc>2)?1:0);

    return(0);
}

int do_yrd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *filename = argv[1];
    printf ("Reading file %s ", filename);

    cmd_yaffs_read_file(filename);

    printf ("done\n");
    return(0);
}

int do_ywr (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *filename = argv[1];
    ulong value = simple_strtoul(argv[2], NULL, 16);
    ulong numValues = simple_strtoul(argv[3], NULL, 16);

    printf ("Writing value (%x) %x times to %s... ", value, numValues, filename);

    cmd_yaffs_write_file(filename,value,numValues);

    printf ("done\n");
    return(0);
}

int do_yrdm (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *filename = argv[1];
    ulong addr = simple_strtoul(argv[2], NULL, 16);

    cmd_yaffs_mread_file(filename, (char *)addr);

    return(0);
}

int do_ywrm (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *filename = argv[1];
    ulong addr = simple_strtoul(argv[2], NULL, 16);
    ulong size = simple_strtoul(argv[3], NULL, 16);

    cmd_yaffs_mwrite_file(filename, (char *)addr, size);

    return(0);
}

int do_ymkdir (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *dirname = argv[1];

    cmd_yaffs_mkdir(dirname);

    return(0);
}

int do_yrmdir (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *dirname = argv[1];

    cmd_yaffs_rmdir(dirname);

    return(0);
}

int do_yrm (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *path = argv[1];

    cmd_yaffs_rm(path);

    return(0);
}

int do_ymv (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *oldPath = argv[1];
    char *newPath = argv[2];

    cmd_yaffs_mv(newPath, oldPath);

    return(0);
}

int do_ydump (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *dirname = argv[1];
    if (yaffs_DumpDevStruct(dirname) != 0)
	printf("yaffs_DumpDevStruct returning error when dumping path: , %s\n", dirname);
    return 0;
}

U_BOOT_CMD(
    ymount, 3,  0,  do_ymount,
    "mount yaffs",
    ""
);

U_BOOT_CMD(
    yumount, 3,  0,  do_yumount,
    "unmount yaffs",
    ""
);

U_BOOT_CMD(
    yls,    4,  0,  do_yls,
    "yaffs ls",
    "[-l] name"
);

U_BOOT_CMD(
    yrd,    2,  0,  do_yrd,
    "read file from yaffs",
    "filename"
);

U_BOOT_CMD(
    ywr,    4,  0,  do_ywr,
    "write file to yaffs",
    "filename value num_vlues"
);

U_BOOT_CMD(
    yrdm,   3,  0,  do_yrdm,
    "read file to memory from yaffs",
    "filename offset"
);

U_BOOT_CMD(
    ywrm,   4,  0,  do_ywrm,
    "write file from memory to yaffs",
    "filename offset size"
);

U_BOOT_CMD(
    ymkdir, 2,  0,  do_ymkdir,
    "YAFFS mkdir",
    "dirname"
);

U_BOOT_CMD(
    yrmdir, 2,  0,  do_yrmdir,
    "YAFFS rmdir",
    "dirname"
);

U_BOOT_CMD(
    yrm,    2,  0,  do_yrm,
    "YAFFS rm",
    "path"
);

U_BOOT_CMD(
    ymv,    4,  0,  do_ymv,
    "YAFFS mv",
    "oldPath newPath"
);

U_BOOT_CMD(
    ydump,  2,  0,  do_ydump,
    "YAFFS device struct",
    "dirname"
);
