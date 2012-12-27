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
 * U-boot - status leds
 *
 * Copyright (c) 2005-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <status_led.h>

static void set_led_f(int pf, int state)
{
	switch (state) {
		case STATUS_LED_OFF:      bfin_write_PORTFIO_CLEAR(pf);  break;
		case STATUS_LED_BLINKING: bfin_write_PORTFIO_TOGGLE(pf); break;
		case STATUS_LED_ON:       bfin_write_PORTFIO_SET(pf);    break;
	}
}
static void set_led_g(int pf, int state)
{
	switch (state) {
		case STATUS_LED_OFF:      bfin_write_PORTGIO_CLEAR(pf);  break;
		case STATUS_LED_BLINKING: bfin_write_PORTGIO_TOGGLE(pf); break;
		case STATUS_LED_ON:       bfin_write_PORTGIO_SET(pf);    break;
	}
}

static void set_leds(led_id_t mask, int state)
{
	if (mask & 0x1) set_led_f(PF8, state);
	if (mask & 0x2) set_led_g(PG11, state);
	if (mask & 0x4) set_led_g(PG12, state);
}

void __led_init(led_id_t mask, int state)
{
	bfin_write_PORTF_FER(bfin_read_PORTF_FER() & ~(PF8));
	bfin_write_PORTG_FER(bfin_read_PORTG_FER() & ~(PG11 | PG12));
	bfin_write_PORTFIO_INEN(bfin_read_PORTFIO_INEN() & ~(PF8));
	bfin_write_PORTGIO_INEN(bfin_read_PORTGIO_INEN() & ~(PG11 | PG12));
	bfin_write_PORTFIO_DIR(bfin_read_PORTFIO_DIR() | (PF8));
	bfin_write_PORTGIO_DIR(bfin_read_PORTGIO_DIR() | (PG11 | PG12));
}

void __led_set(led_id_t mask, int state)
{
	set_leds(mask, state);
}

void __led_toggle(led_id_t mask)
{
	set_leds(mask, STATUS_LED_BLINKING);
}
