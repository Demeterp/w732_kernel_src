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
#include <config.h>
#include <mpc8xx.h>

/* ------------------------------------------------------------------------- */

#define	_NOT_USED_	0xFFFFFFFF

const uint edo_60ns[] =
{ 0x8ffbec24, 0x0ff3ec04, 0x0cf3ec04, 0x00f3ec04,
  0x00f3ec00, 0x37f7ec47, _NOT_USED_, _NOT_USED_,
  0x8fffec24, 0x0ffbec04, 0x08f3ec04, 0x07f3ec08,
  0x08f3ec04, 0x07f3ec48, 0x08f3ec04, 0x07f3ec48,
  0x08f3ec04, 0x07f3ec48, 0x1ff7ec47, _NOT_USED_,
  _NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,
  0x8fffcc24, 0x0fefcc04, 0x0cafcc00, 0x11bfcc47,
  _NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,
  0x8fffcc24, 0x0fefcc04, 0x0cafcc00, 0x03afcc4c,
  0x0cafcc00, 0x03afcc4c, 0x0cafcc00, 0x03afcc4c,
  0x0cafcc00, 0x33bfcc4f, _NOT_USED_, _NOT_USED_,
  _NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,
  0xc0ffcc84, 0x00ffcc04, 0x07ffcc04, 0x3fffcc06,
  0xffffcc85, 0xffffcc05, _NOT_USED_, _NOT_USED_,
  _NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,
  0x33ffcc07, _NOT_USED_, _NOT_USED_, _NOT_USED_ };

/* ------------------------------------------------------------------------- */

/*
 * Check Board Identity:
 */

int checkboard (void)
{
	puts ("Board: AMX860\n");
	return 0;
}

/* ------------------------------------------------------------------------- */

phys_size_t initdram (int board_type)
{

	volatile immap_t     *immap = (immap_t *)CONFIG_SYS_IMMR;
	volatile memctl8xx_t *memctl = &immap->im_memctl;

	/* AMX860: has 4 Mb of 60ns EDO DRAM, so start DRAM at 0 */

	upmconfig(UPMA, (uint *) edo_60ns, sizeof(edo_60ns)/sizeof(uint));

#ifndef CONFIG_AMX_RAM_EXT
	memctl->memc_mptpr = 0x0400; /* divide by 16 */
#else
	memctl->memc_mptpr = 0x0200;
#endif

	memctl->memc_mamr = 0x30a21114;
	memctl->memc_or2 = 0xffc00800;
#ifndef CONFIG_AMX_RAM_EXT
	memctl->memc_br2 = 0x81;

	return (4 << 20);
#else
	memctl->memc_or1 = 0xff000800;
	memctl->memc_br1 = 0x00000081;
	memctl->memc_br2 = 0x01000081;

	return (20 << 20);
#endif
}
