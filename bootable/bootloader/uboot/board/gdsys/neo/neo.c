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
 * (C) Copyright 2007-2008
 * Dirk Eibach,  Guntermann & Drunck GmbH, eibach@gdsys.de
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
#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>

#define HWTYPE_CCX16	1
#define HWREV_300	3

int board_early_init_f(void)
{
	mtdcr(UIC0SR, 0xFFFFFFFF);	/* clear all ints */
	mtdcr(UIC0ER, 0x00000000);	/* disable all ints */
	mtdcr(UIC0CR, 0x00000000);	/* set all to be non-critical */
	mtdcr(UIC0PR, 0xFFFFFF80);	/* set int polarities */
	mtdcr(UIC0TR, 0x10000000);	/* set int trigger levels */
	mtdcr(UIC0VCR, 0x00000001);	/* set vect base=0,INT0 highest prio */
	mtdcr(UIC0SR, 0xFFFFFFFF);	/* clear all ints */

	/*
	 * EBC Configuration Register: set ready timeout to 512 ebc-clks
	 * -> ca. 15 us
	 */
	mtebc(EBC0_CFG, 0xa8400000);	/* ebc always driven */

	return 0;
}

/*
 * Check Board Identity:
 */
int checkboard(void)
{
	char *s = getenv("serial#");
	u16 val = in_le16((void *)CONFIG_FPGA_BASE + 2);
	u8 unit_type;
	u8 hardware_cpu_ports;
	u8 hardware_con_ports;
	u8 hardware_version;

	printf("Board: CATCenter Neo");

	if (s != NULL) {
		puts(", serial# ");
		puts(s);
	}
	puts("\n       ");

	unit_type = (val & 0xf000) >> 12;
	hardware_cpu_ports = ((val & 0x0f00) >> 8) * 8;
	hardware_con_ports = ((val & 0x00f0) >> 4) * 2;
	hardware_version = val & 0x000f;

	switch (unit_type) {
	case HWTYPE_CCX16:
		printf("CCX16-FPGA (80 UARTs)");
		break;

	default:
		printf("UnitType %d, unsupported", unit_type);
		break;
	}

	printf(", %d cpu ports, %d console ports,",
	       hardware_cpu_ports, hardware_con_ports);

	switch (hardware_version) {
	case HWREV_300:
		printf(" HW-Ver 3.00\n");
		break;

	default:
		printf(" HW-Ver %d, unsupported\n",
		       hardware_version);
		break;
	}

	return 0;
}
