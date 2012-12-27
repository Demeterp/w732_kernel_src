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
 * gpio_cfi_flash.c - GPIO-assisted Flash Chip Support
 *
 * Copyright (c) 2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <asm/blackfin.h>
#include <asm/io.h>
#include "gpio_cfi_flash.h"

#define GPIO_PIN_1  PH9
#define GPIO_MASK_1 (1 << 21)
#define GPIO_PIN_2  PG11
#define GPIO_MASK_2 (1 << 22)
#define GPIO_MASK   (GPIO_MASK_1 | GPIO_MASK_2)

void *gpio_cfi_flash_swizzle(void *vaddr)
{
	unsigned long addr = (unsigned long)vaddr;

	if (addr & GPIO_MASK_1)
		bfin_write_PORTHIO_SET(GPIO_PIN_1);
	else
		bfin_write_PORTHIO_CLEAR(GPIO_PIN_1);

#ifdef GPIO_MASK_2
	if (addr & GPIO_MASK_2)
		bfin_write_PORTGIO_SET(GPIO_PIN_2);
	else
		bfin_write_PORTGIO_CLEAR(GPIO_PIN_2);
#endif

	SSYNC();

	return (void *)(addr & ~GPIO_MASK);
}

#define __raw_writeq(value, addr) *(volatile u64 *)addr = value
#define __raw_readq(addr) *(volatile u64 *)addr

#define MAKE_FLASH(size, sfx) \
void flash_write##size(u##size value, void *addr) \
{ \
	__raw_write##sfx(value, gpio_cfi_flash_swizzle(addr)); \
} \
u##size flash_read##size(void *addr) \
{ \
	return __raw_read##sfx(gpio_cfi_flash_swizzle(addr)); \
}
MAKE_FLASH(8, b)  /* flash_write8()  flash_read8() */
MAKE_FLASH(16, w) /* flash_write16() flash_read16() */
MAKE_FLASH(32, l) /* flash_write32() flash_read32() */
MAKE_FLASH(64, q) /* flash_write64() flash_read64() */

void gpio_cfi_flash_init(void)
{
	bfin_write_PORTHIO_DIR(bfin_read_PORTHIO_DIR() | GPIO_PIN_1);
	bfin_write_PORTGIO_DIR(bfin_read_PORTGIO_DIR() | GPIO_PIN_2);
	gpio_cfi_flash_swizzle((void *)CONFIG_SYS_FLASH_BASE);
}
