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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/earlysuspend.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/xlog.h>
#include <linux/jiffies.h>

#include "asm/system.h"

#include "mach/mt6575_clock_manager.h"
#include "mach/mt6575_typedefs.h"
#include "mach/mt6575_sc.h"
#include "mach/mt6575_cpe.h"
#include "mach/mt6575_boot.h"

#include "mach/pmic_mt6329_hw_bank1.h"
#include "mach/pmic_mt6329_sw_bank1.h"
#include "mach/pmic_mt6329_hw.h"
#include "mach/pmic_mt6329_sw.h"
#include "mach/upmu_common_sw.h"
#include "mach/upmu_hw.h"

/**************************************************
* enable this option to do DVFS random test
***************************************************/
//#define MTK_DVFS_RANDOM_TEST

/**************************************************
* enable this option to adjust buck voltage
***************************************************/
#define MTK_BUCK_ADJUST

/*******************************************
 * Add for CPU 26Mhz <-> 1Ghz stress begin
 *******************************************/
#define cpu_ss_dprintk(fmt, args...)    \
do {                                    \
    if (cpu_ss_debug_mode) {            \
        printk(fmt, ##args);            \
    }                                   \
} while(0)

static struct hrtimer cpu_ss_timer;
struct task_struct *cpu_ss_thread = NULL;
static DECLARE_WAIT_QUEUE_HEAD(cpu_ss_timer_waiter);

static int cpu_ss_period_s = 0;
static int cpu_ss_period_ns = 100;
static int cpu_ss_timer_flag = 0;

static bool cpu_ss_debug_mode = false;
static bool cpu_ss_period_mode = false;

/*******************************************
 * Add for CPU 26Mhz <-> 1Ghz stress end
 *******************************************/
 
/***************************
* debug message
****************************/
#define dprintk(fmt, args...)                                       \
do {                                                                \
    if (mtk_cpufreq_debug) {                                        \
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", fmt, ##args);   \
    }                                                               \
} while(0)

/***************************
* cpufreq custom attribute
****************************/
#define cpufreq_attr(_name)                     \
static struct kobj_attribute _name##_attr = {   \
    .attr = {                                   \
        .name = __stringify(_name),             \
        .mode = 0644,                           \
    },                                          \
    .show = _name##_show,                       \
    .store = _name##_store,                     \
}

#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

#ifdef CONFIG_HAS_EARLYSUSPEND
static struct early_suspend mtk_cpufreq_early_suspend_handler =
{
    .level = EARLY_SUSPEND_LEVEL_DISABLE_FB + 200,
    .suspend = NULL,
    .resume  = NULL,
};
#endif

#define ASYNC_MODE  (0x0)
#define SYNC_MODE   (0x1)

#define DVFS_F1 (1167833)   // KHz
#define DVFS_F2 ( 500500)   // KHz
#define DVFS_F3 ( 250250)   // KHz
#define DVFS_F4 ( 166833)   // KHz

/*****************************************
* PMIC settle time, should not be changed
******************************************/
#define PMIC_SETTLE_TIME (70) // us

static unsigned int g_cur_freq = DVFS_F1;  // KHz
static unsigned int g_cur_vol = UPMU_VOLT_1_2_5_0_V;

/**************************************************
* enable DVFS function
***************************************************/
static bool g_enable_dvfs = false;

static int g_test_mode_enable = 0;
static int g_dvfs_disable_count = 0;
static int g_limited_freq = DVFS_F1;
static int g_limited_min_freq = DVFS_F4;

static bool mtk_cpufreq_debug = false;
static bool mtk_cpufreq_ready = false;
static bool mtk_cpufreq_pause = false;

static DEFINE_SPINLOCK(mtk_cpufreq_lock);

struct mtk_cpu_freq_info 
{
    unsigned int cpufreq_mhz;
    int	vcc_core;   /* in mV */
};

/***************************
* Operate Point Definition
****************************/
#define OP(cpufreq, vcore)  \
{                           \
    .cpufreq_mhz = cpufreq, \
    .vcc_core = vcore,      \
}

/***************************
* MT6575 E1 DVFS Table
****************************/
static struct mtk_cpu_freq_info mtk_freqs_e1[] = {
    OP(DVFS_F2, 1175),
    OP(DVFS_F1, 1250),
};

/***************************
* MT6575 E2 DVFS Table
****************************/
static struct mtk_cpu_freq_info mtk_freqs_e2[] = {
    OP(DVFS_F4,  975),
    OP(DVFS_F3, 1075),
    OP(DVFS_F2, 1175),
    OP(DVFS_F1, 1250),
};

static unsigned int mtk_cpu_freqs_num;
static struct mtk_cpu_freq_info *mtk_cpu_freqs;
static struct cpufreq_frequency_table *mtk_cpu_freqs_table;

extern struct kobject *power_kobj;
extern bool clkmgr_high_speed_check(void);
extern int cpufreq_state_set(int enable);

/*******************************************
* switch to synchronous or asynchronous mode
******* not use in current platform ********
********************************************/
void mtk_mode_switch(unsigned int mode)
{
    kal_uint32 temp = 0;
    
    switch (mode)
    {
        case ASYNC_MODE:
            temp = DRV_Reg32(CLK_CTL0);
            temp &= 0xFFF8FFFF; // set syncmodereq_acp = syncmodereq_m1 = syncmodereq_mo = async mode
            temp &= 0xFFFFFFE0; // set div_sel = 0
            DRV_WriteReg32(CLK_CTL0, temp);
            break;
        case SYNC_MODE:
            temp = DRV_Reg32(CLK_CTL0);
            temp &= 0xFFFFFFE0;
            temp |= (DRV_Reg32(TOP_CKDIV0) & 0x0000001F);
            DRV_WriteReg32(CLK_CTL0, temp); // set div_sel in CLK_CTL0 to the value of ckdiv0_sel in TOP_CKDIV0
            
            //while (!(*CLK_CTL0 & 0x08000000)); // check if the CLK_CTL0[27] is 1 (optional)
            
            temp = DRV_Reg32(CLK_CTL0);
            temp &= 0xFFF8FFFF;
            temp |= 0x00070000; // set syncmodereq_acp = syncmodereq_m1 = syncmodereq_mo = sync mode
            DRV_WriteReg32(CLK_CTL0, temp);
            break;
        default:
            break;
    }
}

/***********************************************
* register frequency table to cpufreq subsystem
************************************************/
static int setup_freqs_table(struct cpufreq_policy *policy, struct mtk_cpu_freq_info *freqs, int num)
{
    struct cpufreq_frequency_table *table;
    int i, ret;
    
    table = kzalloc((num + 1) * sizeof(*table), GFP_KERNEL);
    if (table == NULL)
        return -ENOMEM;
    
    for (i = 0; i < num; i++) {
        table[i].index = i;
        table[i].frequency = freqs[i].cpufreq_mhz;
    }
    table[num].frequency = i;
    table[num].frequency = CPUFREQ_TABLE_END;
    
    mtk_cpu_freqs = freqs;
    mtk_cpu_freqs_num = num;
    mtk_cpu_freqs_table = table;
    
    ret = cpufreq_frequency_table_cpuinfo(policy, table);
    if (!ret)
        cpufreq_frequency_table_get_attr(mtk_cpu_freqs_table, policy->cpu);
    
    return 0;
}

static int mtk_cpufreq_verify(struct cpufreq_policy *policy)
{
    dprintk("call mtk_cpufreq_verify!\n");
    return cpufreq_frequency_table_verify(policy, mtk_cpu_freqs_table);
}

static unsigned int mtk_cpufreq_get(unsigned int cpu)
{
    dprintk("call mtk_cpufreq_get: %d!\n", g_cur_freq);
    return g_cur_freq;
}

/*****************************************
* frequency ramp up and ramp down handler
******************************************/
/***********************************************************
* [note]
* 1. frequency ramp up need to wait voltage settle
* 2. frequency ramp down do not need to wait voltage settle
************************************************************/
static void mtk_cpufreq_set(unsigned int freq_old, unsigned int freq_new)
{
    if (get_chip_eco_ver() == CHIP_E1)
    {
        if (freq_new == DVFS_F1)
        {
            #ifdef MTK_BUCK_ADJUST
            /* set VPROC to 1.250V */
            g_cur_vol = UPMU_VOLT_1_2_5_0_V;
            DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x03));
            
            mb();
            udelay(PMIC_SETTLE_TIME);
            #endif
            
            /* set ARMPLL to 1001.00Mhz */
            g_cur_freq = DVFS_F1;
            DRV_WriteReg32(TOP_CKDIV1, 0x00);
        }
        else if (freq_new == DVFS_F2)
        {
            /* set ARMPLL to 500.5Mhz */
            g_cur_freq = DVFS_F2;
            DRV_WriteReg32(TOP_CKDIV1, 0x0A);
            mb();
            
            #ifdef MTK_BUCK_ADJUST
            /* set VPROC to 1.175V */
            DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x02));
            g_cur_vol = UPMU_VOLT_1_1_7_5_V;
            #endif
        }
        else
        {
            dprintk("target frequency not support, ramp up to the highest level\n");
            
            #ifdef MTK_BUCK_ADJUST
            /* set VPROC to 1.250V */
            g_cur_vol = UPMU_VOLT_1_2_5_0_V;
            DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x03));
            
            mb();
            udelay(PMIC_SETTLE_TIME);
            #endif
            
            /* set ARMPLL to 1001.00Mhz */
            g_cur_freq = DVFS_F1;
            DRV_WriteReg32(TOP_CKDIV1, 0x00);
        }
    }
    else
    {
        if (freq_new == DVFS_F1)
        {
            #ifdef MTK_BUCK_ADJUST
            /* set VPROC to 1.250V */
            g_cur_vol = UPMU_VOLT_1_2_5_0_V;
            DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x03));
            
            mb();
            udelay(PMIC_SETTLE_TIME);
            #endif
            
            /* set ARMPLL to 1001.00Mhz */
            g_cur_freq = DVFS_F1;
            DRV_WriteReg32(TOP_CKDIV1, 0x00);
        }
        else if (freq_new == DVFS_F2)
        {
            if (freq_new > freq_old)
            {
                #ifdef MTK_BUCK_ADJUST
                /* set VPROC to 1.175V */
                g_cur_vol = UPMU_VOLT_1_1_7_5_V;
                DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x02));
                
                mb();
                udelay(PMIC_SETTLE_TIME);
                #endif
                
                /* set ARMPLL to 500.5Mhz */
                g_cur_freq = DVFS_F2;
                DRV_WriteReg32(TOP_CKDIV1, 0x0A);
            }
            else
            {
                /* set ARMPLL to 500.5Mhz */
                g_cur_freq = DVFS_F2;
                DRV_WriteReg32(TOP_CKDIV1, 0x0A);
                mb();
                
                #ifdef MTK_BUCK_ADJUST
                /* set VPROC to 1.175V */
                g_cur_vol = UPMU_VOLT_1_1_7_5_V;
                DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x02));
                #endif
            }
        }
        else if (freq_new == DVFS_F3)
        {
            if (freq_new > freq_old)
            {
                #ifdef MTK_BUCK_ADJUST
                /* set VPROC to 1.075V */
                g_cur_vol = UPMU_VOLT_1_0_7_5_V;
                DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x01));
                
                mb();
                udelay(PMIC_SETTLE_TIME);
                #endif
                
                /* set ARMPLL to  250.250Mhz */
                g_cur_freq = DVFS_F3;
                DRV_WriteReg32(TOP_CKDIV1, 0x0B);
            }
            else
            {
                /* set ARMPLL to  250.250Mhz */
                g_cur_freq = DVFS_F3;
                DRV_WriteReg32(TOP_CKDIV1, 0x0B);
                mb();
                
                #ifdef MTK_BUCK_ADJUST
                /* set VPROC to 1.075V */
                g_cur_vol = UPMU_VOLT_1_0_7_5_V;
                DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x01));
                #endif
            }
        }
        else if (freq_new == DVFS_F4)
        {
            /* set ARMPLL to 166.833Mhz */
            g_cur_freq = DVFS_F4;
            DRV_WriteReg32(TOP_CKDIV1, 0x1F);
            mb();
            
            #ifdef MTK_BUCK_ADJUST
            /* set VPROC to 0.975V */
            g_cur_vol = UPMU_VOLT_0_9_7_5_V;
            DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x00));
            #endif
        }
        else
        {
            dprintk("target frequency not support, ramp up to the highest level\n");
            
            #ifdef MTK_BUCK_ADJUST
            /* set VPROC to1.250V */
            g_cur_vol = UPMU_VOLT_1_2_5_0_V;
            DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x03));
            
            mb();
            udelay(PMIC_SETTLE_TIME);
            #endif
            
            /* set ARMPLL to 1001.00Mhz */
            g_cur_freq = DVFS_F1;
            DRV_WriteReg32(TOP_CKDIV1, 0x00);
        }
        
        dprintk("VPROC = %d, ARMPLL_CON0 = 0x%x, TOP_CKDIV1 = 0x%x\n", g_cur_vol, DRV_Reg32(ARMPLL_CON0), DRV_Reg32(TOP_CKDIV1));
    }
}

