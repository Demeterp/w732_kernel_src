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
 * Memory Setup stuff - taken from blob memsetup.S
 *
 * Copyright (C) 2009 Jens Scharsig (js_at_ng@scharsoft.de)
 *
 *  Copyright (C) 2005 HP Labs
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

#include <config.h>
#include <common.h>
#include <asm/sizes.h>
#include <asm/arch/hardware.h>
#include <asm/arch/io.h>
#include <asm/arch/at91_pio.h>

int at91_set_pio_pullup(unsigned port, unsigned pin, int use_pullup)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		if (use_pullup)
			writel(1 << pin, &pio->port[port].puer);
		else
			writel(1 << pin, &pio->port[port].pudr);
		writel(mask, &pio->port[port].per);
	}
	return 0;
}

/*
 * mux the pin to the "GPIO" peripheral role.
 */
int at91_set_pio_periph(unsigned port, unsigned pin, int use_pullup)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		writel(mask, &pio->port[port].idr);
		at91_set_pio_pullup(port, pin, use_pullup);
		writel(mask, &pio->port[port].per);
	}
	return 0;
}

/*
 * mux the pin to the "A" internal peripheral role.
 */
int at91_set_a_periph(unsigned port, unsigned pin, int use_pullup)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		writel(mask, &pio->port[port].idr);
		at91_set_pio_pullup(port, pin, use_pullup);
		writel(mask, &pio->port[port].asr);
		writel(mask, &pio->port[port].pdr);
	}
	return 0;
}

/*
 * mux the pin to the "B" internal peripheral role.
 */
int at91_set_b_periph(unsigned port, unsigned pin, int use_pullup)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		writel(mask, &pio->port[port].idr);
		at91_set_pio_pullup(port, pin, use_pullup);
		writel(mask, &pio->port[port].bsr);
		writel(mask, &pio->port[port].pdr);
	}
	return 0;
}

/*
 * mux the pin to the gpio controller (instead of "A" or "B" peripheral), and
 * configure it for an input.
 */
int at91_set_pio_input(unsigned port, u32 pin, int use_pullup)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		writel(mask, &pio->port[port].idr);
		at91_set_pio_pullup(port, pin, use_pullup);
		writel(mask, &pio->port[port].odr);
		writel(mask, &pio->port[port].per);
	}
	return 0;
}

/*
 * mux the pin to the gpio controller (instead of "A" or "B" peripheral),
 * and configure it for an output.
 */
int at91_set_pio_output(unsigned port, u32 pin, int value)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		writel(mask, &pio->port[port].idr);
		writel(mask, &pio->port[port].pudr);
		if (value)
			writel(mask, &pio->port[port].sodr);
		else
			writel(mask, &pio->port[port].codr);
		writel(mask, &pio->port[port].oer);
		writel(mask, &pio->port[port].per);
	}
	return 0;
}

/*
 * enable/disable the glitch filter. mostly used with IRQ handling.
 */
int at91_set_pio_deglitch(unsigned port, unsigned pin, int is_on)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		if (is_on)
			writel(mask, &pio->port[port].ifer);
		else
			writel(mask, &pio->port[port].ifdr);
	}
	return 0;
}

/*
 * enable/disable the multi-driver. This is only valid for output and
 * allows the output pin to run as an open collector output.
 */
int at91_set_pio_multi_drive(unsigned port, unsigned pin, int is_on)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		if (is_on)
			writel(mask, &pio->port[port].mder);
		else
			writel(mask, &pio->port[port].mddr);
	}
	return 0;
}

/*
 * assuming the pin is muxed as a gpio output, set its value.
 */
int at91_set_pio_value(unsigned port, unsigned pin, int value)
{
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		if (value)
			writel(mask, &pio->port[port].sodr);
		else
			writel(mask, &pio->port[port].codr);
	}
	return 0;
}

/*
 * read the pin's value (works even if it's not muxed as a gpio).
 */
int at91_get_pio_value(unsigned port, unsigned pin)
{
	u32		pdsr	= 0;
	at91_pio_t	*pio 	= (at91_pio_t *) AT91_PIO_BASE;
	u32		mask;

	if ((port < AT91_PIO_PORTS) && (pin < 32)) {
		mask = 1 << pin;
		pdsr = readl(&pio->port[port].pdsr) & mask;
	}
	return pdsr != 0;
}
