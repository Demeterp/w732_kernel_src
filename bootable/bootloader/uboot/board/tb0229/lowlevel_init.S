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
 *  Memory sub-system initialization code for TANBAC Evaluation board TB0229.
 *
 *  Copyright (c) 2003  Masami Komiya <mkomiya@sonare.it>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include <config.h>
#include <asm/regdef.h>


	.globl  lowlevel_init
lowlevel_init:

	/* BCUCNTREG1 = 0x0040 */
	la	t0, 0xaf000000
	li	t1, 0x0040
	sh	t1, 0(t0)

	/* ROMSIZEREG = 0x3333 */
	la	t0, 0xaf000004
	li	t1, 0x3333
	sh	t1, 0(t0)

	/* ROMSPEEDREG = 0x3003 */
	la	t0, 0xaf000006
	li	t1, 0x3003
	sh	t1, 0(t0)

	/* BCUCNTREG3 = 0 */
	la	t0, 0xaf000016
	li	t1, 0x0000
	sh	t1, 0(t0)

	/* CMUCLKMSK */
	la	t0, 0xaf000060
	li	t1, 0x39a2
	sh	t1, 0(t0)

	/* PMUCNTREG */
	la	t0, 0xaf0000c2
	li	t1, 0x0006
	sh	t1, 0(t0)

	/* SDRAMMODEREG = 0x8029 */
	la	t0, 0xaf000400
	li	t1, 0x8029
	sh	t1, 0(t0)

	/* SDRAMCNTREG = 0x2322 */
	la	t0, 0xaf000402
	li	t1, 0x2322
	sh	t1, 0(t0)

	/* BCURFCNTREG = 0x0106 */
	la	t0, 0xaf000404
	li	t1, 0x0106
	sh	t1, 0(t0)

	/* RAMSZEREG = 0x5555 (64MB Bank) */
	la	t0, 0xaf000408
	li	t1, 0x5555
	sh	t1, 0(t0)

	j       ra
	nop
