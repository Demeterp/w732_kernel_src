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
		if (ctrl_num == 0 && i == 1) {
			i2c_address = SPD_EEPROM_ADDRESS2;
		}
		if (ctrl_num == 1 && i == 0) {
			i2c_address = SPD_EEPROM_ADDRESS3;
		}
		if (ctrl_num == 1 && i == 1) {
			i2c_address = SPD_EEPROM_ADDRESS4;
		}
		get_spd(&(ctrl_dimms_spd[i]), i2c_address);
	}
}

typedef struct {
	u32 datarate_mhz_low;
	u32 datarate_mhz_high;
	u32 n_ranks;
	u32 clk_adjust;
	u32 cpo;
	u32 write_data_delay;
} board_specific_parameters_t;

/* XXX: these values need to be checked for all interleaving modes.  */
const board_specific_parameters_t board_specific_parameters[2][16] = {
	{
	/* 	memory controller 0		 	*/
	/*	  lo|  hi|  num|  clk| cpo|wrdata	*/
	/*	 mhz| mhz|ranks|adjst|    | delay	*/
		{  0, 333,    4,    7,   7,     3},
		{334, 400,    4,    7,   9,     3},
		{401, 549,    4,    7,   9,     3},
		{550, 650,    4,    7,  10,     4},

		{  0, 333,    3,    7,   7,     3},
		{334, 400,    3,    7,   9,     3},
		{401, 549,    3,    7,   9,     3},
		{550, 650,    3,    7,  10,     4},

		{  0, 333,    2,    7,   7,     3},
		{334, 400,    2,    7,   9,     3},
		{401, 549,    2,    7,   9,     3},
		{550, 650,    2,    7,  10,     4},

		{  0, 333,    1,    7,   7,     3},
		{334, 400,    1,    7,   9,     3},
		{401, 549,    1,    7,   9,     3},
		{550, 650,    1,    7,  10,     4}
	},

	{
	/* 	memory controller 1			*/
	/*	  lo|  hi|  num|  clk| cpo|wrdata	*/
	/*       mhz| mhz|ranks|adjst|    | delay	*/
		{  0, 333,    4,    7,   7,    3},
		{334, 400,    4,    7,   9,    3},
		{401, 549,    4,    7,   9,    3},
		{550, 650,    4,    7,  10,    4},

		{  0, 333,    3,    7,   7,    3},
		{334, 400,    3,    7,   9,    3},
		{401, 549,    3,    7,   9,    3},
		{550, 650,    3,    7,  10,    4},

		{  0, 333,    2,    7,   7,    3},
		{334, 400,    2,    7,   9,    3},
		{401, 549,    2,    7,   9,    3},
		{550, 650,    2,    7,  10,    4},

		{  0, 333,    1,    7,   7,    3},
		{334, 400,    1,    7,   9,    3},
		{401, 549,    1,    7,   9,    3},
		{550, 650,    1,    7,  10,    4}
	}
};

void fsl_ddr_board_options(memctl_options_t *popts,
			dimm_params_t *pdimm,
			unsigned int ctrl_num)
{
	const board_specific_parameters_t *pbsp =
			&(board_specific_parameters[ctrl_num][0]);
	u32 num_params = sizeof(board_specific_parameters[ctrl_num]) /
			sizeof(board_specific_parameters[0][0]);
	u32 i;
	u32 j;
	ulong ddr_freq;

	/* set odt_rd_cfg and odt_wr_cfg. If the there is only one dimm in
	 * that controller, set odt_wr_cfg to 4 for CS0, and 0 to CS1. If
	 * there are two dimms in the controller, set odt_rd_cfg to 3 and
	 * odt_wr_cfg to 3 for the even CS, 0 for the odd CS.
	 */
	for (i = 0; i < CONFIG_CHIP_SELECTS_PER_CTRL; i++) {
		if (i&1) {      /* odd CS */
			popts->cs_local_opts[i].odt_rd_cfg = 0;
			popts->cs_local_opts[i].odt_wr_cfg = 0;
		} else {	/* even CS */
			if ((CONFIG_DIMM_SLOTS_PER_CTLR == 2) &&
				(pdimm[i/2].n_ranks != 0)) {
				popts->cs_local_opts[i].odt_rd_cfg = 3;
				popts->cs_local_opts[i].odt_wr_cfg = 3;
			} else {
				popts->cs_local_opts[i].odt_rd_cfg = 0;
				popts->cs_local_opts[i].odt_wr_cfg = 4;
			}
		}
	}

	/* Get clk_adjust, cpo, write_data_delay, according to the board ddr
	 * freqency and n_banks specified in board_specific_parameters table.
	 */
	ddr_freq = fsl_ddr_get_mem_data_rate() / 1000000;
	for (j = 0; j < CONFIG_DIMM_SLOTS_PER_CTLR; j++) {
		if (pdimm[j].n_ranks > 0) {
			for (i = 0; i < num_params; i++) {
				if (ddr_freq >= pbsp->datarate_mhz_low &&
				ddr_freq <= pbsp->datarate_mhz_high &&
				pdimm[j].n_ranks == pbsp->n_ranks) {
					popts->clk_adjust = pbsp->clk_adjust;
					popts->cpo_override = pbsp->cpo;
					popts->write_data_delay =
						pbsp->write_data_delay;
					break;
				}
				pbsp++;
			}
		}
	}

	/* 2T timing enable */
	popts->twoT_en = 1;
}
