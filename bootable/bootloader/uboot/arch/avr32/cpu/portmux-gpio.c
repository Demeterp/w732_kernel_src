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
 * Copyright (C) 2008 Atmel Corporation
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

#include <asm/io.h>
#include <asm/arch/memory-map.h>
#include <asm/arch/gpio.h>

void portmux_select_peripheral(void *port, unsigned long pin_mask,
		enum portmux_function func, unsigned long flags)
{
	/* Both pull-up and pull-down set means buskeeper */
	if (flags & PORTMUX_PULL_DOWN)
		gpio_writel(port, PDERS, pin_mask);
	else
		gpio_writel(port, PDERC, pin_mask);
	if (flags & PORTMUX_PULL_UP)
		gpio_writel(port, PUERS, pin_mask);
	else
		gpio_writel(port, PUERC, pin_mask);

	/* Select drive strength */
	if (flags & PORTMUX_DRIVE_LOW)
		gpio_writel(port, ODCR0S, pin_mask);
	else
		gpio_writel(port, ODCR0C, pin_mask);
	if (flags & PORTMUX_DRIVE_HIGH)
		gpio_writel(port, ODCR1S, pin_mask);
	else
		gpio_writel(port, ODCR1C, pin_mask);

	/* Select function */
	if (func & PORTMUX_FUNC_B)
		gpio_writel(port, PMR0S, pin_mask);
	else
		gpio_writel(port, PMR0C, pin_mask);
	if (func & PORTMUX_FUNC_C)
		gpio_writel(port, PMR1S, pin_mask);
	else
		gpio_writel(port, PMR1C, pin_mask);

	/* Disable GPIO (i.e. enable peripheral) */
	gpio_writel(port, GPERC, pin_mask);
}

void portmux_select_gpio(void *port, unsigned long pin_mask,
		unsigned long flags)
{
	/* Both pull-up and pull-down set means buskeeper */
	if (flags & PORTMUX_PULL_DOWN)
		gpio_writel(port, PDERS, pin_mask);
	else
		gpio_writel(port, PDERC, pin_mask);
	if (flags & PORTMUX_PULL_UP)
		gpio_writel(port, PUERS, pin_mask);
	else
		gpio_writel(port, PUERC, pin_mask);

	/* Enable open-drain mode if requested */
	if (flags & PORTMUX_OPEN_DRAIN)
		gpio_writel(port, ODMERS, pin_mask);
	else
		gpio_writel(port, ODMERC, pin_mask);

	/* Select drive strength */
	if (flags & PORTMUX_DRIVE_LOW)
		gpio_writel(port, ODCR0S, pin_mask);
	else
		gpio_writel(port, ODCR0C, pin_mask);
	if (flags & PORTMUX_DRIVE_HIGH)
		gpio_writel(port, ODCR1S, pin_mask);
	else
		gpio_writel(port, ODCR1C, pin_mask);

	/* Select direction and initial pin state */
	if (flags & PORTMUX_DIR_OUTPUT) {
		if (flags & PORTMUX_INIT_HIGH)
			gpio_writel(port, OVRS, pin_mask);
		else
			gpio_writel(port, OVRC, pin_mask);
		gpio_writel(port, ODERS, pin_mask);
	} else {
		gpio_writel(port, ODERC, pin_mask);
	}

	/* Enable GPIO */
	gpio_writel(port, GPERS, pin_mask);
}
