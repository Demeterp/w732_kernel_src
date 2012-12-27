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
 * Marubun MR-SHPC-01 PCMCIA controller device driver
 *
 * (c) 2007 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
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
 *
 */

#include <common.h>
#include <config.h>
#include <pcmcia.h>
#include <asm/io.h>

#undef CONFIG_PCMCIA

#if defined(CONFIG_CMD_PCMCIA)
#define	CONFIG_PCMCIA
#endif

#if defined(CONFIG_CMD_IDE)
#define	CONFIG_PCMCIA
#endif

#if defined(CONFIG_PCMCIA)

/* MR-SHPC-01 register */
#define MRSHPC_MODE	(CONFIG_SYS_MARUBUN_MRSHPC + 4)
#define MRSHPC_OPTION   (CONFIG_SYS_MARUBUN_MRSHPC + 6)
#define MRSHPC_CSR      (CONFIG_SYS_MARUBUN_MRSHPC + 8)
#define MRSHPC_ISR      (CONFIG_SYS_MARUBUN_MRSHPC + 10)
#define MRSHPC_ICR      (CONFIG_SYS_MARUBUN_MRSHPC + 12)
#define MRSHPC_CPWCR    (CONFIG_SYS_MARUBUN_MRSHPC + 14)
#define MRSHPC_MW0CR1   (CONFIG_SYS_MARUBUN_MRSHPC + 16)
#define MRSHPC_MW1CR1   (CONFIG_SYS_MARUBUN_MRSHPC + 18)
#define MRSHPC_IOWCR1   (CONFIG_SYS_MARUBUN_MRSHPC + 20)
#define MRSHPC_MW0CR2   (CONFIG_SYS_MARUBUN_MRSHPC + 22)
#define MRSHPC_MW1CR2   (CONFIG_SYS_MARUBUN_MRSHPC + 24)
#define MRSHPC_IOWCR2   (CONFIG_SYS_MARUBUN_MRSHPC + 26)
#define MRSHPC_CDCR     (CONFIG_SYS_MARUBUN_MRSHPC + 28)
#define MRSHPC_PCIC_INFO (CONFIG_SYS_MARUBUN_MRSHPC + 30)

int pcmcia_on (void)
{
	printf("Enable PCMCIA " PCMCIA_SLOT_MSG "\n");

	/* Init */
	outw( 0x0000 , MRSHPC_MODE );

	if ((inw(MRSHPC_CSR) & 0x000c) == 0){	/* if card detect is true */
		if ((inw(MRSHPC_CSR) & 0x0080) == 0){
			outw(0x0674 ,MRSHPC_CPWCR);  /* Card Vcc is 3.3v? */
		}else{
			outw(0x0678 ,MRSHPC_CPWCR);  /* Card Vcc is 5V */
		}
		udelay( 100000 );   /* wait for power on */
	}else{
		return 1;
	}
	/*
	 *	PC-Card window open
	 *	flag == COMMON/ATTRIBUTE/IO
	 */
	/* common window open */
	outw(0x8a84,MRSHPC_MW0CR1); /* window 0xb8400000 */
	if ((inw(MRSHPC_CSR) & 0x4000) != 0)
		outw(0x0b00,MRSHPC_MW0CR2); /* common mode & bus width 16bit SWAP = 1 */
	else
		outw(0x0300,MRSHPC_MW0CR2); /* common mode & bus width 16bit SWAP = 0 */

	/* attribute window open */
	outw(0x8a85,MRSHPC_MW1CR1); /* window 0xb8500000 */
	if ((inw(MRSHPC_CSR) & 0x4000) != 0)
		outw(0x0a00,MRSHPC_MW1CR2); /* attribute mode & bus width 16bit SWAP = 1 */
	else
		outw(0x0200,MRSHPC_MW1CR2); /* attribute mode & bus width 16bit SWAP = 0 */

	/* I/O window open */
	outw(0x8a86,MRSHPC_IOWCR1); /* I/O window 0xb8600000 */
	outw(0x0008,MRSHPC_CDCR);   /* I/O card mode */
	if ((inw(MRSHPC_CSR) & 0x4000) != 0)
		outw(0x0a00,MRSHPC_IOWCR2); /* bus width 16bit SWAP = 1 */
	else
		outw(0x0200,MRSHPC_IOWCR2); /* bus width 16bit SWAP = 0 */

	outw(0x0000,MRSHPC_ISR);
	outw(0x2000,MRSHPC_ICR);
	outb(0x00,(CONFIG_SYS_MARUBUN_MW2 + 0x206));
	outb(0x42,(CONFIG_SYS_MARUBUN_MW2 + 0x200));

	return 0;
}

int pcmcia_off (void)
{
	printf ("Disable PCMCIA " PCMCIA_SLOT_MSG "\n");

	return 0;
}

#endif /* CONFIG_PCMCIA */
