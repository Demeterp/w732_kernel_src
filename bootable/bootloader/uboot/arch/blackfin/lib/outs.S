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
 * Implementation of outs{bwl} for BlackFin processors using zero overhead loops.
 *
 * Copyright 2005-2009 Analog Devices Inc.
 *                2005 BuyWays BV
 *                      Bas Vermeulen <bas@buyways.nl>
 *
 * Licensed under the GPL-2.
 */

#include <asm/linkage.h>

.align 2

ENTRY(_outsl)
	P0 = R0;	/* P0 = port */
	P1 = R1;	/* P1 = address */
	P2 = R2;	/* P2 = count */

	LSETUP( .Llong_loop_s, .Llong_loop_e) LC0 = P2;
.Llong_loop_s: R0 = [P1++];
.Llong_loop_e: [P0] = R0;
	RTS;
ENDPROC(_outsl)

ENTRY(_outsw)
	P0 = R0;	/* P0 = port */
	P1 = R1;	/* P1 = address */
	P2 = R2;	/* P2 = count */

	LSETUP( .Lword_loop_s, .Lword_loop_e) LC0 = P2;
.Lword_loop_s: R0 = W[P1++];
.Lword_loop_e: W[P0] = R0;
	RTS;
ENDPROC(_outsw)

ENTRY(_outsb)
	P0 = R0;	/* P0 = port */
	P1 = R1;	/* P1 = address */
	P2 = R2;	/* P2 = count */

	LSETUP( .Lbyte_loop_s, .Lbyte_loop_e) LC0 = P2;
.Lbyte_loop_s: R0 = B[P1++];
.Lbyte_loop_e: B[P0] = R0;
	RTS;
ENDPROC(_outsb)

ENTRY(_outsw_8)
	P0 = R0;	/* P0 = port */
	P1 = R1;	/* P1 = address */
	P2 = R2;	/* P2 = count */

	LSETUP( .Lword8_loop_s, .Lword8_loop_e) LC0 = P2;
.Lword8_loop_s: R1 = B[P1++];
		R0 = B[P1++];
		R0 = R0 << 8;
		R0 = R0 + R1;
.Lword8_loop_e: W[P0] = R0;
	RTS;
ENDPROC(_outsw_8)