/**************************************
* check if maximum frequency is needed
***************************************/
static int mtk_cpufreq_keep_max_freq(void)
{
    /* if usb connected, keep maximum frequency */
    if ((DRV_Reg32(UPLL_CON0) & 0x1) == 0)
        return 1;
    
    if (mtk_cpufreq_pause)
        return 1;
    
    return 0;
}

/**********************************
* cpufreq target callback function
***********************************/
/*************************************************
* [note]
* 1. handle frequency change request
* 2. call mtk_cpufreq_set to set target frequency
**************************************************/
static int mtk_cpufreq_target(struct cpufreq_policy *policy, unsigned int target_freq, unsigned int relation)
{
    struct mtk_cpu_freq_info *next;
    struct cpufreq_freqs freqs;
    unsigned long flags;
    int idx;
    
    if (!mtk_cpufreq_ready)
        return -ENOSYS;
    
    if (policy->cpu != 0)
        return -EINVAL;
    
    /******************************
    * look up the target frequency
    *******************************/
    if (cpufreq_frequency_table_target(policy, mtk_cpu_freqs_table, target_freq, relation, &idx))
        return -EINVAL;
    
    #ifdef MTK_DVFS_RANDOM_TEST
    idx = (jiffies & 0xF) / 4;
    #endif
    
    if (get_chip_eco_ver() == CHIP_E1)
        next = &mtk_freqs_e1[idx];
    else
        next = &mtk_freqs_e2[idx];
    
    freqs.old = policy->cur;
    freqs.new = next->cpufreq_mhz;
    freqs.cpu = policy->cpu;
    
    #ifdef MTK_DVFS_RANDOM_TEST
    dprintk("idx = %d, freqs.old = %d, freqs.new = %d\n", idx, freqs.old, freqs.new);
    #endif
    
    #ifndef MTK_DVFS_RANDOM_TEST
    if (mtk_cpufreq_keep_max_freq())
    {
        freqs.new = DVFS_F1;
    }
    
    if (freqs.new > g_limited_freq)
    {
        dprintk("CPU frequency has been limited to %d Mhz, request %d Mhz will be limited\n", g_limited_freq / 1000, freqs.new / 1000);
        freqs.new = g_limited_freq;
    }
    
    if (freqs.new < g_limited_min_freq)
    {
        dprintk("cannot switch CPU frequency to %d Mhz due to voltage limitation\n", g_limited_min_freq / 1000);
        freqs.new = g_limited_min_freq;
    }
    #endif
    
    /************************************************
    * target frequency == existing frequency, skip it
    *************************************************/
    if (freqs.old == freqs.new)
    {
        dprintk("CPU frequency from %d MHz to %d MHz (skipped) due to same frequency\n", freqs.old / 1000, freqs.new / 1000);
        return 0;
    }
    
    cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
    
    spin_lock_irqsave(&mtk_cpufreq_lock, flags);
    
    /******************************
    * set to the target freeuency
    *******************************/
    mtk_cpufreq_set(freqs.old, freqs.new);
    
    spin_unlock_irqrestore(&mtk_cpufreq_lock, flags);
    
    cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
    
    return 0;
}

