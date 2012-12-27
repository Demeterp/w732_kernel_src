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

#ifndef DDR2_DIMM_PARAMS_H
#define DDR2_DIMM_PARAMS_H

/* Parameters for a DDR2 dimm computed from the SPD */
typedef struct dimm_params_s {

	/* DIMM organization parameters */
	char mpart[19];		/* guaranteed null terminated */

	unsigned int n_ranks;
	unsigned long long rank_density;
	unsigned long long capacity;
	unsigned int data_width;
	unsigned int primary_sdram_width;
	unsigned int ec_sdram_width;
	unsigned int registered_dimm;

	/* SDRAM device parameters */
	unsigned int n_row_addr;
	unsigned int n_col_addr;
	unsigned int edc_config;	/* 0 = none, 1 = parity, 2 = ECC */
	unsigned int n_banks_per_sdram_device;
	unsigned int burst_lengths_bitmask;	/* BL=4 bit 2, BL=8 = bit 3 */
	unsigned int row_density;

	/* used in computing base address of DIMMs */
	unsigned long long base_address;
	/* mirrored DIMMs */
	unsigned int mirrored_dimm;	/* only for ddr3 */

	/* DIMM timing parameters */

	unsigned int mtb_ps;	/* medium timebase ps, only for ddr3 */
	unsigned int tAA_ps;	/* minimum CAS latency time, only for ddr3 */
	unsigned int tFAW_ps;	/* four active window delay, only for ddr3 */

	/*
	 * SDRAM clock periods
	 * The range for these are 1000-10000 so a short should be sufficient
	 */
	unsigned int tCKmin_X_ps;
	unsigned int tCKmin_X_minus_1_ps;
	unsigned int tCKmin_X_minus_2_ps;
	unsigned int tCKmax_ps;

	/* SPD-defined CAS latencies */
	unsigned int caslat_X;
	unsigned int caslat_X_minus_1;
	unsigned int caslat_X_minus_2;

	unsigned int caslat_lowest_derated;	/* Derated CAS latency */

	/* basic timing parameters */
	unsigned int tRCD_ps;
	unsigned int tRP_ps;
	unsigned int tRAS_ps;

	unsigned int tWR_ps;	/* maximum = 63750 ps */
	unsigned int tWTR_ps;	/* maximum = 63750 ps */
	unsigned int tRFC_ps;   /* max = 255 ns + 256 ns + .75 ns
				       = 511750 ps */

	unsigned int tRRD_ps;	/* maximum = 63750 ps */
	unsigned int tRC_ps;	/* maximum = 254 ns + .75 ns = 254750 ps */

	unsigned int refresh_rate_ps;

	/* DDR3 doesn't need these as below */
	unsigned int tIS_ps;	/* byte 32, spd->ca_setup */
	unsigned int tIH_ps;	/* byte 33, spd->ca_hold */
	unsigned int tDS_ps;	/* byte 34, spd->data_setup */
	unsigned int tDH_ps;	/* byte 35, spd->data_hold */
	unsigned int tRTP_ps;	/* byte 38, spd->trtp */
	unsigned int tDQSQ_max_ps;	/* byte 44, spd->tdqsq */
	unsigned int tQHS_ps;	/* byte 45, spd->tqhs */
} dimm_params_t;

extern unsigned int ddr_compute_dimm_parameters(
					 const generic_spd_eeprom_t *spd,
					 dimm_params_t *pdimm,
					 unsigned int dimm_number);

#endif
