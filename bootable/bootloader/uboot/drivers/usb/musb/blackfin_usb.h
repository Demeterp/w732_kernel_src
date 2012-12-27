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
 * Blackfin MUSB HCD (Host Controller Driver) for u-boot
 *
 * Copyright (c) 2008-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BLACKFIN_USB_H__
#define __BLACKFIN_USB_H__

#include <linux/types.h>

/* Every register is 32bit aligned, but only 16bits in size */
#define ureg(name) u16 name; u16 __pad_##name;

#define musb_regs musb_regs
struct musb_regs {
	/* common registers */
	ureg(faddr)
	ureg(power)
	ureg(intrtx)
	ureg(intrrx)
	ureg(intrtxe)
	ureg(intrrxe)
	ureg(intrusb)
	ureg(intrusbe)
	ureg(frame)
	ureg(index)
	ureg(testmode)
	ureg(globintr)
	ureg(global_ctl)
	u32	reserved0[3];
	/* indexed registers */
	ureg(txmaxp)
	ureg(txcsr)
	ureg(rxmaxp)
	ureg(rxcsr)
	ureg(rxcount)
	ureg(txtype)
	ureg(txinterval)
	ureg(rxtype)
	ureg(rxinterval)
	u32	reserved1;
	ureg(txcount)
	u32	reserved2[5];
	/* fifo */
	u16	fifox[32];
	/* OTG, dynamic FIFO, version & vendor registers */
	u32	reserved3[16];
	ureg(devctl)
	ureg(vbus_irq)
	ureg(vbus_mask)
	u32 reserved4[15];
	ureg(linkinfo)
	ureg(vplen)
	ureg(hseof1)
	ureg(fseof1)
	ureg(lseof1)
	u32 reserved5[41];
	/* target address registers */
	struct musb_tar_regs {
		ureg(txmaxp)
		ureg(txcsr)
		ureg(rxmaxp)
		ureg(rxcsr)
		ureg(rxcount)
		ureg(txtype)
		ureg(txinternal)
		ureg(rxtype)
		ureg(rxinternal)
		u32	reserved6;
		ureg(txcount)
		u32 reserved7[5];
	} tar[8];
} __attribute__((packed));

struct bfin_musb_dma_regs {
	ureg(interrupt);
	ureg(control);
	ureg(addr_low);
	ureg(addr_high);
	ureg(count_low);
	ureg(count_high);
	u32 reserved0[2];
};

#undef ureg

/* EP5-EP7 are the only ones with 1024 byte FIFOs which BULK really needs */
#define MUSB_BULK_EP 5

/* Blackfin FIFO's are static */
#define MUSB_NO_DYNAMIC_FIFO

/* No HUB support :( */
#define MUSB_NO_MULTIPOINT

#endif
