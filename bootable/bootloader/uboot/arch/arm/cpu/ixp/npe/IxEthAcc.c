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
 * @file IxEthAcc.c
 *
 * @author Intel Corporation
 * @date 20-Feb-2001
 *
 * @brief This file contains the implementation of the IXP425 Ethernet Access Component
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



#include "IxEthAcc.h"
#ifdef CONFIG_IXP425_COMPONENT_ETHDB
#include "IxEthDB.h"
#endif
#include "IxFeatureCtrl.h"

#include "IxEthAcc_p.h"
#include "IxEthAccMac_p.h"
#include "IxEthAccMii_p.h"

/**
 * @addtogroup IxEthAcc
 *@{
 */


/**
 * @brief System-wide information data strucure.
 *
 * @ingroup IxEthAccPri
 *
 */

IxEthAccInfo ixEthAccDataInfo;
extern PUBLIC IxEthAccMacState ixEthAccMacState[];
extern PUBLIC IxOsalMutex ixEthAccControlInterfaceMutex;

/**
 * @brief System-wide information
 *
 * @ingroup IxEthAccPri
 *
 */
BOOL ixEthAccServiceInit = FALSE;

/* global filtering bit mask */
PUBLIC UINT32 ixEthAccNewSrcMask;

/**
 * @brief Per port information data strucure.
 *
 * @ingroup IxEthAccPri
 *
 */

IxEthAccPortDataInfo ixEthAccPortData[IX_ETH_ACC_NUMBER_OF_PORTS];

PUBLIC IxEthAccStatus ixEthAccInit()
{
#ifdef CONFIG_IXP425_COMPONENT_ETHDB
  /*
   * Initialize Control plane
   */
  if (ixEthDBInit() != IX_ETH_ACC_SUCCESS)
  {
      IX_ETH_ACC_WARNING_LOG("ixEthAccInit: EthDB init failed\n", 0, 0, 0, 0, 0, 0);

      return IX_ETH_ACC_FAIL;
  }
#endif

  if (IX_FEATURE_CTRL_SWCONFIG_ENABLED == ixFeatureCtrlSwConfigurationCheck (IX_FEATURECTRL_ETH_LEARNING))
  {
      ixEthAccNewSrcMask = (~0); /* want all the bits */
  }
  else
  {
      ixEthAccNewSrcMask = (~IX_ETHACC_NE_NEWSRCMASK); /* want all but the NewSrc bit */
  }

  /*
   * Initialize Data plane
   */
   if ( ixEthAccInitDataPlane()  != IX_ETH_ACC_SUCCESS )
   {
      IX_ETH_ACC_WARNING_LOG("ixEthAccInit: data plane init failed\n", 0, 0, 0, 0, 0, 0);

       return IX_ETH_ACC_FAIL;
   }


   if ( ixEthAccQMgrQueuesConfig() != IX_ETH_ACC_SUCCESS )
   {
      IX_ETH_ACC_WARNING_LOG("ixEthAccInit: queue config failed\n", 0, 0, 0, 0, 0, 0);

       return IX_ETH_ACC_FAIL;
   }

   /*
    * Initialize MII
    */
   if ( ixEthAccMiiInit() != IX_ETH_ACC_SUCCESS )
   {
      IX_ETH_ACC_WARNING_LOG("ixEthAccInit: Mii init failed\n", 0, 0, 0, 0, 0, 0);

       return IX_ETH_ACC_FAIL;
   }

   /*
    * Initialize MAC I/O memory
    */
   if (ixEthAccMacMemInit() != IX_ETH_ACC_SUCCESS)
   {
      IX_ETH_ACC_WARNING_LOG("ixEthAccInit: Mac init failed\n", 0, 0, 0, 0, 0, 0);

     return IX_ETH_ACC_FAIL;
   }

   /*
    * Initialize control plane interface lock
    */
   if (ixOsalMutexInit(&ixEthAccControlInterfaceMutex) != IX_SUCCESS)
   {
       IX_ETH_ACC_WARNING_LOG("ixEthAccInit: Control plane interface lock initialization failed\n", 0, 0, 0, 0, 0, 0);

       return IX_ETH_ACC_FAIL;
   }

   /* initialiasation is complete */
   ixEthAccServiceInit = TRUE;

   return IX_ETH_ACC_SUCCESS;

}

PUBLIC void ixEthAccUnload(void)
{
    IxEthAccPortId portId;

    if ( IX_ETH_ACC_IS_SERVICE_INITIALIZED() )
    {
       /* check none of the port is still active */
       for (portId = 0; portId < IX_ETH_ACC_NUMBER_OF_PORTS; portId++)
       {
	   if ( IX_ETH_IS_PORT_INITIALIZED(portId) )
	   {
	       if (ixEthAccMacState[portId].portDisableState == ACTIVE)
	       {
		   IX_ETH_ACC_WARNING_LOG("ixEthAccUnload: port %u still active, bail out\n", portId, 0, 0, 0, 0, 0);
		   return;
	       }
	   }
       }

       /* unmap the memory areas */
       ixEthAccMiiUnload();
       ixEthAccMacUnload();

       /* set all ports as uninitialized */
       for (portId = 0; portId < IX_ETH_ACC_NUMBER_OF_PORTS; portId++)
       {
	       ixEthAccPortData[portId].portInitialized = FALSE;
       }

       /* uninitialize the service */
       ixEthAccServiceInit = FALSE;
   }
}

PUBLIC IxEthAccStatus ixEthAccPortInit( IxEthAccPortId portId)
{

  IxEthAccStatus ret=IX_ETH_ACC_SUCCESS;

   if ( ! IX_ETH_ACC_IS_SERVICE_INITIALIZED() )
   {
	return(IX_ETH_ACC_FAIL);
   }

   /*
    * Check for valid port
    */

   if ( ! IX_ETH_ACC_IS_PORT_VALID(portId) )
   {
       return (IX_ETH_ACC_INVALID_PORT);
   }

   if (IX_ETH_ACC_SUCCESS != ixEthAccSingleEthNpeCheck(portId))
   {
       IX_ETH_ACC_WARNING_LOG("EthAcc: Unavailable Eth %d: Cannot initialize Eth port.\n",(INT32) portId,0,0,0,0,0);
       return IX_ETH_ACC_SUCCESS ;
   }

   if ( IX_ETH_IS_PORT_INITIALIZED(portId) )
   {
	/* Already initialized */
	return(IX_ETH_ACC_FAIL);
   }

   if(ixEthAccMacInit(portId)!=IX_ETH_ACC_SUCCESS)
   {
       return IX_ETH_ACC_FAIL;
   }

   /*
     * Set the port init flag.
     */

    ixEthAccPortData[portId].portInitialized = TRUE;

#ifdef CONFIG_IXP425_COMPONENT_ETHDB
    /* init learning/filtering database structures for this port */
    ixEthDBPortInit(portId);
#endif

    return(ret);
}


