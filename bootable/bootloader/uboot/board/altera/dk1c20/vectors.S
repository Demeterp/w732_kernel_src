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
 * (C) Copyright 2003, Psyent Corporation <www.psyent.com>
 * Scott McNutt <smcnutt@psyent.com>
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


/*************************************************************************
 * Exception Vector Table
 *
 * This could have gone in the cpu soure tree, but the whole point of
 * Nios is customization -- and polluting the cpu source tree with
 * board-specific ifdef's really defeats the purpose, no? With this in
 * the board-specific tree, each board has the freedom to organize
 * vectors/traps, etc anyway it wants. The init code copies this table
 * to the proper location.
 *
 * Each board can do what it likes here. But there are four "standard"
 * handlers availble:
 *
 *	_cwp_lolimit	-Handles register window underflows.
 *	_cwp_hilimit	-Handles register window overflows.
 *	_timebase_int	-Increments the timebase.
 *	_brkpt_hw_int	-Hardware breakpoint handler.
 *	_brkpt_sw_int	-Software breakpoint handler.
 *	_def_xhandler	-Default exception handler.
 *
 * _timebase_int handles a Nios Timer interrupt and increments the
 * timestamp used for the get_timer(), reset_timer(), etc. routines. It
 * expects the timer to be configured like the standard-32 low priority
 * timer.
 *
 * _def_xhandler dispatches exceptions/traps via the external_interrupt()
 * routine. This lets you use the irq_install_handler() and handle your
 * interrupts/traps with code written in C.
 ************************************************************************/

	.data
	.global _vectors
	.align	4
_vectors:

	.long	_def_xhandler@h		/* Vector 0  - NMI */
	.long	_cwp_lolimit@h		/* Vector 1  -  underflow */
	.long	_cwp_hilimit@h		/* Vector 2  - overflow	*/
	.long	_brkpt_hw_int@h		/* Vector 3 - Breakpoint */
	.long	_brkpt_sw_int@h		/* Vector 4 - Single step*/
	.long	_def_xhandler@h		/* Vector 5 - GNUPro debug */
	.long	_def_xhandler@h		/* Vector 6 - future reserved */
	.long	_def_xhandler@h		/* Vector 7 - future reserved */
	.long	_def_xhandler@h		/* Vector 8 - future reserved */
	.long	_def_xhandler@h		/* Vector 9 - future reserved */
	.long	_def_xhandler@h		/* Vector 10 - future reserved */
	.long	_def_xhandler@h		/* Vector 11 - future reserved */
	.long	_def_xhandler@h		/* Vector 12 - future reserved */
	.long	_def_xhandler@h		/* Vector 13 - future reserved */
	.long	_def_xhandler@h		/* Vector 14 - future reserved */
	.long	_def_xhandler@h		/* Vector 15 - future reserved */
	.long	_def_xhandler@h		/* Vector 16 */
	.long	_def_xhandler@h		/* Vector 17 */
	.long	_def_xhandler@h		/* Vector 18 */
	.long	_def_xhandler@h		/* Vector 19 */
	.long	_def_xhandler@h		/* Vector 20 */
	.long	_def_xhandler@h		/* Vector 21 */
	.long	_def_xhandler@h		/* Vector 22 */
	.long	_def_xhandler@h		/* Vector 23 */
	.long	_def_xhandler@h		/* Vector 24 */
	.long	_def_xhandler@h		/* Vector 25 */
	.long	_def_xhandler@h		/* Vector 26 */
	.long	_def_xhandler@h		/* Vector 27 */
	.long	_def_xhandler@h		/* Vector 28 */
	.long	_def_xhandler@h		/* Vector 29 */
	.long	_def_xhandler@h		/* Vector 30 */
	.long	_def_xhandler@h		/* Vector 31 */
	.long	_def_xhandler@h		/* Vector 32 */
	.long	_def_xhandler@h		/* Vector 33 */
	.long	_def_xhandler@h		/* Vector 34 */
	.long	_def_xhandler@h		/* Vector 35 */
	.long	_def_xhandler@h		/* Vector 36 */
	.long	_def_xhandler@h		/* Vector 37 */
	.long	_def_xhandler@h		/* Vector 38 */
	.long	_def_xhandler@h		/* Vector 39 */
	.long	_def_xhandler@h		/* Vector 40 */
	.long	_def_xhandler@h		/* Vector 41 */
	.long	_def_xhandler@h		/* Vector 42 */
	.long	_def_xhandler@h		/* Vector 43 */
	.long	_def_xhandler@h		/* Vector 44 */
	.long	_def_xhandler@h		/* Vector 45 */
	.long	_def_xhandler@h		/* Vector 46 */
	.long	_def_xhandler@h		/* Vector 47 */
	.long	_def_xhandler@h		/* Vector 48 */
	.long	_def_xhandler@h		/* Vector 49 */
	.long	_timebase_int@h		/* Vector 50 - lopri timer*/
	.long	_def_xhandler@h		/* Vector 51 */
	.long	_def_xhandler@h		/* Vector 52 */
	.long	_def_xhandler@h		/* Vector 53 */
	.long	_def_xhandler@h		/* Vector 54 */
	.long	_def_xhandler@h		/* Vector 55 */
	.long	_def_xhandler@h		/* Vector 56 */
	.long	_def_xhandler@h		/* Vector 57 */
	.long	_def_xhandler@h		/* Vector 58 */
	.long	_def_xhandler@h		/* Vector 59 */
	.long	_def_xhandler@h		/* Vector 60 */
	.long	_def_xhandler@h		/* Vector 61 */
	.long	_def_xhandler@h		/* Vector 62 */
	.long	_def_xhandler@h		/* Vector 63 */
