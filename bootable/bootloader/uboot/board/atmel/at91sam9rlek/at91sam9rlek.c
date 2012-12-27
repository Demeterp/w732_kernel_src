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
 * (C) Copyright 2007-2008
 * Stelian Pop <stelian.pop@leadtechdesign.com>
 * Lead Tech Design <www.leadtechdesign.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/at91sam9rl.h>
#include <asm/arch/at91sam9rl_matrix.h>
#include <asm/arch/at91sam9_smc.h>
#include <asm/arch/at91_common.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/at91_rstc.h>
#include <asm/arch/clk.h>
#include <asm/arch/gpio.h>
#include <asm/arch/io.h>
#include <lcd.h>
#include <atmel_lcdc.h>
#if defined(CONFIG_RESET_PHY_R) && defined(CONFIG_MACB)
#include <net.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */
/*
 * Miscelaneous platform dependent initialisations
 */

#ifdef CONFIG_CMD_NAND
static void at91sam9rlek_nand_hw_init(void)
{
	unsigned long csa;

	/* Enable CS3 */
	csa = at91_sys_read(AT91_MATRIX_EBICSA);
	at91_sys_write(AT91_MATRIX_EBICSA,
		       csa | AT91_MATRIX_CS3A_SMC_SMARTMEDIA);

	/* Configure SMC CS3 for NAND/SmartMedia */
	at91_sys_write(AT91_SMC_SETUP(3),
		       AT91_SMC_NWESETUP_(1) | AT91_SMC_NCS_WRSETUP_(0) |
		       AT91_SMC_NRDSETUP_(1) | AT91_SMC_NCS_RDSETUP_(0));
	at91_sys_write(AT91_SMC_PULSE(3),
		       AT91_SMC_NWEPULSE_(3) | AT91_SMC_NCS_WRPULSE_(3) |
		       AT91_SMC_NRDPULSE_(3) | AT91_SMC_NCS_RDPULSE_(3));
	at91_sys_write(AT91_SMC_CYCLE(3),
		       AT91_SMC_NWECYCLE_(5) | AT91_SMC_NRDCYCLE_(5));
	at91_sys_write(AT91_SMC_MODE(3),
		       AT91_SMC_READMODE | AT91_SMC_WRITEMODE |
		       AT91_SMC_EXNWMODE_DISABLE |
#ifdef CONFIG_SYS_NAND_DBW_16
		       AT91_SMC_DBW_16 |
#else /* CONFIG_SYS_NAND_DBW_8 */
		       AT91_SMC_DBW_8 |
#endif
		       AT91_SMC_TDF_(2));

	at91_sys_write(AT91_PMC_PCER, 1 << AT91SAM9RL_ID_PIOD);

	/* Configure RDY/BSY */
	at91_set_gpio_input(CONFIG_SYS_NAND_READY_PIN, 1);

	/* Enable NandFlash */
	at91_set_gpio_output(CONFIG_SYS_NAND_ENABLE_PIN, 1);

	at91_set_A_periph(AT91_PIN_PB4, 0);		/* NANDOE */
	at91_set_A_periph(AT91_PIN_PB5, 0);		/* NANDWE */
}
#endif

#ifdef CONFIG_LCD
vidinfo_t panel_info = {
	vl_col:		240,
	vl_row:		320,
	vl_clk:		4965000,
	vl_sync:	ATMEL_LCDC_INVLINE_INVERTED |
			ATMEL_LCDC_INVFRAME_INVERTED,
	vl_bpix:	3,
	vl_tft:		1,
	vl_hsync_len:	5,
	vl_left_margin:	1,
	vl_right_margin:33,
	vl_vsync_len:	1,
	vl_upper_margin:1,
	vl_lower_margin:0,
	mmio:		AT91SAM9RL_LCDC_BASE,
};

void lcd_enable(void)
{
	at91_set_gpio_value(AT91_PIN_PA30, 0);  /* power up */
}

