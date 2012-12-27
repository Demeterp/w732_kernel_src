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
 * Copyright 2008 Freescale Semiconductor, Inc.
 * York Sun <yorksun@freescale.com>
 *
 * FSL DIU Framebuffer driver
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
#include <asm/io.h>

#include "../../../../board/freescale/common/fsl_diu_fb.h"

#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)
#include <stdio_dev.h>
#include <video_fb.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FSL_DIU_LOGO_BMP
extern unsigned int FSL_Logo_BMP[];
#else
#define FSL_Logo_BMP NULL
#endif

static int xres, yres;

void diu_set_pixel_clock(unsigned int pixclock)
{
	volatile immap_t *immap = (immap_t *)CONFIG_SYS_IMMR;
	volatile clk512x_t *clk = &immap->clk;
	volatile unsigned int *clkdvdr = &clk->scfr[0];
	unsigned long speed_ccb, temp, pixval;

	speed_ccb = get_bus_freq(0) * 4;
	temp = 1000000000/pixclock;
	temp *= 1000;
	pixval = speed_ccb / temp;
	debug("DIU pixval = %lu\n", pixval);

	/* Modify PXCLK in GUTS CLKDVDR */
	debug("DIU: Current value of CLKDVDR = 0x%08x\n", in_be32(clkdvdr));
	temp = in_be32(clkdvdr) & 0xFFFFFF00;
	out_be32(clkdvdr, temp | (pixval & 0xFF));
	debug("DIU: Modified value of CLKDVDR = 0x%08x\n", in_be32(clkdvdr));
}

char *valid_bmp(char *addr)
{
	unsigned long h_addr;
	bd_t *bd = gd->bd;

	h_addr = simple_strtoul(addr, NULL, 16);
	if (h_addr < bd->bi_flashstart ||
	    h_addr >= (bd->bi_flashstart + bd->bi_flashsize - 1)) {
		printf("bmp addr %lx is not a valid flash address\n", h_addr);
		return 0;
	} else if ((*(char *)(h_addr) != 'B') || (*(char *)(h_addr+1) != 'M')) {
		printf("bmp addr is not a bmp\n");
		return 0;
	} else
		return (char *)h_addr;
}

int mpc5121_diu_init(void)
{
	unsigned int pixel_format;
	char *bmp = NULL;
	char *bmp_env;

#if defined(CONFIG_VIDEO_XRES) & defined(CONFIG_VIDEO_YRES)
	xres = CONFIG_VIDEO_XRES;
	yres = CONFIG_VIDEO_YRES;
#else
	xres = 1024;
	yres = 768;
#endif
	pixel_format = 0x88883316;

	debug("mpc5121_diu_init\n");
	bmp_env = getenv("diu_bmp_addr");
	if (bmp_env) {
		bmp = valid_bmp(bmp_env);
	}
	if (!bmp)
		bmp = (char *)FSL_Logo_BMP;
	return fsl_diu_init(xres, pixel_format, 0, (unsigned char *)bmp);
}

int mpc5121diu_init_show_bmp(cmd_tbl_t *cmdtp,
			     int flag, int argc, char *argv[])
{
	unsigned int addr;

	if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	if (!strncmp(argv[1], "init", 4)) {
#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)
		fsl_diu_clear_screen();
		drv_video_init();
#else
		return mpc5121_diu_init();
#endif
	} else {
		addr = simple_strtoul(argv[1], NULL, 16);
		fsl_diu_clear_screen();
		fsl_diu_display_bmp((unsigned char *)addr, 0, 0, 0);
	}

	return 0;
}

U_BOOT_CMD(
	diufb, CONFIG_SYS_MAXARGS, 1, mpc5121diu_init_show_bmp,
	"Init or Display BMP file",
	"init\n    - initialize DIU\n"
	"addr\n    - display bmp at address 'addr'"
	);


#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)

/*
 * The Graphic Device
 */
GraphicDevice ctfb;
void *video_hw_init(void)
{
	GraphicDevice *pGD = (GraphicDevice *) &ctfb;
	struct fb_info *info;

	if (mpc5121_diu_init() < 0)
		return NULL;

	/* fill in Graphic device struct */
	sprintf(pGD->modeIdent, "%dx%dx%d %dkHz %dHz",
		xres, yres, 32, 64, 60);

	pGD->frameAdrs = (unsigned int)fsl_fb_open(&info);
	pGD->winSizeX = xres;
	pGD->winSizeY = yres - info->logo_height;
	pGD->plnSizeX = pGD->winSizeX;
	pGD->plnSizeY = pGD->winSizeY;

	pGD->gdfBytesPP = 4;
	pGD->gdfIndex = GDF_32BIT_X888RGB;

	pGD->isaBase = 0;
	pGD->pciBase = 0;
	pGD->memSize = info->screen_size - info->logo_size;

	/* Cursor Start Address */
	pGD->dprBase = 0;
	pGD->vprBase = 0;
	pGD->cprBase = 0;

	return (void *)pGD;
}

/**
  * Set the LUT
  *
  * @index: color number
  * @r: red
  * @b: blue
  * @g: green
  */
void video_set_lut
	(unsigned int index, unsigned char r, unsigned char g, unsigned char b)
{
	return;
}

#endif /* defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE) */
