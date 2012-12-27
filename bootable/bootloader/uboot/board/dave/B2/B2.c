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
 * (C) Copyright 2004
 * DAVE Srl
 * http://www.dave-tech.it
 * http://www.wawnet.biz
 * mailto:info@wawnet.biz
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
#include <netdev.h>
#include <asm/hardware.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Miscelaneous platform dependent initialization
 */

int board_init (void)
{
	u32 temp;

	/* Configuration Port Control Register*/
	/* Port A */
	PCONA = 0x3ff;

	/* Port B */
	PCONB = 0xff;
	PDATB = 0xFFFF;

	/* Port C */
	/*
	PCONC = 0xff55ff15;
	PDATC = 0x0;
	PUPC = 0xffff;
	*/

	/* Port D */
	/*
	PCOND = 0xaaaa;
	PUPD = 0xff;
	*/

	/* Port E */
	PCONE = 0x0001aaa9;
	PDATE = 0x0;
	PUPE = 0xff;

	/* Port F */
	PCONF = 0x124955;
	PDATF  = 0xff; /* B2-eth_reset tied high level */
	/*
	PUPF = 0x1e3;
	*/

	/* Port G */
	PUPG = 0x1;
	PCONG = 0x3; /*PG0= EINT0= ETH_INT prepared for linux kernel*/

	INTMSK = 0x03fffeff;
	INTCON = 0x05;

    /*
	Configure chip ethernet interrupt as High level
	Port G EINT 0-7 EINT0 -> CHIP ETHERNET
    */
	temp = EXTINT;
	temp &= ~0x7;
    temp |= 0x1; /*LEVEL_HIGH*/
	EXTINT = temp;

    /*
	Reset SMSC LAN91C96 chip
    */
    temp= PCONF;
    temp |= 0x00000040;
    PCONF = temp;

	/* Reset high */
    temp = PDATF;
    temp |= (1 << 3);
    PDATF = temp;

    /* Short delay */
    for (temp=0;temp<10;temp++)
    {
		/* NOP */
    }

    /* Reset low */
    temp = PDATF;
    temp &= ~(1 << 3);
    PDATF = temp;

	/* arch number MACH_TYPE_MBA44B0 */
	gd->bd->bi_arch_number = MACH_TYPE_S3C44B0;

	/* location of boot parameters */
	gd->bd->bi_boot_params = 0x0c000100;

	return 0;
}

int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return (0);
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_LAN91C96
	rc = lan91c96_initialize(0, CONFIG_LAN91C96_BASE);
#endif
	return rc;
}
#endif
