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
 *   hal_types.h 
 *
 * @par Project:
 *   MFlexVideo 
 *
 * @par Description:
 *   Hardware Abstraction Layer Type Definitions
 *
 * @par Author:
 *   Jackal Chen (mtk02532)
 *
 * @par $Revision: #4 $
 * @par $Modtime:$
 * @par $Log:$
 *
 */

#ifndef _HAL_TYPES_H_
#define _HAL_TYPES_H_
 
#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @par Structure
 *  HAL_POWER_T
 * @par Description
 *  This is a parameter for eVideoHwPowerCtrl()
 */
typedef struct _HAL_POWER_T
{
    VAL_VOID_T *pvHandle;                ///< [IN]     The video codec driver handle
    VAL_UINT32_T u4HandleSize;           ///< [IN]     The size of video codec driver handle
    VAL_DRIVER_TYPE_T eDriverType;       ///< [IN]     The driver type
    VAL_BOOL_T  fgEnable;                ///< [IN]     Enable or not.
    VAL_VOID_T *pvReserved;              ///< [IN/OUT] The reserved parameter
    VAL_UINT32_T u4ReservedSize;         ///< [IN]     The size of reserved parameter structure
} HAL_POWER_T;

/**
 * @par Structure
 *  HAL_ISR_T
 * @par Description
 *  This is a parameter for eVideoHwPowerCtrl()
 */
typedef struct _HAL_ISR_T
{
    VAL_VOID_T *pvHandle;                ///< [IN]     The video codec driver handle
    VAL_UINT32_T u4HandleSize;           ///< [IN]     The size of video codec driver handle
    VAL_DRIVER_TYPE_T eDriverType;       ///< [IN]     The driver type
    VAL_BOOL_T  fgRegister;              ///< [IN]     Register or un-register.
    VAL_VOID_T *pvReserved;              ///< [IN/OUT] The reserved parameter
    VAL_UINT32_T u4ReservedSize;         ///< [IN]     The size of reserved parameter structure
} HAL_ISR_T;

/**
 * @par Structure
 *  HAL_CLOCK_T
 * @par Description
 *  This is a parameter for eVideoHwEnableClock() and eVideoHwDisableClock()
 */
typedef struct _HAL_CLOCK_T
{
    VAL_VOID_T *pvHandle;                ///< [IN]     The video codec driver handle
    VAL_UINT32_T u4HandleSize;           ///< [IN]     The size of video codec driver handle
    VAL_DRIVER_TYPE_T eDriverType;       ///< [IN]     The driver type
    VAL_VOID_T *pvReserved;              ///< [IN/OUT] The reserved parameter
    VAL_UINT32_T u4ReservedSize;         ///< [IN]     The size of reserved parameter structure
} HAL_CLOCK_T;

#ifdef __cplusplus
}
#endif

#endif // #ifndef _HAL_TYPES_H_
