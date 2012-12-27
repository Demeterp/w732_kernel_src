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
#include <linux/io.h>
#include <linux/module.h>

#include "mach/mt6575_reg_base.h"
#include "mach/sync_write.h"
#include "mach/mt6575_ca9_power.h"

extern void arm_ca9_dynamic_gating_enable(unsigned int n);
#define SCU_CTRL        (SCU_BASE + 0x00)
#define SCU_CPU_PWR     (SCU_BASE + 0x08)
#define L2C_PWR_CTRL    (PL310_BASE + 0xF80)

static DEFINE_SPINLOCK(scu_lock);

int scu_control(const SCU_CONTROL ctrl)
{
    unsigned int iVal;
    int iRet = 0;

    spin_lock(&scu_lock);

    iVal = readl(SCU_CTRL);

    switch (ctrl) {
    case SCU_IC_STANDBY_ON:
        iVal |= 0x40;
        break;

    case SCU_IC_STANDBY_OFF:
        iVal &= ~0x40;
        break;

    case SCU_STANDBY_ON:
        iVal |= 0x20;
        break;

    case SCU_STANDBY_OFF:
        iVal &= ~0x20;
        break;

    default:
        iRet = -1;
    }

    if (iRet >= 0) {
        mt65xx_reg_sync_writel(iVal, SCU_CTRL);
    }

    spin_unlock(&scu_lock);

    return iRet;
}

int scu_set_cpu_pwr_status(const SCU_CPU_PWR_STATE state)
{
    unsigned int iVal;
    int iRet = 0;
    spin_lock(&scu_lock);

    iVal = readl(SCU_CPU_PWR) & ~0x3;

    switch (state) {
    case SCU_CPU_PWR_NORMAL:
        iVal |= SCU_CPU_PWR_NORMAL;
        break;

    case SCU_CPU_PWR_DORMANT:
        iVal |= SCU_CPU_PWR_DORMANT;
        break;

    case SCU_CPU_PWR_OFF:
        iVal |= SCU_CPU_PWR_OFF;
        break;

    default:
        iRet = -1;
    }

    if (iRet >= 0) {
        mt65xx_reg_sync_writel(iVal, SCU_CPU_PWR);
    }

    spin_unlock(&scu_lock);

    return iRet;
}

int mt6575_ca9_power_ctrl(const CA9_CLK_GATING_CTRL ctrl)
{
    if (ctrl != CA9_DYNA_CLK_GATING_DISALBE && ctrl != CA9_DYNA_CLK_GATING_ENABLE) {
        return -1;  // invalid parameter
    }

    arm_ca9_dynamic_gating_enable(ctrl);

    return 0;
}

int mt6575_l2c_power_ctrl(const L2C_POWER_CTRL ctrl)
{
    unsigned int iVal = readl(L2C_PWR_CTRL) & ~0x3;

    if (ctrl & ~(L2C_STANDBY_ENABLE | L2C_DYNA_CLK_GATING_ENABLE)) {
        return -1;  // invalid parameter
    }

    if (ctrl & L2C_STANDBY_ENABLE) {
        iVal |= 0x1;
    }

    if (ctrl & L2C_DYNA_CLK_GATING_ENABLE) {
        iVal |= 0x2;
    }

    mt65xx_reg_sync_writel(iVal, L2C_PWR_CTRL);

    return 0;
}
