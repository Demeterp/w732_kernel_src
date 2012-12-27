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

/*
 * Dos floppy support
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <fdc.h>

/*-----------------------------------------------------------------------------
 * do_fdosboot --
 *-----------------------------------------------------------------------------
 */
int do_fdosboot(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *name;
    char *ep;
    int size;
    int rcode = 0;
    char buf [12];
    int drive = CONFIG_SYS_FDC_DRIVE_NUMBER;

    /* pre-set load_addr */
    if ((ep = getenv("loadaddr")) != NULL) {
	load_addr = simple_strtoul(ep, NULL, 16);
    }

    /* pre-set Boot file name */
    if ((name = getenv("bootfile")) == NULL) {
	name = "uImage";
    }

    switch (argc) {
    case 1:
	break;
    case 2:
	/* only one arg - accept two forms:
	 * just load address, or just boot file name.
	 * The latter form must be written "filename" here.
	 */
	if (argv[1][0] == '"') {	/* just boot filename */
	    name = argv [1];
	} else {			/* load address	*/
	    load_addr = simple_strtoul(argv[1], NULL, 16);
	}
	break;
    case 3:
	load_addr = simple_strtoul(argv[1], NULL, 16);
	name = argv [2];
	break;
    default:
	cmd_usage(cmdtp);
	break;
    }

    /* Init physical layer                                                   */
    if (!fdc_fdos_init (drive)) {
	return (-1);
    }

    /* Open file                                                             */
    if (dos_open (name) < 0) {
	printf ("Unable to open %s\n", name);
	return 1;
    }
    if ((size = dos_read (load_addr)) < 0) {
	printf ("boot error\n");
	return 1;
    }
    flush_cache (load_addr, size);

    sprintf(buf, "%x", size);
    setenv("filesize", buf);

    printf("Floppy DOS load complete: %d bytes loaded to 0x%lx\n",
	   size, load_addr);

    /* Check if we should attempt an auto-start */
    if (((ep = getenv("autostart")) != NULL) && (strcmp(ep,"yes") == 0)) {
	char *local_args[2];
	extern int do_bootm (cmd_tbl_t *, int, int, char *[]);
	local_args[0] = argv[0];
	local_args[1] = NULL;
	printf ("Automatic boot of image at addr 0x%08lX ...\n", load_addr);
	rcode = do_bootm (cmdtp, 0, 1, local_args);
    }
    return rcode;
}

/*-----------------------------------------------------------------------------
 * do_fdosls --
 *-----------------------------------------------------------------------------
 */
int do_fdosls(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *path = "";
    int drive = CONFIG_SYS_FDC_DRIVE_NUMBER;

    switch (argc) {
    case 1:
	break;
    case 2:
	path = argv [1];
	break;
    }

    /* Init physical layer                                                   */
    if (!fdc_fdos_init (drive)) {
	return (-1);
    }
    /* Open directory                                                        */
    if (dos_open (path) < 0) {
	printf ("Unable to open %s\n", path);
	return 1;
    }
    return (dos_dir ());
}

U_BOOT_CMD(
	fdosboot,	3,	0,	do_fdosboot,
	"boot from a dos floppy file",
	"[loadAddr] [filename]"
);

U_BOOT_CMD(
	fdosls,	2,	0,	do_fdosls,
	"list files in a directory",
	"[directory]"
);
