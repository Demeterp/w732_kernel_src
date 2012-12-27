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
* FILENAME:
*
* xdma_channel_i.h
*
* DESCRIPTION:
*
* This file contains data which is shared internal data for the DMA channel
* component. It is also shared with the buffer descriptor component which is
* very tightly coupled with the DMA channel component.
*
* NOTES:
*
* The last buffer descriptor constants must be located here to prevent a
* circular dependency between the DMA channel component and the buffer
* descriptor component.
*
******************************************************************************/

#ifndef XDMA_CHANNEL_I_H	/* prevent circular inclusions */
#define XDMA_CHANNEL_I_H	/* by using protection macros */

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xversion.h"

/************************** Constant Definitions *****************************/

#define XDC_DMA_CHANNEL_V1_00_A		"1.00a"

/* the following constant provides access to the bit fields of the DMA control
 * register (DMACR) which must be shared between the DMA channel component
 * and the buffer descriptor component
 */
#define XDC_CONTROL_LAST_BD_MASK	0x02000000UL	/* last buffer descriptor */

/* the following constant provides access to the bit fields of the DMA status
 * register (DMASR) which must be shared between the DMA channel component
 * and the buffer descriptor component
 */
#define XDC_STATUS_LAST_BD_MASK		0x10000000UL	/* last buffer descriptor */

/* the following constants provide access to each of the registers of a DMA
 * channel
 */
#define XDC_RST_REG_OFFSET	0	/* reset register */
#define XDC_MI_REG_OFFSET	0	/* module information register */
#define XDC_DMAC_REG_OFFSET	4	/* DMA control register */
#define XDC_SA_REG_OFFSET	8	/* source address register */
#define XDC_DA_REG_OFFSET	12	/* destination address register */
#define XDC_LEN_REG_OFFSET	16	/* length register */
#define XDC_DMAS_REG_OFFSET	20	/* DMA status register */
#define XDC_BDA_REG_OFFSET	24	/* buffer descriptor address register */
#define XDC_SWCR_REG_OFFSET 28	/* software control register */
#define XDC_UPC_REG_OFFSET	32	/* unserviced packet count register */
#define	XDC_PCT_REG_OFFSET	36	/* packet count threshold register */
#define XDC_PWB_REG_OFFSET	40	/* packet wait bound register */
#define XDC_IS_REG_OFFSET	44	/* interrupt status register */
#define XDC_IE_REG_OFFSET	48	/* interrupt enable register */

/* the following constant is written to the reset register to reset the
 * DMA channel
 */
#define XDC_RESET_MASK				0x0000000AUL

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

#endif				/* end of protection macro */
