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
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004-2007 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
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
#include <watchdog.h>
#include <asm/processor.h>
#include <asm/immap.h>

#ifdef	CONFIG_M5272
int interrupt_init(void)
{
	volatile intctrl_t *intp = (intctrl_t *) (MMAP_INTC);

	/* disable all external interrupts */
	intp->int_icr1 = 0x88888888;
	intp->int_icr2 = 0x88888888;
	intp->int_icr3 = 0x88888888;
	intp->int_icr4 = 0x88888888;
	intp->int_pitr = 0x00000000;
	/* initialize vector register */
	intp->int_pivr = 0x40;

	enable_interrupts();

	return 0;
}

#if defined(CONFIG_MCFTMR)
void dtimer_intr_setup(void)
{
	volatile intctrl_t *intp = (intctrl_t *) (CONFIG_SYS_INTR_BASE);

	intp->int_icr1 &= ~INT_ICR1_TMR3MASK;
	intp->int_icr1 |= CONFIG_SYS_TMRINTR_PRI;
}
#endif				/* CONFIG_MCFTMR */
#endif				/* CONFIG_M5272 */

#if defined(CONFIG_M5208) || defined(CONFIG_M5282) || \
    defined(CONFIG_M5271) || defined(CONFIG_M5275)
int interrupt_init(void)
{
	volatile int0_t *intp = (int0_t *) (CONFIG_SYS_INTR_BASE);

	/* Make sure all interrupts are disabled */
#if defined(CONFIG_M5208)
	intp->imrl0 = 0xFFFFFFFF;
	intp->imrh0 = 0xFFFFFFFF;
#else
	intp->imrl0 |= 0x1;
#endif

	enable_interrupts();
	return 0;
}

#if defined(CONFIG_MCFTMR)
void dtimer_intr_setup(void)
{
	volatile int0_t *intp = (int0_t *) (CONFIG_SYS_INTR_BASE);

	intp->icr0[CONFIG_SYS_TMRINTR_NO] = CONFIG_SYS_TMRINTR_PRI;
	intp->imrl0 &= 0xFFFFFFFE;
	intp->imrl0 &= ~CONFIG_SYS_TMRINTR_MASK;
}
#endif				/* CONFIG_MCFTMR */
#endif				/* CONFIG_M5282 | CONFIG_M5271 | CONFIG_M5275 */

#if defined(CONFIG_M5249) || defined(CONFIG_M5253)
int interrupt_init(void)
{
	enable_interrupts();

	return 0;
}

#if defined(CONFIG_MCFTMR)
void dtimer_intr_setup(void)
{
	mbar_writeLong(MCFSIM_IMR, mbar_readLong(MCFSIM_IMR) & ~0x00000400);
	mbar_writeByte(MCFSIM_TIMER2ICR, CONFIG_SYS_TMRINTR_PRI);
}
#endif				/* CONFIG_MCFTMR */
#endif				/* CONFIG_M5249 || CONFIG_M5253 */
