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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/


#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/xlog.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include "mach/irqs.h"
#include "mach/sync_write.h"
#include "mach/mt6575_reg_base.h"
#include "mach/mt6575_clock_manager.h"
#include "mach/mt6575_typedefs.h"
#include "mach/mt6575_sc.h"
#include "mach/mt6575_boot.h"
#include "mach/mt6575_ca9_power.h"
#include "mach/mt6575_dcm.h"
#include "mach/pmic_mt6329_sw.h"
#include "mach/upmu_common_sw.h"

#define HW_RESV (0xF1019100)

/****************************************************************
 * GLOBAL DEFINATION
 ****************************************************************/
UINT32 gChipVer; 


bool bCanEnDVFS = true;

/*********************************************************************
 * FUNCTION DEFINATIONS
 ********************************************************************/

#define WPLL197_FREQ 197000 // 3G PLL frequency is 197Mhz
#define CLKSQ_FREQ   26000  // Default frequency is 26Mhz

kal_uint32 mt6575_get_bus_freq(void)
{
    kal_uint32 fout = 0, fbdiv = 0, fbsel = 0, prediv = 0, postdiv = 0, ckdiv = 0;
    
    if ((DRV_Reg32(TOP_CKMUXSEL) & 0x3) == 0x0) // Using CLKSQ
    {
        fout = CLKSQ_FREQ;
    }
    else if ((DRV_Reg32(TOP_CKMUXSEL) & 0x3) == 0x2) // Using WPLL
    {
        fout = WPLL197_FREQ;
    }
    else
    {
        fbdiv = (DRV_Reg32(MAINPLL_CON0) & 0x7F00) >> 8;
        
        fbsel = (DRV_Reg32(MAINPLL_CON0) & 0x0030) >> 4;
        if (fbsel == 0)
            fbsel = 1;
        else if (fbsel == 1)
            fbsel = 2;
        else
            fbsel = 4;
        
        prediv = (DRV_Reg32(MAINPLL_CON0) & 0x00C0) >> 6;
        if (prediv == 0)
            prediv = 1;
        else if (prediv == 1)
            prediv = 2;
        else
            prediv = 4;
        
        postdiv = (DRV_Reg32(MAINPLL_CON1) & 0x0030) >> 4;
        if (postdiv == 0)
            postdiv = 1;
        else if (postdiv == 1)
            postdiv = 2;
        else
            postdiv = 4;
        
        fout = CLKSQ_FREQ * (fbdiv + 1) * fbsel / prediv / postdiv; // KHz
    }
    
    ckdiv = (DRV_Reg32(TOP_CKDIV0) & 0x00000018) >> 3;
    
    switch (ckdiv)
    {
        case 0:
            fout = fout;     // bus clock will not be divided
            break;
        case 1:
            fout = fout / 4; // bus clock will be divided by 4
            break;
        case 2:
            fout = fout / 5; // bus clock will be divided by 5
            break;
        case 3:
            fout = fout / 6; // bus clock will be divided by 6
            break;
        default:
            break; 
    }
    
    return fout;
}
EXPORT_SYMBOL(mt6575_get_bus_freq);

void mt6575_chip_dep_init(void)
{
    scu_control(SCU_IC_STANDBY_ON); // enable GIC auto gated 
    
    scu_control(SCU_STANDBY_ON); // enable SCU auto gated 
    
    mt6575_ca9_power_ctrl(CA9_DYNA_CLK_GATING_ENABLE); // enable dynamic clock gating
    
    mt6575_l2c_power_ctrl(L2C_STANDBY_ENABLE | L2C_DYNA_CLK_GATING_ENABLE); // enable standby mode and high-level dynamic clock gating
    
    scu_set_cpu_pwr_status(SCU_CPU_PWR_NORMAL); // current cpu power state
}

