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

#ifndef _LINUX_LMB_H
#define _LINUX_LMB_H
#ifdef __KERNEL__

#include <asm/types.h>
/*
 * Logical memory blocks.
 *
 * Copyright (C) 2001 Peter Bergner, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#define MAX_LMB_REGIONS 8

struct lmb_property {
	phys_addr_t base;
	phys_size_t size;
};

struct lmb_region {
	unsigned long cnt;
	phys_size_t size;
	struct lmb_property region[MAX_LMB_REGIONS+1];
};

struct lmb {
	struct lmb_region memory;
	struct lmb_region reserved;
};

extern struct lmb lmb;

extern void lmb_init(struct lmb *lmb);
extern long lmb_add(struct lmb *lmb, phys_addr_t base, phys_size_t size);
extern long lmb_reserve(struct lmb *lmb, phys_addr_t base, phys_size_t size);
extern phys_addr_t lmb_alloc(struct lmb *lmb, phys_size_t size, ulong align);
extern phys_addr_t lmb_alloc_base(struct lmb *lmb, phys_size_t size, ulong align,
			    phys_addr_t max_addr);
extern phys_addr_t __lmb_alloc_base(struct lmb *lmb, phys_size_t size, ulong align,
			      phys_addr_t max_addr);
extern int lmb_is_reserved(struct lmb *lmb, phys_addr_t addr);
extern long lmb_free(struct lmb *lmb, phys_addr_t base, phys_size_t size);

extern void lmb_dump_all(struct lmb *lmb);

static inline phys_size_t
lmb_size_bytes(struct lmb_region *type, unsigned long region_nr)
{
	return type->region[region_nr].size;
}

void board_lmb_reserve(struct lmb *lmb);
void arch_lmb_reserve(struct lmb *lmb);

#endif /* __KERNEL__ */

#endif /* _LINUX_LMB_H */
