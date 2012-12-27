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
 * File: memset.S
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
 * C Library function MEMSET
 * R0 = address (leave unchanged to form result)
 * R1 = filler byte
 * R2 = count
 * Favours word aligned data.
 */

.globl _memset;
.type _memset, STT_FUNC;
_memset:
	P0 = R0 ;              /* P0 = address */
	P2 = R2 ;              /* P2 = count   */
	R3 = R0 + R2;          /* end          */
	CC = R2 <= 7(IU);
	IF CC JUMP  .Ltoo_small;
	R1 = R1.B (Z);         /* R1 = fill char */
	R2 =  3;
	R2 = R0 & R2;          /* addr bottom two bits */
	CC =  R2 == 0;             /* AZ set if zero.	*/
	IF !CC JUMP  .Lforce_align ;  /* Jump if addr not aligned. */

.Laligned:
	P1 = P2 >> 2;          /* count = n/4        */
	R2 = R1 <<  8;         /* create quad filler */
	R2.L = R2.L + R1.L(NS);
	R2.H = R2.L + R1.H(NS);
	P2 = R3;

	LSETUP (.Lquad_loop , .Lquad_loop) LC0=P1;
.Lquad_loop:
	[P0++] = R2;

	CC = P0 == P2;
	IF !CC JUMP .Lbytes_left;
	RTS;

.Lbytes_left:
	R2 = R3;                /* end point */
	R3 = P0;                /* current position */
	R2 = R2 - R3;           /* bytes left */
	P2 = R2;

.Ltoo_small:
	CC = P2 == 0;           /* Check zero count */
	IF CC JUMP .Lfinished;    /* Unusual */

.Lbytes:
	LSETUP (.Lbyte_loop , .Lbyte_loop) LC0=P2;
.Lbyte_loop:
	B[P0++] = R1;

.Lfinished:
	RTS;

.Lforce_align:
	CC = BITTST (R0, 0);  /* odd byte */
	R0 = 4;
	R0 = R0 - R2;
	P1 = R0;
	R0 = P0;		    /* Recover return address */
	IF !CC JUMP .Lskip1;
	B[P0++] = R1;
.Lskip1:
	CC = R2 <= 2;          /* 2 bytes */
	P2 -= P1;              /* reduce count */
	IF !CC JUMP .Laligned;
	B[P0++] = R1;
	B[P0++] = R1;
	JUMP .Laligned;

.size _memset, .-_memset
