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
 * cmd_cplbinfo.c - dump the instruction/data cplb tables
 *
 * Copyright (c) 2007-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <asm/blackfin.h>
#include <asm/cplb.h>
#include <asm/mach-common/bits/mpu.h>

/*
 * Translate the PAGE_SIZE bits into a human string
 */
static const char *cplb_page_size(uint32_t data)
{
	static const char page_size_string_table[][4] = { "1K", "4K", "1M", "4M" };
	return page_size_string_table[(data & PAGE_SIZE_MASK) >> PAGE_SIZE_SHIFT];
}

/*
 * show a hardware cplb table
 */
static void show_cplb_table(uint32_t *addr, uint32_t *data)
{
	int i;
	printf("      Address     Data   Size  Valid  Locked\n");
	for (i = 1; i <= 16; ++i) {
		printf(" %2i 0x%p  0x%05X   %s     %c      %c\n",
			i, (void *)*addr, *data,
			cplb_page_size(*data),
			(*data & CPLB_VALID ? 'Y' : 'N'),
			(*data & CPLB_LOCK ? 'Y' : 'N'));
		++addr;
		++data;
	}
}

/*
 * display current instruction and data cplb tables
 */
int do_cplbinfo(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	printf("%s CPLB table [%08x]:\n", "Instruction", *(uint32_t *)DMEM_CONTROL);
	show_cplb_table((uint32_t *)ICPLB_ADDR0, (uint32_t *)ICPLB_DATA0);

	printf("%s CPLB table [%08x]:\n", "Data", *(uint32_t *)IMEM_CONTROL);
	show_cplb_table((uint32_t *)DCPLB_ADDR0, (uint32_t *)DCPLB_DATA0);

	return 0;
}

U_BOOT_CMD(cplbinfo, 1, 0, do_cplbinfo,
	"display current CPLB tables",
	""
);
