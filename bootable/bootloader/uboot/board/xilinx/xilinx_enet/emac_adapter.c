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

/******************************************************************************
*
*     Author: Xilinx, Inc.
*
*
*     This program is free software; you can redistribute it and/or modify it
*     under the terms of the GNU General Public License as published by the
*     Free Software Foundation; either version 2 of the License, or (at your
*     option) any later version.
*
*
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
*     COURTESY TO YOU. BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
*     ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION OR STANDARD,
*     XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION IS FREE
*     FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE FOR OBTAINING
*     ANY THIRD PARTY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
*     XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
*     THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY
*     WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM
*     CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND
*     FITNESS FOR A PARTICULAR PURPOSE.
*
*
*     Xilinx hardware products are not intended for use in life support
*     appliances, devices, or systems. Use in such applications is
*     expressly prohibited.
*
*
*     (c) Copyright 2002-2004 Xilinx Inc.
*     All rights reserved.
*
*
*     You should have received a copy of the GNU General Public License along
*     with this program; if not, write to the Free Software Foundation, Inc.,
*     675 Mass Ave, Cambridge, MA 02139, USA.
*
******************************************************************************/

#include <config.h>
#include <common.h>
#include <net.h>
#include "xemac.h"

#if defined(XPAR_EMAC_0_DEVICE_ID)
/*
 * ENET_MAX_MTU and ENET_MAX_MTU_ALIGNED are set from
 * PKTSIZE and PKTSIZE_ALIGN (include/net.h)
 */

#define ENET_MAX_MTU           PKTSIZE
#define ENET_MAX_MTU_ALIGNED   PKTSIZE_ALIGN
#define ENET_ADDR_LENGTH       6

static XEmac Emac;
static char etherrxbuff[PKTSIZE_ALIGN];	/* Receive buffer */

/* hardcoded MAC address for the Xilinx EMAC Core when env is nowhere*/
#ifdef CONFIG_ENV_IS_NOWHERE
static u8 EMACAddr[ENET_ADDR_LENGTH] = { 0x00, 0x0a, 0x35, 0x00, 0x22, 0x01 };
#endif

static int initialized = 0;

void
eth_halt(void)
{
	if (initialized)
		(void) XEmac_Stop(&Emac);
}

int
eth_init(bd_t * bis)
{
	u32 Options;
	XStatus Result;
	uchar enetaddr[6];

#ifdef DEBUG
	printf("EMAC Initialization Started\n\r");
#endif

	Result = XEmac_Initialize(&Emac, XPAR_EMAC_0_DEVICE_ID);
	if (Result != XST_SUCCESS) {
		return 0;
	}

	/* make sure the Emac is stopped before it is started */
	(void) XEmac_Stop(&Emac);

	if (!eth_getenv_enetaddr("ethaddr", enetaddr)) {
#ifdef CONFIG_ENV_IS_NOWHERE
		memcpy(enetaddr, EMACAddr, 6);
		eth_setenv_enetaddr("ethaddr", enetaddr);
#endif
	}

	Result = XEmac_SetMacAddress(&Emac, enetaddr);
	if (Result != XST_SUCCESS) {
		return 0;
	}

	Options =
	    (XEM_POLLED_OPTION | XEM_UNICAST_OPTION | XEM_BROADCAST_OPTION |
	     XEM_FDUPLEX_OPTION | XEM_INSERT_FCS_OPTION |
	     XEM_INSERT_PAD_OPTION);
	Result = XEmac_SetOptions(&Emac, Options);
	if (Result != XST_SUCCESS) {
		return 0;
	}

	Result = XEmac_Start(&Emac);
	if (Result != XST_SUCCESS) {
		return 0;
	}
#ifdef DEBUG
	printf("EMAC Initialization complete\n\r");
#endif

	initialized = 1;

	return (0);
}

/*-----------------------------------------------------------------------------+
+-----------------------------------------------------------------------------*/
int
eth_send(volatile void *ptr, int len)
{
	XStatus Result;

	if (len > ENET_MAX_MTU)
		len = ENET_MAX_MTU;

	Result = XEmac_PollSend(&Emac, (u8 *) ptr, len);
	if (Result == XST_SUCCESS) {
		return (1);
	} else {
		printf("Error while sending frame\n\r");
		return (0);
	}

}

int
eth_rx(void)
{
	u32 RecvFrameLength;
	XStatus Result;

	RecvFrameLength = PKTSIZE;
	Result = XEmac_PollRecv(&Emac, (u8 *) etherrxbuff, &RecvFrameLength);
	if (Result == XST_SUCCESS) {
#ifndef CONFIG_EMACLITE
		NetReceive((uchar *)etherrxbuff, RecvFrameLength);
#else
		NetReceive(etherrxbuff, RecvFrameLength);
#endif
		return (1);
	} else {
		return (0);
	}
}

#endif
