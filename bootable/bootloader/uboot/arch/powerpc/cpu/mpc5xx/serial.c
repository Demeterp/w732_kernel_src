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
 * (C) Copyright 2003
 * Martin Winistoerfer, martinwinistoerfer@gmx.ch.
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
 * Foundation,
 */

/*
 * File:		serial.c
 *
 * Discription:		Serial interface driver for SCI1 and SCI2.
 *			Since this code will be called from ROM use
 *			only non-static local variables.
 *
 */

#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <mpc5xx.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Local function prototypes
 */

static int ready_to_send(void);

/*
 * Minimal global serial functions needed to use one of the SCI modules.
 */

int serial_init (void)
{
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;

	serial_setbrg();

#if defined(CONFIG_5xx_CONS_SCI1)
	/* 10-Bit, 1 start bit, 8 data bit, no parity, 1 stop bit */
	immr->im_qsmcm.qsmcm_scc1r1 = SCI_M_10;
	immr->im_qsmcm.qsmcm_scc1r1 = SCI_TE | SCI_RE;
#else
	immr->im_qsmcm.qsmcm_scc2r1 = SCI_M_10;
	immr->im_qsmcm.qsmcm_scc2r1 = SCI_TE | SCI_RE;
#endif
	return 0;
}

void serial_putc(const char c)
{
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;

	/* Test for completition */
	if(ready_to_send()) {
#if defined(CONFIG_5xx_CONS_SCI1)
		immr->im_qsmcm.qsmcm_sc1dr = (short)c;
#else
		immr->im_qsmcm.qsmcm_sc2dr = (short)c;
#endif
		if(c == '\n') {
			if(ready_to_send());
#if defined(CONFIG_5xx_CONS_SCI1)
			immr->im_qsmcm.qsmcm_sc1dr = (short)'\r';
#else
			immr->im_qsmcm.qsmcm_sc2dr = (short)'\r';
#endif
		}
	}
}

int serial_getc(void)
{
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
	volatile short status;
	unsigned char tmp;

	/* New data ? */
	do {
#if defined(CONFIG_5xx_CONS_SCI1)
		status = immr->im_qsmcm.qsmcm_sc1sr;
#else
		status = immr->im_qsmcm.qsmcm_sc2sr;
#endif

#if defined(CONFIG_WATCHDOG)
		reset_5xx_watchdog (immr);
#endif
	} while ((status & SCI_RDRF) == 0);

	/* Read data */
#if defined(CONFIG_5xx_CONS_SCI1)
	tmp = (unsigned char)(immr->im_qsmcm.qsmcm_sc1dr & SCI_SCXDR_MK);
#else
	tmp = (unsigned char)( immr->im_qsmcm.qsmcm_sc2dr & SCI_SCXDR_MK);
#endif
	return	tmp;
}

int serial_tstc()
{
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
	short status;

	/* New data character ? */
#if defined(CONFIG_5xx_CONS_SCI1)
	status = immr->im_qsmcm.qsmcm_sc1sr;
#else
	status = immr->im_qsmcm.qsmcm_sc2sr;
#endif
	return (status & SCI_RDRF);
}

void serial_setbrg (void)
{
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
	short scxbr;

	/* Set baudrate */
	scxbr = (gd->cpu_clk / (32 * gd->baudrate));
#if defined(CONFIG_5xx_CONS_SCI1)
	immr->im_qsmcm.qsmcm_scc1r0 = (scxbr & SCI_SCXBR_MK);
#else
	immr->im_qsmcm.qsmcm_scc2r0 = (scxbr & SCI_SCXBR_MK);
#endif
}

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc(*s);
		++s;
	}
}

int ready_to_send(void)
{
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
	volatile short status;

	do {
#if defined(CONFIG_5xx_CONS_SCI1)
		status = immr->im_qsmcm.qsmcm_sc1sr;
#else
		status = immr->im_qsmcm.qsmcm_sc2sr;
#endif

#if defined(CONFIG_WATCHDOG)
		reset_5xx_watchdog (immr);
#endif
	} while ((status & SCI_TDRE) == 0);
	return 1;

}
