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
 * TI's Davinci platform specific USB wrapper functions.
 *
 * Copyright (c) 2008 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Author: Thomas Abraham t-abraham@ti.com, Texas Instruments
 */

#ifndef __DAVINCI_USB_H__
#define __DAVINCI_USB_H__

#include <asm/arch/hardware.h>
#include "musb_core.h"

/* Base address of DAVINCI usb0 wrapper */
#define DAVINCI_USB0_BASE 0x01C64000

/* Base address of DAVINCI musb core */
#define MENTOR_USB0_BASE (DAVINCI_USB0_BASE+0x400)

/*
 * Davinci platform USB wrapper register overlay. Note: Only the required
 * registers are included in this structure. It can be expanded as required.
 */
struct davinci_usb_regs {
	u32	version;
	u32	ctrlr;
	u32	reserved[0x20];
	u32	intclrr;
	u32 	intmskr;
	u32 	intmsksetr;
};

#define DAVINCI_USB_TX_ENDPTS_MASK	0x1f /* ep0 + 4 tx */
#define DAVINCI_USB_RX_ENDPTS_MASK	0x1e /* 4 rx */
#define DAVINCI_USB_USBINT_SHIFT	16
#define DAVINCI_USB_TXINT_SHIFT 	0
#define DAVINCI_USB_RXINT_SHIFT 	8
#define DAVINCI_INTR_DRVVBUS		0x0100

#define DAVINCI_USB_USBINT_MASK 	0x01ff0000	/* 8 Mentor, DRVVBUS */
#define DAVINCI_USB_TXINT_MASK \
		(DAVINCI_USB_TX_ENDPTS_MASK << DAVINCI_USB_TXINT_SHIFT)
#define DAVINCI_USB_RXINT_MASK \
		(DAVINCI_USB_RX_ENDPTS_MASK << DAVINCI_USB_RXINT_SHIFT)
#define MGC_BUSCTL_OFFSET(_bEnd, _bOffset) \
		(0x80 + (8*(_bEnd)) + (_bOffset))

/* Integrated highspeed/otg PHY */
#define USBPHY_CTL_PADDR	(DAVINCI_SYSTEM_MODULE_BASE + 0x34)
#define USBPHY_PHY24MHZ 	(1 << 13)
#define USBPHY_PHYCLKGD 	(1 << 8)
#define USBPHY_SESNDEN		(1 << 7)	/* v(sess_end) comparator */
#define USBPHY_VBDTCTEN 	(1 << 6)	/* v(bus) comparator */
#define USBPHY_PHYPLLON 	(1 << 4)	/* override pll suspend */
#define USBPHY_CLKO1SEL 	(1 << 3)
#define USBPHY_OSCPDWN		(1 << 2)
#define USBPHY_PHYPDWN		(1 << 0)

/* Timeout for Davinci USB module */
#define DAVINCI_USB_TIMEOUT 0x3FFFFFF

/* IO Expander I2C address and VBUS enable mask */
#define IOEXP_I2C_ADDR 0x3A
#define IOEXP_VBUSEN_MASK 1

/* extern functions */
extern void lpsc_on(unsigned int id);
extern int i2c_write(uchar chip, uint addr, int alen, uchar *buffer, int len);
extern int i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len);
extern void enable_vbus(void);
#endif	/* __DAVINCI_USB_H__ */
