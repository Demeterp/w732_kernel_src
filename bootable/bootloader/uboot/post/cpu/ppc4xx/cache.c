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
 * (C) Copyright 2007
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Author: Igor Lisitsin <igor@emcraft.com>
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

/* Cache test
 *
 * This test verifies the CPU data and instruction cache using
 * several test scenarios.
 */

#include <post.h>

#if CONFIG_POST & CONFIG_SYS_POST_CACHE

#include <asm/mmu.h>
#include <watchdog.h>

#define CACHE_POST_SIZE	1024

int cache_post_test1 (int tlb, void *p, int size);
int cache_post_test2 (int tlb, void *p, int size);
int cache_post_test3 (int tlb, void *p, int size);
int cache_post_test4 (int tlb, void *p, int size);
int cache_post_test5 (int tlb, void *p, int size);
int cache_post_test6 (int tlb, void *p, int size);

#ifdef CONFIG_440
static unsigned char testarea[CACHE_POST_SIZE]
__attribute__((__aligned__(CACHE_POST_SIZE)));
#endif

int cache_post_test (int flags)
{
	void *virt = (void *)CONFIG_SYS_POST_CACHE_ADDR;
	int ints;
	int res = 0;
	int tlb = -1;		/* index to the victim TLB entry */

	/*
	 * All 44x variants deal with cache management differently
	 * because they have the address translation always enabled.
	 * The 40x ppc's don't use address translation in U-Boot at all,
	 * so we have to distinguish here between 40x and 44x.
	 */
#ifdef CONFIG_440
	int word0, i;

	/*
	 * Allocate a new TLB entry, since we are going to modify
	 * the write-through and caching inhibited storage attributes.
	 */
	program_tlb((u32)testarea, (u32)virt, CACHE_POST_SIZE,
		    TLB_WORD2_I_ENABLE);

	/* Find the TLB entry */
	for (i = 0;; i++) {
		if (i >= PPC4XX_TLB_SIZE) {
			printf ("Failed to program tlb entry\n");
			return -1;
		}
		word0 = mftlb1(i);
		if (TLB_WORD0_EPN_DECODE(word0) == (u32)virt) {
			tlb = i;
			break;
		}
	}
#endif
	ints = disable_interrupts ();

	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test1 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test2 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test3 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test4 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test5 (tlb, virt, CACHE_POST_SIZE);
	WATCHDOG_RESET ();
	if (res == 0)
		res = cache_post_test6 (tlb, virt, CACHE_POST_SIZE);

	if (ints)
		enable_interrupts ();

#ifdef CONFIG_440
	remove_tlb((u32)virt, CACHE_POST_SIZE);
#endif

	return res;
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_CACHE */
