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
 * (C) Copyright 2004, Li-Pro.Net <www.li-pro.net>
 * Stephan Linz <linz@li-pro.net>
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
#include <command.h>
#include <status_led.h>

#if	defined(CONFIG_STATUS_LED)

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!       Q u i c k   &   D i r t y   H a c k	 !!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!						 !!!!!
 * !!!!! Next type definition was coming from original	 !!!!!
 * !!!!! status LED driver drivers/misc/status_led.c	 !!!!!
 * !!!!! and should be exported for using it here.	 !!!!!
 * !!!!!						 !!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

typedef struct {
	led_id_t mask;
	int state;
	int period;
	int cnt;
} led_dev_t;

extern led_dev_t led_dev[];

#if defined(CONFIG_CMD_BSP)
int do_sled (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int led_id = 0;

	if (argc > 1) {
#ifdef	STATUS_LED_BOOT
		if (!strcmp (argv[1], "boot")) {
			led_id = STATUS_LED_BOOT + 1;
		}
#endif
#ifdef	STATUS_LED_RED
		if (!strcmp (argv[1], "red")) {
			led_id = STATUS_LED_RED + 1;
		}
#endif
#ifdef	STATUS_LED_YELLOW
		if (!strcmp (argv[1], "yellow")) {
			led_id = STATUS_LED_YELLOW + 1;
		}
#endif
#ifdef	STATUS_LED_GREEN
		if (!strcmp (argv[1], "green")) {
			led_id = STATUS_LED_GREEN + 1;
		}
#endif
	}

	switch (argc) {
	case 1:
#if	(STATUS_LED_BITS > 3)
		for (; led_id < 4; led_id++)
#elif	(STATUS_LED_BITS > 2)
		for (; led_id < 3; led_id++)
#elif	(STATUS_LED_BITS > 1)
		for (; led_id < 2; led_id++)
#elif	(STATUS_LED_BITS > 0)
		for (; led_id < 1; led_id++)
#else
#error "*** STATUS_LED_BITS not correct defined ***"
#endif
		{
			printf ("Status LED '%s' is %s\n",
				led_id == STATUS_LED_BOOT ? "boot"
				: led_id == STATUS_LED_RED ? "red"
				: led_id == STATUS_LED_YELLOW ? "yellow"
				: led_id ==
				STATUS_LED_GREEN ? "green" : "unknown",
				led_dev[led_id].state ==
				STATUS_LED_ON ? "on" : led_dev[led_id].
				state ==
				STATUS_LED_OFF ? "off" : led_dev[led_id].
				state ==
				STATUS_LED_BLINKING ? "blinking" : "unknown");
		}
		return 0;
	case 2:
		if (led_id) {
			printf ("Status LED '%s' is %s\n", argv[1],
				led_dev[led_id - 1].state ==
				STATUS_LED_ON ? "on" : led_dev[led_id -
							       1].state ==
				STATUS_LED_OFF ? "off" : led_dev[led_id -
								 1].state ==
				STATUS_LED_BLINKING ? "blinking" : "unknown");
			return 0;
		} else
			break;
	case 3:
		if (led_id) {
			if (!strcmp (argv[2], "on")) {
				status_led_set (led_id - 1, STATUS_LED_ON);
				return 0;
			} else if (!strcmp (argv[2], "off")) {
				status_led_set (led_id - 1, STATUS_LED_OFF);
				return 0;
			} else if (!strcmp (argv[2], "blink")) {
				status_led_set (led_id - 1,
						STATUS_LED_BLINKING);
				return 0;
			} else
				break;
		} else
			break;
	default:
		break;
	}
	cmd_usage(cmdtp);
	return 1;
}

#ifdef	STATUS_LED_BOOT
#ifdef	STATUS_LED_RED
#ifdef	STATUS_LED_YELLOW
#ifdef	STATUS_LED_GREEN
#define __NAME_STR		"    - name: boot|red|yellow|green\n"
#else
#define __NAME_STR		"    - name: boot|red|yellow\n"
#endif
#else
#define __NAME_STR		"    - name: boot|red\n"
#endif
#else
#define __NAME_STR		"    - name: boot\n"
#endif
#else
#define __NAME_STR		"    - name: (no such defined)\n"
#endif

U_BOOT_CMD (sled, 3, 0, do_sled,
	    "check and set status led",
	    "sled [name [state]]\n" __NAME_STR "    - state: on|off|blink");
#endif
#endif	/* CONFIG_STATUS_LED */
