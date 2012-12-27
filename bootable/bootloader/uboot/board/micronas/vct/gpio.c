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
 * (C) Copyright 2008 Stefan Roese <sr@denx.de>, DENX Software Engineering
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
#include "vct.h"

/*
 * Find out to which of the 2 gpio modules the pin specified in the
 * argument belongs:
 * GPIO_MODULE yields 0 for pins  0 to 31,
 *                    1 for pins 32 to 63
 */
#define GPIO_MODULE(pin)	((pin) >> 5)

/*
 * Bit position within a 32-bit peripheral register (where every
 * bit is one bitslice)
 */
#define MASK(pin)		(1 << ((pin) & 0x1F))
#define BASE_ADDR(mod)		module_base[mod]

/*
 * Lookup table for transforming gpio module number 0 to 2 to
 * address offsets
 */
static u32 module_base[] = {
	GPIO1_BASE,
	GPIO2_BASE
};

static void clrsetbits(u32 addr, u32 and_mask, u32 or_mask)
{
	reg_write(addr, (reg_read(addr) & ~and_mask) | or_mask);
}

int vct_gpio_dir(int pin, int dir)
{
	u32 gpio_base;

	gpio_base = BASE_ADDR(GPIO_MODULE(pin));

	if (dir == 0)
		clrsetbits(GPIO_SWPORTA_DDR(gpio_base), MASK(pin), 0);
	else
		clrsetbits(GPIO_SWPORTA_DDR(gpio_base), 0, MASK(pin));

	return 0;
}

void vct_gpio_set(int pin, int val)
{
	u32 gpio_base;

	gpio_base = BASE_ADDR(GPIO_MODULE(pin));

	if (val == 0)
		clrsetbits(GPIO_SWPORTA_DR(gpio_base), MASK(pin), 0);
	else
		clrsetbits(GPIO_SWPORTA_DR(gpio_base), 0, MASK(pin));
}

int vct_gpio_get(int pin)
{
	u32 gpio_base;
	u32 value;

	gpio_base = BASE_ADDR(GPIO_MODULE(pin));
	value = reg_read(GPIO_EXT_PORTA(gpio_base));

	return ((value & MASK(pin)) ? 1 : 0);
}
