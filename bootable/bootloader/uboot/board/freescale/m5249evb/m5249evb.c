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
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <command.h>
#include <malloc.h>
#include <asm/immap.h>


/* Prototypes */
int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);

int checkboard (void) {
	ulong val;
	uchar val8;

	puts ("Board: ");
	puts("Freescale M5249EVB");
	val8 = ((uchar)~((uchar)mbar2_readLong(MCFSIM_GPIO1_READ) >> 4)) & 0xf;
	printf(" (Switch=%1X)\n", val8);

	/*
	 * Set LED on
	 */
	val = mbar2_readLong(MCFSIM_GPIO1_OUT) & ~CONFIG_SYS_GPIO1_LED;
	mbar2_writeLong(MCFSIM_GPIO1_OUT, val);   /* Set LED on */

	return 0;
};


phys_size_t initdram (int board_type) {
	unsigned long	junk = 0xa5a59696;

	/*
	 *  Note:
	 *	RC = ([(RefreshTime/#rows) / (1/BusClk)] / 16) - 1
	 */

#ifdef CONFIG_SYS_FAST_CLK
	/*
	 * Busclk=70MHz, RefreshTime=64ms, #rows=4096 (4K)
	 * SO=1, NAM=0, COC=0, RTIM=01 (6clk refresh), RC=39
	 */
	mbar_writeShort(MCFSIM_DCR, 0x8239);
#elif CONFIG_SYS_PLL_BYPASS
	/*
	 * Busclk=5.6448MHz, RefreshTime=64ms, #rows=8192 (8K)
	 * SO=1, NAM=0, COC=0, RTIM=01 (6clk refresh), RC=02
	 */
	mbar_writeShort(MCFSIM_DCR, 0x8202);
#else
	/*
	 * Busclk=36MHz, RefreshTime=64ms, #rows=4096 (4K)
	 * SO=1, NAM=0, COC=0, RTIM=01 (6clk refresh), RC=22 (562 bus clock cycles)
	 */
	mbar_writeShort(MCFSIM_DCR, 0x8222);
#endif

	/*
	 * SDRAM starts at 0x0000_0000, CASL=10, CBM=010, PS=10 (16bit port),
	 * PM=1 (continuous page mode)
	 */

	/* RE=0 (keep auto-refresh disabled while setting up registers) */
	mbar_writeLong(MCFSIM_DACR0, 0x00003324);

	/* BAM=007c (bits 22,21 are bank selects; 256kB blocks) */
	mbar_writeLong(MCFSIM_DMR0, 0x01fc0001);

	/** Precharge sequence **/
	mbar_writeLong(MCFSIM_DACR0, 0x0000332c); /* Set DACR0[IP] (bit 3) */
	*((volatile unsigned long *) 0x00) = junk; /* write to a memory location to init. precharge */
	udelay(0x10); /* Allow several Precharge cycles */

	/** Refresh Sequence **/
	mbar_writeLong(MCFSIM_DACR0, 0x0000b324); /* Enable the refresh bit, DACR0[RE] (bit 15) */
	udelay(0x7d0); /* Allow gobs of refresh cycles */

	/** Mode Register initialization **/
	mbar_writeLong(MCFSIM_DACR0, 0x0000b364);  /* Enable DACR0[IMRS] (bit 6); RE remains enabled */
	*((volatile unsigned long *) 0x800) = junk; /* Access RAM to initialize the mode register */

	return CONFIG_SYS_SDRAM_SIZE * 1024 * 1024;
};


int testdram (void) {
	/* TODO: XXX XXX XXX */
	printf ("DRAM test not implemented!\n");

	return (0);
}
