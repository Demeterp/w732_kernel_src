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
 * @file IxNpeMhSend_p.h
 *
 * @author Intel Corporation
 * @date 18 Jan 2002
 *
 * @brief This file contains the private API for the Send module.
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

/**
 * @defgroup IxNpeMhSend_p IxNpeMhSend_p
 *
 * @brief The private API for the Send module.
 * 
 * @{
 */

#ifndef IXNPEMHSEND_P_H
#define IXNPEMHSEND_P_H

#include "IxNpeMh.h"
#include "IxOsalTypes.h"

/*
 * #defines for function return types, etc.
 */

/*
 * Prototypes for interface functions.
 */

/**
 * @fn IX_STATUS ixNpeMhSendMessageSend (
           IxNpeMhNpeId npeId,
           IxNpeMhMessage message,
           UINT32 maxSendRetries)
 *
 * @brief This function writes a message to the specified NPE's inFIFO,
 * and must be used when the message being sent does not solicit a response
 * from the NPE. This function will return TIMEOUT status if NPE hang / halt.
 *
 * @param IxNpeMhNpeId npeId (in) - The ID of the NPE to send the message
 * to.
 * @param IxNpeMhMessage message (in) - The message to send.
 * @param UINT32 maxSendRetries (in) - Max num. of retries to perform
 * if the NPE's inFIFO is full.
 *
 * @return The function returns a status indicating success, failure or timeout.
 */

IX_STATUS ixNpeMhSendMessageSend (
    IxNpeMhNpeId npeId,
    IxNpeMhMessage message,
    UINT32 maxSendRetries);

/**
 * @fn IX_STATUS ixNpeMhSendMessageWithResponseSend (
           IxNpeMhNpeId npeId,
           IxNpeMhMessage message,
           IxNpeMhMessageId solicitedMessageId,
           IxNpeMhCallback solicitedCallback,
           UINT32 maxSendRetries)
 *
 * @brief This function writes a message to the specified NPE's inFIFO,
 * and must be used when the message being sent solicits a response from
 * the NPE.  The ID of the solicited response must be specified so that it
 * can be recognised, and a callback provided to pass the response back to
 * the client. This function will return TIMEOUT status if NPE hang / halt.
 *
 * @param IxNpeMhNpeId npeId (in) - The ID of the NPE to send the message
 * to.
 * @param IxNpeMhMessage message (in) - The message to send.
 * @param IxNpeMhMessageId solicitedMessageId (in) - The ID of the
 * solicited response.
 * @param IxNpeMhCallback solicitedCallback (in) - The callback to pass the
 * solicited response back to the client.
 * @param UINT32 maxSendRetries (in) - Max num. of retries to perform
 * if the NPE's inFIFO is full.
 *
 * @return The function returns a status indicating success, failure or timeout.
 */

IX_STATUS ixNpeMhSendMessageWithResponseSend (
    IxNpeMhNpeId npeId,
    IxNpeMhMessage message,
    IxNpeMhMessageId solicitedMessageId,
    IxNpeMhCallback solicitedCallback,
    UINT32 maxSendRetries);

/**
 * @fn void ixNpeMhSendShow (
           IxNpeMhNpeId npeId)
 *
 * @brief This function will display the current state of the Send module.
 *
 * @param IxNpeMhNpeId npeId (in) - The ID of the NPE to display state
 * information for.
 *
 * @return No return value.
 */

void ixNpeMhSendShow (
    IxNpeMhNpeId npeId);

/**
 * @fn void ixNpeMhSendShowReset (
           IxNpeMhNpeId npeId)
 *
 * @brief This function will reset the current state of the Send module.
 *
 * @param IxNpeMhNpeId npeId (in) - The ID of the NPE to reset state
 * information for.
 *
 * @return No return value.
 */

void ixNpeMhSendShowReset (
    IxNpeMhNpeId npeId);

#endif /* IXNPEMHSEND_P_H */

/**
 * @} defgroup IxNpeMhSend_p
 */
