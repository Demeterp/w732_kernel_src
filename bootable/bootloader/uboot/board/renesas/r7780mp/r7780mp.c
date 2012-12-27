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
 * Copyright (C) 2007,2008 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 * Copyright (C) 2008 Yusuke Goda <goda.yusuke@renesas.com>
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
#include <ide.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/pci.h>
#include <netdev.h>
#include "r7780mp.h"

int checkboard(void)
{
#if defined(CONFIG_R7780MP)
	puts("BOARD: Renesas Solutions R7780MP\n");
#else
	puts("BOARD: Renesas Solutions R7780RP\n");
#endif
	return 0;
}

int board_init(void)
{
	/* SCIF Enable */
	writew(0x0, PHCR);

	return 0;
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_memstart = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_memsize = CONFIG_SYS_SDRAM_SIZE;
	printf("DRAM:  %dMB\n", CONFIG_SYS_SDRAM_SIZE / (1024 * 1024));
	return 0;
}

void led_set_state(unsigned short value)
{

}

void ide_set_reset(int idereset)
{
	/* if reset = 1 IDE reset will be asserted */
	if (idereset) {
		writew(0x432, FPGA_CFCTL);
#if defined(CONFIG_R7780MP)
		writew(inw(FPGA_CFPOW)|0x01, FPGA_CFPOW);
#else
		writew(inw(FPGA_CFPOW)|0x02, FPGA_CFPOW);
#endif
		writew(0x01, FPGA_CFCDINTCLR);
	}
}

static struct pci_controller hose;
void pci_init_board(void)
{
	pci_sh7780_init(&hose);
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}
