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
 * U-boot - cpu.c CPU specific functions
 *
 * Copyright (c) 2005-2008 Analog Devices Inc.
 *
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <asm/blackfin.h>
#include <asm/cplb.h>
#include <asm/mach-common/bits/core.h>
#include <asm/mach-common/bits/ebiu.h>
#include <asm/mach-common/bits/trace.h>

#include "cpu.h"
#include "serial.h"

ulong bfin_poweron_retx;

__attribute__ ((__noreturn__))
void cpu_init_f(ulong bootflag, ulong loaded_from_ldr)
{
#ifndef CONFIG_BFIN_BOOTROM_USES_EVT1
	/* Build a NOP slide over the LDR jump block.  Whee! */
	char nops[0xC];
	serial_early_puts("NOP Slide\n");
	memset(nops, 0x00, sizeof(nops));
	memcpy((void *)L1_INST_SRAM, nops, sizeof(nops));
#endif

	if (!loaded_from_ldr) {
		/* Relocate sections into L1 if the LDR didn't do it -- don't
		 * check length because the linker script does the size
		 * checking at build time.
		 */
		serial_early_puts("L1 Relocate\n");
		extern char _stext_l1[], _text_l1_lma[], _text_l1_len[];
		memcpy(&_stext_l1, &_text_l1_lma, (unsigned long)_text_l1_len);
		extern char _sdata_l1[], _data_l1_lma[], _data_l1_len[];
		memcpy(&_sdata_l1, &_data_l1_lma, (unsigned long)_data_l1_len);
	}
#if defined(__ADSPBF537__) || defined(__ADSPBF536__) || defined(__ADSPBF534__)
	/* The BF537 bootrom will reset the EBIU_AMGCTL register on us
	 * after it has finished loading the LDR.  So configure it again.
	 */
	else
		bfin_write_EBIU_AMGCTL(CONFIG_EBIU_AMGCTL_VAL);
#endif

	/* Save RETX so we can pass it while booting Linux */
	bfin_poweron_retx = bootflag;

#ifdef CONFIG_DEBUG_DUMP
	/* Turn on hardware trace buffer */
	bfin_write_TBUFCTL(TBUFPWR | TBUFEN);
#endif

#ifndef CONFIG_PANIC_HANG
	/* Reset upon a double exception rather than just hanging.
	 * Do not do bfin_read on SWRST as that will reset status bits.
	 */
	bfin_write_SWRST(DOUBLE_FAULT);
#endif

	serial_early_puts("Board init flash\n");
	board_init_f(bootflag);
}

int exception_init(void)
{
	bfin_write_EVT3(trap);
	return 0;
}

int irq_init(void)
{
#ifdef SIC_IMASK0
	bfin_write_SIC_IMASK0(0);
	bfin_write_SIC_IMASK1(0);
# ifdef SIC_IMASK2
	bfin_write_SIC_IMASK2(0);
# endif
#elif defined(SICA_IMASK0)
	bfin_write_SICA_IMASK0(0);
	bfin_write_SICA_IMASK1(0);
#else
	bfin_write_SIC_IMASK(0);
#endif
	bfin_write_EVT2(evt_default);	/* NMI */
	bfin_write_EVT5(evt_default);	/* hardware error */
	bfin_write_EVT6(evt_default);	/* core timer */
	bfin_write_EVT7(evt_default);
	bfin_write_EVT8(evt_default);
	bfin_write_EVT9(evt_default);
	bfin_write_EVT10(evt_default);
	bfin_write_EVT11(evt_default);
	bfin_write_EVT12(evt_default);
	bfin_write_EVT13(evt_default);
	bfin_write_EVT14(evt_default);
	bfin_write_EVT15(evt_default);
	bfin_write_ILAT(0);
	CSYNC();
	/* enable hardware error irq */
	irq_flags = 0x3f;
	local_irq_enable();
	return 0;
}
