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
 * (C) Copyright 2005 Freescale Semiconductor, Inc.
 *
 * Roy Zang <tie-fei.zang@freescale.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * modifications for the Tsi108 Emul Board by avb@Tundra
 */

/*
 * board support/init functions for the
 * Freescale MPC7448 HPC2 (High-Performance Computing 2 Platform).
 */

#include <common.h>
#include <74xx_7xx.h>
#include <fdt_support.h>
#include <netdev.h>

#undef	DEBUG

DECLARE_GLOBAL_DATA_PTR;

extern void tsi108_init_f (void);

int display_mem_map (void);

void after_reloc (ulong dest_addr)
{
	/*
	 * Jump to the main U-Boot board init code
	 */
	board_init_r ((gd_t *) gd, dest_addr);
	/* NOTREACHED */
}

/*
 * Check Board Identity:
 * report board type
 */

int checkboard (void)
{
	int l_type = 0;

	printf ("BOARD: %s\n", CONFIG_SYS_BOARD_NAME);
	return (l_type);
}

/*
 * Read Processor ID:
 *
 * report calling processor number
 */

int read_pid (void)
{
	return 0;		/* we are on single CPU platform for a while */
}

long int dram_size (int board_type)
{
	return 0x20000000;	/* 256M bytes */
}

phys_size_t initdram (int board_type)
{
	return dram_size (board_type);
}

#if defined(CONFIG_OF_BOARD_SETUP)
void
ft_board_setup(void *blob, bd_t *bd)
{
	ft_cpu_setup(blob, bd);
	fdt_fixup_memory(blob, (u64)bd->bi_memstart, (u64)bd->bi_memsize);
}
#endif

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#if defined(CONFIG_TSI108_ETH)
	rc = tsi108_eth_initialize(bis);
#endif
	return rc;
}
