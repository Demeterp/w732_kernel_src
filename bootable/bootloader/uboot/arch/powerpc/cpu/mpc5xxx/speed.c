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
 * (C) Copyright 2000-2002
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
#include <mpc5xxx.h>
#include <asm/processor.h>

DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */

/* Bus-to-Core Multipliers */

static int bus2core[] = {
	3, 2, 2, 2, 4, 4, 5, 9,
	6, 11, 8, 10, 3, 12, 7, 0,
	6, 5, 13, 2, 14, 4, 15, 9,
	0, 11, 8, 10, 16, 12, 7, 0
};
/* ------------------------------------------------------------------------- */

/*
 *
 */

int get_clocks (void)
{
	ulong val, vco;

#if !defined(CONFIG_SYS_MPC5XXX_CLKIN)
#error clock measuring not implemented yet - define CONFIG_SYS_MPC5XXX_CLKIN
#endif

	val = *(vu_long *)MPC5XXX_CDM_PORCFG;
	if (val & (1 << 6)) {
		vco = CONFIG_SYS_MPC5XXX_CLKIN * 12;
	} else {
		vco = CONFIG_SYS_MPC5XXX_CLKIN * 16;
	}
	if (val & (1 << 5)) {
		gd->bus_clk = vco / 8;
	} else {
		gd->bus_clk = vco / 4;
	}
	gd->cpu_clk = gd->bus_clk * bus2core[val & 0x1f] / 2;

	val = *(vu_long *)MPC5XXX_CDM_CFG;
	if (val & (1 << 8)) {
		gd->ipb_clk = gd->bus_clk / 2;
	} else {
		gd->ipb_clk = gd->bus_clk;
	}
	switch (val & 3) {
		case 0: gd->pci_clk = gd->ipb_clk; break;
		case 1: gd->pci_clk = gd->ipb_clk / 2; break;
		default: gd->pci_clk = gd->bus_clk / 4; break;
	}

	return (0);
}

int prt_mpc5xxx_clks (void)
{
	char buf1[32], buf2[32], buf3[32];

	printf ("       Bus %s MHz, IPB %s MHz, PCI %s MHz\n",
		strmhz(buf1, gd->bus_clk),
		strmhz(buf2, gd->ipb_clk),
		strmhz(buf3, gd->pci_clk)
	);
	return (0);
}

/* ------------------------------------------------------------------------- */
