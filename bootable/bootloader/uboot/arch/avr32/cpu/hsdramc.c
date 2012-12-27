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
 * Copyright (C) 2005-2006 Atmel Corporation
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

#include <asm/io.h>
#include <asm/sdram.h>

#include <asm/arch/clk.h>
#include <asm/arch/memory-map.h>

#include "hsdramc1.h"

unsigned long sdram_init(void *sdram_base, const struct sdram_config *config)
{
	unsigned long sdram_size;
	uint32_t cfgreg;
	unsigned int i;

	cfgreg = (HSDRAMC1_BF(NC, config->col_bits - 8)
		       | HSDRAMC1_BF(NR, config->row_bits - 11)
		       | HSDRAMC1_BF(NB, config->bank_bits - 1)
		       | HSDRAMC1_BF(CAS, config->cas)
		       | HSDRAMC1_BF(TWR, config->twr)
		       | HSDRAMC1_BF(TRC, config->trc)
		       | HSDRAMC1_BF(TRP, config->trp)
		       | HSDRAMC1_BF(TRCD, config->trcd)
		       | HSDRAMC1_BF(TRAS, config->tras)
		       | HSDRAMC1_BF(TXSR, config->txsr));

	if (config->data_bits == SDRAM_DATA_16BIT)
		cfgreg |= HSDRAMC1_BIT(DBW);

	hsdramc1_writel(CR, cfgreg);

	/* Send a NOP to turn on the clock (necessary on some chips) */
	hsdramc1_writel(MR, HSDRAMC1_MODE_NOP);
	hsdramc1_readl(MR);
	writel(0, sdram_base);

	/*
	 * Initialization sequence for SDRAM, from the data sheet:
	 *
	 * 1. A minimum pause of 200 us is provided to precede any
	 *    signal toggle.
	 */
	udelay(200);

	/*
	 * 2. A Precharge All command is issued to the SDRAM
	 */
	hsdramc1_writel(MR, HSDRAMC1_MODE_BANKS_PRECHARGE);
	hsdramc1_readl(MR);
	writel(0, sdram_base);

	/*
	 * 3. Eight auto-refresh (CBR) cycles are provided
	 */
	hsdramc1_writel(MR, HSDRAMC1_MODE_AUTO_REFRESH);
	hsdramc1_readl(MR);
	for (i = 0; i < 8; i++)
		writel(0, sdram_base);

	/*
	 * 4. A mode register set (MRS) cycle is issued to program
	 *    SDRAM parameters, in particular CAS latency and burst
	 *    length.
	 *
	 * The address will be chosen by the SDRAMC automatically; we
	 * just have to make sure BA[1:0] are set to 0.
	 */
	hsdramc1_writel(MR, HSDRAMC1_MODE_LOAD_MODE);
	hsdramc1_readl(MR);
	writel(0, sdram_base);

	/*
	 * 5. The application must go into Normal Mode, setting Mode
	 *    to 0 in the Mode Register and performing a write access
	 *    at any location in the SDRAM.
	 */
	hsdramc1_writel(MR, HSDRAMC1_MODE_NORMAL);
	hsdramc1_readl(MR);
	writel(0, sdram_base);

	/*
	 * 6. Write refresh rate into SDRAMC refresh timer count
	 *    register (refresh rate = timing between refresh cycles).
	 */
	hsdramc1_writel(TR, config->refresh_period);

	if (config->data_bits == SDRAM_DATA_16BIT)
		sdram_size = 1 << (config->row_bits + config->col_bits
				   + config->bank_bits + 1);
	else
		sdram_size = 1 << (config->row_bits + config->col_bits
				   + config->bank_bits + 2);

	return sdram_size;
}
