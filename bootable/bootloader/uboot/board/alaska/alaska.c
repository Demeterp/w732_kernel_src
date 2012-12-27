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
 * (C) Copyright 2004, Freescale Inc.
 * TsiChung Liew, Tsi-Chung.Liew@freescale.com
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
 */

#include <common.h>
#include <mpc8220.h>
#include <asm/processor.h>
#include <asm/mmu.h>

void setupBat (ulong size)
{
	ulong batu, batl;
	int blocksize = 0;

	/* Flash 0 */
#if defined (CONFIG_SYS_AMD_BOOT)
	batu = CONFIG_SYS_FLASH0_BASE | BATU_BL_512K | BPP_RW | BPP_RX;
#else
	batu = CONFIG_SYS_FLASH0_BASE | BATU_BL_16M | BPP_RW | BPP_RX;
#endif
	batl = CONFIG_SYS_FLASH0_BASE | 0x22;
	write_bat (IBAT0, batu, batl);
	write_bat (DBAT0, batu, batl);

	/* Flash 1 */
#if defined (CONFIG_SYS_AMD_BOOT)
	batu = CONFIG_SYS_FLASH1_BASE | BATU_BL_16M | BPP_RW | BPP_RX;
#else
	batu = CONFIG_SYS_FLASH1_BASE | BATU_BL_512K | BPP_RW | BPP_RX;
#endif
	batl = CONFIG_SYS_FLASH1_BASE | 0x22;
	write_bat (IBAT1, batu, batl);
	write_bat (DBAT1, batu, batl);

	/* CPLD */
	batu = CONFIG_SYS_CPLD_BASE | BATU_BL_512K | BPP_RW | BPP_RX;
	batl = CONFIG_SYS_CPLD_BASE | 0x22;
	write_bat (IBAT2, 0, 0);
	write_bat (DBAT2, batu, batl);

	/* FPGA */
	batu = CONFIG_SYS_FPGA_BASE | BATU_BL_512K | BPP_RW | BPP_RX;
	batl = CONFIG_SYS_FPGA_BASE | 0x22;
	write_bat (IBAT3, 0, 0);
	write_bat (DBAT3, batu, batl);

	/* MBAR - Data only */
	batu = CONFIG_SYS_MBAR | BPP_RW | BPP_RX;
	batl = CONFIG_SYS_MBAR | 0x22;
	mtspr (IBAT4L, 0);
	mtspr (IBAT4U, 0);
	mtspr (DBAT4L, batl);
	mtspr (DBAT4U, batu);

	/* MBAR - SRAM */
	batu = CONFIG_SYS_SRAM_BASE | BPP_RW | BPP_RX;
	batl = CONFIG_SYS_SRAM_BASE | 0x42;
	mtspr (IBAT5L, batl);
	mtspr (IBAT5U, batu);
	mtspr (DBAT5L, batl);
	mtspr (DBAT5U, batu);

	if (size <= 0x800000)	/* 8MB */
		blocksize = BATU_BL_8M;
	else if (size <= 0x1000000)	/* 16MB */
		blocksize = BATU_BL_16M;
	else if (size <= 0x2000000)	/* 32MB */
		blocksize = BATU_BL_32M;
	else if (size <= 0x4000000)	/* 64MB */
		blocksize = BATU_BL_64M;
	else if (size <= 0x8000000)	/* 128MB */
		blocksize = BATU_BL_128M;
	else if (size <= 0x10000000)	/* 256MB */
		blocksize = BATU_BL_256M;

	/* Memory */
	batu = CONFIG_SYS_SDRAM_BASE | blocksize | BPP_RW | BPP_RX;
	batl = CONFIG_SYS_SDRAM_BASE | 0x42;
	mtspr (IBAT6L, batl);
	mtspr (IBAT6U, batu);
	mtspr (DBAT6L, batl);
	mtspr (DBAT6U, batu);

	/* memory size is less than 256MB */
	if (size <= 0x10000000) {
		/* Nothing */
		batu = 0;
		batl = 0;
	} else {
		size -= 0x10000000;
		if (size <= 0x800000)	/* 8MB */
			blocksize = BATU_BL_8M;
		else if (size <= 0x1000000)	/* 16MB */
			blocksize = BATU_BL_16M;
		else if (size <= 0x2000000)	/* 32MB */
			blocksize = BATU_BL_32M;
		else if (size <= 0x4000000)	/* 64MB */
			blocksize = BATU_BL_64M;
		else if (size <= 0x8000000)	/* 128MB */
			blocksize = BATU_BL_128M;
		else if (size <= 0x10000000)	/* 256MB */
			blocksize = BATU_BL_256M;

		batu = (CONFIG_SYS_SDRAM_BASE +
			0x10000000) | blocksize | BPP_RW | BPP_RX;
		batl = (CONFIG_SYS_SDRAM_BASE + 0x10000000) | 0x42;
	}

	mtspr (IBAT7L, batl);
	mtspr (IBAT7U, batu);
	mtspr (DBAT7L, batl);
	mtspr (DBAT7U, batu);
}

phys_size_t initdram (int board_type)
{
	ulong size;

	size = dramSetup ();

/* if iCache ad dCache is defined */
#if defined(CONFIG_CMD_CACHE)
/*    setupBat(size);*/
#endif

	return size;
}

int checkboard (void)
{
	puts ("Board: Alaska MPC8220 Evaluation Board\n");

	return 0;
}
