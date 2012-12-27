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
 * Copyright 2008 Freescale Semiconductor, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * This file provides a shell like 'expr' function to return.
 */

#include <common.h>
#include <config.h>
#include <command.h>

static ulong get_arg(char *s, int w)
{
	ulong *p;

        /*
	 * if the parameter starts with a '*' then assume
	 * it is a pointer to the value we want
	 */

	if (s[0] == '*') {
		p = (ulong *)simple_strtoul(&s[1], NULL, 16);
		switch (w) {
		case 1: return((ulong)(*(uchar *)p));
		case 2: return((ulong)(*(ushort *)p));
		case 4:
		default: return(*p);
		}
	} else {
		return simple_strtoul(s, NULL, 16);
	}
}

int do_setexpr(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong a, b;
	char buf[16];
	int w;

	/* Validate arguments */
	if ((argc != 5) || (strlen(argv[3]) != 1)) {
		cmd_usage(cmdtp);
		return 1;
	}

	w = cmd_get_data_size(argv[0], 4);

	a = get_arg(argv[2], w);
	b = get_arg(argv[4], w);

	switch (argv[3][0]) {
	case '|': sprintf(buf, "%lx", (a | b)); break;
	case '&': sprintf(buf, "%lx", (a & b)); break;
	case '+': sprintf(buf, "%lx", (a + b)); break;
	case '^': sprintf(buf, "%lx", (a ^ b)); break;
	case '-': sprintf(buf, "%lx", (a - b)); break;
	case '*': sprintf(buf, "%lx", (a * b)); break;
	case '/': sprintf(buf, "%lx", (a / b)); break;
	case '%': sprintf(buf, "%lx", (a % b)); break;
	default:
		printf("invalid op\n");
		return 1;
	}

	setenv(argv[1], buf);

	return 0;
}

U_BOOT_CMD(
	setexpr, 5, 0, do_setexpr,
	"set environment variable as the result of eval expression",
	"[.b, .w, .l] name value1 <op> value2\n"
	"    - set environment variable 'name' to the result of the evaluated\n"
	"      express specified by <op>.  <op> can be &, |, ^, +, -, *, /, %\n"
	"      size argument is only meaningful if value1 and/or value2 are memory addresses"
);
