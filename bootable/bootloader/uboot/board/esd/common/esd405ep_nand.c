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
 * (C) Copyright 2007
 * Matthias Fuchs, esd gmbh germany, matthias.fuchs@esd-electronics.com
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

#if defined(CONFIG_CMD_NAND)
#include <asm/io.h>
#include <nand.h>

/*
 * hardware specific access to control-lines
 */
static void esd405ep_nand_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;
	if (ctrl & NAND_CTRL_CHANGE) {
		if ( ctrl & NAND_CLE )
			out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) | CONFIG_SYS_NAND_CLE);
		else
			out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) & ~CONFIG_SYS_NAND_CLE);
		if ( ctrl & NAND_ALE )
			out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) | CONFIG_SYS_NAND_ALE);
		else
			out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) & ~CONFIG_SYS_NAND_ALE);
		if ( ctrl & NAND_NCE )
			out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) & ~CONFIG_SYS_NAND_CE);
		else
			out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) | CONFIG_SYS_NAND_CE);
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, this->IO_ADDR_W);
}


/*
 * read device ready pin
 */
static int esd405ep_nand_device_ready(struct mtd_info *mtdinfo)
{
	if (in_be32((void *)GPIO0_IR) & CONFIG_SYS_NAND_RDY)
		return 1;
	return 0;
}


int board_nand_init(struct nand_chip *nand)
{
	/*
	 * Set NAND-FLASH GPIO signals to defaults
	 */
	out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) & ~(CONFIG_SYS_NAND_CLE | CONFIG_SYS_NAND_ALE));
	out_be32((void *)GPIO0_OR, in_be32((void *)GPIO0_OR) | CONFIG_SYS_NAND_CE);

	/*
	 * Initialize nand_chip structure
	 */
	nand->cmd_ctrl = esd405ep_nand_hwcontrol;
	nand->dev_ready = esd405ep_nand_device_ready;
	nand->ecc.mode = NAND_ECC_SOFT;
	nand->chip_delay = NAND_BIG_DELAY_US;
	nand->options = NAND_SAMSUNG_LP_OPTIONS;
	return 0;
}
#endif
