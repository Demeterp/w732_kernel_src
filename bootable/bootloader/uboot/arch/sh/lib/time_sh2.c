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
 * Copyright (C) 2007,2008 Nobobuhiro Iwamatsu <iwamatsu@nigauri.org>
 * Copyright (C) 2008 Renesas Solutions Corp.
 *
 * (C) Copyright 2003
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
#include <asm/io.h>
#include <asm/processor.h>

#define CMT_CMCSR_INIT	0x0001	/* PCLK/32 */
#define CMT_CMCSR_CALIB 0x0000
#define CMT_MAX_COUNTER (0xFFFFFFFF)
#define CMT_TIMER_RESET (0xFFFF)

static vu_long cmt0_timer;

static void cmt_timer_start(unsigned int timer)
{
	writew(readw(CMSTR) | 0x01, CMSTR);
}

static void cmt_timer_stop(unsigned int timer)
{
	writew(readw(CMSTR) & ~0x01, CMSTR);
}

int timer_init(void)
{
	cmt0_timer = 0;
	/* Divide clock by 32 */
	readw(CMCSR_0);
	writew(CMT_CMCSR_INIT, CMCSR_0);

	/* User Device 0 only */
	cmt_timer_stop(0);
	set_timer(CMT_TIMER_RESET);
	cmt_timer_start(0);

	return 0;
}

unsigned long long get_ticks(void)
{
	return cmt0_timer;
}

static vu_long cmcnt = 0;
static unsigned long get_usec (void)
{
	ulong data = readw(CMCNT_0);

	if (data >= cmcnt)
		cmcnt = data - cmcnt;
	else
		cmcnt = (CMT_TIMER_RESET - cmcnt) + data;

	if ((cmt0_timer + cmcnt) > CMT_MAX_COUNTER)
		cmt0_timer = ((cmt0_timer + cmcnt) - CMT_MAX_COUNTER);
	else
		cmt0_timer += cmcnt;

	cmcnt = data;
	return cmt0_timer;
}

/* return msec */
ulong get_timer(ulong base)
{
	return (get_usec() / 1000) - base;
}

void set_timer(ulong t)
{
	writew((u16) t, CMCOR_0);
}

void reset_timer(void)
{
	cmt_timer_stop(0);
	set_timer(CMT_TIMER_RESET);
	cmt0_timer = 0;
	cmt_timer_start(0);
}

void __udelay(unsigned long usec)
{
	unsigned long end = get_usec() + usec;

	while (get_usec() < end)
		continue;
}

unsigned long get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
