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
 * (C) Copyright 2001
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Command to load a splash screen to the VFDs.
 * NOTE that this will be controlled by a key combination when
 * the keyboard stuff works. For now the user has to enter a
 * bitmap number (only VFD_TEST_LOGO is supported now - 16.10.2002).
 * Added VFD_REMOTE_LOGO (same as VFD_TEST_LOGO but a different color)
 * on 20.10.2002.
 *
 * This rather crudely requires that each bitmap be included as a
 * header file.
 */
#include <common.h>
#include <command.h>

#if defined(CONFIG_CMD_VFD)

#include <vfd_logo.h>
#define VFD_TEST_LOGO_BMPNR 0
#define VFD_REMOTE_LOGO_BMPNR 1

extern int transfer_pic(unsigned char, unsigned char *, int, int);

int trab_vfd (ulong bitmap);

int do_vfd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong bitmap;

	if (argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	if (argv[1][0] == '/') {	/* select bitmap by number */
		bitmap = simple_strtoul(argv[1]+1, NULL, 10);
		return (trab_vfd(bitmap));
	}

	/* display bitmap at given address */
	bitmap = simple_strtoul(argv[1], NULL, 16);
	transfer_pic(3, (uchar *)bitmap, VFD_LOGO_HEIGHT, VFD_LOGO_WIDTH);
	return 0;
}

U_BOOT_CMD(
	vfd,	2,	0,	do_vfd,
	"load a bitmap to the VFDs on TRAB",
	"/N\n"
	"    - load bitmap N to the VFDs (N is _decimal_ !!!)\n"
	"vfd ADDR\n"
	"    - load bitmap at address ADDR"
);
#endif

int trab_vfd (ulong bitmap)
{
	uchar *addr;
	char *s;

	switch (bitmap) {
	case VFD_TEST_LOGO_BMPNR:
		if ((s = getenv ("bitmap0")) != NULL) {
			addr = (uchar *)simple_strtoul (s, NULL, 16);
		} else {
			addr = &vfd_test_logo_bitmap[0];
		}
		break;
	case VFD_REMOTE_LOGO_BMPNR:
		if ((s = getenv ("bitmap1")) != NULL) {
			addr = (uchar *)simple_strtoul (s, NULL, 16);
		} else {
			addr = &vfd_remote_logo_bitmap[0];
		}
		break;
	default:
		printf("Unknown bitmap %ld\n", bitmap);
		return 1;
	}
	transfer_pic(3, addr, VFD_LOGO_HEIGHT, VFD_LOGO_WIDTH);
	return 0;
}
