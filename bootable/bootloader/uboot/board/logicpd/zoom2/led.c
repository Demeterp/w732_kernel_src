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
#include <status_led.h>
#include <asm/arch/cpu.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/gpio.h>

static unsigned int saved_state[2] = {STATUS_LED_OFF, STATUS_LED_OFF};

/*
 * GPIO LEDs
 * 173 red
 * 154 blue
 * 61  blue2
 */
#define ZOOM2_LED_RED	173
#define ZOOM2_LED_BLUE	154
#define ZOOM2_LED_BLUE2	61

void red_LED_off (void)
{
	/* red */
	if (!omap_request_gpio(ZOOM2_LED_RED)) {
		omap_set_gpio_direction(ZOOM2_LED_RED, 0);
		omap_set_gpio_dataout(ZOOM2_LED_RED, 0);
	}
	saved_state[STATUS_LED_RED] = STATUS_LED_OFF;
}

void blue_LED_off (void)
{
	/* blue */
	if (!omap_request_gpio(ZOOM2_LED_BLUE)) {
		omap_set_gpio_direction(ZOOM2_LED_BLUE, 0);
		omap_set_gpio_dataout(ZOOM2_LED_BLUE, 0);
	}

	/* blue 2 */
	if (!omap_request_gpio(ZOOM2_LED_BLUE2)) {
		omap_set_gpio_direction(ZOOM2_LED_BLUE2, 0);
		omap_set_gpio_dataout(ZOOM2_LED_BLUE2, 0);
	}
	saved_state[STATUS_LED_BLUE] = STATUS_LED_OFF;
}

void red_LED_on (void)
{
	blue_LED_off ();

	/* red */
	if (!omap_request_gpio(ZOOM2_LED_RED)) {
		omap_set_gpio_direction(ZOOM2_LED_RED, 0);
		omap_set_gpio_dataout(ZOOM2_LED_RED, 1);
	}
	saved_state[STATUS_LED_RED] = STATUS_LED_ON;
}

void blue_LED_on (void)
{
	red_LED_off ();

	/* blue */
	if (!omap_request_gpio(ZOOM2_LED_BLUE)) {
		omap_set_gpio_direction(ZOOM2_LED_BLUE, 0);
		omap_set_gpio_dataout(ZOOM2_LED_BLUE, 1);
	}

	/* blue 2 */
	if (!omap_request_gpio(ZOOM2_LED_BLUE2)) {
		omap_set_gpio_direction(ZOOM2_LED_BLUE2, 0);
		omap_set_gpio_dataout(ZOOM2_LED_BLUE2, 1);
	}

	saved_state[STATUS_LED_BLUE] = STATUS_LED_ON;
}

void __led_init (led_id_t mask, int state)
{
	__led_set (mask, state);
}

void __led_toggle (led_id_t mask)
{
	if (STATUS_LED_BLUE == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_BLUE])
			blue_LED_off ();
		else
			blue_LED_on ();
	} else if (STATUS_LED_RED == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_RED])
			red_LED_off ();
		else
			red_LED_on ();
	}
}

void __led_set (led_id_t mask, int state)
{
	if (STATUS_LED_BLUE == mask) {
		if (STATUS_LED_ON == state)
			blue_LED_on ();
		else
			blue_LED_off ();
	} else if (STATUS_LED_RED == mask) {
		if (STATUS_LED_ON == state)
			red_LED_on ();
		else
			red_LED_off ();
	}
}
