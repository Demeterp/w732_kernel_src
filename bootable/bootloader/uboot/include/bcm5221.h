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
 * Broadcom BCM5221 Ethernet PHY
 *
 * (C) Copyright 2005 REA Elektronik GmbH <www.rea.de>
 * Anders Larsen <alarsen@rea.de>
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

#define	BCM5221_BMCR		0	/* Basic Mode Control Register */
#define BCM5221_BMSR		1	/* Basic Mode Status Register */
#define BCM5221_PHYID1		2	/* PHY Identifier Register 1 */
#define BCM5221_PHYID2		3	/* PHY Identifier Register 2 */
#define BCM5221_ANAR		4	/* Auto-negotiation Advertisement Register  */
#define BCM5221_ANLPAR		5	/* Auto-negotiation Link Partner Ability Register */
#define BCM5221_ANER		6	/* Auto-negotiation Expansion Register  */
#define BCM5221_ACSR		24	/* Auxiliary Control/Status Register */
#define BCM5221_INTR		26	/* Interrupt Register */

/* --Bit definitions: BCM5221_BMCR */
#define BCM5221_RESET		(1 << 15)	/* 1= Software Reset; 0=Normal Operation */
#define BCM5221_LOOPBACK	(1 << 14)	/* 1=loopback Enabled; 0=Normal Operation */
#define BCM5221_SPEED_SELECT	(1 << 13)	/* 1=100Mbps; 0=10Mbps */
#define BCM5221_AUTONEG		(1 << 12)
#define BCM5221_POWER_DOWN	(1 << 11)
#define BCM5221_ISOLATE		(1 << 10)
#define BCM5221_RESTART_AUTONEG	(1 << 9)
#define BCM5221_DUPLEX_MODE	(1 << 8)
#define BCM5221_COLLISION_TEST	(1 << 7)

/*--Bit definitions: BCM5221_BMSR */
#define BCM5221_100BASE_T4	(1 << 15)
#define BCM5221_100BASE_TX_FD	(1 << 14)
#define BCM5221_100BASE_TX_HD	(1 << 13)
#define BCM5221_10BASE_T_FD	(1 << 12)
#define BCM5221_10BASE_T_HD	(1 << 11)
#define BCM5221_MF_PREAMB_SUPPR	(1 << 6)
#define BCM5221_AUTONEG_COMP	(1 << 5)
#define BCM5221_REMOTE_FAULT	(1 << 4)
#define BCM5221_AUTONEG_ABILITY	(1 << 3)
#define BCM5221_LINK_STATUS	(1 << 2)
#define BCM5221_JABBER_DETECT	(1 << 1)
#define BCM5221_EXTEND_CAPAB	(1 << 0)

/*--definitions: BCM5221_PHYID1 */
#define BCM5221_PHYID1_OUI	0x1018
#define BCM5221_LSB_MASK	0x3F

/*--Bit definitions: BCM5221_ANAR, BCM5221_ANLPAR */
#define BCM5221_NP		(1 << 15)
#define BCM5221_ACK		(1 << 14)
#define BCM5221_RF		(1 << 13)
#define BCM5221_FCS		(1 << 10)
#define BCM5221_T4		(1 << 9)
#define BCM5221_TX_FDX		(1 << 8)
#define BCM5221_TX_HDX		(1 << 7)
#define BCM5221_10_FDX		(1 << 6)
#define BCM5221_10_HDX		(1 << 5)
#define BCM5221_AN_IEEE_802_3	0x0001

/*--Bit definitions: BCM5221_ANER */
#define BCM5221_PDF		(1 << 4)
#define BCM5221_LP_NP_ABLE	(1 << 3)
#define BCM5221_NP_ABLE		(1 << 2)
#define BCM5221_PAGE_RX		(1 << 1)
#define BCM5221_LP_AN_ABLE	(1 << 0)

/*--Bit definitions: BCM5221_ACSR */
#define BCM5221_100		(1 << 1)
#define BCM5221_FDX		(1 << 0)

/*--Bit definitions: BCM5221_INTR */
#define BCM5221_FDX_LED		(1 << 15)
#define BCM5221_INTR_ENABLE	(1 << 14)
#define BCM5221_FDX_MASK	(1 << 11)
#define BCM5221_SPD_MASK	(1 << 10)
#define BCM5221_LINK_MASK	(1 << 9)
#define BCM5221_INTR_MASK	(1 << 8)
#define BCM5221_FDX_CHG		(1 << 3)
#define BCM5221_SPD_CHG		(1 << 2)
#define BCM5221_LINK_CHG	(1 << 1)
#define BCM5221_INTR_STATUS	(1 << 0)

/******************  function prototypes **********************/
unsigned int  bcm5221_IsPhyConnected(AT91PS_EMAC p_mac);
unsigned char bcm5221_GetLinkSpeed(AT91PS_EMAC p_mac);
unsigned char bcm5221_AutoNegotiate(AT91PS_EMAC p_mac, int *status);
unsigned char bcm5221_InitPhy(AT91PS_EMAC p_mac);
