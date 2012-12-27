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
 * (C) Copyright 2002
 * James F. Dougherty, Broadcom Corporation, jfd@broadcom.com
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
#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <net.h>
#include <timestamp.h>
#include <dtt.h>
#include <mpc824x.h>
#include <asm/processor.h>
#include <linux/mtd/doc2000.h>

#include "bmw.h"
#include "m48t59y.h"
#include <pci.h>


int checkboard(void)
{
    ulong busfreq  = get_bus_freq(0);
    char  buf[32];

    puts ("Board: BMW MPC8245/KAHLUA2 - CHRP (MAP B)\n");
    printf("Built: %s at %s\n", U_BOOT_DATE, U_BOOT_TIME);
    /* printf("MPLD:  Revision %d\n", SYS_REVID_GET()); */
    printf("Local Bus at %s MHz\n", strmhz(buf, busfreq));
    return 0;
}

phys_size_t initdram(int board_type)
{
    return 64*1024*1024;
}


void
get_tod(void)
{
    int year, month, day, hour, minute, second;

    m48_tod_get(&year,
		&month,
		&day,
		&hour,
		&minute,
		&second);

    printf("  Current date/time: %d/%d/%d %d:%d:%d \n",
	   month, day, year, hour, minute, second);

}

/*
 * EPIC, PCI, and I/O devices.
 * Initialize Mousse Platform, probe for PCI devices,
 * Query configuration parameters if not set.
 */
int misc_init_f (void)
{
#if 0
    m48_tod_init(); /* Init SGS M48T59Y TOD/NVRAM */
    printf("RTC:   M48T589 TOD/NVRAM (%d) bytes\n",
	   TOD_NVRAM_SIZE);
    get_tod();
#endif

    sys_led_msg("BOOT");
    return 0;
}


/*
 * Initialize PCI Devices, report devices found.
 */
struct pci_controller hose;

void pci_init_board (void)
{
    pci_mpc824x_init(&hose);
    /* pci_dev_init(0); */
}

/*
 * Write characters to LCD display.
 * Note that the bytes for the first character is the last address.
 */
void
sys_led_msg(char* msg)
{
    LED_REG(0) = msg[3];
    LED_REG(1) = msg[2];
    LED_REG(2) = msg[1];
    LED_REG(3) = msg[0];
}

#ifdef CONFIG_CMD_DOC
/*
 * Map onboard TSOP-16MB DOC FLASH chip.
 */
void doc_init (void)
{
    doc_probe(DOC_BASE_ADDR);
}
#endif

#define NV_ADDR	((volatile unsigned char *) CONFIG_ENV_ADDR)

/* Read from NVRAM */
void*
nvram_read(void *dest, const long src, size_t count)
{
    int i;
    volatile unsigned char* d = (unsigned char*)dest;
    volatile unsigned char* s = (unsigned char*)src;

    for( i = 0; i < count;i++)
	d[i] = s[i];

    return dest;
}

/* Write to NVRAM */
void
nvram_write(long dest, const void *src, size_t count)
{
    int i;
    volatile unsigned char* d = (unsigned char*)dest;
    volatile unsigned char* s = (unsigned char*)src;

    SYS_TOD_UNPROTECT();

    for( i = 0; i < count;i++)
	d[i] = s[i];

    SYS_TOD_PROTECT();
}
