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
 * (C) Copyright 2009 Alessandro Rubini
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
#include <asm/arch/mtu.h>

/*
 * The timer is a decrementer, we'll left it free running at 2.4MHz.
 * We have 2.4 ticks per microsecond and an overflow in almost 30min
 */
#define TIMER_CLOCK		(24 * 100 * 1000)
#define COUNT_TO_USEC(x)	((x) * 5 / 12)	/* overflows at 6min */
#define USEC_TO_COUNT(x)	((x) * 12 / 5)	/* overflows at 6min */
#define TICKS_PER_HZ		(TIMER_CLOCK / CONFIG_SYS_HZ)
#define TICKS_TO_HZ(x)		((x) / TICKS_PER_HZ)

/* macro to read the decrementing 32 bit timer as an increasing count */
#define READ_TIMER() (0 - readl(CONFIG_SYS_TIMERBASE + MTU_VAL(0)))

/* Configure a free-running, auto-wrap counter with no prescaler */
int timer_init(void)
{
	writel(MTU_CRn_ENA | MTU_CRn_PRESCALE_1 | MTU_CRn_32BITS,
	       CONFIG_SYS_TIMERBASE + MTU_CR(0));
	reset_timer();
	return 0;
}

/* Restart counting from 0 */
void reset_timer(void)
{
	ulong val;
	writel(0, CONFIG_SYS_TIMERBASE + MTU_LR(0));
	/*
	 * The load-register isn't really immediate: it changes on clock
	 * edges, so we must wait for our newly-written value to appear.
	 * Since we might miss reading 0, wait for any change in value.
	 */
	val = READ_TIMER();
	while (READ_TIMER() == val)
		;
}

/* Return how many HZ passed since "base" */
ulong get_timer(ulong base)
{
	return  TICKS_TO_HZ(READ_TIMER()) - base;
}

/* Delay x useconds */
void __udelay(unsigned long usec)
{
	ulong ini, end;

	ini = READ_TIMER();
	end = ini + USEC_TO_COUNT(usec);
	while ((signed)(end - READ_TIMER()) > 0)
		;
}
