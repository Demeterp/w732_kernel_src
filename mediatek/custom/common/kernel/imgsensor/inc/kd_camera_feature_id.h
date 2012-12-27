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

#ifndef _KD_CAMERA_FEATURE_ID_H_
#define _KD_CAMERA_FEATURE_ID_H_


#define ENUM_MISC_FIDS_SI(fids...)\
    FID_PRE_BEGIN_MISC_SI       = FID_PRE_BEGIN_SI, \
        fids    \
    FID_OVER_LAST_MISC_SI
#define ENUM_RAW_ONLY_FIDS_SI(fids...)\
    FID_PRE_BEGIN_RAW_ONLY_SI   = FID_OVER_LAST_MISC_SI - 1, \
        fids    \
    FID_OVER_LAST_RAW_ONLY_SI
#define ENUM_RAW_YUV_SHARED_FIDS_SI(fids...)\
    FID_PRE_BEGIN_RAW_YUV_SI    = FID_OVER_LAST_RAW_ONLY_SI - 1, \
        fids    \
    FID_OVER_LAST_RAW_YUV_SI
#define ENUM_YUV_ONLY_FIDS_SI(fids...)\
    FID_PRE_BEGIN_YUV_ONLY_SI   = FID_OVER_LAST_RAW_YUV_SI - 1, \
        fids    \
    FID_OVER_LAST_YUV_ONLY_SI
#define ENUM_MISC_FIDS_SD(fids...)\
    FID_PRE_BEGIN_MISC_SD       = FID_PRE_BEGIN_SD, \
        fids    \
    FID_OVER_LAST_MISC_SD
#define ENUM_RAW_ONLY_SD(fids...)\
    FID_PRE_BEGIN_RAW_ONLY_SD   = FID_OVER_LAST_MISC_SD - 1, \
        fids    \
    FID_OVER_LAST_RAW_ONLY_SD
#define ENUM_RAW_YUV_SHARED_FIDS_SD(fids...)\
    FID_PRE_BEGIN_RAW_YUV_SD = FID_OVER_LAST_RAW_ONLY_SD - 1, \
        fids    \
    FID_OVER_LAST_RAW_YUV_SD
#define ENUM_YUV_ONLY_SD(fids...)\
    FID_PRE_BEGIN_YUV_ONLY_SD = FID_OVER_LAST_RAW_YUV_SD - 1, \
        fids    \
    FID_OVER_LAST_YUV_ONLY_SD


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Definition of Feature ID.
//  ps: FID = 0 is reserved.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
FID_PRE_BEGIN = 0, 
  //////////////////////////////////////////////////////////////////////////////
  // Scene-Independent (SI) feature id.
  FID_PRE_BEGIN_SI = FID_PRE_BEGIN, 
  //----------------------------------------------------------------------------
    //Misc. feature id.
    ENUM_MISC_FIDS_SI           (
        FID_ZOOM_FACTOR, // Legacy symbol (Remove if possible)
        FID_AE_STROBE, 
        FID_EIS, 
        FID_ZSD,
        FID_AF_LAMP,        
    ), 
    //RAW-only feature id.
    ENUM_RAW_ONLY_FIDS_SI       (

    ), 
    //RAW-YUV-shared feature id.
    ENUM_RAW_YUV_SHARED_FIDS_SI (
        FID_SCENE_MODE, 
        FID_COLOR_EFFECT, 
        FID_CAPTURE_MODE, 
        FID_CAP_SIZE, 
        FID_PREVIEW_SIZE, 
        FID_VIDEO_PREVIEW_SIZE,         
        FID_FRAME_RATE, 
        FID_FRAME_RATE_RANGE, 
        FID_AE_FLICKER, 
        FID_FOCUS_DIST_NORMAL, 
        FID_FOCUS_DIST_MACRO, 
    ), 
    //YUV-only feature id.
    ENUM_YUV_ONLY_FIDS_SI       (

    ), 
  //----------------------------------------------------------------------------
  FID_OVER_LAST_SI = FID_OVER_LAST_YUV_ONLY_SI, 
  //////////////////////////////////////////////////////////////////////////////
  // Scene-Dependent (SD) feature id.
  FID_PRE_BEGIN_SD = FID_OVER_LAST_SI - 1, 
  //----------------------------------------------------------------------------
    //Misc. feature id.
    ENUM_MISC_FIDS_SD           (
        FID_FD_ON_OFF, 
    ), 
    //RAW-only feature id.
    ENUM_RAW_ONLY_SD            (

    ), 
    //RAW-YUV-shared feature id.
    ENUM_RAW_YUV_SHARED_FIDS_SD (
        FID_AE_SCENE_MODE, 
        FID_AE_METERING, 
        FID_AE_ISO, 
        FID_AE_EV, 
        FID_AF_MODE, 
        FID_AF_METERING, 
        FID_AWB_MODE, 
        FID_ISP_EDGE, 
        FID_ISP_HUE, 
        FID_ISP_SAT, 
        FID_ISP_BRIGHT, 
        FID_ISP_CONTRAST, 
    ), 
    //YUV-only feature id.
    ENUM_YUV_ONLY_SD            (
    
    ), 
  //----------------------------------------------------------------------------
  FID_OVER_LAST_SD = FID_OVER_LAST_YUV_ONLY_SD, 
  //////////////////////////////////////////////////////////////////////////////
FID_OVER_LAST = FID_OVER_LAST_SD, 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

} FEATURE_ID;


#endif  //_KD_CAMERA_FEATURE_ID_H_

