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
 *    Copyright (c) 2008 Nuovation System Designs, LLC
 *	Grant Erickson <gerickson@nuovations.com>
 *
 *    Copyright (c) 2007-2009 DENX Software Engineering, GmbH
 *	Stefan Roese <sr@denx.de>
 *
 *    See file CREDITS for list of people who contributed to this
 *    project.
 *
 *    This program is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU General Public License as
 *    published by the Free Software Foundation; either version 2 of
 *    the License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will abe useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA 02111-1307 USA
 *
 *    Description:
 *	This file implements ECC initialization for PowerPC processors
 *	using the IBM SDRAM DDR1 & DDR2 controller.
 *
 */

#ifndef _ECC_H_
#define _ECC_H_

/*
 * Since the IBM DDR controller used on 440GP/GX/EP/GR is not register
 * compatible to the IBM DDR/2 controller used on 405EX/440SP/SPe/460EX/GT
 * we need to make some processor dependant defines used later on by the
 * driver.
 */

/* For 440GP/GX/EP/GR */
#if defined(CONFIG_SDRAM_PPC4xx_IBM_DDR)
#define SDRAM_MCOPT1		SDRAM_CFG0
#define SDRAM_MCOPT1_MCHK_MASK	SDRAM_CFG0_MCHK_MASK
#define SDRAM_MCOPT1_MCHK_NON	SDRAM_CFG0_MCHK_NON
#define SDRAM_MCOPT1_MCHK_GEN	SDRAM_CFG0_MCHK_GEN
#define SDRAM_MCOPT1_MCHK_CHK	SDRAM_CFG0_MCHK_CHK
#define SDRAM_MCOPT1_MCHK_CHK_REP SDRAM_CFG0_MCHK_CHK
#define SDRAM_MCOPT1_DMWD_MASK	SDRAM_CFG0_DMWD_MASK
#define SDRAM_MCOPT1_DMWD_32	SDRAM_CFG0_DMWD_32

#define SDRAM_MCSTAT		SDRAM0_MCSTS
#define SDRAM_MCSTAT_IDLE_MASK	SDRAM_MCSTS_CIS
#define SDRAM_MCSTAT_IDLE_NOT	SDRAM_MCSTS_IDLE_NOT

#define SDRAM_ECCES		SDRAM0_ECCESR
#endif

void ecc_init(unsigned long * const start, unsigned long size);
void do_program_ecc(unsigned long tlb_word2_i_value);

static void inline blank_string(int size)
{
	int i;

	for (i = 0; i < size; i++)
		putc('\b');
	for (i = 0; i < size; i++)
		putc(' ');
	for (i = 0; i < size; i++)
		putc('\b');
}

#endif /* _ECC_H_ */
