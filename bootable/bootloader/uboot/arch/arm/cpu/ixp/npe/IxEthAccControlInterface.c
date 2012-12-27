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
 * @file IxEthAccControlInterface.c
 *
 * @author Intel Corporation
 * @date 
 *
 * @brief IX_ETH_ACC_PUBLIC wrappers for control plane functions
 *
 * Design Notes:
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

#include "IxOsal.h"
#include "IxEthAcc.h"
#include "IxEthAcc_p.h"

PUBLIC IxOsalMutex ixEthAccControlInterfaceMutex;

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortEnable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
        printf("EthAcc: (Mac) cannot enable port %d, service not initialized\n", portId);
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortEnablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortDisable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    /* check the context is iinitialized */
    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortDisablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortEnabledQuery(IxEthAccPortId portId, BOOL *enabled)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortEnabledQueryPriv(portId, enabled);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortPromiscuousModeClear(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortPromiscuousModeClearPriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortPromiscuousModeSet(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortPromiscuousModeSetPriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortUnicastMacAddressSet(IxEthAccPortId portId, IxEthAccMacAddr *macAddr)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortUnicastMacAddressSetPriv(portId, macAddr);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortUnicastMacAddressGet(IxEthAccPortId portId, IxEthAccMacAddr *macAddr)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortUnicastMacAddressGetPriv(portId, macAddr);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortMulticastAddressJoin(IxEthAccPortId portId, IxEthAccMacAddr *macAddr)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortMulticastAddressJoinPriv(portId, macAddr);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortMulticastAddressJoinAll(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortMulticastAddressJoinAllPriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortMulticastAddressLeave(IxEthAccPortId portId, IxEthAccMacAddr *macAddr)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortMulticastAddressLeavePriv(portId, macAddr);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortMulticastAddressLeaveAll(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortMulticastAddressLeaveAllPriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortUnicastAddressShow(IxEthAccPortId portId)
{
    IxEthAccStatus result;
 
    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortUnicastAddressShowPriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC void 
ixEthAccPortMulticastAddressShow(IxEthAccPortId portId)
{
    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return;
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    ixEthAccPortMulticastAddressShowPriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortDuplexModeSet(IxEthAccPortId portId, IxEthAccDuplexMode mode)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortDuplexModeSetPriv(portId, mode);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortDuplexModeGet(IxEthAccPortId portId, IxEthAccDuplexMode *mode)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortDuplexModeGetPriv(portId, mode);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortTxFrameAppendPaddingEnable(IxEthAccPortId portId)
{
    IxEthAccStatus result;
     
    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortTxFrameAppendPaddingEnablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortTxFrameAppendPaddingDisable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortTxFrameAppendPaddingDisablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortTxFrameAppendFCSEnable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortTxFrameAppendFCSEnablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortTxFrameAppendFCSDisable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortTxFrameAppendFCSDisablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortRxFrameAppendFCSEnable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortRxFrameAppendFCSEnablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccPortRxFrameAppendFCSDisable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortRxFrameAppendFCSDisablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccTxSchedulingDisciplineSet(IxEthAccPortId portId, IxEthAccSchedulerDiscipline sched)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccTxSchedulingDisciplineSetPriv(portId, sched);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus
ixEthAccRxSchedulingDisciplineSet(IxEthAccSchedulerDiscipline sched)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccRxSchedulingDisciplineSetPriv(sched);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortNpeLoopbackEnable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccNpeLoopbackEnablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortTxEnable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortTxEnablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortRxEnable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortRxEnablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortNpeLoopbackDisable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccNpeLoopbackDisablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortTxDisable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortTxDisablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortRxDisable(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortRxDisablePriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}

IX_ETH_ACC_PUBLIC IxEthAccStatus 
ixEthAccPortMacReset(IxEthAccPortId portId)
{
    IxEthAccStatus result;

    if (!IX_ETH_ACC_IS_SERVICE_INITIALIZED())
    {
	return (IX_ETH_ACC_FAIL);
    }

    ixOsalMutexLock(&ixEthAccControlInterfaceMutex, IX_OSAL_WAIT_FOREVER);
    result = ixEthAccPortMacResetPriv(portId);
    ixOsalMutexUnlock(&ixEthAccControlInterfaceMutex);
    return result;
}
