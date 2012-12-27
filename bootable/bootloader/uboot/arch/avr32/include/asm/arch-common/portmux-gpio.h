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
#ifndef __AVR32_PORTMUX_GPIO_H__
#define __AVR32_PORTMUX_GPIO_H__

#include <asm/io.h>

/* Register layout for this specific device */
#include <asm/arch/gpio-impl.h>

/* Register access macros */
#define gpio_readl(port, reg)						\
	__raw_readl(&((struct gpio_regs *)port)->reg)
#define gpio_writel(gpio, reg, value)					\
	__raw_writel(value, &((struct gpio_regs *)port)->reg)

/* Portmux API starts here. See doc/README.AVR32-port-muxing */

enum portmux_function {
	PORTMUX_FUNC_A,
	PORTMUX_FUNC_B,
	PORTMUX_FUNC_C,
	PORTMUX_FUNC_D,
};

#define PORTMUX_DIR_INPUT	(0 << 0)
#define PORTMUX_DIR_OUTPUT	(1 << 0)
#define PORTMUX_INIT_LOW	(0 << 1)
#define PORTMUX_INIT_HIGH	(1 << 1)
#define PORTMUX_PULL_UP		(1 << 2)
#define PORTMUX_PULL_DOWN	(2 << 2)
#define PORTMUX_BUSKEEPER	(3 << 2)
#define PORTMUX_DRIVE_MIN	(0 << 4)
#define PORTMUX_DRIVE_LOW	(1 << 4)
#define PORTMUX_DRIVE_HIGH	(2 << 4)
#define PORTMUX_DRIVE_MAX	(3 << 4)
#define PORTMUX_OPEN_DRAIN	(1 << 6)

void portmux_select_peripheral(void *port, unsigned long pin_mask,
		enum portmux_function func, unsigned long flags);
void portmux_select_gpio(void *port, unsigned long pin_mask,
		unsigned long flags);

/* Internal helper functions */

static inline void *gpio_pin_to_port(unsigned int pin)
{
	return (void *)GPIO_BASE + (pin >> 5) * 0x200;
}

static inline void __gpio_set_output_value(void *port, unsigned int pin,
		int value)
{
	if (value)
		gpio_writel(port, OVRS, 1 << pin);
	else
		gpio_writel(port, OVRC, 1 << pin);
}

static inline int __gpio_get_input_value(void *port, unsigned int pin)
{
	return (gpio_readl(port, PVR) >> pin) & 1;
}

void gpio_set_output_value(unsigned int pin, int value);
int gpio_get_input_value(unsigned int pin);

/* GPIO API starts here */

/*
 * GCC doesn't realize that the constant case is extremely trivial,
 * so we need to help it make the right decision by using
 * always_inline.
 */
__attribute__((always_inline))
static inline void gpio_set_value(unsigned int pin, int value)
{
	if (__builtin_constant_p(pin))
		__gpio_set_output_value(gpio_pin_to_port(pin),
				pin & 0x1f, value);
	else
		gpio_set_output_value(pin, value);
}

__attribute__((always_inline))
static inline int gpio_get_value(unsigned int pin)
{
	if (__builtin_constant_p(pin))
		return __gpio_get_input_value(gpio_pin_to_port(pin),
				pin & 0x1f);
	else
		return gpio_get_input_value(pin);
}

#endif /* __AVR32_PORTMUX_GPIO_H__ */
