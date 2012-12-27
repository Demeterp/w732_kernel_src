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
 * (c) 2007 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <common.h>
#include "ax88796.h"

/*
 * Set 1 bit data
 */
static void ax88796_bitset(u32 bit)
{
	/* DATA1 */
	if( bit )
		EEDI_HIGH;
	else
		EEDI_LOW;

	EECLK_LOW;
	udelay(1000);
	EECLK_HIGH;
	udelay(1000);
	EEDI_LOW;
}

/*
 * Get 1 bit data
 */
static u8 ax88796_bitget(void)
{
	u8 bit;

	EECLK_LOW;
	udelay(1000);
	/* DATA */
	bit = EEDO;
	EECLK_HIGH;
	udelay(1000);

	return bit;
}

/*
 * Send COMMAND to EEPROM
 */
static void ax88796_eep_cmd(u8 cmd)
{
	ax88796_bitset(BIT_DUMMY);
	switch(cmd){
		case MAC_EEP_READ:
			ax88796_bitset(1);
			ax88796_bitset(1);
			ax88796_bitset(0);
			break;

		case MAC_EEP_WRITE:
			ax88796_bitset(1);
			ax88796_bitset(0);
			ax88796_bitset(1);
			break;

		case MAC_EEP_ERACE:
			ax88796_bitset(1);
			ax88796_bitset(1);
			ax88796_bitset(1);
			break;

		case MAC_EEP_EWEN:
			ax88796_bitset(1);
			ax88796_bitset(0);
			ax88796_bitset(0);
			break;

		case MAC_EEP_EWDS:
			ax88796_bitset(1);
			ax88796_bitset(0);
			ax88796_bitset(0);
			break;
		default:
			break;
	}
}

static void ax88796_eep_setaddr(u16 addr)
{
	int i ;

	for( i = 7 ; i >= 0 ; i-- )
		ax88796_bitset(addr & (1 << i));
}

/*
 * Get data from EEPROM
 */
static u16 ax88796_eep_getdata(void)
{
	ushort data = 0;
	int i;

	ax88796_bitget();	/* DUMMY */
	for( i = 0 ; i < 16 ; i++ ){
		data <<= 1;
		data |= ax88796_bitget();
	}
	return data;
}

static void ax88796_mac_read(u8 *buff)
{
	int i ;
	u16 data;
	u16 addr = 0;

	for( i = 0 ; i < 3; i++ )
	{
		EECS_HIGH;
		EEDI_LOW;
		udelay(1000);
		/* READ COMMAND */
		ax88796_eep_cmd(MAC_EEP_READ);
		/* ADDRESS */
		ax88796_eep_setaddr(addr++);
		/* GET DATA */
		data = ax88796_eep_getdata();
		*buff++ = (uchar)(data & 0xff);
		*buff++ = (uchar)((data >> 8) & 0xff);
		EECLK_LOW;
		EEDI_LOW;
		EECS_LOW;
	}
}

int get_prom(u8* mac_addr, u8* base_addr)
{
	u8 prom[32];
	int i;

	ax88796_mac_read(prom);
	for (i = 0; i < 6; i++){
		mac_addr[i] = prom[i];
	}
	return 1;
}
