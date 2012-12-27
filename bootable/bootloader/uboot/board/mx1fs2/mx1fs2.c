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
 * Copyright (C) 2004 Sascha Hauer, Pengutronix
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
 *
 */

#include <common.h>
#include <asm/arch/imx-regs.h>

DECLARE_GLOBAL_DATA_PTR;

#define SHOW_BOOT_PROGRESS(arg)        show_boot_progress(arg)

extern void imx_gpio_mode(int gpio_mode);

static void logo_init(void)
{
	imx_gpio_mode(PD15_PF_LD0);
	imx_gpio_mode(PD16_PF_LD1);
	imx_gpio_mode(PD17_PF_LD2);
	imx_gpio_mode(PD18_PF_LD3);
	imx_gpio_mode(PD19_PF_LD4);
	imx_gpio_mode(PD20_PF_LD5);
	imx_gpio_mode(PD21_PF_LD6);
	imx_gpio_mode(PD22_PF_LD7);
	imx_gpio_mode(PD23_PF_LD8);
	imx_gpio_mode(PD24_PF_LD9);
	imx_gpio_mode(PD25_PF_LD10);
	imx_gpio_mode(PD26_PF_LD11);
	imx_gpio_mode(PD27_PF_LD12);
	imx_gpio_mode(PD28_PF_LD13);
	imx_gpio_mode(PD29_PF_LD14);
	imx_gpio_mode(PD30_PF_LD15);
	imx_gpio_mode(PD14_PF_FLM_VSYNC);
	imx_gpio_mode(PD13_PF_LP_HSYNC);
	imx_gpio_mode(PD6_PF_LSCLK);
	imx_gpio_mode(GPIO_PORTD | GPIO_OUT | GPIO_DR);
	imx_gpio_mode(PD11_PF_CONTRAST);
	imx_gpio_mode(PD10_PF_SPL_SPR);

	LCDC_RMCR = 0x00000000;
	LCDC_PCR = PCR_COLOR | PCR_PBSIZ_8 | PCR_BPIX_16 | PCR_PCD(5);
	LCDC_HCR = HCR_H_WIDTH(2);
	LCDC_VCR = VCR_V_WIDTH(2);

	LCDC_PWMR = 0x00000380;   /* contrast to 0x80 middle (is best !!!) */
	LCDC_SSA  = 0x10040000;   /* image in flash */

	LCDC_SIZE = SIZE_XMAX(320) | SIZE_YMAX(240);   /* screen size */

	LCDC_VPW  = 0x000000A0;   /* Virtual Page Width Register */
	LCDC_POS  = 0x00000000;   /* panning offset 0 (0 pixel offset) */

	/* disable Cursor */
	LCDC_CPOS  = 0x00000000;

	/* fixed burst length */
	LCDC_DMACR = DMACR_BURST | DMACR_HM(8) | DMACR_TM(2);

	/* enable LCD */
	DR(3)   |= 0x00001000;
	LCDC_RMCR = RMCR_LCDC_EN;

}

int
board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_MX1FS2;
	gd->bd->bi_boot_params = 0x08000100;
serial_init();
	logo_init();
	return 0;
}

int
dram_init(void)
{
#if ( CONFIG_NR_DRAM_BANKS > 0 )
	gd->bd->bi_dram[0].start = MX1FS2_SDRAM_1;
	gd->bd->bi_dram[0].size = MX1FS2_SDRAM_1_SIZE;
#endif
	return 0;
}

/**
 * show_boot_progress: - indicate state of the boot process
 *
 * @param status: Status number - see README for details.
 *
 */

void
show_boot_progress(int status)
{
	/* We use this as a hook to disable serial ports just before booting
	 * This way we suppress the "uncompressing linux..." message
	 */
#ifdef CONFIG_SILENT_CONSOLE
	if( status == 8) {
		if( getenv("silent") != NULL ) {
			*(volatile unsigned long *)0x206080 &= ~1;
			*(volatile unsigned long *)0x207080 &= ~1;
		}
	}
#endif
	return;
}