/*********************************************************
* set up frequency table and register to cpufreq subsystem
**********************************************************/
static __init int mtk_cpufreq_init(struct cpufreq_policy *policy)
{
    int ret = -EINVAL;
    
    if (policy->cpu != 0)
        return -EINVAL;
    
    /*********************************************
    * set default policy and cpuinfo, unit : Khz
    **********************************************/
    
    if (get_chip_eco_ver() == CHIP_E1)
    {
        policy->cpuinfo.min_freq = DVFS_F2;
        policy->cpuinfo.max_freq = DVFS_F1;
    }
    else
    {
        policy->cpuinfo.min_freq = DVFS_F4;
        policy->cpuinfo.max_freq = DVFS_F1;
    }
    
    /*******************************************************
    * 1 us, assumed, will be overwrited by min_sampling_rate
    ********************************************************/
    policy->cpuinfo.transition_latency = 1000;
    
    policy->cur = policy->min = policy->max = DVFS_F1;
    
    if (get_chip_eco_ver() == CHIP_E1)
        ret = setup_freqs_table(policy, ARRAY_AND_SIZE(mtk_freqs_e1));
    else
        ret = setup_freqs_table(policy, ARRAY_AND_SIZE(mtk_freqs_e2));
    
    if (ret) {
        xlog_printk(ANDROID_LOG_ERROR, "Power/DVFS", "failed to setup frequency table\n");
        return ret;
    }
    
    return 0;
}

