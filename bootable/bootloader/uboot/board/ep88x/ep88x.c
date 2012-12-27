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
 * Copyright (C) 2005 Arabella Software Ltd.
 * Yuli Barcohen <yuli@arabellasw.com>
 *
 * Support for Embedded Planet EP88x boards.
 * Tested on EP88xC with MPC885 CPU, 64MB SDRAM and 16MB flash.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <mpc8xx.h>

/*
 * SDRAM uses two Micron chips.
 * Minimal CPU frequency is 40MHz.
 */
static uint sdram_table[] = {
	/* Single read	(offset 0x00 in UPM RAM) */
	0xEFCBCC04, 0x0F37C804, 0x0EEEC004, 0x01B98404,
	0x1FF74C00, 0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05,

	/* Burst read	(offset 0x08 in UPM RAM) */
	0xEFCBCC04, 0x0F37C804, 0x0EEEC004, 0x00BDC404,
	0x00FFCC00, 0x00FFCC00, 0x01FB8C00, 0x1FF74C00,
	0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05,
	0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05,

	/* Single write (offset 0x18 in UPM RAM) */
	0xEFCBCC04, 0x0F37C804, 0x0EEE8002, 0x01B90404,
	0x1FF74C05, 0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05,

	/* Burst write	(offset 0x20 in UPM RAM) */
	0xEFCBCC04, 0x0F37C804, 0x0EEE8000, 0x00BD4400,
	0x00FFCC00, 0x00FFCC02, 0x01FB8C04, 0x1FF74C05,
	0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05,
	0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05, 0xFFFFCC05,

	/* Refresh	(offset 0x30 in UPM RAM) */
	0xEFFACC04, 0x0FF5CC04, 0x0FFFCC04, 0x1FFFCC04,
	0xFFFFCC05, 0xFFFFCC05, 0xEFFB8C34, 0x0FF74C34,
	0x0FFACCB4, 0x0FF5CC34, 0x0FFFC034, 0x0FFFC0B4,

	/* Exception	(offset 0x3C in UPM RAM) */
	0x0FEA8034, 0x1FB54034,	0xFFFFCC34, 0xFFFFCC05
};

int board_early_init_f (void)
{
	vu_char *bcsr = (vu_char *)CONFIG_SYS_BCSR;

	bcsr[0] |= 0x0C; /* Turn the LEDs off */
	bcsr[2] |= 0x08; /* Enable flash WE# line - necessary for
			    flash detection by CFI driver
			 */

#if defined(CONFIG_8xx_CONS_SMC1)
	bcsr[6] |= 0x10; /* Enables RS-232 transceiver */
#endif
#if defined(CONFIG_8xx_CONS_SCC2)
	bcsr[7] |= 0x10; /* Enables RS-232 transceiver */
#endif
#ifdef CONFIG_ETHER_ON_FEC1
	bcsr[8] |= 0xC0; /* Enable Ethernet 1 PHY */
#endif
#ifdef CONFIG_ETHER_ON_FEC2
	bcsr[8] |= 0x30; /* Enable Ethernet 2 PHY */
#endif

	return 0;
}

phys_size_t initdram (int board_type)
{
	long int msize;
	volatile immap_t     *immap  = (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile memctl8xx_t *memctl = &immap->im_memctl;

	upmconfig(UPMA, sdram_table, sizeof(sdram_table) / sizeof(uint));

	/* Configure SDRAM refresh */
	memctl->memc_mptpr = MPTPR_PTP_DIV2; /* BRGCLK/2 */

	memctl->memc_mamr = (65 << 24) | CONFIG_SYS_MAMR; /* No refresh */
	udelay(100);

	/* Run MRS pattern from location 0x36 */
	memctl->memc_mar = 0x88;
	memctl->memc_mcr = 0x80002236;
	udelay(100);

	memctl->memc_mamr |=  MAMR_PTAE; /* Enable refresh */
	memctl->memc_or1   = ~(CONFIG_SYS_SDRAM_MAX_SIZE - 1) | OR_CSNT_SAM;
	memctl->memc_br1   =  CONFIG_SYS_SDRAM_BASE | BR_PS_32 | BR_MS_UPMA | BR_V;

	msize = get_ram_size(CONFIG_SYS_SDRAM_BASE, CONFIG_SYS_SDRAM_MAX_SIZE);
	memctl->memc_or1  |= ~(msize - 1);

	return msize;
}

int checkboard( void )
{
	vu_char *bcsr = (vu_char *)CONFIG_SYS_BCSR;

	puts("Board: ");
	switch (bcsr[15]) {
	case 0xE7:
		puts("EP88xC 1.0");
		break;
	default:
		printf("unknown ID=%02X", bcsr[15]);
	}
	printf("  CPLD revision %d\n", bcsr[14]);

	return 0;
}
