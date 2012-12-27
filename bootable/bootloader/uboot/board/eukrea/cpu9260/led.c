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
 * Copyright (c) 2009 Wind River Systems, Inc.
 * Tom Rix <Tom.Rix@windriver.com>
 * (C) Copyright 2009
 * Eric Benard <eric@eukrea.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <status_led.h>
#include <asm/arch/at91sam9260.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/gpio.h>
#include <asm/arch/io.h>

static unsigned int saved_state[4] = {STATUS_LED_OFF, STATUS_LED_OFF,
		STATUS_LED_OFF, STATUS_LED_OFF};

void coloured_LED_init(void)
{
	/* Enable clock */
	at91_sys_write(AT91_PMC_PCER, 1 << AT91SAM9260_ID_PIOC);

	at91_set_gpio_output(CONFIG_RED_LED, 1);
	at91_set_gpio_output(CONFIG_GREEN_LED, 1);
	at91_set_gpio_output(CONFIG_YELLOW_LED, 1);
	at91_set_gpio_output(CONFIG_BLUE_LED, 1);

	at91_set_gpio_value(CONFIG_RED_LED, 1);
	at91_set_gpio_value(CONFIG_GREEN_LED, 1);
	at91_set_gpio_value(CONFIG_YELLOW_LED, 1);
	at91_set_gpio_value(CONFIG_BLUE_LED, 1);
}

void red_LED_off(void)
{
	at91_set_gpio_value(CONFIG_RED_LED, 1);
	saved_state[STATUS_LED_RED] = STATUS_LED_OFF;
}

void green_LED_off(void)
{
	at91_set_gpio_value(CONFIG_GREEN_LED, 1);
	saved_state[STATUS_LED_GREEN] = STATUS_LED_OFF;
}

void yellow_LED_off(void)
{
	at91_set_gpio_value(CONFIG_YELLOW_LED, 1);
	saved_state[STATUS_LED_YELLOW] = STATUS_LED_OFF;
}

void blue_LED_off(void)
{
	at91_set_gpio_value(CONFIG_BLUE_LED, 1);
	saved_state[STATUS_LED_BLUE] = STATUS_LED_OFF;
}

void red_LED_on(void)
{
	at91_set_gpio_value(CONFIG_RED_LED, 0);
	saved_state[STATUS_LED_RED] = STATUS_LED_ON;
}

void green_LED_on(void)
{
	at91_set_gpio_value(CONFIG_GREEN_LED, 0);
	saved_state[STATUS_LED_GREEN] = STATUS_LED_ON;
}

void yellow_LED_on(void)
{
	at91_set_gpio_value(CONFIG_YELLOW_LED, 0);
	saved_state[STATUS_LED_YELLOW] = STATUS_LED_ON;
}

void blue_LED_on(void)
{
	at91_set_gpio_value(CONFIG_BLUE_LED, 0);
	saved_state[STATUS_LED_BLUE] = STATUS_LED_ON;
}

void __led_init(led_id_t mask, int state)
{
	__led_set(mask, state);
}

void __led_toggle(led_id_t mask)
{
	if (STATUS_LED_BLUE == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_BLUE])
			blue_LED_off();
		else
			blue_LED_on();
	} else if (STATUS_LED_RED == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_RED])
			red_LED_off();
		else
			red_LED_on();
	} else if (STATUS_LED_GREEN == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_GREEN])
			green_LED_off();
		else
			green_LED_on();
	} else if (STATUS_LED_YELLOW == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_YELLOW])
			yellow_LED_off();
		else
			yellow_LED_on();
	}
}

void __led_set(led_id_t mask, int state)
{
	if (STATUS_LED_BLUE == mask) {
		if (STATUS_LED_ON == state)
			blue_LED_on();
		else
			blue_LED_off();
	} else if (STATUS_LED_RED == mask) {
		if (STATUS_LED_ON == state)
			red_LED_on();
		else
			red_LED_off();
	} else if (STATUS_LED_GREEN == mask) {
		if (STATUS_LED_ON == state)
			green_LED_on();
		else
			green_LED_off();
	} else if (STATUS_LED_YELLOW == mask) {
		if (STATUS_LED_ON == state)
			yellow_LED_on();
		else
			yellow_LED_off();
	}
}
