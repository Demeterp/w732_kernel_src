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
 * Copyright (C) 2008-2009 Samsung Electronics
 * Kyungmin Park <kyungmin.park@samsung.com>
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
#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/onenand.h>
#include <linux/mtd/samsung_onenand.h>

#include <onenand_uboot.h>

#include <asm/io.h>
#include <asm/arch/clock.h>

void onenand_board_init(struct mtd_info *mtd)
{
	struct onenand_chip *this = mtd->priv;
	struct s5pc100_clock *clk = (struct s5pc100_clock *)S5PC1XX_CLOCK_BASE;
	struct samsung_onenand *onenand;
	int value;

	this->base = (void *)S5PC100_ONENAND_BASE;
	onenand = (struct samsung_onenand *)this->base;

	/* D0 Domain memory clock gating */
	value = readl(&clk->gate_d01);
	value &= ~(1 << 2);		/* CLK_ONENANDC */
	value |= (1 << 2);
	writel(value, &clk->gate_d01);

	value = readl(&clk->src0);
	value &= ~(1 << 24);		/* MUX_1nand: 0 from HCLKD0 */
	value &= ~(1 << 20);		/* MUX_HREF: 0 from FIN_27M */
	writel(value, &clk->src0);

	value = readl(&clk->div1);
	value &= ~(3 << 16);		/* PCLKD1_RATIO */
	value |= (1 << 16);
	writel(value, &clk->div1);

	writel(ONENAND_MEM_RESET_COLD, &onenand->mem_reset);

	while (!(readl(&onenand->int_err_stat) & RST_CMP))
		continue;

	writel(RST_CMP, &onenand->int_err_ack);

	/*
	 * Access_Clock [2:0]
	 * 166 MHz, 134 Mhz : 3
	 * 100 Mhz, 60 Mhz  : 2
	 */
	writel(0x3, &onenand->acc_clock);

	writel(INT_ERR_ALL, &onenand->int_err_mask);
	writel(1 << 0, &onenand->int_pin_en);	/* Enable */

	value = readl(&onenand->int_err_mask);
	value &= ~RDY_ACT;
	writel(value, &onenand->int_err_mask);

	s3c_onenand_init(mtd);
}
