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
 * (C) Copyright 2008
 * Sergei Poselenov, Emcraft Systems, sposelenov@emcraft.com.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.         See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include <common.h>
#include <asm/processor.h>
#include <asm/immap_85xx.h>
#include <asm/fsl_ddr_sdram.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <spd_sdram.h>


#if !defined(CONFIG_SPD_EEPROM)
/*
 * Autodetect onboard DDR SDRAM on 85xx platforms
 *
 * NOTE: Some of the hardcoded values are hardware dependant,
 *       so this should be extended for other future boards
 *       using this routine!
 */
long int sdram_setup(int casl)
{
	volatile ccsr_ddr_t *ddr = (void *)(CONFIG_SYS_MPC85xx_DDR_ADDR);

	/*
	 * Disable memory controller.
	 */
	ddr->cs0_config = 0;
	ddr->sdram_cfg = 0;

	ddr->cs0_bnds = CONFIG_SYS_DDR_CS0_BNDS;
	ddr->cs0_config = CONFIG_SYS_DDR_CS0_CONFIG;
	ddr->timing_cfg_0 = CONFIG_SYS_DDR_TIMING_0;
	ddr->timing_cfg_1 = CONFIG_SYS_DDR_TIMING_1;
	ddr->timing_cfg_2 = CONFIG_SYS_DDR_TIMING_2;
	ddr->sdram_mode = CONFIG_SYS_DDR_MODE;
	ddr->sdram_interval = CONFIG_SYS_DDR_INTERVAL;
	ddr->sdram_cfg_2 = CONFIG_SYS_DDR_CONFIG_2;
	ddr->sdram_clk_cntl = CONFIG_SYS_DDR_CLK_CONTROL;

	asm ("sync;isync;msync");
	udelay(1000);

	ddr->sdram_cfg = CONFIG_SYS_DDR_CONFIG;
	asm ("sync; isync; msync");
	udelay(1000);

	if (get_ram_size(0, CONFIG_SYS_SDRAM_SIZE<<20) == CONFIG_SYS_SDRAM_SIZE<<20) {
		/*
		 * OK, size detected -> all done
		 */
		return CONFIG_SYS_SDRAM_SIZE<<20;
	}

	return 0;				/* nothing found !		*/
}
#endif

phys_size_t initdram (int board_type)
{
	long dram_size = 0;
#if defined(CONFIG_SPD_EEPROM)
	dram_size = fsl_ddr_sdram();
	dram_size = setup_ddr_tlbs(dram_size / 0x100000);
	dram_size *= 0x100000;
#else
	dram_size = sdram_setup(CONFIG_DDR_DEFAULT_CL);
#endif
	return dram_size;
}

#if defined(CONFIG_SYS_DRAM_TEST)
int testdram (void)
{
	uint *pstart = (uint *) CONFIG_SYS_MEMTEST_START;
	uint *pend = (uint *) CONFIG_SYS_MEMTEST_END;
	uint *p;

	printf ("SDRAM test phase 1:\n");
	for (p = pstart; p < pend; p++)
		*p = 0xaaaaaaaa;

	for (p = pstart; p < pend; p++) {
		if (*p != 0xaaaaaaaa) {
			printf ("SDRAM test fails at: %08x\n", (uint) p);
			return 1;
		}
	}

	printf ("SDRAM test phase 2:\n");
	for (p = pstart; p < pend; p++)
		*p = 0x55555555;

	for (p = pstart; p < pend; p++) {
		if (*p != 0x55555555) {
			printf ("SDRAM test fails at: %08x\n", (uint) p);
			return 1;
		}
	}

	printf ("SDRAM test passed.\n");
	return 0;
}
#endif