static struct cpufreq_driver mtk_cpufreq_driver = {
    .verify = mtk_cpufreq_verify,
    .target = mtk_cpufreq_target,
    .init   = mtk_cpufreq_init,
    .get    = mtk_cpufreq_get,
    .name   = "mtk-cpufreq",
};

/*********************************
* early suspend callback function
**********************************/
void mtk_cpufreq_early_suspend(struct early_suspend *h)
{
    if (!g_enable_dvfs)
        return;
    
    #ifndef MTK_DVFS_RANDOM_TEST
    
    cpufreq_state_set(0);
    
    #ifdef MTK_BUCK_ADJUST
    /* set VPROC to 1.250V */
    g_cur_vol = UPMU_VOLT_1_2_5_0_V;
    DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x03));
    
    mb();
    udelay(PMIC_SETTLE_TIME);
    #endif
            
    /* change dvs_00 to 0.9V for low power saving */
    if (get_chip_eco_ver() != CHIP_E1)
    {
        g_limited_min_freq = DVFS_F3;
        
        upmu_buck_vosel_dvs_00(BUCK_VPROC, 0x08); // 0.9V DVS_VOL_00
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "enter early suspend - change dvs_00 to 0.9 for low power saving\n");
    }
    
    return;
    
    #endif
}

/*******************************
* late resume callback function
********************************/
void mtk_cpufreq_late_resume(struct early_suspend *h)
{
    if (!g_enable_dvfs)
        return;
    
    #ifndef MTK_DVFS_RANDOM_TEST
    
    /* change dvs_00 to 0.975V for DVFS */
    if (get_chip_eco_ver() != CHIP_E1)
    {
        upmu_buck_vosel_dvs_00(BUCK_VPROC, 0x0B); // 0.975V DVS_VOL_00
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "enter late resume - change dvs_00 to 0.975 for DVFS\n");
        
        g_limited_min_freq = DVFS_F4;
    }
    
    cpufreq_state_set(1);
    
    #endif
    
    return;
}

