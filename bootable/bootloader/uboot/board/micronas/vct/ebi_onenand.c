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
 * (C) Copyright 2008 Stefan Roese <sr@denx.de>, DENX Software Engineering
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
#include <linux/mtd/mtd.h>
#include <linux/mtd/onenand.h>
#include "vct.h"

#define BURST_SIZE_WORDS		4

static u16 ebi_nand_read_word(void __iomem *addr)
{
	reg_write(EBI_CPU_IO_ACCS(EBI_BASE), (EXT_DEVICE_CHANNEL_2 | (u32)addr));
	ebi_wait();

	return reg_read(EBI_IO_ACCS_DATA(EBI_BASE)) >> 16;
}

static void ebi_nand_write_word(u16 data, void __iomem * addr)
{
	ebi_wait();
	reg_write(EBI_IO_ACCS_DATA(EBI_BASE), (data << 16));
	reg_write(EBI_CPU_IO_ACCS(EBI_BASE),
		  EXT_DEVICE_CHANNEL_2 | EBI_CPU_WRITE | (u32)addr);
	ebi_wait();
}

/*
 * EBI initialization for OneNAND FLASH access
 */
int ebi_init_onenand(void)
{
	reg_write(EBI_DEV1_CONFIG1(EBI_BASE), 0x83000);

	reg_write(EBI_DEV2_CONFIG1(EBI_BASE), 0x00403002);
	reg_write(EBI_DEV2_CONFIG2(EBI_BASE), 0x50);

	reg_write(EBI_DEV3_CONFIG1(EBI_BASE), 0x00403002);
	reg_write(EBI_DEV3_CONFIG2(EBI_BASE), 0x0); /* byte/word ordering */

	reg_write(EBI_DEV2_TIM1_RD1(EBI_BASE), 0x00504000);
	reg_write(EBI_DEV2_TIM1_RD2(EBI_BASE), 0x00001000);
	reg_write(EBI_DEV2_TIM1_WR1(EBI_BASE), 0x12002223);
	reg_write(EBI_DEV2_TIM1_WR2(EBI_BASE), 0x3FC02220);
	reg_write(EBI_DEV3_TIM1_RD1(EBI_BASE), 0x00504000);
	reg_write(EBI_DEV3_TIM1_RD2(EBI_BASE), 0x00001000);
	reg_write(EBI_DEV3_TIM1_WR1(EBI_BASE), 0x05001000);
	reg_write(EBI_DEV3_TIM1_WR2(EBI_BASE), 0x00010200);

	reg_write(EBI_DEV2_TIM_EXT(EBI_BASE), 0xFFF00000);
	reg_write(EBI_DEV2_EXT_ACC(EBI_BASE), 0x0FFFFFFF);

	reg_write(EBI_DEV3_TIM_EXT(EBI_BASE), 0xFFF00000);
	reg_write(EBI_DEV3_EXT_ACC(EBI_BASE), 0x0FFFFFFF);

	/* prepare DMA configuration for EBI */
	reg_write(EBI_DEV3_FIFO_CONFIG(EBI_BASE), 0x0101ff00);

	/* READ only no byte order change, TAG 1 used */
	reg_write(EBI_DEV3_DMA_CONFIG2(EBI_BASE), 0x00000004);

	reg_write(EBI_TAG1_SYS_ID(EBI_BASE), 0x0); /* SCC DMA channel 0 */
	reg_write(EBI_TAG2_SYS_ID(EBI_BASE), 0x1);
	reg_write(EBI_TAG3_SYS_ID(EBI_BASE), 0x2);
	reg_write(EBI_TAG4_SYS_ID(EBI_BASE), 0x3);

	return 0;
}

static void *memcpy_16_from_onenand(void *dst, const void *src, unsigned int len)
{
	void *ret = dst;
	u16 *d = dst;
	u16 *s = (u16 *)src;

	len >>= 1;
	while (len-- > 0)
		*d++ = ebi_nand_read_word(s++);

	return ret;
}

static void *memcpy_32_from_onenand(void *dst, const void *src, unsigned int len)
{
	void *ret = dst;
	u32 *d = (u32 *)dst;
	u32 s = (u32)src;
	u32 bytes_per_block = BURST_SIZE_WORDS * sizeof(int);
	u32 n_blocks = len / bytes_per_block;
	u32 block = 0;
	u32 burst_word;

	for (block = 0; block < n_blocks; block++) {
		/* Trigger read channel 3 */
		reg_write(EBI_CPU_IO_ACCS(EBI_BASE),
			  (EXT_DEVICE_CHANNEL_3 | (s + (block * bytes_per_block))));
		/* Poll status to see whether read has finished */
		ebi_wait();

		/* Squirrel the data away in a safe place */
		for (burst_word = 0; burst_word < BURST_SIZE_WORDS; burst_word++)
			*d++ = reg_read(EBI_IO_ACCS_DATA(EBI_BASE));
	}

	return ret;
}

static void *memcpy_16_to_onenand(void *dst, const void *src, unsigned int len)
{
	void *ret = dst;
	u16 *d = dst;
	u16 *s = (u16 *)src;

	len >>= 1;
	while (len-- > 0)
		ebi_nand_write_word(*s++, d++);

	return ret;
}

static inline int onenand_bufferram_offset(struct mtd_info *mtd, int area)
{
	struct onenand_chip *this = mtd->priv;

	if (ONENAND_CURRENT_BUFFERRAM(this)) {
		if (area == ONENAND_DATARAM)
			return mtd->writesize;
		if (area == ONENAND_SPARERAM)
			return mtd->oobsize;
	}

	return 0;
}

static int ebi_read_bufferram(struct mtd_info *mtd, loff_t addr, int area,
			      unsigned char *buffer, int offset,
			      size_t count)
{
	struct onenand_chip *this = mtd->priv;
	void __iomem *bufferram;

	bufferram = this->base + area;
	bufferram += onenand_bufferram_offset(mtd, area);

	if (count < 4)
		memcpy_16_from_onenand(buffer, bufferram + offset, count);
	else
		memcpy_32_from_onenand(buffer, bufferram + offset, count);

	return 0;
}

static int ebi_write_bufferram(struct mtd_info *mtd, loff_t addr, int area,
			       const unsigned char *buffer, int offset,
			       size_t count)
{
	struct onenand_chip *this = mtd->priv;
	void __iomem *bufferram;

	bufferram = this->base + area;
	bufferram += onenand_bufferram_offset(mtd, area);

	memcpy_16_to_onenand(bufferram + offset, buffer, count);

	return 0;
}

void onenand_board_init(struct mtd_info *mtd)
{
	struct onenand_chip *chip = mtd->priv;

	/*
	 * Insert board specific OneNAND access functions
	 */
	chip->read_word = ebi_nand_read_word;
	chip->write_word = ebi_nand_write_word;

	chip->read_bufferram = ebi_read_bufferram;
	chip->write_bufferram = ebi_write_bufferram;
}
