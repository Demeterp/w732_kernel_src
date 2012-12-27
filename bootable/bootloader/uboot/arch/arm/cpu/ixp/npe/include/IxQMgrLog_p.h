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
 * @file    IxQMgrLog_p.h
 *
 * @author Intel Corporation
 * @date    07-Feb-2002
 *
 * @brief   This file contains the internal functions for config
 *
 * 
 * @par
 * IXP400 SW Release version 2.0
 * 
 * -- Copyright Notice --
 * 
 * @par
 * Copyright 2001-2005, Intel Corporation.
 * All rights reserved.
 * 
 * @par
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * @par
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * @par
 * -- End of Copyright Notice --
*/

#ifndef IXQMGRLOG_P_H
#define IXQMGRLOG_P_H

/*
 * User defined header files
 */
#include "IxOsal.h"

/*
 * Macros
 */

#define IX_QMGR_LOG0(string) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_USER, IX_OSAL_LOG_DEV_STDOUT, string, 0, 0, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG1(string, arg1) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_USER, IX_OSAL_LOG_DEV_STDOUT, string, (int)arg1, 0, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG2(string, arg1, arg2) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_USER, IX_OSAL_LOG_DEV_STDOUT, string, (int)arg1, (int)arg2, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG3(string, arg1, arg2, arg3) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_USER, IX_OSAL_LOG_DEV_STDOUT, string, (int)arg1, (int)arg2, (int)arg3, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG6(string, arg1, arg2, arg3, arg4, arg5, arg6) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_USER, IX_OSAL_LOG_DEV_STDOUT, string, (int)arg1, (int)arg2, (int)arg3, (int)arg4, (int)arg5, (int)arg6); \
}while(0);

#define IX_QMGR_LOG_WARNING0(string) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_WARNING, IX_OSAL_LOG_DEV_STDOUT, string, 0, 0, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG_WARNING1(string, arg1) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_WARNING, IX_OSAL_LOG_DEV_STDOUT, string, (int)arg1, 0, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG_WARNING2(string, arg1, arg2) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_WARNING, IX_OSAL_LOG_DEV_STDOUT, string, (int)arg1, (int)arg2, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG_ERROR0(string) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_ERROR, IX_OSAL_LOG_DEV_STDERR, string, 0, 0, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG_ERROR1(string, arg1) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_ERROR, IX_OSAL_LOG_DEV_STDERR, string, (int)arg1, 0, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG_ERROR2(string, arg1, arg2) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_ERROR, IX_OSAL_LOG_DEV_STDERR, string, (int)arg1, (int)arg2, 0, 0, 0, 0);\
}while(0);

#define IX_QMGR_LOG_ERROR3(string, arg1, arg2, arg3) do\
{\
    ixOsalLog(IX_OSAL_LOG_LVL_ERROR, IX_OSAL_LOG_DEV_STDERR, string, (int)arg1, (int)arg2, (int)arg3, 0, 0, 0);\
}while(0);
#endif /* IX_QMGRLOG_P_H */




