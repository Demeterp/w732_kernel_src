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


/*! \file
    \brief  Declaration of library functions

    Any definitions in this file will be shared among GLUE Layer and internal Driver Stack.
*/

/*******************************************************************************
* Copyright (c) 2009 MediaTek Inc.
*
* All rights reserved. Copying, compilation, modification, distribution
* or any other use whatsoever of this material is strictly prohibited
* except in accordance with a Software License Agreement with
* MediaTek Inc.
********************************************************************************
*/

/*******************************************************************************
* LEGAL DISCLAIMER
*
* BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK
* SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE
* PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY
* DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE
* ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
* WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE
* FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR TO
* CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
* LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL
* BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT
* ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY
* BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
* WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT
* OF LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING
* THEREOF AND RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN
* FRANCISCO, CA, UNDER THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE
* (ICC).
********************************************************************************
*/


/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/


/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/
#include "osal_typedef.h"

#include "mtk_wcn_cmb_hw.h"
#include "wmt_plat.h"
#include "wmt_lib.h"


/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/



/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/



/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

PWR_SEQ_TIME gPwrSeqTime;




/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/



/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/

INT32 wmt_cmb_hw_pwr_off(VOID)
{
    INT32 iRet = 0;
    WMT_INFO_FUNC("WMT-CMB, hw_pwr_off start\n");

/*1. disable irq --> should be done when do wmt-ic swDeinit period*/

/*2. set bgf eint/all eint to deinit state, namely input low state*/
//    wmt_plat_gpio_ctrl(PIN_ALL_EINT,  PIN_STA_DEINIT);
    iRet += wmt_plat_gpio_ctrl(PIN_BGF_EINT,  PIN_STA_DEINIT);

/*3. set audio interface to CMB_STUB_AIF_0, BT PCM OFF, I2S OFF*/
    iRet += wmt_plat_audio_ctrl(CMB_STUB_AIF_0, CMB_STUB_AIF_CTRL_DIS);

/*4. set control gpio into deinit state, namely input low state*/
    iRet += wmt_plat_gpio_ctrl(PIN_LDO,       PIN_STA_DEINIT);
    iRet += wmt_plat_gpio_ctrl(PIN_PMU,       PIN_STA_DEINIT);
    iRet += wmt_plat_gpio_ctrl(PIN_RST,       PIN_STA_DEINIT);

/*5. set uart tx/rx into deinit state, namely input low state*/
    iRet += wmt_plat_gpio_ctrl(PIN_UART_GRP,  PIN_STA_DEINIT);

    iRet += wmt_plat_gpio_ctrl(PIN_GPS_LNA,  PIN_STA_DEINIT);
    iRet += wmt_plat_gpio_ctrl(PIN_GPS_SYNC,  PIN_STA_DEINIT);
    WMT_INFO_FUNC("WMT-CMB, hw_pwr_off finish\n");
    return iRet;
}



INT32 wmt_cmb_hw_pwr_on(VOID)
{
    static UINT32 _pwr_first_time = 0;
    INT32 iRet = 0;
    WMT_INFO_FUNC("WMT-CMB, hw_pwr_on start\n");
/*set all control and eint gpio to init state, namely input low mode*/
    iRet += wmt_plat_gpio_ctrl(PIN_LDO,       PIN_STA_INIT);
    iRet += wmt_plat_gpio_ctrl(PIN_PMU,       PIN_STA_INIT);
    iRet += wmt_plat_gpio_ctrl(PIN_RST,       PIN_STA_INIT);
    iRet += wmt_plat_gpio_ctrl(PIN_BGF_EINT,  PIN_STA_INIT);
    iRet += wmt_plat_gpio_ctrl(PIN_GPS_LNA,  PIN_STA_INIT);
    iRet += wmt_plat_gpio_ctrl(PIN_GPS_SYNC,  PIN_STA_INIT);
//    wmt_plat_gpio_ctrl(PIN_WIFI_EINT, PIN_STA_INIT);
//    wmt_plat_gpio_ctrl(PIN_ALL_EINT,  PIN_STA_INIT);

/*1. pull high LDO to supply power to chip*/
    iRet += wmt_plat_gpio_ctrl(PIN_LDO,       PIN_STA_OUT_H);
    osal_msleep(gPwrSeqTime.ldoStableTime);

/*2. export RTC clock to chip*/
    if (0 == _pwr_first_time)
    {
    /*rtc clock should be output all the time, so no need to enable output again*/
        iRet += wmt_plat_gpio_ctrl(PIN_RTC,       PIN_STA_INIT);
        osal_msleep(gPwrSeqTime.rtcStableTime);
        _pwr_first_time = 1;
        WMT_INFO_FUNC("WMT-CMB, rtc clock exported\n");
    }

/*3. set UART Tx/Rx to UART mode*/
    iRet += wmt_plat_gpio_ctrl(PIN_UART_GRP,  PIN_STA_INIT);

/*4. PMU->output low, RST->output low, sleep off stable time*/
    iRet += wmt_plat_gpio_ctrl(PIN_PMU,       PIN_STA_OUT_L);
    iRet += wmt_plat_gpio_ctrl(PIN_RST,       PIN_STA_OUT_L);

/*5. PMU->output high, sleep rst stable time*/
    iRet += wmt_plat_gpio_ctrl(PIN_PMU,       PIN_STA_OUT_H);
    osal_msleep(gPwrSeqTime.rstStableTime);

/*6. RST->output high, sleep on stable time*/
    iRet += wmt_plat_gpio_ctrl(PIN_RST,       PIN_STA_OUT_H);
    osal_msleep(gPwrSeqTime.onStableTime);

/*7. set audio interface to CMB_STUB_AIF_1, BT PCM ON, I2S OFF*/
    iRet += wmt_plat_audio_ctrl(CMB_STUB_AIF_1, CMB_STUB_AIF_CTRL_DIS);

/*8. set EINT< -ommited-> move this to WMT-IC module, where common sdio interface will be identified and do proper operation*/

    WMT_INFO_FUNC("WMT-CMB, hw_pwr_on finish\n");
    return iRet;

}

