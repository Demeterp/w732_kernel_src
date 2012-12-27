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
 * (C) Copyright 2005
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef NPE_H
#define NPE_H

/*
 * defines...
 */
#define CONFIG_SYS_NPE_NUMS		1
#ifdef CONFIG_HAS_ETH1
#undef CONFIG_SYS_NPE_NUMS
#define CONFIG_SYS_NPE_NUMS		2
#endif

#define NPE_NUM_PORTS		3
#define ACTIVE_PORTS		1

#define NPE_PKT_SIZE		1600

#define CONFIG_DEVS_ETH_INTEL_NPE_MAX_RX_DESCRIPTORS	64
#define CONFIG_DEVS_ETH_INTEL_NPE_MAX_TX_DESCRIPTORS	2

#define NPE_MBUF_POOL_SIZE					\
	((CONFIG_DEVS_ETH_INTEL_NPE_MAX_TX_DESCRIPTORS +	\
	  CONFIG_DEVS_ETH_INTEL_NPE_MAX_RX_DESCRIPTORS) *	\
	 sizeof(IX_OSAL_MBUF) * ACTIVE_PORTS)

#define NPE_PKT_POOL_SIZE					\
	((CONFIG_DEVS_ETH_INTEL_NPE_MAX_TX_DESCRIPTORS +	\
	  CONFIG_DEVS_ETH_INTEL_NPE_MAX_RX_DESCRIPTORS) *	\
	 NPE_PKT_SIZE * ACTIVE_PORTS)

#define NPE_MEM_POOL_SIZE (NPE_MBUF_POOL_SIZE + NPE_PKT_POOL_SIZE)

#define PHY_AUTONEGOTIATE_TIMEOUT 4000	/* 4000 ms autonegotiate timeout */

/*
 * structs...
 */
struct npe {
	u8			active;           /* NPE active				*/
	u8			eth_id;           /* IX_ETH_PORT_1 or IX_ETH_PORT_2	*/
	u8			phy_no;           /* which PHY (0 - 31)			*/
	u8			mac_address[6];

	IX_OSAL_MBUF		*rxQHead;
	IX_OSAL_MBUF		*txQHead;

	u8			*tx_pkts;
	u8			*rx_pkts;
	IX_OSAL_MBUF		*rx_mbufs;
	IX_OSAL_MBUF		*tx_mbufs;

	int			print_speed;

	int			rx_read;
	int			rx_write;
	int			rx_len[PKTBUFSRX];
};

/*
 * prototypes...
 */
extern int npe_miiphy_read (char *devname, unsigned char addr,
			    unsigned char reg, unsigned short *value);
extern int npe_miiphy_write (char *devname, unsigned char addr,
			     unsigned char reg, unsigned short value);

#endif /* ifndef NPE_H */
