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
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <mpc8220.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Breath some life into the CPU...
 *
 * Set up the memory map,
 * initialize a bunch of registers.
 */
void cpu_init_f (void)
{
	volatile flexbus8220_t *flexbus = (volatile flexbus8220_t *) MMAP_FB;
	volatile pcfg8220_t *portcfg = (volatile pcfg8220_t *) MMAP_PCFG;
	volatile xlbarb8220_t *xlbarb = (volatile xlbarb8220_t *) MMAP_XLBARB;

	/* Pointer is writable since we allocated a register for it */
	gd = (gd_t *) (CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_GBL_DATA_OFFSET);

	/* Clear initial global data */
	memset ((void *) gd, 0, sizeof (gd_t));

	/* Clear all port configuration */
	portcfg->pcfg0 = 0;
	portcfg->pcfg1 = 0;
	portcfg->pcfg2 = 0;
	portcfg->pcfg3 = 0;
	portcfg->pcfg2 = CONFIG_SYS_GP1_PORT2_CONFIG;
	portcfg->pcfg3 = CONFIG_SYS_PCI_PORT3_CONFIG | CONFIG_SYS_GP2_PORT3_CONFIG;

	/*
	 * Flexbus Controller: configure chip selects and enable them
	 */
#if defined (CONFIG_SYS_CS0_BASE)
	flexbus->csar0 = CONFIG_SYS_CS0_BASE;

/* Sorcery-C can hang-up after CTRL reg initialization */
#if defined (CONFIG_SYS_CS0_CTRL)
	flexbus->cscr0 = CONFIG_SYS_CS0_CTRL;
#endif
	flexbus->csmr0 = ((CONFIG_SYS_CS0_MASK - 1) & 0xffff0000) | 1;
	__asm__ volatile ("sync");
#endif
#if defined (CONFIG_SYS_CS1_BASE)
	flexbus->csar1 = CONFIG_SYS_CS1_BASE;
	flexbus->cscr1 = CONFIG_SYS_CS1_CTRL;
	flexbus->csmr1 = ((CONFIG_SYS_CS1_MASK - 1) & 0xffff0000) | 1;
	__asm__ volatile ("sync");
#endif
#if defined (CONFIG_SYS_CS2_BASE)
	flexbus->csar2 = CONFIG_SYS_CS2_BASE;
	flexbus->cscr2 = CONFIG_SYS_CS2_CTRL;
	flexbus->csmr2 = ((CONFIG_SYS_CS2_MASK - 1) & 0xffff0000) | 1;
	portcfg->pcfg3 |= CONFIG_SYS_CS2_PORT3_CONFIG;
	__asm__ volatile ("sync");
#endif
#if defined (CONFIG_SYS_CS3_BASE)
	flexbus->csar3 = CONFIG_SYS_CS3_BASE;
	flexbus->cscr3 = CONFIG_SYS_CS3_CTRL;
	flexbus->csmr3 = ((CONFIG_SYS_CS3_MASK - 1) & 0xffff0000) | 1;
	portcfg->pcfg3 |= CONFIG_SYS_CS3_PORT3_CONFIG;
	__asm__ volatile ("sync");
#endif
#if defined (CONFIG_SYS_CS4_BASE)
	flexbus->csar4 = CONFIG_SYS_CS4_BASE;
	flexbus->cscr4 = CONFIG_SYS_CS4_CTRL;
	flexbus->csmr4 = ((CONFIG_SYS_CS4_MASK - 1) & 0xffff0000) | 1;
	portcfg->pcfg3 |= CONFIG_SYS_CS4_PORT3_CONFIG;
	__asm__ volatile ("sync");
#endif
#if defined (CONFIG_SYS_CS5_BASE)
	flexbus->csar5 = CONFIG_SYS_CS5_BASE;
	flexbus->cscr5 = CONFIG_SYS_CS5_CTRL;
	flexbus->csmr5 = ((CONFIG_SYS_CS5_MASK - 1) & 0xffff0000) | 1;
	portcfg->pcfg3 |= CONFIG_SYS_CS5_PORT3_CONFIG;
	__asm__ volatile ("sync");
#endif

	/* This section of the code cannot place in cpu_init_r(),
	   it will cause the system to hang */
	/* enable timebase */
	xlbarb->addrTenTimeOut = 0x1000;
	xlbarb->dataTenTimeOut = 0x1000;
	xlbarb->busActTimeOut = 0x2000;

	xlbarb->config = 0x00002000;

	/* Master Priority Enable */
	xlbarb->mastPriority = 0;
	xlbarb->mastPriEn = 0xff;
}

/*
 * initialize higher level parts of CPU like time base and timers
 */
int cpu_init_r (void)
{
	/* this may belongs to disable interrupt section */
	/* mask all interrupts */
	*(vu_long *) 0xf0000700 = 0xfffffc00;
	*(vu_long *) 0xf0000714 |= 0x0001ffff;
	*(vu_long *) 0xf0000710 &= ~0x00000f00;

	/* route critical ints to normal ints */
	*(vu_long *) 0xf0000710 |= 0x00000001;

#if defined(CONFIG_CMD_NET) && defined(CONFIG_MPC8220_FEC)
	/* load FEC microcode */
	loadtask (0, 2);
#endif
	return (0);
}
