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

/*
 * lh7a400 SoC interface
 */

#ifndef __LH7A400_H__
#define __LH7A400_H__

#include "lh7a40x.h"

/* Interrupt Controller (userguide 8.2.1) */
typedef struct {
	volatile u32  intsr;
	volatile u32  intrsr;
	volatile u32  intens;
	volatile u32  intenc;
	volatile u32  rsvd1;
	volatile u32  rsvd2;
	volatile u32  rsvd3;
} /*__attribute__((__packed__))*/ lh7a400_interrupt_t;
#define LH7A400_INTERRUPT_BASE    (0x80000500)
#define LH7A400_INTERRUPT_PTR     ((lh7a400_interrupt_t*) LH7A400_INTERRUPT_BASE)

/* (DMA) Direct Memory Access Controller (userguide 9.2.1) */
typedef struct {
	lh7a40x_dmachan_t  chan[15];
	volatile u32       glblint;
	volatile u32       rsvd1;
	volatile u32       rsvd2;
	volatile u32       rsvd3;
} /*__attribute__((__packed__))*/ lh7a400_dma_t;

#define LH7A400_DMA_BASE      (0x80002800)
#define DMA_USBTX_OFFSET      (0x000)
#define DMA_USBRX_OFFSET      (0x040)
#define DMA_MMCTX_OFFSET      (0x080)
#define DMA_MMCRX_OFFSET      (0x0C0)
#define DMA_AC97_BASE         (0x80002A00)

#define LH7A400_DMA_PTR    ((lh7a400_dma_t*) LH7A400_DMA_BASE)
#define LH7A400_DMA_USBTX \
	((lh7a400_dmachan_t*) (LH7A400_DMA_BASE + DMA_USBTX_OFFSET))
#define LH7A400_DMA_USBRX \
	((lh7a400_dmachan_t*) (LH7A400_DMA_BASE + DMA_USBRX_OFFSET))
#define LH7A400_DMA_MMCTX \
	((lh7a400_dmachan_t*) (LH7A400_DMA_BASE + DMA_MMCTX_OFFSET))
#define LH7A400_DMA_MMCRX \
	((lh7a400_dmachan_t*) (LH7A400_DMA_BASE + DMA_MMCRX_OFFSET))
#define LH7A400_AC97RX(n) \
	((lh7a400_dmachan_t*) (LH7A400_AC97_BASE + \
	((2*n) * sizeof(lh7a400_dmachan_t))))
#define LH7A400_AC97TX(n) \
	((lh7a400_dmachan_t*) (LH7A400_AC97_BASE + \
	(((2*n)+1) * sizeof(lh7a400_dmachan_t))))

#endif  /* __LH7A400_H__ */