void mt6575_pmic_low_power_init(void)
{
    /* VPROC */
    
    upmu_buck_vosel_srclken_0(BUCK_VPROC, 0x08);    // 0.9V in sleep mode
    
    if (get_chip_eco_ver() == CHIP_E1)
    {
        upmu_buck_vosel_dvs_00(BUCK_VPROC, 0x13);   // 1.175V DVS_VOL_00
        
        if ((DRV_Reg32(HW_RESV) & (0x1 << 29)))
        {
            upmu_buck_vosel_dvs_01(BUCK_VPROC, 0x17);   // 1.250V DVS_VOL_01
        }
        else
        {
            upmu_buck_vosel_dvs_01(BUCK_VPROC, 0x16);   // 1.250V DVS_VOL_01
        }
        
        upmu_buck_vosel_dvs_10(BUCK_VPROC, 0x13);   // 1.175V DVS_VOL_10
        
        if ((DRV_Reg32(HW_RESV) & (0x1 << 29)))
        {
            upmu_buck_vosel_dvs_11(BUCK_VPROC, 0x17);   // 1.250V DVS_VOL_11
        }
        else
        {
            upmu_buck_vosel_dvs_11(BUCK_VPROC, 0x16);   // 1.250V DVS_VOL_11
        }
    }
    else
    {
        upmu_buck_vosel_dvs_00(BUCK_VPROC, 0x08);   // 0.900V DVS_VOL_00
        upmu_buck_vosel_dvs_01(BUCK_VPROC, 0x0F);   // 1.075V DVS_VOL_01
        upmu_buck_vosel_dvs_10(BUCK_VPROC, 0x13);   // 1.175V DVS_VOL_10
        
        if ((DRV_Reg32(HW_RESV) & (0x1 << 29)))
        {
            upmu_buck_vosel_dvs_11(BUCK_VPROC, 0x17);   // 1.250V DVS_VOL_11
        }
        else
        {
            upmu_buck_vosel_dvs_11(BUCK_VPROC, 0x16);   // 1.250V DVS_VOL_11
        }
    }
    
    DRV_WriteReg32(SC_AP_DVFS_CON, ((DRV_Reg32(SC_AP_DVFS_CON) & 0xFFFFFFFC) | 0x03)); // set cpu to top voltage
    
    upmu_buck_ctrl(BUCK_VPROC, 0x3); // VPROC controlled by SRCLKEN and AP_DVFS_CON1/0
    
    /* VCORE */
    
    upmu_buck_vosel(BUCK_VCORE, UPMU_VOLT_0_9_0_0_V);   // 0.9V in sleep mode
    
    /* Others */
	
    pmic_config_interface(0x8b, 0x08, 0x1f, 0x0);   // VM12_INT = 0.9V in sleep mode
    pmic_config_interface(0x8C, 0x10, 0x1F, 0x0);   // VM12_INT_LOW_BOUND
    pmic_config_interface(0x8f, 0x1, 0x1, 0x4);
    pmic_config_interface(0x90, 0x1, 0x1, 0x0);     // VM12_INT_LP_SEL HW control
    
    pmic_config_interface(0x85, 0x1, 0x1, 0x0);     // VM12_1_LP_SEL HW control
    pmic_config_interface(0x89, 0x1, 0x1, 0x0);     // VM12_2_LP_SEL HW control
    
    pmic_config_interface(0xA9, 0x1, 0x1, 0x0);     // VMC_LP_SEL HW control
    pmic_config_interface(0xAD, 0x1, 0x1, 0x0);     // VMCH_LP_SEL HW control
    
    pmic_config_interface(0xC6, 0x1, 0x1, 0x0);     // VA1_LP_SEL HW control
    
    pmic_config_interface(0xC1, 0x1, 0x1, 0x1);     // VTCXO_ON_CTRL HW control
    
    pmic_config_interface(0x4F, 0x1, 0x1, 0x6);     // BUCK clock keep 2MHz select
    
    pmic_config_interface(0x4F, 0x1, 0x1, 0x7);     // OSC10M and 2M auto select function enable
    
    pmic_config_interface(0xD1, 0x1, 0x1, 0x1);     // RG_VM12_INT_DVS_SEL = 1
}

void mt6575_power_management_init(void)
{
    #if !defined (CONFIG_MT6575_FPGA)
    gChipVer = get_chip_eco_ver(); // get chip version
    
    xlog_printk(ANDROID_LOG_INFO, "Power/PM_INIT", "Chip Version = 0x%x, Bus Frequency = %d KHz\n", gChipVer, mt6575_get_bus_freq());
    
    mt6575_chip_dep_init(); // set specific chip setting
    
    slp_mod_init(); // sleep controller init
    
    mt6575_clk_mgr_init(); // clock manager init, including clock gating init
    
    mt6575_pm_log_init(); // power mamagement log init
    
    mt6575_dcm_init(); // dcm init
    #endif
}