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
 * BMW/MPC8245 Board definitions.
 * For more info, see http://www.vooha.com/
 *
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * James Dougherty (jfd@broadcom.com)
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

#ifndef __BMW_H
#define __BMW_H

/* System addresses */

#define PCI_SPECIAL_BASE	0xfe000000
#define PCI_SPECIAL_SIZE	0x01000000

#define EUMBBAR_VAL		0x80500000	/* Location of EUMB region */
#define EUMBSIZE		0x00100000	/* Size of EUMB region */

/* Extended ROM space devices */
#define DOC_BASE_ADDR           0xff000000      /* Onboard DOC TSOP 16MB */
#define DOC2_BASE_ADDR          0x70000000      /* DIP32 socket -> 1GB */
#define XROM_BASE_ADDR          0x7c000000      /* RCS2 (PAL / Satellite IO) */
#define PLD_REG_BASE		XROM_BASE_ADDR
#define LED_REG_BASE		(XROM_BASE_ADDR | 0x2000)
#define TOD_BASE		(XROM_BASE_ADDR | 0x4000)
#define LED_REG(x)              (*(volatile unsigned char *) \
				  (LED_REG_BASE + (x)))
#define XROM_DEV_SIZE		0x00006000

#define ENET_DEV_BASE		0x80000000

#define PLD_REG(off)		(*(volatile unsigned char *)\
				  (PLD_REG_BASE + (off)))

#define PLD_REVID_B1		0x7f	/* Fix me */
#define PLD_REVID_B2		0x01	/* Fix me */

#define SYS_HARD_RESET()	{ for (;;) PLD_REG(0) = 0; } /* clr 0x80 bit */
#define SYS_REVID_GET()		((int) PLD_REG(0) & 0x7f)
#define SYS_LED_OFF()		(PLD_REG(1) |= 0x80)
#define SYS_LED_ON()		(PLD_REG(1) &= ~0x80)
#define SYS_WATCHDOG_IRQ3()	(PLD_REG(2) |= 0x80)
#define SYS_WATCHDOG_RESET()	(PLD_REG(2) &= ~0x80)
#define SYS_TOD_PROTECT()	(PLD_REG(3) |= 0x80)
#define SYS_TOD_UNPROTECT()	(PLD_REG(3) &= ~0x80)

#define	TOD_REG_BASE		(TOD_BASE | 0x1ff0)
#define	TOD_NVRAM_BASE		TOD_BASE
#define	TOD_NVRAM_SIZE		0x1ff0
#define	TOD_NVRAM_LIMIT		(TOD_NVRAM_BASE + TOD_NVRAM_SIZE)
#define RTC(r)		        (TOD_BASE + r)

/* Onboard BCM570x device */
#define	PCI_ENET_IOADDR		0x80000000
#define	PCI_ENET_MEMADDR	0x80000000


#ifndef __ASSEMBLY__
/* C Function prototypes */
void sys_led_msg(char* msg);

#endif /* !__ASSEMBLY__ */

#endif /* __BMW_H */
