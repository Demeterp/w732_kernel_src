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
 * Copyright 2009 Freescale Semiconductor, Inc.
 *
 * Kumar Gala <kumar.gala@freescale.com>
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

/* This file is intended to be included by other asm code since
 * we will want to execute this on both the primary core when
 * it does a bootm and the secondary core's that get released
 * out of the spin table */

#define SET_IVOR(vector_number, vector_offset)	\
	li	r3,vector_offset@l; 		\
	mtspr	SPRN_IVOR##vector_number,r3;

#define SET_GIVOR(vector_number, vector_offset)	\
	li	r3,vector_offset@l; 		\
	mtspr	SPRN_GIVOR##vector_number,r3;

	SET_IVOR(0, 0x020) /* Critical Input */
	SET_IVOR(1, 0x000) /* Machine Check */
	SET_IVOR(2, 0x060) /* Data Storage */
	SET_IVOR(3, 0x080) /* Instruction Storage */
	SET_IVOR(4, 0x0a0) /* External Input */
	SET_IVOR(5, 0x0c0) /* Alignment */
	SET_IVOR(6, 0x0e0) /* Program */
	SET_IVOR(7, 0x100) /* FP Unavailable */
	SET_IVOR(8, 0x120) /* System Call */
	SET_IVOR(9, 0x140) /* Auxiliary Processor Unavailable */
	SET_IVOR(10, 0x160) /* Decrementer */
	SET_IVOR(11, 0x180) /* Fixed Interval Timer */
	SET_IVOR(12, 0x1a0) /* Watchdog Timer */
	SET_IVOR(13, 0x1c0) /* Data TLB Error */
	SET_IVOR(14, 0x1e0) /* Instruction TLB Error */
	SET_IVOR(15, 0x040) /* Debug */

/* e500v1 & e500v2 only */
#ifndef CONFIG_E500MC
	SET_IVOR(32, 0x200) /* SPE Unavailable */
	SET_IVOR(33, 0x220) /* Embedded FP Data */
	SET_IVOR(34, 0x240) /* Embedded FP Round */
#endif

	SET_IVOR(35, 0x260) /* Performance monitor */

/* e500mc only */
#ifdef CONFIG_E500MC
	SET_IVOR(36, 0x280) /* Processor doorbell */
	SET_IVOR(37, 0x2a0) /* Processor doorbell critical */
	SET_IVOR(38, 0x2c0) /* Guest Processor doorbell */
	SET_IVOR(39, 0x2e0) /* Guest Processor critical & machine check */
	SET_IVOR(40, 0x300) /* Hypervisor system call */
	SET_IVOR(41, 0x320) /* Hypervisor Priviledge */

	SET_GIVOR(2, 0x060) /* Guest Data Storage */
	SET_GIVOR(3, 0x080) /* Guest Instruction Storage */
	SET_GIVOR(4, 0x0a0) /* Guest External Input */
	SET_GIVOR(8, 0x120) /* Guest System Call */
	SET_GIVOR(13, 0x1c0) /* Guest Data TLB Error */
	SET_GIVOR(14, 0x1e0) /* Guest Instruction TLB Error */
#endif
