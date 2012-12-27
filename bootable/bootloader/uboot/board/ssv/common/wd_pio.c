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
#include <nios.h>
#include <nios-io.h>

#if	defined(CONFIG_HW_WATCHDOG)

#if !defined(CONFIG_HW_WDENA_BASE)
#error "*** CONFIG_HW_WDENA_BASE not defined ***"
#if !defined(CONFIG_HW_WDENA_BIT)
#error "*** CONFIG_HW_WDENA_BIT not defined ***"
#endif
#endif

#if !defined(CONFIG_HW_WDTOG_BASE)
#error "*** CONFIG_HW_WDTOG_BASE not defined ***"
#if !defined(CONFIG_HW_WDTOG_BIT)
#error "*** CONFIG_HW_WDTOG_BIT not defined ***"
#endif
#endif

#ifdef	CONFIG_HW_WDPORT_WRONLY	/* emulate read access */
static unsigned __wd_ena_pio_portval = 0;
#endif

#define	WD_PIO_INIT_DONE(V)	((V) & (1 << CONFIG_HW_WDENA_BIT))

void ssv_wd_pio_init(void)
{
	nios_pio_t *ena_piop	 = (nios_pio_t*)CONFIG_HW_WDENA_BASE;
	nios_pio_t *trg_piop	 = (nios_pio_t*)CONFIG_HW_WDTOG_BASE;

	trg_piop->data		&= ~(1 << CONFIG_HW_WDTOG_BIT);

#ifdef	CONFIG_HW_WDPORT_WRONLY	/* emulate read access */

	__wd_ena_pio_portval	|= (1 << CONFIG_HW_WDENA_BIT);
	ena_piop->data		 = __wd_ena_pio_portval;

#else	/* !CONFIG_HW_WDPORT_WRONLY */

	trg_piop->direction	|= (1 << CONFIG_HW_WDTOG_BIT);

	ena_piop->data		|= (1 << CONFIG_HW_WDENA_BIT);
	ena_piop->direction	|= (1 << CONFIG_HW_WDENA_BIT);

#endif	/* CONFIG_HW_WDPORT_WRONLY */
}

void ssv_wd_pio_done(void)
{
	nios_pio_t *piop	 = (nios_pio_t*)CONFIG_HW_WDENA_BASE;

#ifdef	CONFIG_HW_WDPORT_WRONLY	/* emulate read access */

	__wd_ena_pio_portval	&= ~(1 << CONFIG_HW_WDENA_BIT);
	piop->data		 = __wd_ena_pio_portval;

#else	/* !CONFIG_HW_WDPORT_WRONLY */

	piop->data		&= ~(1 << CONFIG_HW_WDENA_BIT);

#endif	/* CONFIG_HW_WDPORT_WRONLY */
}

void ssv_wd_pio_reset(void)
{
	nios_pio_t *trg_piop	 = (nios_pio_t*)CONFIG_HW_WDTOG_BASE;

#ifdef	CONFIG_HW_WDPORT_WRONLY
	if (WD_PIO_INIT_DONE(__wd_ena_pio_portval))
#else
	nios_pio_t *ena_piop	 = (nios_pio_t*)CONFIG_HW_WDENA_BASE;

	if (WD_PIO_INIT_DONE(ena_piop->data))
#endif
	{
		trg_piop->data	|=  (1 << CONFIG_HW_WDTOG_BIT);
		trg_piop->data	&= ~(1 << CONFIG_HW_WDTOG_BIT);
	}
}

void hw_watchdog_reset(void)
{
	int re_enable = disable_interrupts ();

	ssv_wd_pio_reset();
	if (re_enable)
		enable_interrupts ();
}

#if defined(CONFIG_CMD_BSP)
int do_wd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	nios_pio_t *ena_piop	 = (nios_pio_t*)CONFIG_HW_WDENA_BASE;

	switch (argc)
	{
	case 1:
		printf ("Watchdog timer status is %s\n",
#ifdef	CONFIG_HW_WDPORT_WRONLY
				WD_PIO_INIT_DONE(__wd_ena_pio_portval)
#else
				WD_PIO_INIT_DONE(ena_piop->data)
#endif
				? "on" : "off");
		return 0;
	case 2:
		if (!strcmp(argv[1],"on"))
		{
			ssv_wd_pio_init();
			printf("Watchdog timer now is on\n");
			return 0;
		}
		else if (!strcmp(argv[1],"off"))
		{
			ssv_wd_pio_done();
			printf("Watchdog timer now is off\n");
			return 0;
		}
		break;
	default:
		break;
	}
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	wd,	2,	1,	do_wd,
	"check and set watchdog",
	"on   - switch watchDog on\n"
	"wd off  - switch watchdog off\n"
	"wd      - print current status"
);
#endif
#endif	/* CONFIG_HW_WATCHDOG */
