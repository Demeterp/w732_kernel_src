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
 * bfin_mac.h - some defines/structures for the Blackfin on-chip MAC.
 *
 * Copyright (c) 2005-2008 Analog Device, Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BFIN_MAC_H__
#define __BFIN_MAC_H__

#define RECV_BUFSIZE		(0x614)

typedef struct ADI_DMA_CONFIG_REG {
	u16 b_DMA_EN:1;		/* 0	Enabled				*/
	u16 b_WNR:1;		/* 1	Direction			*/
	u16 b_WDSIZE:2;		/* 2:3	Transfer word size		*/
	u16 b_DMA2D:1;		/* 4	DMA mode			*/
	u16 b_RESTART:1;	/* 5	Retain FIFO			*/
	u16 b_DI_SEL:1;		/* 6	Data interrupt timing select	*/
	u16 b_DI_EN:1;		/* 7	Data interrupt enabled		*/
	u16 b_NDSIZE:4;		/* 8:11	Flex descriptor size		*/
	u16 b_FLOW:3;		/* 12:14Flow				*/
} ADI_DMA_CONFIG_REG;

typedef struct adi_ether_frame_buffer {
	u16 NoBytes;		/* the no. of following bytes	*/
	u8 Dest[6];		/* destination MAC address	*/
	u8 Srce[6];		/* source MAC address		*/
	u16 LTfield;		/* length/type field		*/
	u8 Data[0];		/* payload bytes		*/
} ADI_ETHER_FRAME_BUFFER;
/* 16 bytes/struct	*/

typedef struct dma_descriptor {
	struct dma_descriptor *NEXT_DESC_PTR;
	u32 START_ADDR;
	union {
		u16 CONFIG_DATA;
		ADI_DMA_CONFIG_REG CONFIG;
	};
} DMA_DESCRIPTOR;
/* 10 bytes/struct in 12 bytes */

typedef struct adi_ether_buffer {
	DMA_DESCRIPTOR Dma[2];		/* first for the frame, second for the status */
	ADI_ETHER_FRAME_BUFFER *FrmData;/* pointer to data */
	struct adi_ether_buffer *pNext;	/* next buffer */
	struct adi_ether_buffer *pPrev;	/* prev buffer */
	u16 IPHdrChksum;		/* the IP header checksum */
	u16 IPPayloadChksum;		/* the IP header and payload checksum */
	volatile u32 StatusWord;	/* the frame status word */
} ADI_ETHER_BUFFER;
/* 40 bytes/struct in 44 bytes */

static ADI_ETHER_BUFFER *SetupRxBuffer(int no);
static ADI_ETHER_BUFFER *SetupTxBuffer(int no);

static int bfin_EMAC_init(struct eth_device *dev, bd_t *bd);
static void bfin_EMAC_halt(struct eth_device *dev);
static int bfin_EMAC_send(struct eth_device *dev, volatile void *packet, int length);
static int bfin_EMAC_recv(struct eth_device *dev);
static int bfin_EMAC_setup_addr(struct eth_device *dev);

#endif
