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

#ifndef __MT65XX_MON_H__
#define __MT65XX_MON_H__

struct mt65xx_mon_log
{
    __u32 cpu_cyc;
    __u32 cpu_cnt0;
    __u32 cpu_cnt1;
    __u32 cpu_cnt2;
    __u32 cpu_cnt3;
    __u32 cpu_cnt4;
    __u32 cpu_cnt5;
    __u32 l2c_cnt0;
    __u32 l2c_cnt1;

    __u32 BM_BCNT;
    __u32 BM_TACT;
    __u32 BM_TSCT;
    __u32 BM_WACT;
    __u32 BM_WSCT;
    __u32 BM_BACT;
    __u32 BM_BSCT;
    __u32 BM_TSCT2;
    __u32 BM_WSCT2;
    __u32 BM_TSCT3;
    __u32 BM_WSCT3;
    __u32 BM_WSCT4;

    __u32 BM_TPCT1;
    
    __u32 DRAMC_PageHit;
    __u32 DRAMC_PageMiss;
    __u32 DRAMC_Interbank;
    __u32 DRAMC_Idle;   
};


typedef enum 
{
    MODE_SCHED_SWITCH,
    MODE_PERIODIC,
    MODE_MANUAL    
} MonitorMode;

extern void set_mt65xx_mon_period(long time_ns);
extern long get_mt65xx_mon_period(void);
extern void set_mt65xx_mon_manual_start(unsigned int bStart);
extern unsigned int get_mt65xx_mon_manual_start(void);
extern void set_mt65xx_mon_mode(MonitorMode mode);
extern MonitorMode get_mt65xx_mon_mode(void);

extern int mt65xx_mon_init(void);
extern int mt65xx_mon_deinit(void);
extern int mt65xx_mon_enable(void);
extern int mt65xx_mon_disable(void);
extern unsigned int mt65xx_mon_log(void);
extern int mt65xx_mon_print_log(unsigned int log, struct trace_iterator *iter);

#endif  /* !__MT65XX_MON_H__ */
