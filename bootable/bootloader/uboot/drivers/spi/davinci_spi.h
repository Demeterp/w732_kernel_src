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
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Register definitions for the DaVinci SPI Controller
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

#ifndef _DAVINCI_SPI_H_
#define _DAVINCI_SPI_H_

struct davinci_spi_regs {
	dv_reg	gcr0;		/* 0x00 */
	dv_reg	gcr1;		/* 0x04 */
	dv_reg	int0;		/* 0x08 */
	dv_reg	lvl;		/* 0x0c */
	dv_reg	flg;		/* 0x10 */
	dv_reg	pc0;		/* 0x14 */
	dv_reg	pc1;		/* 0x18 */
	dv_reg	pc2;		/* 0x1c */
	dv_reg	pc3;		/* 0x20 */
	dv_reg	pc4;		/* 0x24 */
	dv_reg	pc5;		/* 0x28 */
	dv_reg	rsvd[3];
	dv_reg	dat0;		/* 0x38 */
	dv_reg	dat1;		/* 0x3c */
	dv_reg	buf;		/* 0x40 */
	dv_reg	emu;		/* 0x44 */
	dv_reg	delay;		/* 0x48 */
	dv_reg	def;		/* 0x4c */
	dv_reg	fmt0;		/* 0x50 */
	dv_reg	fmt1;		/* 0x54 */
	dv_reg	fmt2;		/* 0x58 */
	dv_reg	fmt3;		/* 0x5c */
	dv_reg	intvec0;	/* 0x60 */
	dv_reg	intvec1;	/* 0x64 */
};

#define BIT(x)			(1 << (x))

/* SPIGCR0 */
#define SPIGCR0_SPIENA_MASK	0x1
#define SPIGCR0_SPIRST_MASK	0x0

/* SPIGCR0 */
#define SPIGCR1_CLKMOD_MASK	BIT(1)
#define SPIGCR1_MASTER_MASK	BIT(0)
#define SPIGCR1_SPIENA_MASK	BIT(24)

/* SPIPC0 */
#define SPIPC0_DIFUN_MASK	BIT(11)		/* SIMO */
#define SPIPC0_DOFUN_MASK	BIT(10)		/* SOMI */
#define SPIPC0_CLKFUN_MASK	BIT(9)		/* CLK */
#define SPIPC0_EN0FUN_MASK	BIT(0)

/* SPIFMT0 */
#define SPIFMT_SHIFTDIR_SHIFT	20
#define SPIFMT_POLARITY_SHIFT	17
#define SPIFMT_PHASE_SHIFT	16
#define SPIFMT_PRESCALE_SHIFT	8

/* SPIDAT1 */
#define SPIDAT1_CSHOLD_SHIFT	28
#define SPIDAT1_CSNR_SHIFT	16

/* SPIDELAY */
#define SPI_C2TDELAY_SHIFT	24
#define SPI_T2CDELAY_SHIFT	16

/* SPIBUF */
#define SPIBUF_RXEMPTY_MASK	BIT(31)
#define SPIBUF_TXFULL_MASK	BIT(29)

/* SPIDEF */
#define SPIDEF_CSDEF0_MASK	BIT(0)

struct davinci_spi_slave {
	struct spi_slave slave;
	struct davinci_spi_regs *regs;
	unsigned int freq;
};

static inline struct davinci_spi_slave *to_davinci_spi(struct spi_slave *slave)
{
	return container_of(slave, struct davinci_spi_slave, slave);
}

#endif /* _DAVINCI_SPI_H_ */
