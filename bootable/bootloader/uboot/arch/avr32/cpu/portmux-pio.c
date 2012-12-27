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
 * Copyright (C) 2006, 2008 Atmel Corporation
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
	if (flags & PORTMUX_PULL_UP)
		pio_writel(port, PUER, pin_mask);
	else
		pio_writel(port, PUDR, pin_mask);

	switch (func) {
	case PORTMUX_FUNC_A:
		pio_writel(port, ASR, pin_mask);
		break;
	case PORTMUX_FUNC_B:
		pio_writel(port, BSR, pin_mask);
		break;
	}

	pio_writel(port, PDR, pin_mask);
}

void portmux_select_gpio(void *port, unsigned long pin_mask,
		unsigned long flags)
{
	if (flags & PORTMUX_PULL_UP)
		pio_writel(port, PUER, pin_mask);
	else
		pio_writel(port, PUDR, pin_mask);

	if (flags & PORTMUX_OPEN_DRAIN)
		pio_writel(port, MDER, pin_mask);
	else
		pio_writel(port, MDDR, pin_mask);

	if (flags & PORTMUX_DIR_OUTPUT) {
		if (flags & PORTMUX_INIT_HIGH)
			pio_writel(port, SODR, pin_mask);
		else
			pio_writel(port, CODR, pin_mask);
		pio_writel(port, OER, pin_mask);
	} else {
		pio_writel(port, ODR, pin_mask);
	}

	pio_writel(port, PER, pin_mask);
}

void pio_set_output_value(unsigned int pin, int value)
{
	void *port = pio_pin_to_port(pin);

	if (!port)
		panic("Invalid GPIO pin %u\n", pin);

	__pio_set_output_value(port, pin & 0x1f, value);
}

int pio_get_input_value(unsigned int pin)
{
	void *port = pio_pin_to_port(pin);

	if (!port)
		panic("Invalid GPIO pin %u\n", pin);

	return __pio_get_input_value(port, pin & 0x1f);
}
