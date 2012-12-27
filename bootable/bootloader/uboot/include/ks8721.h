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
 * NOTE:	MICREL ethernet Physical layer
 *
 * Version:	KS8721.h
 *
 * Authors:	Eric Benard (based on dm9161.h)
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */

/* MICREL PHYSICAL LAYER TRANSCEIVER KS8721 */

#define	KS8721_BMCR		0
#define KS8721_BMSR		1
#define KS8721_PHYID1		2
#define KS8721_PHYID2		3
#define KS8721_ANAR		4
#define KS8721_ANLPAR		5
#define KS8721_ANER		6
#define KS8721_RECR		15
#define KS8721_MDINTR		27
#define KS8721_100BT		31

/* --Bit definitions: KS8721_BMCR */
#define KS8721_RESET		(1 << 15)
#define KS8721_LOOPBACK		(1 << 14)
#define KS8721_SPEED_SELECT	(1 << 13)
#define KS8721_AUTONEG		(1 << 12)
#define KS8721_POWER_DOWN	(1 << 11)
#define KS8721_ISOLATE		(1 << 10)
#define KS8721_RESTART_AUTONEG	(1 << 9)
#define KS8721_DUPLEX_MODE	(1 << 8)
#define KS8721_COLLISION_TEST	(1 << 7)
#define	KS8721_DISABLE		(1 << 0)

/*--Bit definitions: KS8721_BMSR */
#define KS8721_100BASE_T4	(1 << 15)
#define KS8721_100BASE_TX_FD	(1 << 14)
#define KS8721_100BASE_T4_HD	(1 << 13)
#define KS8721_10BASE_T_FD	(1 << 12)
#define KS8721_10BASE_T_HD	(1 << 11)
#define KS8721_MF_PREAMB_SUPPR	(1 << 6)
#define KS8721_AUTONEG_COMP	(1 << 5)
#define KS8721_REMOTE_FAULT	(1 << 4)
#define KS8721_AUTONEG_ABILITY	(1 << 3)
#define KS8721_LINK_STATUS	(1 << 2)
#define KS8721_JABBER_DETECT	(1 << 1)
#define KS8721_EXTEND_CAPAB	(1 << 0)

/*--Bit definitions: KS8721_PHYID */
#define KS8721_PHYID_OUI	0x0885
#define KS8721_LSB_MASK		0x3F

#define	KS8721BL_MODEL		0x21
#define	KS8721_MODELMASK	0x3F0
#define	KS8721BL_REV		0x9
#define KS8721_REVMASK		0xF

/*--Bit definitions: KS8721_ANAR, KS8721_ANLPAR */
#define KS8721_NP		(1 << 15)
#define KS8721_ACK		(1 << 14)
#define KS8721_RF		(1 << 13)
#define KS8721_PAUSE		(1 << 10)
#define KS8721_T4		(1 << 9)
#define KS8721_TX_FDX		(1 << 8)
#define KS8721_TX_HDX		(1 << 7)
#define KS8721_10_FDX		(1 << 6)
#define KS8721_10_HDX		(1 << 5)
#define KS8721_AN_IEEE_802_3	0x0001

/******************  function prototypes **********************/
unsigned int  ks8721_isphyconnected(AT91PS_EMAC p_mac);
unsigned char ks8721_getlinkspeed(AT91PS_EMAC p_mac);
unsigned char ks8721_autonegotiate(AT91PS_EMAC p_mac, int *status);
unsigned char ks8721_initphy(AT91PS_EMAC p_mac);
