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
 * (C) Copyright 2003
 * Tait Electronics Limited, Christchurch, New Zealand
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
 * This file provides a shell like 'test' function to return
 * true/false from an integer or string compare of two memory
 * locations or a location and a scalar/literal.
 * A few parts were lifted from bash 'test' command
 */

#include <common.h>
#include <config.h>
#include <command.h>

#define EQ	0
#define NE	1
#define LT	2
#define GT	3
#define LE	4
#define GE	5

struct op_tbl_s {
	char	*op;		/* operator string */
	int	opcode;		/* internal representation of opcode */
};

typedef struct op_tbl_s op_tbl_t;

op_tbl_t op_table [] = {
	{ "-lt", LT },
	{ "<"  , LT },
	{ "-gt", GT },
	{ ">"  , GT },
	{ "-eq", EQ },
	{ "==" , EQ },
	{ "-ne", NE },
	{ "!=" , NE },
	{ "<>" , NE },
	{ "-ge", GE },
	{ ">=" , GE },
	{ "-le", LE },
	{ "<=" , LE },
};

#define op_tbl_size (sizeof(op_table)/sizeof(op_table[0]))

static long evalexp(char *s, int w)
{
	long l = 0;
	long *p;

	/* if the parameter starts with a * then assume is a pointer to the value we want */
	if (s[0] == '*') {
		p = (long *)simple_strtoul(&s[1], NULL, 16);
		switch (w) {
		case 1: return((long)(*(unsigned char *)p));
		case 2: return((long)(*(unsigned short *)p));
		case 4: return(*p);
		}
	} else {
		l = simple_strtoul(s, NULL, 16);
	}

	return (l & ((1 << (w * 8)) - 1));
}

static char * evalstr(char *s)
{
	/* if the parameter starts with a * then assume a string pointer else its a literal */
	if (s[0] == '*') {
		return (char *)simple_strtoul(&s[1], NULL, 16);
	} else {
		return s;
	}
}

static int stringcomp(char *s, char *t, int op)
{
	int n, p;
	char *l, *r;

	l = evalstr(s);
	r = evalstr(t);

	/* we'll do a compare based on the length of the shortest string */
	n = min(strlen(l), strlen(r));

	p = strncmp(l, r, n);
	switch (op) {
	case EQ: return (p == 0);
	case NE: return (p != 0);
	case LT: return (p < 0);
	case GT: return (p > 0);
	case LE: return (p <= 0);
	case GE: return (p >= 0);
	}
	return (0);
}

static int arithcomp (char *s, char *t, int op, int w)
{
	long l, r;

	l = evalexp (s, w);
	r = evalexp (t, w);

	switch (op) {
	case EQ: return (l == r);
	case NE: return (l != r);
	case LT: return (l < r);
	case GT: return (l > r);
	case LE: return (l <= r);
	case GE: return (l >= r);
	}
	return (0);
}

int binary_test (char *op, char *arg1, char *arg2, int w)
{
	int len, i;
	op_tbl_t *optp;

	len = strlen(op);

	for (optp = (op_tbl_t *)&op_table, i = 0;
	     i < op_tbl_size;
	     optp++, i++) {

		if ((strncmp (op, optp->op, len) == 0) && (len == strlen (optp->op))) {
			if (w == 0) {
				return (stringcomp(arg1, arg2, optp->opcode));
			} else {
				return (arithcomp (arg1, arg2, optp->opcode, w));
			}
		}
	}

	printf("Unknown operator '%s'\n", op);
	return 0;	/* op code not found */
}

/* command line interface to the shell test */
int do_itest ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[] )
{
	int	value, w;

	/* Validate arguments */
	if ((argc != 4)){
		cmd_usage(cmdtp);
		return 1;
	}

	/* Check for a data width specification.
	 * Defaults to long (4) if no specification.
	 * Uses -2 as 'width' for .s (string) so as not to upset existing code
	 */
	switch (w = cmd_get_data_size(argv[0], 4)) {
	case 1:
	case 2:
	case 4:
		value = binary_test (argv[2], argv[1], argv[3], w);
		break;
	case -2:
		value = binary_test (argv[2], argv[1], argv[3], 0);
		break;
	case -1:
	default:
		puts("Invalid data width specifier\n");
		value = 0;
		break;
	}

	return !value;
}

U_BOOT_CMD(
	itest, 4, 0, do_itest,
	"return true/false on integer compare",
	"[.b, .w, .l, .s] [*]value1 <op> [*]value2"
);
