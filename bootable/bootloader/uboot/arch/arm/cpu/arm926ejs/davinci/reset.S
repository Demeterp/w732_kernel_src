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
 * Processor reset using WDT for TI TMS320DM644x SoC.
 *
 * Copyright (C) 2007 Sergey Kubushyn <ksi@koi8.net>
 *
 * -----------------------------------------------------
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

.globl reset_cpu
reset_cpu:
	ldr	r0, WDT_TGCR
	mov	r1, $0x08
	str	r1, [r0]
	ldr	r1, [r0]
	orr	r1, r1, $0x03
	str	r1, [r0]
	mov	r1, $0
	ldr	r0, WDT_TIM12
	str	r1, [r0]
	ldr	r0, WDT_TIM34
	str	r1, [r0]
	ldr	r0, WDT_PRD12
	str	r1, [r0]
	ldr	r0, WDT_PRD34
	str	r1, [r0]
	ldr	r0, WDT_TCR
	ldr	r1, [r0]
	orr	r1, r1, $0x40
	str	r1, [r0]
	ldr	r0, WDT_WDTCR
	ldr	r1, [r0]
	orr	r1, r1, $0x4000
	str	r1, [r0]
	ldr	r1, WDTCR_VAL1
	str	r1, [r0]
	ldr	r1, WDTCR_VAL2
	str	r1, [r0]
	/* Write an invalid value to the WDKEY field to trigger
	 * an immediate watchdog reset */
	mov     r1, $0x4000
	str     r1, [r0]
	nop
	nop
	nop
	nop
reset_cpu_loop:
	b	reset_cpu_loop

WDT_TGCR:
	.word	0x01c21c24
WDT_TIM12:
	.word	0x01c21c10
WDT_TIM34:
	.word	0x01c21c14
WDT_PRD12:
	.word	0x01c21c18
WDT_PRD34:
	.word	0x01c21c1c
WDT_TCR:
	.word	0x01c21c20
WDT_WDTCR:
	.word	0x01c21c28
WDTCR_VAL1:
	.word	0xa5c64000
WDTCR_VAL2:
	.word	0xda7e4000
