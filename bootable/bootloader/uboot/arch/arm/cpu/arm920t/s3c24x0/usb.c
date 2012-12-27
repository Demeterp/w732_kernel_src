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
 * (C) Copyright 2006
 * DENX Software Engineering <mk@denx.de>
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

#include <common.h>

#if defined(CONFIG_USB_OHCI_NEW) && \
    defined(CONFIG_SYS_USB_OHCI_CPU_INIT) && \
    defined(CONFIG_S3C24X0)

#include <asm/arch/s3c24x0_cpu.h>
#include <asm/io.h>

int usb_cpu_init(void)
{
	struct s3c24x0_clock_power *clk_power = s3c24x0_get_base_clock_power();
	struct s3c24x0_gpio *gpio = s3c24x0_get_base_gpio();

	/*
	 * Set the 48 MHz UPLL clocking. Values are taken from
	 * "PLL value selection guide", 6-23, s3c2400_UM.pdf.
	 */
	writel((40 << 12) + (1 << 4) + 2, &clk_power->UPLLCON);
	/* 1 = use pads related USB for USB host */
	writel(readl(&gpio->MISCCR) | 0x8, &gpio->MISCCR);

	/*
	 * Enable USB host clock.
	 */
	writel(readl(&clk_power->CLKCON) | (1 << 4), &clk_power->CLKCON);

	return 0;
}

int usb_cpu_stop(void)
{
	struct s3c24x0_clock_power *clk_power = s3c24x0_get_base_clock_power();
	/* may not want to do this */
	writel(readl(&clk_power->CLKCON) & ~(1 << 4), &clk_power->CLKCON);
	return 0;
}

int usb_cpu_init_fail(void)
{
	struct s3c24x0_clock_power *clk_power = s3c24x0_get_base_clock_power();
	writel(readl(&clk_power->CLKCON) & ~(1 << 4), &clk_power->CLKCON);
	return 0;
}

#endif /* defined(CONFIG_USB_OHCI_NEW) && \
	   defined(CONFIG_SYS_USB_OHCI_CPU_INIT) && \
	   defined(CONFIG_S3C24X0) */
