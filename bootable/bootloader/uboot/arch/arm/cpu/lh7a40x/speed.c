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
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
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

#include <common.h>
#include <lh7a40x.h>


/* ------------------------------------------------------------------------- */
/* NOTE: This describes the proper use of this file.
 *
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_FCLK(), get_HCLK(), get_PCLK() return the clock of
 * the specified bus in HZ.
 */
/* ------------------------------------------------------------------------- */

ulong get_PLLCLK (void)
{
	return CONFIG_SYS_CLK_FREQ;
}

/* return FCLK frequency */
ulong get_FCLK (void)
{
	lh7a40x_csc_t* csc = LH7A40X_CSC_PTR;
	ulong maindiv1, maindiv2, prediv, ps;

	/*
	 * from userguide 6.1.1.2
	 *
	 * FCLK = ((MAINDIV1 +2) * (MAINDIV2 +2) * 14.7456MHz) /
	 *                   ((PREDIV+2) * (2^PS))
	 */
	maindiv2 = (csc->clkset & CLKSET_MAINDIV2) >> 11;
	maindiv1 = (csc->clkset & CLKSET_MAINDIV1) >> 7;
	prediv = (csc->clkset & CLKSET_PREDIV) >> 2;
	ps = (csc->clkset & CLKSET_PS) >> 16;

	return (((maindiv2 + 2) * (maindiv1 + 2) * CONFIG_SYS_CLK_FREQ) /
		((prediv + 2) * (1 << ps)));
}


/* return HCLK frequency */
ulong get_HCLK (void)
{
	lh7a40x_csc_t* csc = LH7A40X_CSC_PTR;

	return (get_FCLK () / ((csc->clkset & CLKSET_HCLKDIV) + 1));
}

/* return PCLK frequency */
ulong get_PCLK (void)
{
	lh7a40x_csc_t* csc = LH7A40X_CSC_PTR;

	return (get_HCLK () /
		(1 << (((csc->clkset & CLKSET_PCLKDIV) >> 16) + 1)));
}
