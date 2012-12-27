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
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

/**
 * hcWriteWord - write a 16 bit value into the USB controller
 * @base: base address to access the chip registers
 * @value: 16 bit value to write into register @offset
 * @offset: register to write the @value into
 *
 */
static void inline hcWriteWord (unsigned long base, unsigned int value,
				unsigned int offset)
{
	out_le16 ((volatile u16*)(base + 2), offset | 0x80);
	out_le16 ((volatile u16*)base, value);
}

/**
 * hcWriteDWord - write a 32 bit value into the USB controller
 * @base: base address to access the chip registers
 * @value: 32 bit value to write into register @offset
 * @offset: register to write the @value into
 *
 */

static void inline hcWriteDWord (unsigned long base, unsigned long value,
				unsigned int offset)
{
	out_le16 ((volatile u16*)(base + 2), offset | 0x80);
	out_le16 ((volatile u16*)base, value);
	out_le16 ((volatile u16*)base, value >> 16);
}

/**
 * hcReadWord - read a 16 bit value from the USB controller
 * @base: base address to access the chip registers
 * @offset: register to read from
 *
 * Returns the readed register value
 */

static unsigned int inline hcReadWord (unsigned long base, unsigned int offset)
{
	out_le16 ((volatile u16*)(base + 2), offset);
	return (in_le16 ((volatile u16*)base));
}

/**
 * hcReadDWord - read a 32 bit value from the USB controller
 * @base: base address to access the chip registers
 * @offset: register to read from
 *
 * Returns the readed register value
 */

static unsigned long inline hcReadDWord (unsigned long base, unsigned int offset)
{
	unsigned long val, val16;

	out_le16 ((volatile u16*)(base + 2), offset);
	val = in_le16((volatile u16*)base);
	val16 = in_le16((volatile u16*)base);
	return (val | (val16 << 16));
}

/* control and status registers isp1161 */
#define HcRevision		0x00
#define HcControl		0x01
#define HcCommandStatus		0x02
#define HcInterruptStatus	0x03
#define HcInterruptEnable	0x04
#define HcInterruptDisable	0x05
#define HcFmInterval		0x0D
#define HcFmRemaining		0x0E
#define HcFmNumber		0x0F
#define HcLSThreshold		0x11
#define HcRhDescriptorA		0x12
#define HcRhDescriptorB		0x13
#define HcRhStatus		0x14
#define HcRhPortStatus1		0x15
#define HcRhPortStatus2		0x16

#define HcHardwareConfiguration 0x20
#define HcDMAConfiguration	0x21
#define HcTransferCounter	0x22
#define HcuPInterrupt		0x24
#define HcuPInterruptEnable	0x25
#define HcChipID		0x27
#define HcScratch		0x28
#define HcSoftwareReset		0x29
#define HcITLBufferLength	0x2A
#define HcATLBufferLength	0x2B
#define HcBufferStatus		0x2C
#define HcReadBackITL0Length	0x2D
#define HcReadBackITL1Length	0x2E
#define HcITLBufferPort		0x40
#define HcATLBufferPort		0x41
