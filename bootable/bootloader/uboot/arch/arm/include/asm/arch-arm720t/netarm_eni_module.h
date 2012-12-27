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

/*
 * include/asm-armnommu/arch-netarm/netarm_eni_module.h
 *
 * Copyright (C) 2000 NETsilicon, Inc.
 * Copyright (C) 2000 WireSpeed Communications Corporation
 *
 * This software is copyrighted by WireSpeed. LICENSEE agrees that
 * it will not delete this copyright notice, trademarks or protective
 * notices from any copy made by LICENSEE.
 *
 * This software is provided "AS-IS" and any express or implied
 * warranties or conditions, including but not limited to any
 * implied warranties of merchantability and fitness for a particular
 * purpose regarding this software. In no event shall WireSpeed
 * be liable for any indirect, consequential, or incidental damages,
 * loss of profits or revenue, loss of use or data, or interruption
 * of business, whether the alleged damages are labeled in contract,
 * tort, or indemnity.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * author(s) : David Smith
 */

#ifndef __NETARM_ENI_MODULE_REGISTERS_H
#define __NETARM_ENI_MODULE_REGISTERS_H

/* ENI unit register offsets */

/* #ifdef CONFIG_ARCH_NETARM */
#define	NETARM_ENI_MODULE_BASE		(0xFFA00000)
/* #endif / * CONFIG_ARCH_NETARM */

#define get_eni_reg_addr(c) ((volatile unsigned int *)(NETARM_ENI_MODULE_BASE + (c)))
#define get_eni_ctl_reg_addr(minor) \
	(get_eni_reg_addr(NETARM_ENI_1284_PORT1_CONTROL) + (minor))

#define	NETARM_ENI_GENERAL_CONTROL	(0x00)
#define	NETARM_ENI_STATUS_CONTROL	(0x04)
#define	NETARM_ENI_FIFO_MODE_DATA	(0x08)

#define	NETARM_ENI_1284_PORT1_CONTROL	(0x10)
#define	NETARM_ENI_1284_PORT2_CONTROL	(0x14)
#define	NETARM_ENI_1284_PORT3_CONTROL	(0x18)
#define	NETARM_ENI_1284_PORT4_CONTROL	(0x1c)

#define	NETARM_ENI_1284_CHANNEL1_DATA	(0x20)
#define	NETARM_ENI_1284_CHANNEL2_DATA	(0x24)
#define	NETARM_ENI_1284_CHANNEL3_DATA	(0x28)
#define	NETARM_ENI_1284_CHANNEL4_DATA	(0x2c)

#define	NETARM_ENI_ENI_CONTROL		(0x30)
#define	NETARM_ENI_ENI_PULSED_INTR	(0x34)
#define	NETARM_ENI_ENI_SHARED_RAM_ADDR	(0x38)
#define	NETARM_ENI_ENI_SHARED		(0x3c)

/* select bitfield defintions */

/* General Control Register (0xFFA0_0000) */

#define NETARM_ENI_GCR_ENIMODE_IEEE1284	(0x00000001)
#define NETARM_ENI_GCR_ENIMODE_SHRAM16	(0x00000004)
#define NETARM_ENI_GCR_ENIMODE_SHRAM8	(0x00000005)
#define NETARM_ENI_GCR_ENIMODE_FIFO16	(0x00000006)
#define NETARM_ENI_GCR_ENIMODE_FIFO8	(0x00000007)

#define NETARM_ENI_GCR_ENIMODE_MASK	(0x00000007)

/* IEEE 1284 Port Control Registers 1-4 (0xFFA0_0010, 0xFFA0_0014,
   0xFFA0_0018, 0xFFA0_001c) */

#define NETARM_ENI_1284PC_PORT_ENABLE	(0x80000000)
#define NETARM_ENI_1284PC_DMA_ENABLE	(0x40000000)
#define NETARM_ENI_1284PC_OBE_INT_EN	(0x20000000)
#define NETARM_ENI_1284PC_ACK_INT_EN	(0x10000000)
#define NETARM_ENI_1284PC_ECP_MODE	(0x08000000)
#define NETARM_ENI_1284PC_LOOPBACK_MODE	(0x04000000)

#define NETARM_ENI_1284PC_STROBE_TIME0	(0x00000000) /* 0.5 uS */
#define NETARM_ENI_1284PC_STROBE_TIME1	(0x01000000) /* 1.0 uS */
#define NETARM_ENI_1284PC_STROBE_TIME2	(0x02000000) /* 5.0 uS */
#define NETARM_ENI_1284PC_STROBE_TIME3	(0x03000000) /* 10.0 uS */
#define NETARM_ENI_1284PC_STROBE_MASK	(0x03000000)

#define NETARM_ENI_1284PC_MAN_STROBE_EN	(0x00800000)
#define NETARM_ENI_1284PC_FAST_MODE	(0x00400000)
#define NETARM_ENI_1284PC_BIDIR_MODE	(0x00200000)

#define NETARM_ENI_1284PC_MAN_STROBE	(0x00080000)
#define NETARM_ENI_1284PC_AUTO_FEED	(0x00040000)
#define NETARM_ENI_1284PC_INIT		(0x00020000)
#define NETARM_ENI_1284PC_HSELECT	(0x00010000)
#define NETARM_ENI_1284PC_FE_INT_EN	(0x00008000)
#define NETARM_ENI_1284PC_EPP_MODE	(0x00004000)
#define NETARM_ENI_1284PC_IBR_INT_EN	(0x00002000)
#define NETARM_ENI_1284PC_IBR		(0x00001000)

#define NETARM_ENI_1284PC_RXFDB_1BYTE	(0x00000400)
#define NETARM_ENI_1284PC_RXFDB_2BYTE	(0x00000800)
#define NETARM_ENI_1284PC_RXFDB_3BYTE	(0x00000c00)
#define NETARM_ENI_1284PC_RXFDB_4BYTE	(0x00000000)

#define NETARM_ENI_1284PC_RBCC		(0x00000200)
#define NETARM_ENI_1284PC_RBCT		(0x00000100)
#define NETARM_ENI_1284PC_ACK		(0x00000080)
#define NETARM_ENI_1284PC_FIFO_E	(0x00000040)
#define NETARM_ENI_1284PC_OBE		(0x00000020)
#define NETARM_ENI_1284PC_ACK_INT	(0x00000010)
#define NETARM_ENI_1284PC_BUSY		(0x00000008)
#define NETARM_ENI_1284PC_PE		(0x00000004)
#define NETARM_ENI_1284PC_PSELECT	(0x00000002)
#define NETARM_ENI_1284PC_FAULT		(0x00000001)

#endif /* __NETARM_ENI_MODULE_REGISTERS_H */