void lcd_disable(void)
{
	at91_set_gpio_value(AT91_PIN_PA30, 1);  /* power down */
}
static void at91sam9rlek_lcd_hw_init(void)
{
	at91_set_B_periph(AT91_PIN_PC1, 0);	/* LCDPWR */
	at91_set_A_periph(AT91_PIN_PC5, 0);	/* LCDHSYNC */
	at91_set_A_periph(AT91_PIN_PC6, 0);	/* LCDDOTCK */
	at91_set_A_periph(AT91_PIN_PC7, 0);	/* LCDDEN */
	at91_set_A_periph(AT91_PIN_PC3, 0);	/* LCDCC */
	at91_set_B_periph(AT91_PIN_PC9, 0);	/* LCDD3 */
	at91_set_B_periph(AT91_PIN_PC10, 0);	/* LCDD4 */
	at91_set_B_periph(AT91_PIN_PC11, 0);	/* LCDD5 */
	at91_set_B_periph(AT91_PIN_PC12, 0);	/* LCDD6 */
	at91_set_B_periph(AT91_PIN_PC13, 0);	/* LCDD7 */
	at91_set_B_periph(AT91_PIN_PC15, 0);	/* LCDD11 */
	at91_set_B_periph(AT91_PIN_PC16, 0);	/* LCDD12 */
	at91_set_B_periph(AT91_PIN_PC17, 0);	/* LCDD13 */
	at91_set_B_periph(AT91_PIN_PC18, 0);	/* LCDD14 */
	at91_set_B_periph(AT91_PIN_PC19, 0);	/* LCDD15 */
	at91_set_B_periph(AT91_PIN_PC20, 0);	/* LCDD18 */
	at91_set_B_periph(AT91_PIN_PC21, 0);	/* LCDD19 */
	at91_set_B_periph(AT91_PIN_PC22, 0);	/* LCDD20 */
	at91_set_B_periph(AT91_PIN_PC23, 0);	/* LCDD21 */
	at91_set_B_periph(AT91_PIN_PC24, 0);	/* LCDD22 */
	at91_set_B_periph(AT91_PIN_PC25, 0);	/* LCDD23 */

	at91_sys_write(AT91_PMC_PCER, 1 << AT91SAM9RL_ID_LCDC);

	gd->fb_base = 0;
}

#ifdef CONFIG_LCD_INFO
#include <nand.h>
#include <version.h>

void lcd_show_board_info(void)
{
	ulong dram_size, nand_size;
	int i;
	char temp[32];

	lcd_printf ("%s\n", U_BOOT_VERSION);
	lcd_printf ("(C) 2008 ATMEL Corp\n");
	lcd_printf ("at91support@atmel.com\n");
	lcd_printf ("%s CPU at %s MHz\n",
		CONFIG_SYS_AT91_CPU_NAME,
		strmhz(temp, get_cpu_clk_rate()));

	dram_size = 0;
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
		dram_size += gd->bd->bi_dram[i].size;
	nand_size = 0;
	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++)
		nand_size += nand_info[i].size;
	lcd_printf ("  %ld MB SDRAM, %ld MB NAND\n",
		dram_size >> 20,
		nand_size >> 20 );
}
#endif /* CONFIG_LCD_INFO */
#endif


int board_init(void)
{
	/* Enable Ctrlc */
	console_init_f();

	/* arch number of AT91SAM9RLEK-Board */
	gd->bd->bi_arch_number = MACH_TYPE_AT91SAM9RLEK;
	/* adress of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	at91_serial_hw_init();
#ifdef CONFIG_CMD_NAND
	at91sam9rlek_nand_hw_init();
#endif
#ifdef CONFIG_HAS_DATAFLASH
	at91_spi0_hw_init(1 << 0);
#endif
#ifdef CONFIG_LCD
	at91sam9rlek_lcd_hw_init();
#endif
	return 0;
}

int dram_init(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_SIZE;
	return 0;
}
