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
 * Blackfin cache control code
 *
 * Copyright 2003-2008 Analog Devices Inc.
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Licensed under the GPL-2 or later.
 */

#include <asm/linkage.h>
#include <config.h>
#include <asm/blackfin.h>

.text
/* Since all L1 caches work the same way, we use the same method for flushing
 * them.  Only the actual flush instruction differs.  We write this in asm as
 * GCC can be hard to coax into writing nice hardware loops.
 *
 * Also, we assume the following register setup:
 * R0 = start address
 * R1 = end address
 */
.macro do_flush flushins:req optflushins optnopins label

	R2 = -L1_CACHE_BYTES;

	/* start = (start & -L1_CACHE_BYTES) */
	R0 = R0 & R2;

	/* end = ((end - 1) & -L1_CACHE_BYTES) + L1_CACHE_BYTES; */
	R1 += -1;
	R1 = R1 & R2;
	R1 += L1_CACHE_BYTES;

	/* count = (end - start) >> L1_CACHE_SHIFT */
	R2 = R1 - R0;
	R2 >>= L1_CACHE_SHIFT;
	P1 = R2;

.ifnb \label
\label :
.endif
	P0 = R0;
	LSETUP (1f, 2f) LC1 = P1;
1:
.ifnb \optflushins
	\optflushins [P0];
.endif
#if ANOMALY_05000443
.ifb \optnopins
2:
.endif
	\flushins [P0++];
.ifnb \optnopins
2:	\optnopins;
.endif
#else
2:	\flushins [P0++];
#endif

	RTS;
.endm

/* Invalidate all instruction cache lines assocoiated with this memory area */
ENTRY(_blackfin_icache_flush_range)
	do_flush IFLUSH, , nop
ENDPROC(_blackfin_icache_flush_range)

/* Flush all cache lines assocoiated with this area of memory. */
ENTRY(_blackfin_icache_dcache_flush_range)
	do_flush FLUSH, IFLUSH
ENDPROC(_blackfin_icache_dcache_flush_range)

/* Throw away all D-cached data in specified region without any obligation to
 * write them back.  Since the Blackfin ISA does not have an "invalidate"
 * instruction, we use flush/invalidate.  Perhaps as a speed optimization we
 * could bang on the DTEST MMRs ...
 */
ENTRY(_blackfin_dcache_flush_invalidate_range)
	do_flush FLUSHINV
ENDPROC(_blackfin_dcache_flush_invalidate_range)

/* Flush all data cache lines assocoiated with this memory area */
ENTRY(_blackfin_dcache_flush_range)
	do_flush FLUSH, , , .Ldfr
ENDPROC(_blackfin_dcache_flush_range)
