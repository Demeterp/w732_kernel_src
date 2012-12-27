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
 * Memory mapped IO
 *
 * (C) Copyright 2002
 * Hyperion Entertainment, ThomasF@hyperion-entertainment.com
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * You may also use this under a BSD license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _MEMIO_H
#define _MEMIO_H

#include "short_types.h"

#define IOBASE 0xFE000000

#define in_byte(from) read_byte( (uint8 *)(IOBASE | (from)))
#define in_word(from) read_word_little((uint16 *)(IOBASE | (from)))
#define in_long(from) read_long_little((uint32 *)(IOBASE | (from)))
#define out_byte(to, val) write_byte((uint8 *)(IOBASE | (to)), val)
#define out_word(to, val) write_word_little((uint16 *)(IOBASE | (to)), val)
#define out_long(to, val) write_long_little((uint32 *)(IOBASE | (to)), val)


static inline uint8 read_byte(volatile uint8 *from)
{
    int x;
    asm volatile ("lbz %0,%1\n eieio\n sync" : "=r" (x) : "m" (*from));
    return (uint8)x;
}


static inline void write_byte(volatile uint8 *to, uint8 x)
{
    asm volatile ("stb %1,%0\n eieio\n sync" : "=m" (*to) : "r" (x));
}

static inline uint16 read_word_little(volatile uint16 *from)
{
    int x;
    asm volatile ("lhbrx %0,0,%1\n eieio\n sync" : "=r" (x) : "r" (from), "m" (*from));
    return (uint16)x;
}

static inline uint16 read_word_big(volatile uint16 *from)
{
    int x;
    asm volatile ("lhz %0,%1\n eieio\n sync" : "=r" (x) : "m" (*from));
    return (uint16)x;
}

static inline void write_word_little(volatile uint16 *to, int x)
{
    asm volatile ("sthbrx %1,0,%2\n eieio\n sync" : "=m" (*to) : "r" (x), "r" (to));
}

static inline void write_word_big(volatile uint16 *to, int x)
{
    asm volatile ("sth %1,%0\n eieio\n sync" : "=m" (*to) : "r" (x));
}

static inline uint32 read_long_little(volatile uint32 *from)
{
    unsigned long x;
    asm volatile ("lwbrx %0,0,%1\n eieio\n sync" : "=r" (x) : "r" (from), "m"(*from));
    return (uint32)x;
}

static inline uint32 read_long_big(volatile uint32 *from)
{
    unsigned long x;
    asm volatile ("lwz %0,%1\n eieio\n sync" : "=r" (x) : "m" (*from));
    return (uint32)x;
}

static inline void write_long_little(volatile uint32 *to, uint32 x)
{
    asm volatile ("stwbrx %1,0,%2\n eieio\n sync" : "=m" (*to) : "r" (x), "r" (to));
}

static inline void write_long_big(volatile uint32 *to, uint32 x)
{
    asm volatile ("stw %1,%0\n eieio\n sync" : "=m" (*to) : "r" (x));
}

#define CONFIG_ADDR(bus, devfn, offset) \
    write_long_big((uint32 *)0xFEC00CF8,           \
		   ((offset & 0xFC)<<24) | (devfn << 16)  \
		   | (bus<<8) | 0x80);
#define CONFIG_DATA(offset,mask) ((void *)(0xFEE00CFC+(offset & mask)))


uint8 pci_read_cfg_byte(int32 bus, int32 devfn, int32 offset);
void pci_write_cfg_byte(int32 bus, int32 devfn, int32 offset, uint8 x);
uint16 pci_read_cfg_word(int32 bus, int32 devfn, int32 offset);
void pci_write_cfg_word(int32 bus, int32 devfn, int32 offset, uint16 x);
uint32 pci_read_cfg_long(int32 bus, int32 devfn, int32 offset);
void pci_write_cfg_long(int32 bus, int32 devfn, int32 offset, uint32 x);


#endif
