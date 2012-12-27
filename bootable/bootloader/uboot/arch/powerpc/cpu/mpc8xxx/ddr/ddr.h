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

#ifndef FSL_DDR_MAIN_H
#define FSL_DDR_MAIN_H

#include <asm/fsl_ddr_sdram.h>
#include <asm/fsl_ddr_dimm_params.h>

#include "common_timing_params.h"

/*
 * Bind the main DDR setup driver's generic names
 * to this specific DDR technology.
 */
static __inline__ int
compute_dimm_parameters(const generic_spd_eeprom_t *spd,
			dimm_params_t *pdimm,
			unsigned int dimm_number)
{
	return ddr_compute_dimm_parameters(spd, pdimm, dimm_number);
}

/*
 * Data Structures
 *
 * All data structures have to be on the stack
 */
#define CONFIG_SYS_NUM_DDR_CTLRS CONFIG_NUM_DDR_CONTROLLERS
#define CONFIG_SYS_DIMM_SLOTS_PER_CTLR CONFIG_DIMM_SLOTS_PER_CTLR

typedef struct {
	generic_spd_eeprom_t
	   spd_installed_dimms[CONFIG_SYS_NUM_DDR_CTLRS][CONFIG_SYS_DIMM_SLOTS_PER_CTLR];
	struct dimm_params_s
	   dimm_params[CONFIG_SYS_NUM_DDR_CTLRS][CONFIG_SYS_DIMM_SLOTS_PER_CTLR];
	memctl_options_t memctl_opts[CONFIG_SYS_NUM_DDR_CTLRS];
	common_timing_params_t common_timing_params[CONFIG_SYS_NUM_DDR_CTLRS];
	fsl_ddr_cfg_regs_t fsl_ddr_config_reg[CONFIG_SYS_NUM_DDR_CTLRS];
} fsl_ddr_info_t;

/* Compute steps */
#define STEP_GET_SPD                 (1 << 0)
#define STEP_COMPUTE_DIMM_PARMS      (1 << 1)
#define STEP_COMPUTE_COMMON_PARMS    (1 << 2)
#define STEP_GATHER_OPTS             (1 << 3)
#define STEP_ASSIGN_ADDRESSES        (1 << 4)
#define STEP_COMPUTE_REGS            (1 << 5)
#define STEP_PROGRAM_REGS            (1 << 6)
#define STEP_ALL                     0xFFF

extern unsigned long long
fsl_ddr_compute(fsl_ddr_info_t *pinfo, unsigned int start_step);

extern const char * step_to_string(unsigned int step);

extern unsigned int
compute_fsl_memctl_config_regs(const memctl_options_t *popts,
			       fsl_ddr_cfg_regs_t *ddr,
			       const common_timing_params_t *common_dimm,
			       const dimm_params_t *dimm_parameters,
			       unsigned int dbw_capacity_adjust);
extern unsigned int
compute_lowest_common_dimm_parameters(const dimm_params_t *dimm_params,
				      common_timing_params_t *outpdimm,
				      unsigned int number_of_dimms);
extern unsigned int populate_memctl_options(int all_DIMMs_registered,
				memctl_options_t *popts,
				dimm_params_t *pdimm,
				unsigned int ctrl_num);

extern unsigned int mclk_to_picos(unsigned int mclk);
extern unsigned int get_memory_clk_period_ps(void);
extern unsigned int picos_to_mclk(unsigned int picos);

#endif
