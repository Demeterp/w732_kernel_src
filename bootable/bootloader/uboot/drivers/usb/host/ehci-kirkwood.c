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
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Prafulla Wadaskar <prafulla@marvell.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <common.h>
#include <asm/io.h>
#include <usb.h>
#include "ehci.h"
#include "ehci-core.h"
#include <asm/arch/kirkwood.h>

#define rdl(off)	readl(KW_USB20_BASE + (off))
#define wrl(off, val)	writel((val), KW_USB20_BASE + (off))

#define USB_WINDOW_CTRL(i)	(0x320 + ((i) << 4))
#define USB_WINDOW_BASE(i)	(0x324 + ((i) << 4))
#define USB_TARGET_DRAM		0x0

/*
 * USB 2.0 Bridge Address Decoding registers setup
 */
static void usb_brg_adrdec_setup(void)
{
	int i;
	u32 size, attrib;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {

		/* Enable DRAM bank */
		switch (i) {
		case 0:
			attrib = KWCPU_ATTR_DRAM_CS0;
			break;
		case 1:
			attrib = KWCPU_ATTR_DRAM_CS1;
			break;
		case 2:
			attrib = KWCPU_ATTR_DRAM_CS2;
			break;
		case 3:
			attrib = KWCPU_ATTR_DRAM_CS3;
			break;
		default:
			/* invalide bank, disable access */
			attrib = 0;
			break;
		}

		size = kw_sdram_bs(i);
		if ((size) && (attrib))
			wrl(USB_WINDOW_CTRL(i),
				KWCPU_WIN_CTRL_DATA(size, USB_TARGET_DRAM,
					attrib, KWCPU_WIN_ENABLE));
		else
			wrl(USB_WINDOW_CTRL(i), KWCPU_WIN_DISABLE);

		wrl(USB_WINDOW_BASE(i), kw_sdram_bar(i));
	}
}

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 */
int ehci_hcd_init(void)
{
	usb_brg_adrdec_setup();

	hccr = (struct ehci_hccr *)(KW_USB20_BASE + 0x100);
	hcor = (struct ehci_hcor *)((uint32_t) hccr
			+ HC_LENGTH(ehci_readl(&hccr->cr_capbase)));

	debug("Kirkwood-ehci: init hccr %x and hcor %x hc_length %d\n",
		(uint32_t)hccr, (uint32_t)hcor,
		(uint32_t)HC_LENGTH(ehci_readl(&hccr->cr_capbase)));

	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(void)
{
	return 0;
}
