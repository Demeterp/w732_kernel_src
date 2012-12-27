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
 * Load/store string instructions:	lswi, stswi, lswx, stswx
 *
 * Several consecutive bytes from a source memory buffer are loaded
 * left to right into GPRs. After that, the bytes are stored
 * from the GPRs into a target memory buffer. The contents
 * of the source and target buffers are then compared.
 */

#include <post.h>
#include "cpu_asm.h"

#if CONFIG_POST & CONFIG_SYS_POST_CPU

extern void cpu_post_exec_02 (ulong *code, ulong op1, ulong op2);
extern void cpu_post_exec_04 (ulong *code, ulong op1, ulong op2, ulong op3,
    ulong op4);

#include <bedbug/regs.h>
int cpu_post_test_string (void)
{
    int ret = 0;
    unsigned int i;
    int flag = disable_interrupts();

    if (ret == 0)
    {
	char src [31], dst [31];

	ulong code[] =
	{
	    ASM_LSWI(5, 3, 31),
	    ASM_STSWI(5, 4, 31),
	    ASM_BLR,
	};

	for (i = 0; i < sizeof(src); i ++)
	{
	    src[i] = (char) i;
	    dst[i] = 0;
	}

	cpu_post_exec_02(code, (ulong)src, (ulong)dst);

	ret = memcmp(src, dst, sizeof(dst)) == 0 ? 0 : -1;
    }

    if (ret == 0)
    {
	char src [95], dst [95];

	ulong code[] =
	{
	    ASM_LSWX(8, 3, 5),
	    ASM_STSWX(8, 4, 5),
	    ASM_BLR,
	};

	for (i = 0; i < sizeof(src); i ++)
	{
	    src[i] = (char) i;
	    dst[i] = 0;
	}

	cpu_post_exec_04(code, (ulong)src, (ulong)dst, 0, sizeof(src));

	ret = memcmp(src, dst, sizeof(dst)) == 0 ? 0 : -1;
    }

    if (ret != 0)
    {
	post_log ("Error at string test !\n");
    }

    if (flag)
	enable_interrupts();

    return ret;
}

#endif
