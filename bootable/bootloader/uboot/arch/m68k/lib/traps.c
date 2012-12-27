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
 * (C) Copyright 2003
 * Josef Baumgartner <josef.baumgartner@telex.de>
 *
 * (C) Copyright 2000
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
#include <watchdog.h>
#include <command.h>
#include <asm/processor.h>


extern void _exc_handler(void);
extern void _int_handler(void);

static void show_frame(struct pt_regs *fp)
{
	printf ("Vector Number: %d  Format: %02x  Fault Status: %01x\n\n", (fp->vector & 0x3fc) >> 2,
	        fp->format, (fp->vector & 0x3) | ((fp->vector & 0xc00) >> 8));
	printf ("PC: %08lx    SR: %08lx    SP: %08lx\n", fp->pc, (long) fp->sr, (long) fp);
	printf ("D0: %08lx    D1: %08lx    D2: %08lx    D3: %08lx\n",
		fp->d0, fp->d1, fp->d2, fp->d3);
	printf ("D4: %08lx    D5: %08lx    D6: %08lx    D7: %08lx\n",
		fp->d4, fp->d5, fp->d6, fp->d7);
	printf ("A0: %08lx    A1: %08lx    A2: %08lx    A3: %08lx\n",
		fp->a0, fp->a1, fp->a2, fp->a3);
	printf ("A4: %08lx    A5: %08lx    A6: %08lx\n",
		fp->a4, fp->a5, fp->a6);
}

void exc_handler(struct pt_regs *fp) {
	printf("\n\n*** Unexpected exception ***\n");
	show_frame (fp);
	printf("\n*** Please Reset Board! ***\n");
	for(;;);
}

void trap_init(ulong value) {
	unsigned long *vec = (ulong *)value;
	int i;

	for(i = 2; i < 25; i++) {
		vec[i] = (unsigned long)_exc_handler;
	}
	for(i = 25; i < 32; i++) {
		vec[i] = (unsigned long)_int_handler;
	}
	for(i = 32; i < 64; i++) {
		vec[i] = (unsigned long)_exc_handler;
	}
	for(i = 64; i < 256; i++) {
		vec[i] = (unsigned long)_int_handler;
	}

	setvbr(value);		/* set vector base register to new table */
}
