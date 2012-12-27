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
#include <netdev.h>
#include <asm/io.h>
#include "vct.h"

/*
 * EBI initialization for SMC911x access
 */
int ebi_init_smc911x(void)
{
	reg_write(EBI_DEV1_CONFIG1(EBI_BASE), 0x00003020);
	reg_write(EBI_DEV1_CONFIG2(EBI_BASE), 0x0000004F);

	reg_write(EBI_DEV1_TIM1_RD1(EBI_BASE), 0x00501100);
	reg_write(EBI_DEV1_TIM1_RD2(EBI_BASE), 0x0FF02111);

	reg_write(EBI_DEV1_TIM_EXT(EBI_BASE), 0xFFF00000);
	reg_write(EBI_DEV1_EXT_ACC(EBI_BASE), 0x0FFFFFFF);

	reg_write(EBI_DEV1_TIM1_WR1(EBI_BASE), 0x05001100);
	reg_write(EBI_DEV1_TIM1_WR2(EBI_BASE), 0x3FC21110);

	return 0;
}

/*
 * Accessor functions replacing the "weak" functions in
 * drivers/net/smc911x.c
 */
u32 smc911x_reg_read(struct eth_device *dev, u32 addr)
{
	volatile u32 data;

	addr += dev->iobase;
	reg_write(EBI_DEV1_CONFIG2(EBI_BASE), 0x0000004F);
	ebi_wait();
	reg_write(EBI_CPU_IO_ACCS(EBI_BASE), (EXT_DEVICE_CHANNEL_1 | addr));
	ebi_wait();
	data = reg_read(EBI_IO_ACCS_DATA(EBI_BASE));

	return (data);
}

void smc911x_reg_write(struct eth_device *dev, u32 addr, u32 data)
{
	addr += dev->iobase;
	reg_write(EBI_DEV1_CONFIG2(EBI_BASE), 0x0000004F);
	ebi_wait();
	reg_write(EBI_IO_ACCS_DATA(EBI_BASE), data);
	reg_write(EBI_CPU_IO_ACCS(EBI_BASE),
		  EXT_DEVICE_CHANNEL_1 | EBI_CPU_WRITE | addr);
	ebi_wait();
}

void pkt_data_push(struct eth_device *dev, u32 addr, u32 data)
{
	addr += dev->iobase;
	reg_write(EBI_DEV1_CONFIG2(EBI_BASE), 0x0000004A);
	ebi_wait();
	reg_write(EBI_IO_ACCS_DATA(EBI_BASE), data);
	reg_write(EBI_CPU_IO_ACCS(EBI_BASE),
		  EXT_DEVICE_CHANNEL_1 | EBI_CPU_WRITE | addr);
	ebi_wait();

	return;
}

u32 pkt_data_pull(struct eth_device *dev, u32 addr)
{
	volatile u32 data;

	addr += dev->iobase;
	reg_write(EBI_DEV1_CONFIG2(EBI_BASE), 0x0000004A);
	ebi_wait();
	reg_write(EBI_CPU_IO_ACCS(EBI_BASE), (EXT_DEVICE_CHANNEL_1 | addr));
	ebi_wait();
	data = reg_read(EBI_IO_ACCS_DATA(EBI_BASE));

	return data;
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, CONFIG_DRIVER_SMC911X_BASE);
#endif
	return rc;
}
