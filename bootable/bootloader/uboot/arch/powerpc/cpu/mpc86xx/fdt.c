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
 * Copyright 2008 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 */

#include <common.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <asm/mp.h>

DECLARE_GLOBAL_DATA_PTR;

extern void ft_fixup_num_cores(void *blob);

void ft_cpu_setup(void *blob, bd_t *bd)
{
#ifdef CONFIG_MP
	int off;
	u32 bootpg = determine_mp_bootpg();
#endif

	do_fixup_by_prop_u32(blob, "device_type", "cpu", 4,
			     "timebase-frequency", bd->bi_busfreq / 4, 1);
	do_fixup_by_prop_u32(blob, "device_type", "cpu", 4,
			     "bus-frequency", bd->bi_busfreq, 1);
	do_fixup_by_prop_u32(blob, "device_type", "cpu", 4,
			     "clock-frequency", bd->bi_intfreq, 1);
	do_fixup_by_prop_u32(blob, "device_type", "soc", 4,
			     "bus-frequency", bd->bi_busfreq, 1);

#if defined(CONFIG_MPC8641)
	do_fixup_by_compat_u32(blob, "fsl,mpc8641-localbus",
			       "bus-frequency", gd->lbc_clk, 1);
#endif
	do_fixup_by_compat_u32(blob, "fsl,elbc",
			       "bus-frequency", gd->lbc_clk, 1);

	fdt_fixup_memory(blob, (u64)bd->bi_memstart, (u64)bd->bi_memsize);

#if defined(CONFIG_HAS_ETH0) || defined(CONFIG_HAS_ETH1) \
    || defined(CONFIG_HAS_ETH2) || defined(CONFIG_HAS_ETH3)
	fdt_fixup_ethernet(blob);
#endif

#ifdef CONFIG_SYS_NS16550
	do_fixup_by_compat_u32(blob, "ns16550",
			       "clock-frequency", CONFIG_SYS_NS16550_CLK, 1);
#endif

#ifdef CONFIG_MP
	/* Reserve the boot page so OSes dont use it */
	off = fdt_add_mem_rsv(blob, bootpg, (u64)4096);
	if (off < 0)
		printf("%s: %s\n", __FUNCTION__, fdt_strerror(off));
#endif
	ft_fixup_num_cores(blob);
}
