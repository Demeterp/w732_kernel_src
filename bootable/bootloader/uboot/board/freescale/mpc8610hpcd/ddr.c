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
 * Copyright 2008 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 */

#include <common.h>
#include <i2c.h>

#include <asm/fsl_ddr_sdram.h>
#include <asm/fsl_ddr_dimm_params.h>

static void
get_spd(ddr2_spd_eeprom_t *spd, unsigned char i2c_address)
{
	i2c_read(i2c_address, 0, 1, (uchar *)spd, sizeof(ddr2_spd_eeprom_t));
}

unsigned int fsl_ddr_get_mem_data_rate(void)
{
	return get_bus_freq(0);
}

void fsl_ddr_get_spd(ddr2_spd_eeprom_t *ctrl_dimms_spd,
		      unsigned int ctrl_num)
{
	unsigned int i;
	unsigned int i2c_address = 0;

	for (i = 0; i < CONFIG_DIMM_SLOTS_PER_CTLR; i++) {
		if (ctrl_num == 0 && i == 0) {
			i2c_address = SPD_EEPROM_ADDRESS1;
		}
		get_spd(&(ctrl_dimms_spd[i]), i2c_address);
	}
}

void fsl_ddr_board_options(memctl_options_t *popts,
				dimm_params_t *pdimm,
				unsigned int ctrl_num)
{
	/*
	 * Factors to consider for clock adjust:
	 *	- number of chips on bus
	 *	- position of slot
	 *	- DDR1 vs. DDR2?
	 *	- ???
	 *
	 * This needs to be determined on a board-by-board basis.
	 *	0110	3/4 cycle late
	 *	0111	7/8 cycle late
	 */
	popts->clk_adjust = 7;

	/*
	 * Factors to consider for CPO:
	 *	- frequency
	 *	- ddr1 vs. ddr2
	 */
	popts->cpo_override = 10;

	/*
	 * Factors to consider for write data delay:
	 *	- number of DIMMs
	 *
	 * 1 = 1/4 clock delay
	 * 2 = 1/2 clock delay
	 * 3 = 3/4 clock delay
	 * 4 = 1   clock delay
	 * 5 = 5/4 clock delay
	 * 6 = 3/2 clock delay
	 */
	popts->write_data_delay = 3;

	/* 2T timing enable */
	popts->twoT_en = 1;

	/*
	 * Factors to consider for half-strength driver enable:
	 *	- number of DIMMs installed
	 */
	popts->half_strength_driver_enable = 0;
}
