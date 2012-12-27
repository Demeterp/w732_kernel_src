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
//----------------------------------------------------------------------------
#ifndef MT6575_RES_MGR_H
#define MT6575_RES_MGR_H
//----------------------------------------------------------------------------
#define RES_MGR_DEV_NAME    "Res_Mgr"
#define RES_MGR_MAGIC_NO    'p'
//----------------------------------------------------------------------------
#define RES_MGR_LOG_MSG     0x00000001
#define RES_MGR_LOG_WRN     0x00000002
#define RES_MGR_LOG_ERR     0x00000004
//----------------------------------------------------------------------------
#define RES_MGR_RES_ISP     0x00000001
//----------------------------------------------------------------------------
typedef struct
{
    unsigned long   ResMask;
    unsigned long   PermanentMask;
    unsigned long   Timeout;
}RES_MGR_RES_LOCK_STRUCT;

typedef struct
{
    unsigned long   ResLockMask;
    unsigned long   PermanentMask;
}RES_MGR_RES_CHECK_STRUCT;
//----------------------------------------------------------------------------
typedef enum
{
    RES_MGR_CMD_RES_LOCK,
    RES_MGR_CMD_RES_UNLOCK,
    RES_MGR_CMD_RES_CHECK,
    RES_MGR_CMD_RES_LOG_ENABLE,
    RES_MGR_CMD_RES_LOG_DISABLE
}RES_MGR_CMD_ENUM;
//----------------------------------------------------------------------------
#define RES_MGR_RES_LOCK        _IOW(RES_MGR_MAGIC_NO, RES_MGR_CMD_RES_LOCK,        RES_MGR_RES_LOCK_STRUCT)
#define RES_MGR_RES_UNLOCK      _IOW(RES_MGR_MAGIC_NO, RES_MGR_CMD_RES_UNLOCK,      unsigned long)
#define RES_MGR_RES_CHECK       _IOR(RES_MGR_MAGIC_NO, RES_MGR_CMD_RES_CHECK,       RES_MGR_RES_CHECK_STRUCT)
#define RES_MGR_LOG_ENABLE      _IOR(RES_MGR_MAGIC_NO, RES_MGR_CMD_RES_LOG_ENABLE,  unsigned long)
#define RES_MGR_LOG_DISABLE     _IOR(RES_MGR_MAGIC_NO, RES_MGR_CMD_RES_LOG_DISABLE, unsigned long)
//----------------------------------------------------------------------------
#endif
