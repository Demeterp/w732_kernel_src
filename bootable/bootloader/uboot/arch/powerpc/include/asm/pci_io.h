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

/* originally from linux source (asm-ppc/io.h).
 * Sanity added by Rob Taylor, Flying Pig Systems, 2000
 */
#ifndef _PCI_IO_H_
#define _PCI_IO_H_

#include "io.h"


#define pci_read_le16(addr, dest) \
    __asm__ __volatile__("lhbrx %0,0,%1" : "=r" (dest) : \
		  "r" (addr), "m" (*addr));

#define pci_write_le16(addr, val) \
    __asm__ __volatile__("sthbrx %1,0,%2" : "=m" (*addr) : \
		  "r" (val), "r" (addr));


#define pci_read_le32(addr, dest) \
    __asm__ __volatile__("lwbrx %0,0,%1" : "=r" (dest) : \
		 "r" (addr), "m" (*addr));

#define pci_write_le32(addr, val) \
__asm__ __volatile__("stwbrx %1,0,%2" : "=m" (*addr) : \
		 "r" (val), "r" (addr));

#define pci_readb(addr,b) ((b) = *(volatile u8 *) (addr))
#define pci_writeb(b,addr) ((*(volatile u8 *) (addr)) = (b))

#if !defined(__BIG_ENDIAN)
#define pci_readw(addr,b) ((b) = *(volatile u16 *) (addr))
#define pci_readl(addr,b) ((b) = *(volatile u32 *) (addr))
#define pci_writew(b,addr) ((*(volatile u16 *) (addr)) = (b))
#define pci_writel(b,addr) ((*(volatile u32 *) (addr)) = (b))
#else
#define pci_readw(addr,b) pci_read_le16((volatile u16 *)(addr),(b))
#define pci_readl(addr,b) pci_read_le32((volatile u32 *)(addr),(b))
#define pci_writew(b,addr) pci_write_le16((volatile u16 *)(addr),(b))
#define pci_writel(b,addr) pci_write_le32((volatile u32 *)(addr),(b))
#endif


#endif /* _PCI_IO_H_ */
