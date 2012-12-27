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
 * Copyright 2000-2009
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

#include <common.h>
#include <command.h>

int do_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char **ap;
	int left, adv, expr, last_expr, neg, last_cmp;

	/* args? */
	if (argc < 3)
		return 1;

#if 0
	{
		printf("test:");
		left = 1;
		while (argv[left])
			printf(" %s", argv[left++]);
	}
#endif

	last_expr = 0;
	left = argc - 1; ap = argv + 1;
	if (left > 0 && strcmp(ap[0], "!") == 0) {
		neg = 1;
		ap++;
		left--;
	} else
		neg = 0;

	expr = -1;
	last_cmp = -1;
	last_expr = -1;
	while (left > 0) {

		if (strcmp(ap[0], "-o") == 0 || strcmp(ap[0], "-a") == 0)
			adv = 1;
		else if (strcmp(ap[0], "-z") == 0 || strcmp(ap[0], "-n") == 0)
			adv = 2;
		else
			adv = 3;

		if (left < adv) {
			expr = 1;
			break;
		}

		if (adv == 1) {
			if (strcmp(ap[0], "-o") == 0) {
				last_expr = expr;
				last_cmp = 0;
			} else if (strcmp(ap[0], "-a") == 0) {
				last_expr = expr;
				last_cmp = 1;
			} else {
				expr = 1;
				break;
			}
		}

		if (adv == 2) {
			if (strcmp(ap[0], "-z") == 0)
				expr = strlen(ap[1]) == 0 ? 1 : 0;
			else if (strcmp(ap[0], "-n") == 0)
				expr = strlen(ap[1]) == 0 ? 0 : 1;
			else {
				expr = 1;
				break;
			}

			if (last_cmp == 0)
				expr = last_expr || expr;
			else if (last_cmp == 1)
				expr = last_expr && expr;
			last_cmp = -1;
		}

		if (adv == 3) {
			if (strcmp(ap[1], "=") == 0)
				expr = strcmp(ap[0], ap[2]) == 0;
			else if (strcmp(ap[1], "!=") == 0)
				expr = strcmp(ap[0], ap[2]) != 0;
			else if (strcmp(ap[1], ">") == 0)
				expr = strcmp(ap[0], ap[2]) > 0;
			else if (strcmp(ap[1], "<") == 0)
				expr = strcmp(ap[0], ap[2]) < 0;
			else if (strcmp(ap[1], "-eq") == 0)
				expr = simple_strtol(ap[0], NULL, 10) == simple_strtol(ap[2], NULL, 10);
			else if (strcmp(ap[1], "-ne") == 0)
				expr = simple_strtol(ap[0], NULL, 10) != simple_strtol(ap[2], NULL, 10);
			else if (strcmp(ap[1], "-lt") == 0)
				expr = simple_strtol(ap[0], NULL, 10) < simple_strtol(ap[2], NULL, 10);
			else if (strcmp(ap[1], "-le") == 0)
				expr = simple_strtol(ap[0], NULL, 10) <= simple_strtol(ap[2], NULL, 10);
			else if (strcmp(ap[1], "-gt") == 0)
				expr = simple_strtol(ap[0], NULL, 10) > simple_strtol(ap[2], NULL, 10);
			else if (strcmp(ap[1], "-ge") == 0)
				expr = simple_strtol(ap[0], NULL, 10) >= simple_strtol(ap[2], NULL, 10);
			else {
				expr = 1;
				break;
			}

			if (last_cmp == 0)
				expr = last_expr || expr;
			else if (last_cmp == 1)
				expr = last_expr && expr;
			last_cmp = -1;
		}

		ap += adv; left -= adv;
	}

	if (neg)
		expr = !expr;

	expr = !expr;

	debug (": returns %d\n", expr);

	return expr;
}

U_BOOT_CMD(
	test,	CONFIG_SYS_MAXARGS,	1,	do_test,
	"minimal test like /bin/sh",
	"[args..]"
);

int do_false(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	return 1;
}

U_BOOT_CMD(
	false,	CONFIG_SYS_MAXARGS,	1,	do_false,
	"do nothing, unsuccessfully",
	NULL
);

int do_true(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	return 0;
}

U_BOOT_CMD(
	true,	CONFIG_SYS_MAXARGS,	1,	do_true,
	"do nothing, successfully",
	NULL
);
