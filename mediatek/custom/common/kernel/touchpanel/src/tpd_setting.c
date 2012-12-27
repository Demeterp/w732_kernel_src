/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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

#include "tpd.h"

int TPD_RES_X = 480;
int TPD_RES_Y = 800;

//#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
int tpd_calmat[8]     = {0};
int tpd_def_calmat[8] = {0};
int tpd_calmat_size   = 8;
int tpd_def_calmat_size   = 8;
module_param_array(tpd_calmat, int, &tpd_calmat_size, 00664);
module_param_array(tpd_def_calmat, int, &tpd_def_calmat_size, 00444);
//#endif
//#ifdef TPD_TYPE_CAPACITIVE
int tpd_type_cap = 0;

module_param(tpd_type_cap, int, 00444);
int tpd_firmware_version[2] = {0,0};
int tpd_firmware_version_size = 2;
module_param_array(tpd_firmware_version, int, &tpd_firmware_version_size, 00444);

int tpd_mode = TPD_MODE_NORMAL;
int tpd_mode_axis = 0;
int tpd_mode_min = 400; //TPD_RES_Y/2;
int tpd_mode_max = 800; //TPD_RES_Y;
int tpd_mode_keypad_tolerance = 480*480/1600; //TPD_RES_X*TPD_RES_X/1600;
module_param(tpd_mode, int, 00664);
module_param(tpd_mode_axis, int, 00664);
module_param(tpd_mode_min, int, 00664);
module_param(tpd_mode_max, int, 00664);
module_param(tpd_mode_keypad_tolerance, int, 00664);

/* ATTENTION! all the default values should sync with tpd_adc_init()@tpd_adc.c */
int tpd_em_debounce_time0 = 1;
int tpd_em_debounce_time = 0;
int tpd_em_debounce_time1 = 4;
module_param(tpd_em_debounce_time0, int, 00664);
module_param(tpd_em_debounce_time1, int, 00664);
module_param(tpd_em_debounce_time, int, 00664);

int tpd_em_spl_num = 1;
module_param(tpd_em_spl_num, int, 00664);

int tpd_em_pressure_threshold = 0;
module_param(tpd_em_pressure_threshold, int, 00664);

int tpd_em_auto_time_interval = 10;
module_param(tpd_em_auto_time_interval, int, 00664);

int tpd_em_sample_cnt = 16;
module_param(tpd_em_sample_cnt, int, 00664);

int tpd_load_status = 0;
module_param(tpd_load_status, int, 00664);

int tpd_em_asamp = 1;
module_param(tpd_em_asamp, int, 00664);
