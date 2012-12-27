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
 * include/asm-microblaze/io.h -- Misc I/O operations
 *
 *  Copyright (C) 2003     John Williams <jwilliams@itee.uq.edu.au>
 *  Copyright (C) 2001,02  NEC Corporation
 *  Copyright (C) 2001,02  Miles Bader <miles@gnu.org>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file COPYING in the main directory of this
 * archive for more details.
 *
 * Written by Miles Bader <miles@gnu.org>
 * Microblaze port by John Williams
 */

#ifndef __MICROBLAZE_IO_H__
#define __MICROBLAZE_IO_H__

#include <asm/types.h>

#define IO_SPACE_LIMIT 0xFFFFFFFF

#define readb(addr) \
  ({ unsigned char __v = (*(volatile unsigned char *) (addr)); __v; })
#define readw(addr) \
  ({ unsigned short __v = (*(volatile unsigned short *) (addr)); __v; })
#define readl(addr) \
  ({ unsigned long __v = (*(volatile unsigned long *) (addr)); __v; })

#define writeb(b, addr) \
  (void)((*(volatile unsigned char *) (addr)) = (b))
#define writew(b, addr) \
  (void)((*(volatile unsigned short *) (addr)) = (b))
#define writel(b, addr) \
  (void)((*(volatile unsigned int *) (addr)) = (b))

#define memset_io(a,b,c)        memset((void *)(a),(b),(c))
#define memcpy_fromio(a,b,c)    memcpy((a),(void *)(b),(c))
#define memcpy_toio(a,b,c)      memcpy((void *)(a),(b),(c))

#define inb(addr)	readb (addr)
#define inw(addr)	readw (addr)
#define inl(addr)	readl (addr)
#define outb(x, addr)	((void) writeb (x, addr))
#define outw(x, addr)	((void) writew (x, addr))
#define outl(x, addr)	((void) writel (x, addr))

/* Some #definitions to keep strange Xilinx code happy */
#define in_8(addr)	readb (addr)
#define in_be16(addr)	readw (addr)
#define in_be32(addr)	readl (addr)

#define out_8(addr,x )	outb (x,addr)
#define out_be16(addr,x )	outw (x,addr)
#define out_be32(addr,x )	outl (x,addr)


#define inb_p(port)		inb((port))
#define outb_p(val, port)	outb((val), (port))
#define inw_p(port)		inw((port))
#define outw_p(val, port)	outw((val), (port))
#define inl_p(port)		inl((port))
#define outl_p(val, port)	outl((val), (port))

/* Some defines to keep the MTD flash drivers happy */

#define __raw_readb readb
#define __raw_readw readw
#define __raw_readl readl
#define __raw_writeb writeb
#define __raw_writew writew
#define __raw_writel writel

static inline void io_insb (unsigned long port, void *dst, unsigned long count)
{
	unsigned char *p = dst;
	while (count--)
		*p++ = inb (port);
}
static inline void io_insw (unsigned long port, void *dst, unsigned long count)
{
	unsigned short *p = dst;
	while (count--)
		*p++ = inw (port);
}
static inline void io_insl (unsigned long port, void *dst, unsigned long count)
{
	unsigned long *p = dst;
	while (count--)
		*p++ = inl (port);
}

static inline void
io_outsb (unsigned long port, const void *src, unsigned long count)
{
	const unsigned char *p = src;
	while (count--)
		outb (*p++, port);
}
static inline void
io_outsw (unsigned long port, const void *src, unsigned long count)
{
	const unsigned short *p = src;
	while (count--)
		outw (*p++, port);
}
static inline void
io_outsl (unsigned long port, const void *src, unsigned long count)
{
	const unsigned long *p = src;
	while (count--)
		outl (*p++, port);
}

#define outsb(a,b,l) io_outsb(a,b,l)
#define outsw(a,b,l) io_outsw(a,b,l)
#define outsl(a,b,l) io_outsl(a,b,l)

#define insb(a,b,l) io_insb(a,b,l)
#define insw(a,b,l) io_insw(a,b,l)
#define insl(a,b,l) io_insl(a,b,l)


#define iounmap(addr)				((void)0)
#define ioremap(physaddr, size)			(physaddr)
#define ioremap_nocache(physaddr, size)		(physaddr)
#define ioremap_writethrough(physaddr, size)	(physaddr)
#define ioremap_fullcache(physaddr, size)	(physaddr)

static inline void sync(void)
{
}

/*
 * Given a physical address and a length, return a virtual address
 * that can be used to access the memory range with the caching
 * properties specified by "flags".
 */
#define MAP_NOCACHE	(0)
#define MAP_WRCOMBINE	(0)
#define MAP_WRBACK	(0)
#define MAP_WRTHROUGH	(0)

static inline void *
map_physmem(phys_addr_t paddr, unsigned long len, unsigned long flags)
{
	return (void *)paddr;
}

/*
 * Take down a mapping set up by map_physmem().
 */
static inline void unmap_physmem(void *vaddr, unsigned long flags)
{

}

static inline phys_addr_t virt_to_phys(void * vaddr)
{
	return (phys_addr_t)(vaddr);
}

#endif /* __MICROBLAZE_IO_H__ */