/************************************************
* frequency adjust interface for thermal protect
*************************************************/
/************************************************************
* parameter: target frequency (0 means no frequency limited)
*************************************************************/
void cpufreq_thermal_protect(int limited_freq)
{
    if (!g_enable_dvfs)
        return;
    
    if (limited_freq)
    {
        g_limited_freq = limited_freq;
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "lower frequency upper bound due to hot temperature protect, g_limited_freq = %d\n", g_limited_freq);
    }
    else
    {
        g_limited_freq = DVFS_F1;
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "no frequency limited due to cooling temperature, g_limited_freq = %d\n", g_limited_freq);
    }
    
    return;
}
EXPORT_SYMBOL(cpufreq_thermal_protect);

/*****************************
* set DVFS status (on/off)
******************************/
int cpufreq_state_set(int enabled)
{
    if (!g_enable_dvfs)
        return 0;
    
    /***********************************
    * use to start/stop DVFS by request
    ************************************/
    
    if (enabled)
    {
        if (!mtk_cpufreq_pause)
        {
            xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "cpufreq has already enabled\n");
            return 0;
        }
        
        /*************
        * enable DVFS
        **************/
        g_dvfs_disable_count--;
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "enable DVFS: g_dvfs_disable_count = %d\n", g_dvfs_disable_count);
        
        /***********************************************
        * enable DVFS if no any module still disable it
        ************************************************/
        if (g_dvfs_disable_count <= 0)
        {
            mtk_cpufreq_pause = false;
        }
        else
        {
            xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "someone still disable cpufreq, cannot enable it\n");
        }
    }
    else
    {
        /**************
        * disable DVFS
        ***************/
        g_dvfs_disable_count++;
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "disable DVFS: g_dvfs_disable_count = %d\n", g_dvfs_disable_count);
        
        if (mtk_cpufreq_pause)
        {
            xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "cpufreq has already disabled\n");
            return 0;
        }
        
        mtk_cpufreq_pause = true;
    }
    
    return 0;
}
EXPORT_SYMBOL(cpufreq_state_set);

/***************************
* show current DVFS stauts
****************************/
static ssize_t cpufreq_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int len = 0;
    char *p = buf;
    
    if (!mtk_cpufreq_pause)
        p += sprintf(p, "current governor is ondemand governor - enable DVFS\n");
    else
        p += sprintf(p, "current governor is userdefine governor - disable DVFS\n");
    
    len = p - buf;
    return len;
}

/************************************
* set DVFS stauts by sysfs interface
*************************************/
static ssize_t cpufreq_state_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t n)
{
    char cpufreq_mode[20];
    
    if (sscanf(buf, "%s", cpufreq_mode) == 1)
    {
        if (!strncmp(cpufreq_mode, "enable", 6))
        {
            cpufreq_state_set(1);
        }
        else if (!strncmp(cpufreq_mode, "disable", 7))
        {
            cpufreq_state_set(0);
        }
        else
        {
            xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "bad argument!! argument should be \"enable\" or \"disable\"\n");
        }
    }
    else
    {
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "bad argument!! argument should be \"enable\" or \"disable\"\n");
    }
    
    return n;
}
cpufreq_attr(cpufreq_state);

/*******************************
* show current test mode status
********************************/
static ssize_t cpufreq_freq_test_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int len = 0;
    char *p = buf;
    
    if (g_test_mode_enable)
        p += sprintf(p, "freq test mode enable, g_limited_freq = %d\n", g_limited_freq);
    else
        p += sprintf(p, "freq test mode disable\n");
    
    len = p - buf;
    return len;
}

