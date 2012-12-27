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
 * CF IDE addon card code
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Copyright (c) 2005-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <config.h>
#include <asm/blackfin.h>
#include "bf533-stamp.h"

void cf_outb(unsigned char val, volatile unsigned char *addr)
{
	/* "ETHERNET" means the expansion memory banks */
	swap_to(ETHERNET);

	*addr = val;
	SSYNC();

	swap_to(FLASH);
}

unsigned char cf_inb(volatile unsigned char *addr)
{
	unsigned char c;

	swap_to(ETHERNET);

	c = *addr;
	SSYNC();

	swap_to(FLASH);

	return c;
}

void cf_insw(unsigned short *sect_buf, unsigned short *addr, int words)
{
	int i;

	swap_to(ETHERNET);

	for (i = 0; i < words; i++) {
		*(sect_buf + i) = *addr;
		SSYNC();
	}

	swap_to(FLASH);
}

void cf_outsw(unsigned short *addr, unsigned short *sect_buf, int words)
{
	int i;

	swap_to(ETHERNET);

	for (i = 0; i < words; i++) {
		*addr = *(sect_buf + i);
		SSYNC();
	}

	swap_to(FLASH);
}

void cf_ide_init(void)
{
	int i, cf_stat;

	/* Check whether CF card is inserted */
	bfin_write_FIO_EDGE(FIO_EDGE_CF_BITS);
	bfin_write_FIO_POLAR(FIO_POLAR_CF_BITS);
	for (i = 0; i < 0x300; i++)
		asm volatile("nop;");

	cf_stat = bfin_read_FIO_FLAG_S() & CF_STAT_BITS;

	bfin_write_FIO_EDGE(FIO_EDGE_BITS);
	bfin_write_FIO_POLAR(FIO_POLAR_BITS);

	if (!cf_stat) {
		for (i = 0; i < 0x3000; i++)
			asm volatile("nop;");

		ide_init();
	}
}
