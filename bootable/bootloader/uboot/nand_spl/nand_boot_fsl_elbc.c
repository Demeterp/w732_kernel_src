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
 * NAND boot for Freescale Enhanced Local Bus Controller, Flash Control Machine
 *
 * (C) Copyright 2006-2008
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * Copyright (c) 2008 Freescale Semiconductor, Inc.
 * Author: Scott Wood <scottwood@freescale.com>
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
#include <asm/io.h>
#include <asm/fsl_lbc.h>
#include <linux/mtd/nand.h>

#define WINDOW_SIZE 8192

static void nand_wait(void)
{
	fsl_lbus_t *regs = (fsl_lbus_t *)(CONFIG_SYS_IMMR + 0x5000);

	for (;;) {
		uint32_t status = in_be32(&regs->ltesr);

		if (status == 1)
			return;

		if (status & 1) {
			puts("read failed (ltesr)\n");
			for (;;);
		}
	}
}

static void nand_load(unsigned int offs, int uboot_size, uchar *dst)
{
	fsl_lbus_t *regs = (fsl_lbus_t *)(CONFIG_SYS_IMMR + 0x5000);
	uchar *buf = (uchar *)CONFIG_SYS_NAND_BASE;
	int large = in_be32(&regs->bank[0].or) & OR_FCM_PGS;
	int block_shift = large ? 17 : 14;
	int block_size = 1 << block_shift;
	int page_size = large ? 2048 : 512;
	int bad_marker = large ? page_size + 0 : page_size + 5;
	int fmr = (15 << FMR_CWTO_SHIFT) | (2 << FMR_AL_SHIFT) | 2;
	int pos = 0;

	if (offs & (block_size - 1)) {
		puts("bad offset\n");
		for (;;);
	}

	if (large) {
		fmr |= FMR_ECCM;
		out_be32(&regs->fcr, (NAND_CMD_READ0 << FCR_CMD0_SHIFT) |
		                     (NAND_CMD_READSTART << FCR_CMD1_SHIFT));
		out_be32(&regs->fir,
		         (FIR_OP_CW0 << FIR_OP0_SHIFT) |
		         (FIR_OP_CA  << FIR_OP1_SHIFT) |
		         (FIR_OP_PA  << FIR_OP2_SHIFT) |
		         (FIR_OP_CW1 << FIR_OP3_SHIFT) |
		         (FIR_OP_RBW << FIR_OP4_SHIFT));
	} else {
		out_be32(&regs->fcr, NAND_CMD_READ0 << FCR_CMD0_SHIFT);
		out_be32(&regs->fir,
		         (FIR_OP_CW0 << FIR_OP0_SHIFT) |
		         (FIR_OP_CA  << FIR_OP1_SHIFT) |
		         (FIR_OP_PA  << FIR_OP2_SHIFT) |
		         (FIR_OP_RBW << FIR_OP3_SHIFT));
	}

	out_be32(&regs->fbcr, 0);
	clrsetbits_be32(&regs->bank[0].br, BR_DECC, BR_DECC_CHK_GEN);

	while (pos < uboot_size) {
		int i = 0;
		out_be32(&regs->fbar, offs >> block_shift);

		do {
			int j;
			unsigned int page_offs = (offs & (block_size - 1)) << 1;

			out_be32(&regs->ltesr, ~0);
			out_be32(&regs->lteatr, 0);
			out_be32(&regs->fpar, page_offs);
			out_be32(&regs->fmr, fmr);
			out_be32(&regs->lsor, 0);
			nand_wait();

			page_offs %= WINDOW_SIZE;

			/*
			 * If either of the first two pages are marked bad,
			 * continue to the next block.
			 */
			if (i++ < 2 && buf[page_offs + bad_marker] != 0xff) {
				puts("skipping\n");
				offs = (offs + block_size) & ~(block_size - 1);
				pos &= ~(block_size - 1);
				break;
			}

			for (j = 0; j < page_size; j++)
				dst[pos + j] = buf[page_offs + j];

			pos += page_size;
			offs += page_size;
		} while ((offs & (block_size - 1)) && (pos < uboot_size));
	}
}

/*
 * The main entry for NAND booting. It's necessary that SDRAM is already
 * configured and available since this code loads the main U-Boot image
 * from NAND into SDRAM and starts it from there.
 */
void nand_boot(void)
{
	__attribute__((noreturn)) void (*uboot)(void);

	/*
	 * Load U-Boot image from NAND into RAM
	 */
	nand_load(CONFIG_SYS_NAND_U_BOOT_OFFS, CONFIG_SYS_NAND_U_BOOT_SIZE,
	          (uchar *)CONFIG_SYS_NAND_U_BOOT_DST);

	/*
	 * Jump to U-Boot image
	 */
	puts("transfering control\n");
	/*
	 * Clean d-cache and invalidate i-cache, to
	 * make sure that no stale data is executed.
	 */
	flush_cache(CONFIG_SYS_NAND_U_BOOT_DST, CONFIG_SYS_NAND_U_BOOT_SIZE);
	uboot = (void *)CONFIG_SYS_NAND_U_BOOT_START;
	uboot();
}
