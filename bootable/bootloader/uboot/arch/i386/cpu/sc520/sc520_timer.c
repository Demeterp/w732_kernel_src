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
 * Daniel Engström, Omicron Ceti AB <daniel@omicron.se>.
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

/* stuff specific for the sc520, but independent of implementation */

#include <common.h>
#include <asm/io.h>
#include <asm/interrupt.h>
#include <asm/ic/sc520.h>

void sc520_timer_isr(void)
{
	/* Ack the GP Timer Interrupt */
	writeb(0x02, &sc520_mmcr->gptmrsta);
}

int timer_init(void)
{
	/* Register the SC520 specific timer interrupt handler */
	register_timer_isr (sc520_timer_isr);

	/* Install interrupt handler for GP Timer 1 */
	irq_install_handler (0, timer_isr, NULL);

	/* Map GP Timer 1 to Master PIC IR0  */
	writeb(0x01, &sc520_mmcr->gp_tmr_int_map[1]);

	/* Disable GP Timers 1 & 2 - Allow configuration writes */
	writew(0x4000, &sc520_mmcr->gptmr1ctl);
	writew(0x4000, &sc520_mmcr->gptmr2ctl);

	/* Reset GP Timers 1 & 2 */
	writew(0x0000, &sc520_mmcr->gptmr1cnt);
	writew(0x0000, &sc520_mmcr->gptmr2cnt);

	/* Setup GP Timer 2 as a 100kHz (10us) prescaler */
	writew(83, &sc520_mmcr->gptmr2maxcmpa);
	writew(0xc001, &sc520_mmcr->gptmr2ctl);

	/* Setup GP Timer 1 as a 1000 Hz (1ms) interrupt generator */
	writew(100, &sc520_mmcr->gptmr1maxcmpa);
	writew(0xe009, &sc520_mmcr->gptmr1ctl);

	unmask_irq (0);

	/* Clear the GP Timer 1 status register to get the show rolling*/
	writeb(0x02, &sc520_mmcr->gptmrsta);

	return 0;
}

/* Allow boards to override udelay implementation */
void __udelay(unsigned long usec)
	__attribute__((weak, alias("sc520_udelay")));

void sc520_udelay(unsigned long usec)
{
	int m = 0;
	long u;
	long temp;

	temp = readw(&sc520_mmcr->swtmrmilli);
	temp = readw(&sc520_mmcr->swtmrmicro);

	do {
		m += readw(&sc520_mmcr->swtmrmilli);
		u = readw(&sc520_mmcr->swtmrmicro) + (m * 1000);
	} while (u < usec);
}
