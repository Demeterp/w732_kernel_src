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

#ifndef __MT6575_DRAMC_H__
#define __MT6575_DRAMC_H__

typedef enum
{
    /* LPDDR allowable clock range -- begin */
    LPDDR_DRAM_CLOCK_156 = 156,
    LPDDR_DRAM_CLOCK_161 = 161,
    LPDDR_DRAM_CLOCK_166 = 166,
    LPDDR_DRAM_CLOCK_171 = 171,
    LPDDR_DRAM_CLOCK_176 = 176,
    LPDDR_DRAM_CLOCK_182 = 182,
    LPDDR_DRAM_CLOCK_187 = 187,
    LPDDR_DRAM_CLOCK_192 = 192, /* LPDDR default clock */
    /* LPDDR allowable clock range -- end */

    /* LPDDR2 allowable clock range -- begin */
    LPDDR2_DRAM_CLOCK_195 = 195,
    LPDDR2_DRAM_CLOCK_201 = 201,
    LPDDR2_DRAM_CLOCK_208 = 208,
    LPDDR2_DRAM_CLOCK_214 = 214,
    LPDDR2_DRAM_CLOCK_221 = 221,
    LPDDR2_DRAM_CLOCK_227 = 227,
    LPDDR2_DRAM_CLOCK_234 = 234,
    LPDDR2_DRAM_CLOCK_240 = 240,
    LPDDR2_DRAM_CLOCK_247 = 247,
    LPDDR2_DRAM_CLOCK_253 = 254,
    LPDDR2_DRAM_CLOCK_260 = 260, /* LPDDR2 default clock */
    /* LPDDR2 allowable clock range -- end */
}
EN_DRAM_CLOCK;

int get_dram_refresh_clock(void);
int get_dram_default_refresh_clock(void);

/*
 * refresh_interval is proportional to 1/refresh_clock
 */
int set_dram_refresh_clock(int clock);

int get_dram_clock(void);
int get_dram_default_clock(void);
int set_dram_clock(EN_DRAM_CLOCK clock);


#endif
