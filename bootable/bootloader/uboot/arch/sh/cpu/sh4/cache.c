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
 * Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
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
#include <asm/processor.h>
#include <asm/io.h>

/*
 * Jump to P2 area.
 * When handling TLB or caches, we need to do it from P2 area.
 */
#define jump_to_P2()			\
  do {					\
    unsigned long __dummy;		\
    __asm__ __volatile__(		\
		"mov.l	1f, %0\n\t"	\
		"or	%1, %0\n\t"	\
		"jmp	@%0\n\t"	\
		" nop\n\t"		\
		".balign 4\n"		\
		"1:	.long 2f\n"	\
		"2:"			\
		: "=&r" (__dummy)	\
		: "r" (0x20000000));	\
  } while (0)

/*
 * Back to P1 area.
 */
#define back_to_P1()					\
  do {							\
    unsigned long __dummy;				\
    __asm__ __volatile__(				\
		"nop;nop;nop;nop;nop;nop;nop\n\t"	\
		"mov.l	1f, %0\n\t"			\
		"jmp	@%0\n\t"			\
		" nop\n\t"				\
		".balign 4\n"				\
		"1:	.long 2f\n"			\
		"2:"					\
		: "=&r" (__dummy));			\
  } while (0)

#define CACHE_VALID       1
#define CACHE_UPDATED     2

static inline void cache_wback_all(void)
{
	unsigned long addr, data, i, j;

	jump_to_P2();
	for (i = 0; i < CACHE_OC_NUM_ENTRIES; i++){
		for (j = 0; j < CACHE_OC_NUM_WAYS; j++) {
			addr = CACHE_OC_ADDRESS_ARRAY | (j << CACHE_OC_WAY_SHIFT)
				| (i << CACHE_OC_ENTRY_SHIFT);
			data = inl(addr);
			if (data & CACHE_UPDATED) {
				data &= ~CACHE_UPDATED;
				outl(data, addr);
			}
		}
	}
	back_to_P1();
}


#define CACHE_ENABLE      0
#define CACHE_DISABLE     1

int cache_control(unsigned int cmd)
{
	unsigned long ccr;

	jump_to_P2();
	ccr = inl(CCR);

	if (ccr & CCR_CACHE_ENABLE)
		cache_wback_all();

	if (cmd == CACHE_DISABLE)
		outl(CCR_CACHE_STOP, CCR);
	else
		outl(CCR_CACHE_INIT, CCR);
	back_to_P1();

	return 0;
}
