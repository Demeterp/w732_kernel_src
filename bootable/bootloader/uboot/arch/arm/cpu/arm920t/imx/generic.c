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
 *  arch/arm/mach-imx/generic.c
 *
 *  author: Sascha Hauer
 *  Created: april 20th, 2004
 *  Copyright: Synertronixx GmbH
 *
 *  Common code for i.MX machines
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>

#ifdef CONFIG_IMX

#include <asm/arch/imx-regs.h>

void imx_gpio_mode(int gpio_mode)
{
	unsigned int pin = gpio_mode & GPIO_PIN_MASK;
	unsigned int port = (gpio_mode & GPIO_PORT_MASK) >> 5;
	unsigned int ocr = (gpio_mode & GPIO_OCR_MASK) >> 10;
	unsigned int tmp;

	/* Pullup enable */
	if(gpio_mode & GPIO_PUEN)
		PUEN(port) |= (1<<pin);
	else
		PUEN(port) &= ~(1<<pin);

	/* Data direction */
	if(gpio_mode & GPIO_OUT)
		DDIR(port) |= 1<<pin;
	else
		DDIR(port) &= ~(1<<pin);

	/* Primary / alternate function */
	if(gpio_mode & GPIO_AF)
		GPR(port) |= (1<<pin);
	else
		GPR(port) &= ~(1<<pin);

	/* use as gpio? */
	if( ocr == 3 )
		GIUS(port) |= (1<<pin);
	else
		GIUS(port) &= ~(1<<pin);

	/* Output / input configuration */
	/* FIXME: I'm not very sure about OCR and ICONF, someone
	 * should have a look over it
	 */
	if(pin<16) {
		tmp = OCR1(port);
		tmp &= ~( 3<<(pin*2));
		tmp |= (ocr << (pin*2));
		OCR1(port) = tmp;

		if( gpio_mode &	GPIO_AOUT )
			ICONFA1(port) &= ~( 3<<(pin*2));
		if( gpio_mode &	GPIO_BOUT )
			ICONFB1(port) &= ~( 3<<(pin*2));
	} else {
		tmp = OCR2(port);
		tmp &= ~( 3<<((pin-16)*2));
		tmp |= (ocr << ((pin-16)*2));
		OCR2(port) = tmp;

		if( gpio_mode &	GPIO_AOUT )
			ICONFA2(port) &= ~( 3<<((pin-16)*2));
		if( gpio_mode &	GPIO_BOUT )
			ICONFB2(port) &= ~( 3<<((pin-16)*2));
	}
}

#endif /* CONFIG_IMX */
