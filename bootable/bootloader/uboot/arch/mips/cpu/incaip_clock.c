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
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <asm/inca-ip.h>


/*******************************************************************************
*
* get_cpuclk - returns the frequency of the CPU.
*
* Gets the value directly from the INCA-IP hardware.
*
* RETURNS:
*          150.000.000 for 150 MHz
*          133.333.333 for 133 MHz (= 400MHz/3)
*          100.000.000 for 100 MHz (= 400MHz/4)
* NOTE:
*   This functions should be used by the hardware driver to get the correct
*   frequency of the CPU. Don't use the macros, which are set to init the CPU
*   frequency in the ROM code.
*/
uint incaip_get_cpuclk (void)
{
	/*-------------------------------------------------------------------------*/
	/* CPU Clock Input Multiplexer (MUX I)                                     */
	/* Multiplexer MUX I selects the maximum input clock to the CPU.           */
	/*-------------------------------------------------------------------------*/
	if (*((volatile ulong *) INCA_IP_CGU_CGU_MUXCR) &
	    INCA_IP_CGU_CGU_MUXCR_MUXI) {
		/* MUX I set to 150 MHz clock */
		return 150000000;
	} else {
		/* MUX I set to 100/133 MHz clock */
		if (*((volatile ulong *) INCA_IP_CGU_CGU_DIVCR) & 0x40) {
			/* Division value is 1/3, maximum CPU operating */
			/* frequency is 133.3 MHz                       */
			return 133333333;
		} else {
			/* Division value is 1/4, maximum CPU operating */
			/* frequency is 100 MHz                         */
			return 100000000;
		}
	}
}

/*******************************************************************************
*
* get_fpiclk - returns the frequency of the FPI bus.
*
* Gets the value directly from the INCA-IP hardware.
*
* RETURNS: Frquency in Hz
*
* NOTE:
*   This functions should be used by the hardware driver to get the correct
*   frequency of the CPU. Don't use the macros, which are set to init the CPU
*   frequency in the ROM code.
*   The calculation for the
*/
uint incaip_get_fpiclk (void)
{
	uint clkCPU;

	clkCPU = incaip_get_cpuclk ();

	switch (*((volatile ulong *) INCA_IP_CGU_CGU_DIVCR) & 0xC) {
	case 0x4:
		return clkCPU >> 1;	/* devided by 2 */
		break;
	case 0x8:
		return clkCPU >> 2;	/* devided by 4 */
		break;
	default:
		return clkCPU;
		break;
	}
}

int incaip_set_cpuclk (void)
{
	extern void ebu_init(long);
	extern void cgu_init(long);
	extern void sdram_init(long);
	char tmp[64];
	ulong cpuclk;

	if (getenv_r ("cpuclk", tmp, sizeof (tmp)) > 0) {
		cpuclk = simple_strtoul (tmp, NULL, 10) * 1000000;
		cgu_init (cpuclk);
		ebu_init (cpuclk);
		sdram_init (cpuclk);
	}

	return 0;
}
