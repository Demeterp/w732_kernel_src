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
 * (C) Copyright 2001
 * Josh Huber <huber@mclx.com>, Mission Critical Linux, Inc.
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

/*
 * interrupts.c - just enough support for the decrementer/timer
 */

#include <common.h>
#include <mpc8xx.h>
#include <mpc8xx_irq.h>
#include <asm/processor.h>
#include <commproc.h>
#include <command.h>

int interrupt_init_cpu (unsigned *decrementer_count)
{
#if defined(DEBUG) && !defined(CONFIG_AMIGAONEG3SE)
	printf("interrupt_init: GT main cause reg: %08x:%08x\n",
	       GTREGREAD(LOW_INTERRUPT_CAUSE_REGISTER),
	       GTREGREAD(HIGH_INTERRUPT_CAUSE_REGISTER));
	printf("interrupt_init: ethernet cause regs: %08x %08x %08x\n",
	       GTREGREAD(ETHERNET0_INTERRUPT_CAUSE_REGISTER),
	       GTREGREAD(ETHERNET1_INTERRUPT_CAUSE_REGISTER),
	       GTREGREAD(ETHERNET2_INTERRUPT_CAUSE_REGISTER));
	printf("interrupt_init: ethernet mask regs:  %08x %08x %08x\n",
	       GTREGREAD(ETHERNET0_INTERRUPT_MASK_REGISTER),
	       GTREGREAD(ETHERNET1_INTERRUPT_MASK_REGISTER),
	       GTREGREAD(ETHERNET2_INTERRUPT_MASK_REGISTER));
	puts("interrupt_init: setting decrementer_count\n");
#endif
	*decrementer_count = get_tbclk() / CONFIG_SYS_HZ;

	return (0);
}

/****************************************************************************/

/*
 * Handle external interrupts
 */
void
external_interrupt(struct pt_regs *regs)
{
	puts("external_interrupt (oops!)\n");
}

volatile ulong timestamp = 0;

/*
 * timer_interrupt - gets called when the decrementer overflows,
 * with interrupts disabled.
 * Trivial implementation - no need to be really accurate.
 */
void
timer_interrupt_cpu (struct pt_regs *regs)
{
	/* nothing to do here */
	return;
}

/****************************************************************************/

/*
 * Install and free a interrupt handler.
 */

void
irq_install_handler(int vec, interrupt_handler_t *handler, void *arg)
{

}

void
irq_free_handler(int vec)
{

}

/****************************************************************************/

void
do_irqinfo(cmd_tbl_t *cmdtp, bd_t *bd, int flag, int argc, char *argv[])
{
	puts("IRQ related functions are unimplemented currently.\n");
}
