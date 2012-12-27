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
 *  linux/include/asm-arm/arch-pxa/hardware.h
 *
 *  Author:	Nicolas Pitre
 *  Created:	Jun 15, 2001
 *  Copyright:	MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Note: This file was taken from linux-2.4.19-rmk4-pxa1
 *
 * - 2003/01/20 implementation specifics activated
 *   Robert Schwebel <r.schwebel@pengutronix.de>
 */

#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <linux/config.h>
#include <asm/mach-types.h>


/*
 * These are statically mapped PCMCIA IO space for designs using it as a
 * generic IO bus, typically with ISA parts, hardwired IDE interfaces, etc.
 * The actual PCMCIA code is mapping required IO region at run time.
 */
#define PCMCIA_IO_0_BASE	0xf6000000
#define PCMCIA_IO_1_BASE	0xf7000000


/*
 * We requires absolute addresses.
 */
#define PCIO_BASE		0

/*
 * Workarounds for at least 2 errata so far require this.
 * The mapping is set in mach-pxa/generic.c.
 */
#define UNCACHED_PHYS_0		0xff000000
#define UNCACHED_ADDR		UNCACHED_PHYS_0

/*
 * Intel PXA internal I/O mappings:
 *
 * 0x40000000 - 0x41ffffff <--> 0xf8000000 - 0xf9ffffff
 * 0x44000000 - 0x45ffffff <--> 0xfa000000 - 0xfbffffff
 * 0x48000000 - 0x49ffffff <--> 0xfc000000 - 0xfdffffff
 */

/* FIXME: Only this does work for u-boot... find out why... [RS] */
#define UBOOT_REG_FIX 1

#ifndef UBOOT_REG_FIX
#ifndef __ASSEMBLY__

#define io_p2v(x)	( ((x) | 0xbe000000) ^ (~((x) >> 1) & 0x06000000) )
#define io_v2p( x )	( ((x) & 0x41ffffff) ^ ( ((x) & 0x06000000) << 1) )

/*
 * This __REG() version gives the same results as the one above,  except
 * that we are fooling gcc somehow so it generates far better and smaller
 * assembly code for access to contigous registers.  It's a shame that gcc
 * doesn't guess this by itself.
 */
#include <asm/types.h>
typedef struct { volatile u32 offset[4096]; } __regbase;
# define __REGP(x)	((__regbase *)((x)&~4095))->offset[((x)&4095)>>2]
# define __REG(x)	__REGP(io_p2v(x))
#endif

/* Let's kick gcc's ass again... */
# define __REG2(x,y)	\
	( __builtin_constant_p(y) ? (__REG((x) + (y))) \
				  : (*(volatile u32 *)((u32)&__REG(x) + (y))) )

# define __PREG(x)	(io_v2p((u32)&(x)))

#else

# define __REG(x)	io_p2v(x)
# define __PREG(x)	io_v2p(x)

# undef io_p2v
# undef __REG
# ifndef __ASSEMBLY__
#  define io_p2v(PhAdd)	   (PhAdd)
#  define __REG(x)	(*((volatile u32 *)io_p2v(x)))
#  define __REG2(x,y)	(*(volatile u32 *)((u32)&__REG(x) + (y)))
# else
#  define __REG(x) (x)
#  ifdef CONFIG_CPU_MONAHANS /* Hack to make this work with mona's pxa-regs.h */
#   define __REG_2(x) (x)
#   define __REG_3(x) (x)
#  endif
# endif
#endif /* UBOOT_REG_FIX */

#include "pxa-regs.h"

#ifndef __ASSEMBLY__

/*
 * GPIO edge detection for IRQs:
 * IRQs are generated on Falling-Edge, Rising-Edge, or both.
 * This must be called *before* the corresponding IRQ is registered.
 * Use this instead of directly setting GRER/GFER.
 */
#define GPIO_FALLING_EDGE	1
#define GPIO_RISING_EDGE	2
#define GPIO_BOTH_EDGES		3
extern void set_GPIO_IRQ_edge( int gpio_nr, int edge_mask );

/*
 * Handy routine to set GPIO alternate functions
 */
extern void set_GPIO_mode( int gpio_mode );

/*
 * return current lclk frequency in units of 10kHz
 */
extern unsigned int get_lclk_frequency_10khz(void);

#endif


/*
 * Implementation specifics
 */

#ifdef CONFIG_ARCH_LUBBOCK
#include "lubbock.h"
#endif

#ifdef CONFIG_ARCH_PXA_IDP
#include "idp.h"
#endif

#ifdef CONFIG_ARCH_PXA_CERF
#include "cerf.h"
#endif

#ifdef CONFIG_ARCH_CSB226
#include "csb226.h"
#endif

#ifdef CONFIG_ARCH_INNOKOM
#include "innokom.h"
#endif

#ifdef CONFIG_ARCH_PLEB
#include "pleb.h"
#endif

#endif	/* _ASM_ARCH_HARDWARE_H */
