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
 * Copyright 2007 Freescale Semiconductor.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>

#include "bcsr.h"

void enable_8568mds_duart()
{
	volatile uint* duart_mux	= (uint *)(CONFIG_SYS_CCSRBAR + 0xe0060);
	volatile uint* devices		= (uint *)(CONFIG_SYS_CCSRBAR + 0xe0070);
	volatile u8 *bcsr		= (u8 *)(CONFIG_SYS_BCSR);

	*duart_mux = 0x80000000;	/* Set the mux to Duart on PMUXCR */
	*devices  = 0;			/* Enable all peripheral devices */
	bcsr[5] |= 0x01;		/* Enable Duart in BCSR*/
}

void enable_8568mds_flash_write()
{
	volatile u8 *bcsr = (u8 *)(CONFIG_SYS_BCSR);

	bcsr[9] |= 0x01;
}

void disable_8568mds_flash_write()
{
	volatile u8 *bcsr = (u8 *)(CONFIG_SYS_BCSR);

	bcsr[9] &= ~(0x01);
}

void enable_8568mds_qe_mdio()
{
	u8 *bcsr = (u8 *)(CONFIG_SYS_BCSR);

	bcsr[7] |= 0x01;
}

#if defined(CONFIG_UEC_ETH1) || defined(CONFIG_UEC_ETH2)
void reset_8568mds_uccs(void)
{
	volatile u8 *bcsr = (u8 *)(CONFIG_SYS_BCSR);

	/* Turn off UCC1 & UCC2 */
	out_8(&bcsr[8], in_8(&bcsr[8]) & ~BCSR_UCC1_GETH_EN);
	out_8(&bcsr[9], in_8(&bcsr[9]) & ~BCSR_UCC2_GETH_EN);

	/* Mode is RGMII, all bits clear */
	out_8(&bcsr[11], in_8(&bcsr[11]) & ~(BCSR_UCC1_MODE_MSK |
					     BCSR_UCC2_MODE_MSK));

	/* Turn UCC1 & UCC2 on */
	out_8(&bcsr[8], in_8(&bcsr[8]) | BCSR_UCC1_GETH_EN);
	out_8(&bcsr[9], in_8(&bcsr[9]) | BCSR_UCC2_GETH_EN);
}
#endif
