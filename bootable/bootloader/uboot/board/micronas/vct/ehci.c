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
 * (C) Copyright 2009 Stefan Roese <sr@denx.de>, DENX Software Engineering
 *
 * Original Author Guenter Gebhardt
 * Copyright (C) 2006 Micronas GmbH
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

#include <common.h>

#include "vct.h"

int vct_ehci_hcd_init(u32 *hccr, u32 *hcor)
{
	int retval;
	u32 val;
	u32 addr;

	dcgu_set_reset_switch(DCGU_HW_MODULE_USB_24, DCGU_SWITCH_ON);
	dcgu_set_reset_switch(DCGU_HW_MODULE_USB_60, DCGU_SWITCH_ON);
	dcgu_set_clk_switch(DCGU_HW_MODULE_USB_24, DCGU_SWITCH_ON);
	dcgu_set_clk_switch(DCGU_HW_MODULE_USB_PLL, DCGU_SWITCH_ON);
	dcgu_set_reset_switch(DCGU_HW_MODULE_USB_24, DCGU_SWITCH_OFF);

	/* Wait until (DCGU_USBPHY_STAT == 7) */
	addr = DCGU_USBPHY_STAT(DCGU_BASE);
	val = reg_read(addr);
	while (val != 7)
		val = reg_read(addr);

	dcgu_set_clk_switch(DCGU_HW_MODULE_USB_60, DCGU_SWITCH_ON);
	dcgu_set_reset_switch(DCGU_HW_MODULE_USB_60, DCGU_SWITCH_OFF);

	retval = scc_reset(SCC_USB_RW, 0);
	if (retval) {
		printf("scc_reset(SCC_USB_RW, 0) returned: 0x%x\n", retval);
		return retval;
	} else {
		retval = scc_reset(SCC_CPU1_SPDMA_RW, 0);
		if (retval) {
			printf("scc_reset(SCC_CPU1_SPDMA_RW, 0) returned: 0x%x\n",
			       retval);
			return retval;
		}
	}

	if (!retval) {
		/*
		 * For the AGU bypass, where the  SCC client provides full
		 * physical address
		 */
		scc_set_usb_address_generation_mode(1);
		scc_setup_dma(SCC_USB_RW, BCU_USB_BUFFER_1, DMA_LINEAR,
			      USE_NO_FH, DMA_READ, 0);
		scc_setup_dma(SCC_CPU1_SPDMA_RW, BCU_USB_BUFFER_1, DMA_LINEAR,
			      USE_NO_FH, DMA_WRITE, 0);
		scc_setup_dma(SCC_USB_RW, BCU_USB_BUFFER_0, DMA_LINEAR,
			      USE_NO_FH, DMA_WRITE, 0);
		scc_setup_dma(SCC_CPU1_SPDMA_RW, BCU_USB_BUFFER_0, DMA_LINEAR,
			      USE_NO_FH, DMA_READ, 0);

		/* Enable memory interface */
		scc_enable(SCC_USB_RW, 1);

		/* Start (start_cmd=0) DMAs */
		scc_dma_cmd(SCC_USB_RW, DMA_START, 0, DMA_READ);
		scc_dma_cmd(SCC_USB_RW, DMA_START, 0, DMA_WRITE);
	} else {
		printf("Cannot configure USB memory channel.\n");
		printf("USB can not access RAM. SCC configuration failed.\n");
		return retval;
	}

	/* Wait a short while */
	udelay(300000);

	reg_write(USBH_BURSTSIZE(USBH_BASE), 0x00001c1c);

	/* Set EHCI structures and DATA in RAM */
	reg_write(USBH_USBHMISC(USBH_BASE), 0x00840003);
	/* Set USBMODE to bigendian and set host mode */
	reg_write(USBH_USBMODE(USBH_BASE), 0x00000007);

	/*
	 * USBH_BURSTSIZE MUST EQUAL 0x00001c1c in order for
	 * 512 byte USB transfers on the bulk pipe to work properly.
	 * Set USBH_BURSTSIZE to 0x00001c1c
	 */
	reg_write(USBH_BURSTSIZE(USBH_BASE), 0x00001c1c);

	/* Insert access register addresses */
	*hccr = REG_GLOBAL_START_ADDR + USBH_CAPLENGTH(USBH_BASE);
	*hcor = REG_GLOBAL_START_ADDR + USBH_USBCMD(USBH_BASE);

	return 0;
}
