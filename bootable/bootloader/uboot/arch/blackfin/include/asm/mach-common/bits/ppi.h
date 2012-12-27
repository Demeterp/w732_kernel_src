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
 * PPI Masks
 */

#ifndef __BFIN_PERIPHERAL_PPI__
#define __BFIN_PERIPHERAL_PPI__

/* PPI_CONTROL Masks */
#define PORT_EN			0x0001	/* PPI Port Enable */
#define PORT_DIR		0x0002	/* PPI Port Direction */
#define XFR_TYPE		0x000C	/* PPI Transfer Type */
#define PORT_CFG		0x0030	/* PPI Port Configuration */
#define FLD_SEL			0x0040	/* PPI Active Field Select */
#define PACK_EN			0x0080	/* PPI Packing Mode */
#define DMA32			0x0100	/* PPI 32-bit DMA Enable */
#define SKIP_EN			0x0200	/* PPI Skip Element Enable */
#define SKIP_EO			0x0400	/* PPI Skip Even/Odd Elements */
#define DLENGTH			0x3800	/* PPI Data Length */
#define DLEN_8			0x0000	/* Data Length = 8 Bits */
#define DLEN_10			0x0800	/* Data Length = 10 Bits */
#define DLEN_11			0x1000	/* Data Length = 11 Bits */
#define DLEN_12			0x1800	/* Data Length = 12 Bits */
#define DLEN_13			0x2000	/* Data Length = 13 Bits */
#define DLEN_14			0x2800	/* Data Length = 14 Bits */
#define DLEN_15			0x3000	/* Data Length = 15 Bits */
#define DLEN_16			0x3800	/* Data Length = 16 Bits */
#define POLC			0x4000	/* PPI Clock Polarity */
#define POLS			0x8000	/* PPI Frame Sync Polarity */

/* PPI_STATUS Masks */
#define FLD			0x0400	/* Field Indicator */
#define FT_ERR			0x0800	/* Frame Track Error */
#define OVR			0x1000	/* FIFO Overflow Error */
#define UNDR			0x2000	/* FIFO Underrun Error */
#define ERR_DET			0x4000	/* Error Detected Indicator */
#define ERR_NCOR		0x8000	/* Error Not Corrected Indicator */

#endif
