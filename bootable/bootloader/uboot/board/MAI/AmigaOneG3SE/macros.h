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


#ifndef _MACROS_H
#define _MACROS_H

	/*
	** Load a long integer into a register
	*/
	.macro liw reg, value
		lis \reg, \value@h
		ori \reg, \reg, \value@l
	.endm


	/*
	** Generate config_addr request
	** This macro expects the values in registers:
	** r3 - bus
	** r4 - devfn
	** r5 - offset
	*/
	.macro config_addr
		rlwinm	r9, r5, 24, 0, 6
		rlwinm	r8, r4, 16, 0, 31
		rlwinm	r7, r3, 8, 0, 31
		or	r9, r8, r9
		or	r9, r7, r9
		ori	r9, r9, 0x80
		liw	r10, 0xfec00cf8
		stw	r9, 0(r10)
		eieio
		sync
	.endm


	/*
	** Generate config_data address
	*/
	.macro config_data mask
		andi.	r9, r5, \mask
		addi	r9, r9, 0xcfc
		oris	r9, r9, 0xfee0
	.endm


	/*
	** Write a byte value to an output port
	*/
	.macro outb port, value
		lis     r2, 0xfe00
		li      r0, \value
		stb     r0, \port(r2)
	.endm


	/*
	** Write a register byte value to an output port
	*/
	.macro outbr port, value
		lis     r2, 0xfe00
		stb     \value, \port(r2)
	.endm


	/*
	** Read a byte value from a port into a specified register
	*/
	.macro inb reg, port
		lis     r2, 0xfe00
		lbz     \reg, \port(r2)
	.endm


	/*
	** Write a byte to the SuperIO config area
	*/
	.macro siowb offset, value
		li      r3, 0
		li      r4, (7<<3)
		li      r5, \offset
		li      r6, \value
		bl      pci_write_cfg_byte
	.endm

#endif
