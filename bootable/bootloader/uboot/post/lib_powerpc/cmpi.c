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

/*
 * CPU test
 * Integer compare instructions:	cmpwi, cmplwi
 *
 * To verify these instructions the test runs them with
 * different combinations of operands, reads the condition
 * register value and compares it with the expected one.
 * The test contains a pre-built table
 * containing the description of each test case: the instruction,
 * the values of the operands, the condition field to save
 * the result in and the expected result.
 */

#include <post.h>
#include "cpu_asm.h"

#if CONFIG_POST & CONFIG_SYS_POST_CPU

extern void cpu_post_exec_11 (ulong *code, ulong *res, ulong op1);

static struct cpu_post_cmpi_s
{
    ulong cmd;
    ulong op1;
    ushort op2;
    ulong cr;
    ulong res;
} cpu_post_cmpi_table[] =
{
    {
	OP_CMPWI,
	123,
	123,
	2,
	0x02
    },
    {
	OP_CMPWI,
	123,
	133,
	3,
	0x08
    },
    {
	OP_CMPWI,
	123,
	-133,
	4,
	0x04
    },
    {
	OP_CMPLWI,
	123,
	123,
	2,
	0x02
    },
    {
	OP_CMPLWI,
	123,
	-133,
	3,
	0x08
    },
    {
	OP_CMPLWI,
	123,
	113,
	4,
	0x04
    },
};
static unsigned int cpu_post_cmpi_size =
    sizeof (cpu_post_cmpi_table) / sizeof (struct cpu_post_cmpi_s);

int cpu_post_test_cmpi (void)
{
    int ret = 0;
    unsigned int i;
    int flag = disable_interrupts();

    for (i = 0; i < cpu_post_cmpi_size && ret == 0; i++)
    {
	struct cpu_post_cmpi_s *test = cpu_post_cmpi_table + i;
	unsigned long code[] =
	{
	    ASM_1IC(test->cmd, test->cr, 3, test->op2),
	    ASM_MFCR(3),
	    ASM_BLR
	};
	ulong res;

	cpu_post_exec_11 (code, & res, test->op1);

	ret = ((res >> (28 - 4 * test->cr)) & 0xe) == test->res ? 0 : -1;

	if (ret != 0)
	{
	    post_log ("Error at cmpi test %d !\n", i);
	}
    }

    if (flag)
	enable_interrupts();

    return ret;
}

#endif
