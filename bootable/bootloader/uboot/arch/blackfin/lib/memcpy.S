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
 * File: memcpy.S
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

.globl _memcpy_ASM;
.type _memcpy_ASM, STT_FUNC;
_memcpy_ASM:
	CC = R2 <=  0;	/* length not positive?*/
	IF CC JUMP  .L_P1L2147483647;	/* Nothing to do */

	P0 = R0 ;	/* dst*/
	P1 = R1 ;	/* src*/
	P2 = R2 ;	/* length */

	/* check for overlapping data */
	CC = R1 < R0;	/* src < dst */
	IF !CC JUMP .Lno_overlap;
	R3 = R1 + R2;
	CC = R0 < R3;	/* and dst < src+len */
	IF CC JUMP .Lhas_overlap;

.Lno_overlap:
	/* Check for aligned data.*/

	R3 = R1 | R0;
	R0 = 0x3;
	R3 = R3 & R0;
	CC = R3;	/* low bits set on either address? */
	IF CC JUMP .Lnot_aligned;

	/* Both addresses are word-aligned, so we can copy
	at least part of the data using word copies.*/
	P2 = P2 >> 2;
	CC = P2 <= 2;
	IF !CC JUMP .Lmore_than_seven;
	/* less than eight bytes... */
	P2 = R2;
	LSETUP(.Lthree_start, .Lthree_end) LC0=P2;
	R0 = R1;	/* setup src address for return */
.Lthree_start:
	R3 = B[P1++] (X);
.Lthree_end:
	B[P0++] = R3;

	RTS;

.Lmore_than_seven:
	/* There's at least eight bytes to copy. */
	P2 += -1;	/* because we unroll one iteration */
	LSETUP(.Lword_loop, .Lword_loop) LC0=P2;
	R0 = R1;
	I1 = P1;
	R3 = [I1++];
.Lword_loop:
	MNOP || [P0++] = R3 || R3 = [I1++];

	[P0++] = R3;
	/* Any remaining bytes to copy? */
	R3 = 0x3;
	R3 = R2 & R3;
	CC = R3 == 0;
	P1 = I1;	/* in case there's something left, */
	IF !CC JUMP .Lbytes_left;
	RTS;
.Lbytes_left:	P2 = R3;
.Lnot_aligned:
	/* From here, we're copying byte-by-byte. */
	LSETUP (.Lbyte_start , .Lbyte_end) LC0=P2;
	R0 = R1;	/* Save src address for return */
.Lbyte_start:
	R1 = B[P1++] (X);
.Lbyte_end:
	B[P0++] = R1;

.L_P1L2147483647:
	RTS;

.Lhas_overlap:
/* Need to reverse the copying, because the
 * dst would clobber the src.
 * Don't bother to work out alignment for
 * the reverse case.
 */
	R0 = R1;	/* save src for later. */
	P0 = P0 + P2;
	P0 += -1;
	P1 = P1 + P2;
	P1 += -1;
	LSETUP(.Lover_start, .Lover_end) LC0=P2;
.Lover_start:
	R1 = B[P1--] (X);
.Lover_end:
	B[P0--] = R1;

	RTS;

.size _memcpy_ASM, .-_memcpy_ASM
