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

#include <common.h>
#include <asm/io.h>
#include "davinci.h"
#include <asm/arch/hardware.h>

/* MUSB platform configuration */
struct musb_config musb_cfg = {
	(struct	musb_regs *)MENTOR_USB0_BASE,
	DAVINCI_USB_TIMEOUT,
	0
};

/* MUSB module register overlay */
struct davinci_usb_regs *dregs;

/*
 * Enable the USB phy
 */
static u8 phy_on(void)
{
	u32 timeout;
#ifdef DAVINCI_DM365EVM
	u32 val;
#endif
	/* Wait until the USB phy is turned on */
#ifdef DAVINCI_DM365EVM
	writel(USBPHY_PHY24MHZ | USBPHY_SESNDEN |
			USBPHY_VBDTCTEN, USBPHY_CTL_PADDR);
#else
	writel(USBPHY_SESNDEN | USBPHY_VBDTCTEN, USBPHY_CTL_PADDR);
#endif
	timeout = musb_cfg.timeout;

#ifdef DAVINCI_DM365EVM
	/* Set the ownership of GIO33 to USB */
	val = readl(PINMUX4);
	val &= ~(PINMUX4_USBDRVBUS_BITCLEAR);
	val |= PINMUX4_USBDRVBUS_BITSET;
	writel(val, PINMUX4);
#endif
	while (timeout--)
		if (readl(USBPHY_CTL_PADDR) & USBPHY_PHYCLKGD)
			return 1;

	/* USB phy was not turned on */
	return 0;
}

/*
 * Disable the USB phy
 */
static void phy_off(void)
{
	/* powerdown the on-chip PHY and its oscillator */
	writel(USBPHY_OSCPDWN | USBPHY_PHYPDWN, USBPHY_CTL_PADDR);
}

/*
 * This function performs Davinci platform specific initialization for usb0.
 */
int musb_platform_init(void)
{
	u32  revision;

	/* enable USB VBUS */
#ifndef DAVINCI_DM365EVM
	enable_vbus();
#endif
	/* start the on-chip USB phy and its pll */
	if (!phy_on())
		return -1;

	/* reset the controller */
	dregs = (struct davinci_usb_regs *)DAVINCI_USB0_BASE;
	writel(1, &dregs->ctrlr);
	udelay(5000);

	/* Returns zero if e.g. not clocked */
	revision = readl(&dregs->version);
	if (!revision)
		return -1;

	/* Disable all interrupts */
	writel(DAVINCI_USB_USBINT_MASK | DAVINCI_USB_RXINT_MASK |
			DAVINCI_USB_TXINT_MASK , &dregs->intmsksetr);
	return 0;
}

/*
 * This function performs Davinci platform specific deinitialization for usb0.
 */
void musb_platform_deinit(void)
{
	/* Turn of the phy */
	phy_off();

	/* flush any interrupts */
	writel(DAVINCI_USB_USBINT_MASK | DAVINCI_USB_TXINT_MASK |
			DAVINCI_USB_RXINT_MASK , &dregs->intclrr);
}