/*******************************
* test mode for thermal protect
********************************/
static ssize_t cpufreq_freq_test_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t n)
{
    int level = 0;
    
    if (sscanf(buf, "%d", &level) == 1)
    {
        if (level == 0)
        {
            /***********************************
            * 0 means disable frequency limited
            ************************************/
            g_test_mode_enable = 0;
            cpufreq_thermal_protect(0);
            return n;
        }
        else if (level == 1)
        {
            g_test_mode_enable = 1;
            cpufreq_thermal_protect(DVFS_F1);
            return n;
        }
        else if (level == 2)
        {
            g_test_mode_enable = 1;
            cpufreq_thermal_protect(DVFS_F2);
            return n;
        }
        else if (level == 3)
        {
            g_test_mode_enable = 1;
            cpufreq_thermal_protect(DVFS_F3);
            return n;
        }
        else if (level == 4)
        {
            g_test_mode_enable = 1;
            cpufreq_thermal_protect(DVFS_F4);
            return n;
        }
        else
        {
            xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "bad argument!! argument should be 0 ~ 4 [0: restore to default setting]\n");
        }
    }
    else
    {
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "bad argument!! argument should be 0 ~ 4 [0: restore to default setting]\n");
    }
    
    return -EINVAL;
}
cpufreq_attr(cpufreq_freq_test);

/***************************
* show current debug status
****************************/
static ssize_t cpufreq_debug_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int len = 0;
    char *p = buf;
    
    if (mtk_cpufreq_debug)
        p += sprintf(p, "cpufreq debug enabled\n");
    else
        p += sprintf(p, "cpufreq debug disabled\n");
    
    len = p - buf;
    return len;
}

/***********************
* enable debug message
************************/
static ssize_t cpufreq_debug_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t n)
{
    int debug = 0;
    
    if (sscanf(buf, "%d", &debug) == 1)
    {
        if (debug == 0) 
        {
            mtk_cpufreq_debug = 0;
            return n;
        }
        else if (debug == 1)
        {
            mtk_cpufreq_debug = 1;
            return n;
        }
        else
        {
            xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "bad argument!! argument should be 0 or 1 [0: disable, 1: enable]\n");
        }
    }
    else
    {
        xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "bad argument!! argument should be 0 or 1 [0: disable, 1: enable]\n");
    }
    
    return -EINVAL;
}
cpufreq_attr(cpufreq_debug);

/*******************************************
* cpufrqe platform driver callback function
********************************************/
static int cpufreq_pdrv_probe(struct platform_device *pdev)
{
    kal_uint32 ret;
    
    #ifdef CONFIG_HAS_EARLYSUSPEND
    mtk_cpufreq_early_suspend_handler.suspend = mtk_cpufreq_early_suspend;
    mtk_cpufreq_early_suspend_handler.resume = mtk_cpufreq_late_resume;
    register_early_suspend(&mtk_cpufreq_early_suspend_handler);
    #endif
    
    xlog_printk(ANDROID_LOG_INFO, "Power/DVFS", "mediatek cpufreq initialized\n");
    
    /************************************************
    * voltage scaling need to wait PMIC driver ready
    *************************************************/
    mtk_cpufreq_ready = true;
    
    ret = sysfs_create_file(power_kobj, &cpufreq_state_attr.attr);
    ret = sysfs_create_file(power_kobj, &cpufreq_freq_test_attr.attr);
    ret = sysfs_create_file(power_kobj, &cpufreq_debug_attr.attr);
    
    return cpufreq_register_driver(&mtk_cpufreq_driver);
}

/***************************************
* this function should never be called
****************************************/
static int cpufreq_pdrv_remove(struct platform_device *pdev)
{
    return 0;
}

static struct platform_driver mtk_cpufreq_pdrv = {
    .probe      = cpufreq_pdrv_probe,
    .remove     = cpufreq_pdrv_remove,
    .suspend    = NULL,
    .resume     = NULL,
    .driver     = {
        .name   = "mtk-cpufreq",
        .owner  = THIS_MODULE,
    },
};

/*******************************************
 * Add for CPU 26Mhz <-> 1Ghz stress begin
 *******************************************/
enum hrtimer_restart cpu_ss_timer_func(struct hrtimer *timer)
{
    cpu_ss_dprintk("[%s]: enter timer function\n", __FUNCTION__);
    
    cpu_ss_timer_flag = 1; wake_up_interruptible(&cpu_ss_timer_waiter);
    
    return HRTIMER_NORESTART;
}