INT32 wmt_cmb_hw_rst(VOID)
{
    WMT_INFO_FUNC("WMT-CMB, hw_rst start, eirq should be disabled before this step\n");
    wmt_plat_gpio_ctrl(PIN_RST,       PIN_STA_OUT_L);
    osal_msleep(gPwrSeqTime.rstStableTime);
    wmt_plat_gpio_ctrl(PIN_RST,       PIN_STA_OUT_H);
    WMT_INFO_FUNC("WMT-CMB, hw_rst finish, eirq should be enabled after this step\n");
    return 0;
}

static VOID wmt_cmb_hw_dmp_seq (VOID)
{
    PUINT32 pTimeSlot = (PUINT32)&gPwrSeqTime;
    WMT_INFO_FUNC("combo chip power on sequence time, RTC (%d), LDO (%d),RST(%d),  OFF(%d), ON(%d)\n", *pTimeSlot++, *pTimeSlot++, *pTimeSlot++, *pTimeSlot++, *pTimeSlot++);
    return;
}

INT32 wmt_cmb_hw_init(P_PWR_SEQ_TIME pPwrSeqTime)
{
    if (NULL == pPwrSeqTime)
    {
        WMT_WARN_FUNC("pPwrSeqTime = NULL, use default hw init sequence parameters\n");
        gPwrSeqTime.ldoStableTime = DFT_LDO_STABLE_TIME;
        gPwrSeqTime.offStableTime = DFT_OFF_STABLE_TIME;
        gPwrSeqTime.onStableTime = DFT_ON_STABLE_TIME;
        gPwrSeqTime.rstStableTime = DFT_RST_STABLE_TIME;
        gPwrSeqTime.rtcStableTime = DFT_RTC_STABLE_TIME;
    }
    else
    {
    #if 0
        gPwrSeqTime.ldoStableTime = pPwrSeqTime->ldoStableTime;
        gPwrSeqTime.offStableTime = pPwrSeqTime->offStableTime;
        gPwrSeqTime.onStableTime = pPwrSeqTime->onStableTime;
        gPwrSeqTime.rstStableTime = pPwrSeqTime->rstStableTime;
        gPwrSeqTime.rtcStableTime = pPwrSeqTime->rtcStableTime;
    #endif
    /*memcpy may be more performance*/
        WMT_DBG_FUNC("setting hw init sequence parameters\n");
        osal_memcpy(&gPwrSeqTime, pPwrSeqTime, osal_sizeof(gPwrSeqTime));
    }
    wmt_cmb_hw_dmp_seq();
    return 0;
}

INT32 wmt_cmb_hw_deinit(VOID)
{

    WMT_WARN_FUNC("wmt_cmb_hw_deinit start, set to default hw init sequence parameters\n");
    gPwrSeqTime.ldoStableTime = DFT_LDO_STABLE_TIME;
    gPwrSeqTime.offStableTime = DFT_OFF_STABLE_TIME;
    gPwrSeqTime.onStableTime = DFT_ON_STABLE_TIME;
    gPwrSeqTime.rstStableTime = DFT_RST_STABLE_TIME;
    gPwrSeqTime.rtcStableTime = DFT_RTC_STABLE_TIME;
    WMT_WARN_FUNC("wmt_cmb_hw_deinit finish\n");
    return 0;
}







