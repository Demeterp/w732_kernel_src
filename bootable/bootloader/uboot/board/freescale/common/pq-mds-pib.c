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
 * Copyright (C) 2007 Freescale Semiconductor, Inc.
 *
 * Tony Li <tony.li@freescale.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>

#include "pq-mds-pib.h"

int pib_init(void)
{
	u8 val8;
	u8 orig_i2c_bus;

	/* Switch temporarily to I2C bus #2 */
	orig_i2c_bus = i2c_get_bus_num();
	i2c_set_bus_num(1);

	val8 = 0;
#if defined(CONFIG_PCI) && !defined(CONFIG_PCISLAVE)
	/* Assign PIB PMC slot to desired PCI bus */
	i2c_write(0x23, 0x6, 1, &val8, 1);
	i2c_write(0x23, 0x7, 1, &val8, 1);
	val8 = 0xff;
	i2c_write(0x23, 0x2, 1, &val8, 1);
	i2c_write(0x23, 0x3, 1, &val8, 1);

	val8 = 0;
	i2c_write(0x26, 0x6, 1, &val8, 1);
	val8 = 0x34;
	i2c_write(0x26, 0x7, 1, &val8, 1);
#if defined(CONFIG_MPC832XEMDS)
	val8 = 0xf9;            /* PMC2, PMC3 slot to PCI bus */
#else
	val8 = 0xf3;		/* PMC1, PMC2, PMC3 slot to PCI bus */
#endif
	i2c_write(0x26, 0x2, 1, &val8, 1);
	val8 = 0xff;
	i2c_write(0x26, 0x3, 1, &val8, 1);

	val8 = 0;
	i2c_write(0x27, 0x6, 1, &val8, 1);
	i2c_write(0x27, 0x7, 1, &val8, 1);
	val8 = 0xff;
	i2c_write(0x27, 0x2, 1, &val8, 1);
	val8 = 0xef;
	i2c_write(0x27, 0x3, 1, &val8, 1);

	eieio();

#if defined(CONFIG_MPC832XEMDS)
	printf("PCI 32bit bus on PMC2 &PMC3\n");
#else
	printf("PCI 32bit bus on PMC1 & PMC2 &PMC3\n");
#endif
#endif

#if defined(CONFIG_PQ_MDS_PIB_ATM)
#if defined(CONFIG_MPC8360EMDS) || defined(CONFIG_MPC8569MDS)
	val8 = 0;
	i2c_write(0x20, 0x6, 1, &val8, 1);
	i2c_write(0x20, 0x7, 1, &val8, 1);

	val8 = 0xdf;
	i2c_write(0x20, 0x2, 1, &val8, 1);
	val8 = 0xf7;
	i2c_write(0x20, 0x3, 1, &val8, 1);

	eieio();

	printf("QOC3 ATM card on PMC0\n");
#elif defined(CONFIG_MPC832XEMDS)
	val8 = 0;
	i2c_write(0x26, 0x7, 1, &val8, 1);
	val8 = 0xf7;
	i2c_write(0x26, 0x3, 1, &val8, 1);

	val8 = 0;
	i2c_write(0x21, 0x6, 1, &val8, 1);
	i2c_write(0x21, 0x7, 1, &val8, 1);

	val8 = 0xdf;
	i2c_write(0x21, 0x2, 1, &val8, 1);
	val8 = 0xef;
	i2c_write(0x21, 0x3, 1, &val8, 1);

	eieio();

	printf("QOC3 ATM card on PMC1\n");
#endif
#endif
	/* Reset to original I2C bus */
	i2c_set_bus_num(orig_i2c_bus);
	return 0;
}