int cpu_ss_thread_handler(void *unused)
{
    kal_uint32 flag = 0;
    
    do
    {
        ktime_t ktime = ktime_set(cpu_ss_period_s, cpu_ss_period_ns);
        
        wait_event_interruptible(cpu_ss_timer_waiter, cpu_ss_timer_flag != 0);
        cpu_ss_timer_flag = 0;
        
        if (!flag)
        {
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) & 0x01ff));
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) & 0x0ff3));
            flag = 1;
        }
        else
        {
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) | 0x0008));
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) | 0x0200));
            flag = 0;
        }
        
        cpu_ss_dprintk("[%s]: CLKMUX = 0x%x\n", __FUNCTION__, DRV_Reg32(TOP_CKMUXSEL));
        
        hrtimer_start(&cpu_ss_timer, ktime, HRTIMER_MODE_REL);
        
    } while (!kthread_should_stop());
    
    return 0;
}

static int cpu_ss_mode_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    
    if ((DRV_Reg32(TOP_CKMUXSEL) & 0x000C) == 0)
        p += sprintf(p, "CA9 runs at 26Mhz\n");
    else
        p += sprintf(p, "CA9 runs at 1001Mhz\n");
        
    *start = buf + off;
    
    len = p - buf;
    if (len > off)
        len -= off;
    else
        len = 0;

    return len < count ? len  : count;
}

static ssize_t cpu_ss_mode_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len = 0, mode = 0;
    char desc[32];

    len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
    if (copy_from_user(desc, buffer, len))
    {
        return 0;
    }
    desc[len] = '\0';
    
    printk("\n=============== CPU Speed Switch Mode ================\n");
    if (sscanf(desc, "%d", &mode) == 1)
    {
        if (mode)
        {
            printk("  config cpu speed switch mode = 1001Mhz\n");
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) | 0x0008));
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) | 0x0200));
        }
        else
        {
            printk("  config cpu speed switch mode = 26Mhz\n");
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) & 0x01ff));
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) & 0x0ff3));
        }
        
        printk("==========================================\n");
        return count;
    }
    else
    {
        printk("  bad argument!!\n");
        printk(  "==========================================\n");
    }
    
    return -EINVAL;
}


static int cpu_ss_period_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    
    p += sprintf(p, "%d (s) %d (ns)\n", cpu_ss_period_s, cpu_ss_period_ns);

    *start = buf + off;
    
    len = p - buf;
    if (len > off)
        len -= off;
    else
        len = 0;

    return len < count ? len  : count;
}

static ssize_t cpu_ss_period_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len = 0, s = 0, ns = 0;
    char desc[32];

    len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
    if (copy_from_user(desc, buffer, len))
    {
        return 0;
    }
    desc[len] = '\0';
    
    printk("\n=============== CPU Speed Switch Period Mode ================\n");
    if (sscanf(desc, "%d %d", &s, &ns) == 2)
    {
        printk("  config cpu speed switch period = %d (s), %d (ns)\n", s, ns);
        cpu_ss_period_s = s;
        cpu_ss_period_ns = ns;
        printk("==========================================\n");
        return count;
    }
    else
    {
        printk("  bad argument!!\n");
        printk(  "==========================================\n");
    }
    
    return -EINVAL;
}

static int cpu_ss_period_mode_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    
    if (cpu_ss_period_mode)
        p += sprintf(p, "enable");
    else
        p += sprintf(p, "disable");

    *start = buf + off;
    
    len = p - buf;
    if (len > off)
        len -= off;
    else
        len = 0;

    return len < count ? len  : count;
}

static ssize_t cpu_ss_period_mode_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len = 0;
    char mode[20], desc[32];
    ktime_t ktime = ktime_set(cpu_ss_period_s, cpu_ss_period_ns);
    
    len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
    if (copy_from_user(desc, buffer, len)) 
    {
        return 0;
    }
    desc[len] = '\0';
        
    printk("\n=============== CPU Speed Switch Period Mode ================\n");
    if (sscanf(desc, "%s", mode) == 1)
    {
        if (!strcmp(mode, "enable"))
        {
            printk("  enable cpu speed switch period mode\n");
            cpu_ss_period_mode = true;
                    
            cpu_ss_thread = kthread_run(cpu_ss_thread_handler, 0, "cpu speed switch");
            if (IS_ERR(cpu_ss_thread))
            {
                printk("[%s]: failed to create cpu speed switch thread\n", __FUNCTION__);
            }
    
            hrtimer_start(&cpu_ss_timer, ktime, HRTIMER_MODE_REL);
            printk("==========================================\n");
            return count;
        }
        else if (!strcmp(mode, "disable"))
        {
            printk("  disable cpu speed switch period mode\n");
            cpu_ss_period_mode = false;
            
            kthread_stop(cpu_ss_thread);
            
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) | 0x0008));
            DRV_WriteReg32(TOP_CKMUXSEL, (DRV_Reg32(TOP_CKMUXSEL) | 0x0200));
            
            cpu_ss_dprintk("[%s]: CLKMUX = 0x%x\n", __FUNCTION__, DRV_Reg32(TOP_CKMUXSEL));
            
            hrtimer_cancel(&cpu_ss_timer);
            printk("==========================================\n");
            return count;
        }
        else
        {
            printk("  bad argument!! argument should be \"enable\" or \"disable\"\n");
            printk(  "==========================================\n");
        }
    }
    else
    {
        printk("  bad argument!!\n");
        printk(  "==========================================\n");
    }
    
    return -EINVAL;
}

