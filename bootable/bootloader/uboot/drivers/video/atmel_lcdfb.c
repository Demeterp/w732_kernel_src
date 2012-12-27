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
 * Driver for AT91/AT32 LCD Controller
 *
 * Copyright (C) 2007 Atmel Corporation
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
#include <asm/arch/gpio.h>
#include <asm/arch/clk.h>
#include <lcd.h>
#include <atmel_lcdc.h>

int lcd_line_length;
int lcd_color_fg;
int lcd_color_bg;

void *lcd_base;				/* Start of framebuffer memory	*/
void *lcd_console_address;		/* Start of console buffer	*/

short console_col;
short console_row;

/* configurable parameters */
#define ATMEL_LCDC_CVAL_DEFAULT		0xc8
#define ATMEL_LCDC_DMA_BURST_LEN	8
#ifndef ATMEL_LCDC_GUARD_TIME
#define ATMEL_LCDC_GUARD_TIME		1
#endif

#if defined(CONFIG_AT91SAM9263) || defined(CONFIG_AT91CAP9)
#define ATMEL_LCDC_FIFO_SIZE		2048
#else
#define ATMEL_LCDC_FIFO_SIZE		512
#endif

#define lcdc_readl(mmio, reg)		__raw_readl((mmio)+(reg))
#define lcdc_writel(mmio, reg, val)	__raw_writel((val), (mmio)+(reg))

void lcd_setcolreg(ushort regno, ushort red, ushort green, ushort blue)
{
#if defined(CONFIG_ATMEL_LCD_BGR555)
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_LUT(regno),
		    (red >> 3) | ((green & 0xf8) << 2) | ((blue & 0xf8) << 7));
#else
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_LUT(regno),
		    (blue >> 3) | ((green & 0xfc) << 3) | ((red & 0xf8) << 8));
#endif
}

void lcd_ctrl_init(void *lcdbase)
{
	unsigned long value;

	/* Turn off the LCD controller and the DMA controller */
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_PWRCON,
		    ATMEL_LCDC_GUARD_TIME << ATMEL_LCDC_GUARDT_OFFSET);

	/* Wait for the LCDC core to become idle */
	while (lcdc_readl(panel_info.mmio, ATMEL_LCDC_PWRCON) & ATMEL_LCDC_BUSY)
		udelay(10);

	lcdc_writel(panel_info.mmio, ATMEL_LCDC_DMACON, 0);

	/* Reset LCDC DMA */
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_DMACON, ATMEL_LCDC_DMARST);

	/* ...set frame size and burst length = 8 words (?) */
	value = (panel_info.vl_col * panel_info.vl_row *
		 NBITS(panel_info.vl_bpix)) / 32;
	value |= ((ATMEL_LCDC_DMA_BURST_LEN - 1) << ATMEL_LCDC_BLENGTH_OFFSET);
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_DMAFRMCFG, value);

	/* Set pixel clock */
	value = get_lcdc_clk_rate(0) / panel_info.vl_clk;
	if (get_lcdc_clk_rate(0) % panel_info.vl_clk)
		value++;
	value = (value / 2) - 1;

	if (!value) {
		lcdc_writel(panel_info.mmio, ATMEL_LCDC_LCDCON1, ATMEL_LCDC_BYPASS);
	} else
		lcdc_writel(panel_info.mmio, ATMEL_LCDC_LCDCON1,
			    value << ATMEL_LCDC_CLKVAL_OFFSET);

	/* Initialize control register 2 */
#ifdef CONFIG_AVR32
	value = ATMEL_LCDC_MEMOR_BIG | ATMEL_LCDC_CLKMOD_ALWAYSACTIVE;
#else
	value = ATMEL_LCDC_MEMOR_LITTLE | ATMEL_LCDC_CLKMOD_ALWAYSACTIVE;
#endif
	if (panel_info.vl_tft)
		value |= ATMEL_LCDC_DISTYPE_TFT;

	value |= panel_info.vl_sync;
	value |= (panel_info.vl_bpix << 5);
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_LCDCON2, value);

	/* Vertical timing */
	value = (panel_info.vl_vsync_len - 1) << ATMEL_LCDC_VPW_OFFSET;
	value |= panel_info.vl_upper_margin << ATMEL_LCDC_VBP_OFFSET;
	value |= panel_info.vl_lower_margin;
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_TIM1, value);

	/* Horizontal timing */
	value = (panel_info.vl_right_margin - 1) << ATMEL_LCDC_HFP_OFFSET;
	value |= (panel_info.vl_hsync_len - 1) << ATMEL_LCDC_HPW_OFFSET;
	value |= (panel_info.vl_left_margin - 1);
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_TIM2, value);

	/* Display size */
	value = (panel_info.vl_col - 1) << ATMEL_LCDC_HOZVAL_OFFSET;
	value |= panel_info.vl_row - 1;
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_LCDFRMCFG, value);

	/* FIFO Threshold: Use formula from data sheet */
	value = ATMEL_LCDC_FIFO_SIZE - (2 * ATMEL_LCDC_DMA_BURST_LEN + 3);
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_FIFO, value);

	/* Toggle LCD_MODE every frame */
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_MVAL, 0);

	/* Disable all interrupts */
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_IDR, ~0UL);

	/* Set contrast */
	value = ATMEL_LCDC_PS_DIV8 |
		ATMEL_LCDC_POL_POSITIVE |
		ATMEL_LCDC_ENA_PWMENABLE;
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_CONTRAST_CTR, value);
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_CONTRAST_VAL, ATMEL_LCDC_CVAL_DEFAULT);

	/* Set framebuffer DMA base address and pixel offset */
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_DMABADDR1, (u_long)lcdbase);

	lcdc_writel(panel_info.mmio, ATMEL_LCDC_DMACON, ATMEL_LCDC_DMAEN);
	lcdc_writel(panel_info.mmio, ATMEL_LCDC_PWRCON,
		    (ATMEL_LCDC_GUARD_TIME << ATMEL_LCDC_GUARDT_OFFSET) | ATMEL_LCDC_PWR);
}

ulong calc_fbsize(void)
{
	return ((panel_info.vl_col * panel_info.vl_row *
		NBITS(panel_info.vl_bpix)) / 8) + PAGE_SIZE;
}
