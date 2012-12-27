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
 * Copyright (C) 2009 Matthias Kaehlcke <matthias@kaehlcke.net>
 *
 * Copyright (C) 2004, 2005
 * Cory T. Tusar, Videon Central, Inc., <ctusar@videon-central.com>
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
 *
 */

#ifndef _EP93XX_ETH_H
#define _EP93XX_ETH_H

#include <net.h>

/**
 * #define this to dump device status and queue info during initialization and
 * following errors.
 */
#undef EP93XX_MAC_DEBUG

/**
 * Number of descriptor and status entries in our RX queues.
 * It must be power of 2 !
 */
#define NUMRXDESC		PKTBUFSRX

/**
 * Number of descriptor and status entries in our TX queues.
 */
#define NUMTXDESC		1

/**
 * 944 = (1024 - 64) - 16, Fifo size - Minframesize - 16 (Chip FACT)
 */
#define TXSTARTMAX		944

/**
 * Receive descriptor queue entry
 */
struct rx_descriptor {
	uint32_t word1;
	uint32_t word2;
};

/**
 * Receive status queue entry
 */
struct rx_status {
	uint32_t word1;
	uint32_t word2;
};

#define RX_STATUS_RWE(rx_status) ((rx_status->word1 >> 30) & 0x01)
#define RX_STATUS_RFP(rx_status) ((rx_status->word1 >> 31) & 0x01)
#define RX_STATUS_FRAME_LEN(rx_status) (rx_status->word2 & 0xFFFF)

/**
 * Transmit descriptor queue entry
 */
struct tx_descriptor {
	uint32_t word1;
	uint32_t word2;
};

#define TX_DESC_EOF (1 << 31)

/**
 * Transmit status queue entry
 */
struct tx_status {
	uint32_t word1;
};

#define TX_STATUS_TXWE(tx_status) (((tx_status)->word1 >> 30) & 0x01)
#define TX_STATUS_TXFP(tx_status) (((tx_status)->word1 >> 31) & 0x01)

/**
 * Transmit descriptor queue
 */
struct tx_descriptor_queue {
	struct tx_descriptor *base;
	struct tx_descriptor *current;
	struct tx_descriptor *end;
};

/**
 * Transmit status queue
 */
struct tx_status_queue {
	struct tx_status *base;
	volatile struct tx_status *current;
	struct tx_status *end;
};

/**
 * Receive descriptor queue
 */
struct rx_descriptor_queue {
	struct rx_descriptor *base;
	struct rx_descriptor *current;
	struct rx_descriptor *end;
};

/**
 * Receive status queue
 */
struct rx_status_queue {
	struct rx_status *base;
	volatile struct rx_status *current;
	struct rx_status *end;
};

/**
 * EP93xx MAC private data structure
 */
struct ep93xx_priv {
	struct rx_descriptor_queue	rx_dq;
	struct rx_status_queue		rx_sq;
	void				*rx_buffer[NUMRXDESC];

	struct tx_descriptor_queue	tx_dq;
	struct tx_status_queue		tx_sq;

	struct mac_regs			*regs;
};

#endif
