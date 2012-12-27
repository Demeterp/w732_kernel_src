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
 * (C) Copyright 2008
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
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
 *
 */


/*
 * UBIFS command support
 */

#undef DEBUG

#include <common.h>
#include <config.h>
#include <command.h>

static int ubifs_initialized;
static int ubifs_mounted;

/* Prototypes */
int ubifs_init(void);
int ubifs_mount(char *vol_name);
int ubifs_ls(char *dir_name);
int ubifs_load(char *filename, u32 addr, u32 size);

int do_ubifs_mount(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *vol_name;
	int ret;

	if (argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}
	vol_name = argv[1];
	debug("Using volume %s\n", vol_name);

	if (ubifs_initialized == 0) {
		ubifs_init();
		ubifs_initialized = 1;
	}

	ret = ubifs_mount(vol_name);
	if (ret)
		return -1;

	ubifs_mounted = 1;

	return 0;
}

int do_ubifs_ls(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *filename = "/";
	int ret;

	if (!ubifs_mounted) {
		printf("UBIFS not mounted, use ubifs mount to mount volume first!\n");
		return -1;
	}

	if (argc == 2)
		filename = argv[1];
	debug("Using filename %s\n", filename);

	ret = ubifs_ls(filename);
	if (ret)
		printf("%s not found!\n", filename);

	return ret;
}

int do_ubifs_load(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *filename;
	char *endp;
	int ret;
	u32 addr;
	u32 size = 0;

	if (!ubifs_mounted) {
		printf("UBIFS not mounted, use ubifs mount to mount volume first!\n");
		return -1;
	}

	if (argc < 3) {
		cmd_usage(cmdtp);
		return -1;
	}

	addr = simple_strtoul(argv[1], &endp, 16);
	if (endp == argv[1]) {
		cmd_usage(cmdtp);
		return 1;
	}

	filename = argv[2];

	if (argc == 4) {
		size = simple_strtoul(argv[3], &endp, 16);
		if (endp == argv[3]) {
			cmd_usage(cmdtp);
			return 1;
		}
	}
	debug("Loading file '%s' to address 0x%08x (size %d)\n", filename, addr, size);

	ret = ubifs_load(filename, addr, size);
	if (ret)
		printf("%s not found!\n", filename);

	return ret;
}

U_BOOT_CMD(
	ubifsmount, 2, 0, do_ubifs_mount,
	"mount UBIFS volume",
	"<volume-name>\n"
	"    - mount 'volume-name' volume"
);

U_BOOT_CMD(ubifsls, 2, 0, do_ubifs_ls,
	"list files in a directory",
	"[directory]\n"
	"    - list files in a 'directory' (default '/')"
);

U_BOOT_CMD(ubifsload, 4, 0, do_ubifs_load,
	"load file from an UBIFS filesystem",
	"<addr> <filename> [bytes]\n"
	"    - load file 'filename' to address 'addr'"
);
