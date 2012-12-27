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
 * FSL UPM NAND driver
 *
 * Copyright (C) 2007 MontaVista Software, Inc.
 *                    Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/fsl_upm.h>
#include <nand.h>

static void fsl_upm_start_pattern(struct fsl_upm *upm, u32 pat_offset)
{
	clrsetbits_be32(upm->mxmr, MxMR_MAD_MSK, MxMR_OP_RUNP | pat_offset);
}

static void fsl_upm_end_pattern(struct fsl_upm *upm)
{
	clrbits_be32(upm->mxmr, MxMR_OP_RUNP);

	while (in_be32(upm->mxmr) & MxMR_OP_RUNP)
		eieio();
}

static void fsl_upm_run_pattern(struct fsl_upm *upm, int width,
				void __iomem *io_addr, u32 mar)
{
	out_be32(upm->mar, mar);
	switch (width) {
	case 8:
		out_8(io_addr, 0x0);
		break;
	case 16:
		out_be16(io_addr, 0x0);
		break;
	case 32:
		out_be32(io_addr, 0x0);
		break;
	}
}

static void fun_wait(struct fsl_upm_nand *fun)
{
	if (fun->dev_ready) {
		while (!fun->dev_ready(fun->chip_nr))
			debug("unexpected busy state\n");
	} else {
		/*
		 * If the R/B pin is not connected, like on the TQM8548,
		 * a short delay is necessary.
		 */
		udelay(1);
	}
}

#if CONFIG_SYS_NAND_MAX_CHIPS > 1
static void fun_select_chip(struct mtd_info *mtd, int chip_nr)
{
	struct nand_chip *chip = mtd->priv;
	struct fsl_upm_nand *fun = chip->priv;

	if (chip_nr >= 0) {
		fun->chip_nr = chip_nr;
		chip->IO_ADDR_R = chip->IO_ADDR_W =
			fun->upm.io_addr + fun->chip_offset * chip_nr;
	} else if (chip_nr == -1) {
		chip->cmd_ctrl(mtd, NAND_CMD_NONE, 0 | NAND_CTRL_CHANGE);
	}
}
#endif

static void fun_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;
	struct fsl_upm_nand *fun = chip->priv;
	void __iomem *io_addr;
	u32 mar;

	if (!(ctrl & fun->last_ctrl)) {
		fsl_upm_end_pattern(&fun->upm);

		if (cmd == NAND_CMD_NONE)
			return;

		fun->last_ctrl = ctrl & (NAND_ALE | NAND_CLE);
	}

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_ALE)
			fsl_upm_start_pattern(&fun->upm, fun->upm_addr_offset);
		else if (ctrl & NAND_CLE)
			fsl_upm_start_pattern(&fun->upm, fun->upm_cmd_offset);
	}

	mar = cmd << (32 - fun->width);
	io_addr = fun->upm.io_addr;
#if CONFIG_SYS_NAND_MAX_CHIPS > 1
	if (fun->chip_nr > 0) {
		io_addr += fun->chip_offset * fun->chip_nr;
		if (fun->upm_mar_chip_offset)
			mar |= fun->upm_mar_chip_offset * fun->chip_nr;
	}
#endif
	fsl_upm_run_pattern(&fun->upm, fun->width, io_addr, mar);

	/*
	 * Some boards/chips needs this. At least the MPC8360E-RDK and
	 * TQM8548 need it. Probably weird chip, because I don't see
	 * any need for this on MPC8555E + Samsung K9F1G08U0A. Usually
	 * here are 0-2 unexpected busy states per block read.
	 */
	if (fun->wait_flags & FSL_UPM_WAIT_RUN_PATTERN)
		fun_wait(fun);
}

static u8 nand_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	return in_8(chip->IO_ADDR_R);
}

static void nand_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;
	struct fsl_upm_nand *fun = chip->priv;

	for (i = 0; i < len; i++) {
		out_8(chip->IO_ADDR_W, buf[i]);
		if (fun->wait_flags & FSL_UPM_WAIT_WRITE_BYTE)
			fun_wait(fun);
	}

	if (fun->wait_flags & FSL_UPM_WAIT_WRITE_BUFFER)
		fun_wait(fun);
}

static void nand_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;

	for (i = 0; i < len; i++)
		buf[i] = in_8(chip->IO_ADDR_R);
}

static int nand_verify_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;

	for (i = 0; i < len; i++) {
		if (buf[i] != in_8(chip->IO_ADDR_R))
			return -EFAULT;
	}

	return 0;
}

static int nand_dev_ready(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct fsl_upm_nand *fun = chip->priv;

	return fun->dev_ready(fun->chip_nr);
}

int fsl_upm_nand_init(struct nand_chip *chip, struct fsl_upm_nand *fun)
{
	if (fun->width != 8 && fun->width != 16 && fun->width != 32)
		return -ENOSYS;

	fun->last_ctrl = NAND_CLE;

	chip->priv = fun;
	chip->chip_delay = fun->chip_delay;
	chip->ecc.mode = NAND_ECC_SOFT;
	chip->cmd_ctrl = fun_cmd_ctrl;
#if CONFIG_SYS_NAND_MAX_CHIPS > 1
	chip->select_chip = fun_select_chip;
#endif
	chip->read_byte = nand_read_byte;
	chip->read_buf = nand_read_buf;
	chip->write_buf = nand_write_buf;
	chip->verify_buf = nand_verify_buf;
	if (fun->dev_ready)
		chip->dev_ready = nand_dev_ready;

	return 0;
}
