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
 * File: memcmp.S
 *
 * Copyright 2004-2007 Analog Devices Inc.
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

.align 2

/*
 * C Library function MEMCMP
 * R0 = First Address
 * R1 = Second Address
 * R2 = count
 * Favours word aligned data.
 */

.globl _memcmp;
.type _memcmp, STT_FUNC;
_memcmp:
	I1 = P3;
	P0 = R0;			/* P0 = s1 address */
	P3 = R1;			/* P3 = s2 Address  */
	P2 = R2 ;			/* P2 = count */
	CC = R2 <= 7(IU);
	IF CC JUMP  .Ltoo_small;
	I0 = R1;			/* s2 */
	R1 = R1 | R0;		/* OR addresses together */
	R1 <<= 30;		/* check bottom two bits */
	CC =  AZ;			/* AZ set if zero. */
	IF !CC JUMP  .Lbytes ;	/* Jump if addrs not aligned. */

	P1 = P2 >> 2;		/* count = n/4 */
	R3 =  3;
	R2 = R2 & R3;		/* remainder */
	P2 = R2;			/* set remainder */

	LSETUP (.Lquad_loop_s , .Lquad_loop_e) LC0=P1;
.Lquad_loop_s:
	NOP;
	R0 = [P0++];
	R1 = [I0++];
	CC = R0 == R1;
	IF !CC JUMP .Lquad_different;
.Lquad_loop_e:
	NOP;

	P3 = I0;			/* s2 */
.Ltoo_small:
	CC = P2 == 0;		/* Check zero count*/
	IF CC JUMP .Lfinished;	/* very unlikely*/

.Lbytes:
	LSETUP (.Lbyte_loop_s , .Lbyte_loop_e) LC0=P2;
.Lbyte_loop_s:
	R1 = B[P3++](Z);	/* *s2 */
	R0 = B[P0++](Z);	/* *s1 */
	CC = R0 == R1;
	IF !CC JUMP .Ldifferent;
.Lbyte_loop_e:
	NOP;

.Ldifferent:
	R0 = R0 - R1;
	P3 = I1;
	RTS;

.Lquad_different:
/* We've read two quads which don't match.
 * Can't just compare them, because we're
 * a little-endian machine, so the MSBs of
 * the regs occur at later addresses in the
 * string.
 * Arrange to re-read those two quads again,
 * byte-by-byte.
 */
	P0 += -4;		/* back up to the start of the */
	P3 = I0;		/* quads, and increase the*/
	P2 += 4;		/* remainder count*/
	P3 += -4;
	JUMP .Lbytes;

.Lfinished:
	R0 = 0;
	P3 = I1;
	RTS;

.size _memcmp, .-_memcmp
