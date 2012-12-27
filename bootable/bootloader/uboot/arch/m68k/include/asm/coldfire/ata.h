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
 * ATA Internal Memory Map
 *
 * Copyright (C) 2004-2008 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
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

#ifndef __ATA_H__
#define __ATA_H__

/* ATA */
typedef struct atac {
	/* PIO */
	u8 toff;		/* 0x00 */
	u8 ton;			/* 0x01 */
	u8 t1;			/* 0x02 */
	u8 t2w;			/* 0x03 */
	u8 t2r;			/* 0x04 */
	u8 ta;			/* 0x05 */
	u8 trd;			/* 0x06 */
	u8 t4;			/* 0x07 */
	u8 t9;			/* 0x08 */

	/* DMA */
	u8 tm;			/* 0x09 */
	u8 tn;			/* 0x0A */
	u8 td;			/* 0x0B */
	u8 tk;			/* 0x0C */
	u8 tack;		/* 0x0D */
	u8 tenv;		/* 0x0E */
	u8 trp;			/* 0x0F */
	u8 tzah;		/* 0x10 */
	u8 tmli;		/* 0x11 */
	u8 tdvh;		/* 0x12 */
	u8 tdzfs;		/* 0x13 */
	u8 tdvs;		/* 0x14 */
	u8 tcvh;		/* 0x15 */
	u8 tss;			/* 0x16 */
	u8 tcyc;		/* 0x17 */

	/* FIFO */
	u32 fifo32;		/* 0x18 */
	u16 fifo16;		/* 0x1C */
	u8 rsvd0[2];
	u8 ffill;		/* 0x20 */
	u8 rsvd1[3];

	/* ATA */
	u8 cr;			/* 0x24 */
	u8 rsvd2[3];
	u8 isr;			/* 0x28 */
	u8 rsvd3[3];
	u8 ier;			/* 0x2C */
	u8 rsvd4[3];
	u8 icr;			/* 0x30 */
	u8 rsvd5[3];
	u8 falarm;		/* 0x34 */
	u8 rsvd6[106];
} atac_t;

#endif				/* __ATA_H__ */
