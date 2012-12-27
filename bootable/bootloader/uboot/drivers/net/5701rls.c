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

/******************************************************************************/
/*                                                                            */
/* Broadcom BCM5700 Linux Network Driver, Copyright (c) 2000 Broadcom         */
/* Corporation.                                                               */
/* All rights reserved.                                                       */
/*                                                                            */
/* This program is free software; you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation, located in the file LICENSE.                 */
/*                                                                            */
/* History:                                                                   */
/*                                                                            */
/******************************************************************************/

#if INCLUDE_5701_AX_FIX

#include "bcm570x_mm.h"
#include "5701rls.h"

LM_STATUS LM_LoadRlsFirmware(PLM_DEVICE_BLOCK pDevice)
{
  T3_FWIMG_INFO FwImgInfo;

  FwImgInfo.StartAddress = t3FwStartAddr;
  FwImgInfo.Text.Buffer = (PLM_UINT8)t3FwText;
  FwImgInfo.Text.Offset  = t3FwTextAddr;
  FwImgInfo.Text.Length  = t3FwTextLen;
  FwImgInfo.ROnlyData.Buffer = (PLM_UINT8)t3FwRodata;
  FwImgInfo.ROnlyData.Offset  = t3FwRodataAddr;
  FwImgInfo.ROnlyData.Length  = t3FwRodataLen;
  FwImgInfo.Data.Buffer = (PLM_UINT8)t3FwData;
  FwImgInfo.Data.Offset  = t3FwDataAddr;
  FwImgInfo.Data.Length  = t3FwDataLen;

  if (LM_LoadFirmware(pDevice,
		      &FwImgInfo,
		      T3_RX_CPU_ID | T3_TX_CPU_ID,
		      T3_RX_CPU_ID) != LM_STATUS_SUCCESS)
    {
      return LM_STATUS_FAILURE;
    }

  return LM_STATUS_SUCCESS;
}

#endif /* INCLUDE_5701_AX_FIX */
