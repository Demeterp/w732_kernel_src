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
 * (C) Copyright 2007
 * Stefano Babic, DENX Gmbh, sbabic@denx.de
 *
 * (C) Copyright 2004
 * Robert Whaley, Applied Data Systems, Inc. rwhaley@applieddata.net
 *
 * (C) Copyright 2002
 * Kyle Harris, Nexus Technologies, Inc. kharris@nexus-tech.net
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
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
#include <asm/arch/pxa-regs.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

#define		RH_A_PSM	(1 << 8)	/* power switching mode */
#define		RH_A_NPS	(1 << 9)	/* no power switching */

extern struct serial_device serial_ffuart_device;
extern struct serial_device serial_btuart_device;
extern struct serial_device serial_stuart_device;

#if CONFIG_POLARIS
#define BOOT_CONSOLE	"serial_stuart"
#else
#define BOOT_CONSOLE	"serial_ffuart"
#endif
/* ------------------------------------------------------------------------- */

/*
 * Miscelaneous platform dependent initialisations
 */

int usb_board_init(void)
{
	UHCHR = (UHCHR | UHCHR_PCPL | UHCHR_PSPL) &
		~(UHCHR_SSEP0 | UHCHR_SSEP1 | UHCHR_SSEP2 | UHCHR_SSE);

	UHCHR |= UHCHR_FSBIR;

	while (UHCHR & UHCHR_FSBIR);

	UHCHR &= ~UHCHR_SSE;
	UHCHIE = (UHCHIE_UPRIE | UHCHIE_RWIE);

	/* Clear any OTG Pin Hold */
	if (PSSR & PSSR_OTGPH)
		PSSR |= PSSR_OTGPH;

	UHCRHDA &= ~(RH_A_NPS);
	UHCRHDA |= RH_A_PSM;

	/* Set port power control mask bits, only 3 ports. */
	UHCRHDB |= (0x7<<17);

	return 0;
}

void usb_board_init_fail(void)
{
	return;
}

void usb_board_stop(void)
{
	UHCHR |= UHCHR_FHR;
	udelay(11);
	UHCHR &= ~UHCHR_FHR;

	UHCCOMS |= 1;
	udelay(10);

	CKEN &= ~CKEN10_USBHOST;

	return;
}

int board_init (void)
{
	/* memory and cpu-speed are setup before relocation */
	/* so we do _nothing_ here */

	/* arch number of ConXS Board */
	gd->bd->bi_arch_number = 776;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0xa000003c;

	return 0;
}

int board_late_init(void)
{
#if defined(CONFIG_SERIAL_MULTI)
	char *console=getenv("boot_console");

	if ((console == NULL) || (strcmp(console,"serial_btuart") &&
		strcmp(console,"serial_stuart") &&
		strcmp(console,"serial_ffuart"))) {
			console = BOOT_CONSOLE;
	}
	setenv("stdout",console);
	setenv("stdin", console);
	setenv("stderr",console);
#endif
	return 0;
}

struct serial_device *default_serial_console (void)
{
	return &serial_ffuart_device;
}

int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
	gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
	gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
	gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
	gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;

	return 0;
}

#ifdef CONFIG_DRIVER_DM9000
int board_eth_init(bd_t *bis)
{
	return dm9000_initialize(bis);
}
#endif