static int cpu_ss_debug_mode_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    
    if (cpu_ss_debug_mode)
        p += sprintf(p, "enable");
    else
        p += sprintf(p, "disable");

    *start = buf + off;
    
    len = p - buf;
    if (len > off)
        len -= off;
    else
        len = 0;

    return len < count ? len  : count;
}

static ssize_t cpu_ss_debug_mode_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len = 0;
    char mode[20], desc[32];
    
    len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
    if (copy_from_user(desc, buffer, len)) 
    {
        return 0;
    }
    desc[len] = '\0';
        
    printk("\n=============== CPU Speed Switch Debug Mode ================\n");
    if (sscanf(desc, "%s", mode) == 1)
    {
        if (!strcmp(mode, "enable"))
        {
            printk("  enable cpu speed switch debug mode\n");
            cpu_ss_debug_mode = true;
            printk("==========================================\n");
            return count;
        }
        else if (!strcmp(mode, "disable"))
        {
            printk("  disable cpu speed switch debug mode\n");
            cpu_ss_debug_mode = false;
            printk("==========================================\n");
            return count;
        }
        else
        {
            printk("  bad argument!! argument should be \"enable\" or \"disable\"\n");
            printk(  "==========================================\n");
        }
    }
    else
    {
        printk("  bad argument!!\n");
        printk(  "==========================================\n");
    }
    
    return -EINVAL;
}
/*******************************************
 * Add for CPU 26Mhz <-> 1Ghz stress end
 *******************************************/

/***********************************************************
* cpufreq initialization to register cpufreq platform driver
************************************************************/
static int __init cpufreq_init(void)
{
    int ret = 0;
    
    /*******************************************
     * Add for CPU 26Mhz <-> 1Ghz stress begin 
     *******************************************/
    struct proc_dir_entry *entry = NULL;
    struct proc_dir_entry *cpu_ss_dir = NULL;
    
    hrtimer_init(&cpu_ss_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    cpu_ss_timer.function = cpu_ss_timer_func;
    
    cpu_ss_dir = proc_mkdir("cpu_ss", NULL);
    if (!cpu_ss_dir) 
    {
        pr_err("[%s]: mkdir /proc/cpu_ss failed\n", __FUNCTION__);
    } 
    else 
    {
        entry = create_proc_entry("cpu_ss_debug_mode", S_IRUGO | S_IWUGO, cpu_ss_dir);
        if (entry) 
        {
            entry->read_proc = cpu_ss_debug_mode_read;
            entry->write_proc = cpu_ss_debug_mode_write;
        }
        
        entry = create_proc_entry("cpu_ss_period_mode", S_IRUGO | S_IWUGO, cpu_ss_dir);
        if (entry) 
        {
            entry->read_proc = cpu_ss_period_mode_read;
            entry->write_proc = cpu_ss_period_mode_write;
        }
        
        entry = create_proc_entry("cpu_ss_period", S_IRUGO | S_IWUGO, cpu_ss_dir);
        if (entry) 
        {
            entry->read_proc = cpu_ss_period_read;
            entry->write_proc = cpu_ss_period_write;
        }
        
        entry = create_proc_entry("cpu_ss_mode", S_IRUGO | S_IWUGO, cpu_ss_dir);
        if (entry) 
        {
            entry->read_proc = cpu_ss_mode_read;
            entry->write_proc = cpu_ss_mode_write;
        }
    }
    /*******************************************
     * Add for CPU 26Mhz <-> 1Ghz stress end 
     *******************************************/
    
    ret = platform_driver_register(&mtk_cpufreq_pdrv);
    if (ret)
    {
        xlog_printk(ANDROID_LOG_ERROR, "Power/DVFS", "failed to register cpufreq driver\n");
        return ret;
    }
    else
    {
        xlog_printk(ANDROID_LOG_ERROR, "Power/DVFS", "cpufreq driver registration done\n");
        return 0;
    }
}
module_init(cpufreq_init);

static void __exit cpufreq_exit(void)
{
    cpufreq_unregister_driver(&mtk_cpufreq_driver);
}
module_exit(cpufreq_exit);

MODULE_DESCRIPTION("MediaTek CPU Frequency Scaling driver");
MODULE_LICENSE("GPL");
