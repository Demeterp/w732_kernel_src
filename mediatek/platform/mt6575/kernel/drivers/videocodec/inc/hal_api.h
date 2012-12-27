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

/** 
 * @file 
 *   hal_api.h 
 *
 * @par Project:
 *   MFlexVideo 
 *
 * @par Description:
 *   Hardware Abstraction Layer APIs
 *
 * @par Author:
 *   Jackal Chen (mtk02532)
 *
 * @par $Revision: #9 $
 * @par $Modtime:$
 * @par $Log:$
 *
 */

#ifndef _HAL_API_H_
#define _HAL_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "val_types.h"
#include "hal_types.h"
#include "mfv_drv_base.h"


typedef enum
{     
    HAL_CMD_SET_CMD_QUEUE,
    HAL_CMD_SET_POWER,
    HAL_CMD_SET_ISR,
    HAL_CMD_GET_CACHE_CTRL_ADDR,
    HAL_CMD_MAX = 0xFFFFFFFF
} HAL_CMD_T;

/*=============================================================================
 *                             Function Declaration
 *===========================================================================*/
/**
* @par Function       
*   eHalInit
* @par Description    
*   Init HAL
* @param              
*   *a_phHalHandle         [IN/OUT]   The VAL_HANDLE_T structure
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eHalInit(
    VAL_HANDLE_T    *a_phHalHandle
    );
/**
* @par Function       
*   eHalDeInit
* @par Description    
*   DeInit HAL
* @param              
*   *a_phHalHandle         [IN/OUT]   The VAL_HANDLE_T structure
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eHalDeInit(
    VAL_HANDLE_T    *a_phHalHandle
    );
/**
* @par Function       
*   eHalCmdProc
* @par Description    
*   set command to MFV HW
* @param              
*   a_prParam         [IN]   The VAL_HANDLE_T structure
* @param              
*   a_prParam         [IN]   The HAL_CMD_T structure
* @param              
*   a_prParam         [IN]   The VAL_VOID_T structure
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eHalCmdProc(
    VAL_HANDLE_T    a_hHalHandle,
    HAL_CMD_T       a_eHalCmd,
    VAL_VOID_T      *a_pvInParam,
    VAL_VOID_T      *a_pvOutParam
    );    



#ifdef __cplusplus
}
#endif

#endif // #ifndef _HAL_API_H_
