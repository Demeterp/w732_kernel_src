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
 * (C) Copyright 2006
 * Markus Klotzbuecher, DENX Software Engineering <mk@denx.de>
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

#if defined(CONFIG_USB_OHCI_NEW) && defined(CONFIG_SYS_USB_OHCI_CPU_INIT)
# if defined(CONFIG_CPU_MONAHANS) || defined(CONFIG_PXA27X)

#include <asm/arch/pxa-regs.h>
#include <usb.h>

int usb_cpu_init(void)
{
#if defined(CONFIG_CPU_MONAHANS)
	/* Enable USB host clock. */
	CKENA |= (CKENA_2_USBHOST |  CKENA_20_UDC);
	udelay(100);
#endif
#if defined(CONFIG_PXA27X)
	/* Enable USB host clock. */
	CKEN |= CKEN10_USBHOST;
#endif

#if defined(CONFIG_CPU_MONAHANS)
	/* Configure Port 2 for Host (USB Client Registers) */
	UP2OCR = 0x3000c;
#endif

	UHCHR |= UHCHR_FHR;
	wait_ms(11);
	UHCHR &= ~UHCHR_FHR;

	UHCHR |= UHCHR_FSBIR;
	while (UHCHR & UHCHR_FSBIR)
		udelay(1);

#if defined(CONFIG_CPU_MONAHANS)
	UHCHR &= ~UHCHR_SSEP0;
#endif
#if defined(CONFIG_PXA27X)
	UHCHR &= ~UHCHR_SSEP2;
#endif
	UHCHR &= ~UHCHR_SSEP1;
	UHCHR &= ~UHCHR_SSE;

	return 0;
}

int usb_cpu_stop(void)
{
	UHCHR |= UHCHR_FHR;
	udelay(11);
	UHCHR &= ~UHCHR_FHR;

	UHCCOMS |= 1;
	udelay(10);

#if defined(CONFIG_CPU_MONAHANS)
	UHCHR |= UHCHR_SSEP0;
#endif
#if defined(CONFIG_PXA27X)
	UHCHR |= UHCHR_SSEP2;
#endif
	UHCHR |= UHCHR_SSEP1;
	UHCHR |= UHCHR_SSE;

	return 0;
}

int usb_cpu_init_fail(void)
{
	UHCHR |= UHCHR_FHR;
	udelay(11);
	UHCHR &= ~UHCHR_FHR;

	UHCCOMS |= 1;
	udelay(10);

#if defined(CONFIG_CPU_MONAHANS)
	UHCHR |= UHCHR_SSEP0;
#endif
#if defined(CONFIG_PXA27X)
	UHCHR |= UHCHR_SSEP2;
#endif
	UHCHR |= UHCHR_SSEP1;
	UHCHR |= UHCHR_SSE;

	return 0;
}

# endif /* defined(CONFIG_CPU_MONAHANS) || defined(CONFIG_PXA27X) */
#endif /* defined(CONFIG_USB_OHCI) && defined(CONFIG_SYS_USB_OHCI_CPU_INIT) */
