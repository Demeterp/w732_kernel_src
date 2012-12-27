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
 * DENX Software Engineering <mk@denx.de>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#if defined(CONFIG_USB_OHCI_NEW) && defined(CONFIG_SYS_USB_OHCI_CPU_INIT)

#include <asm/arch/hardware.h>
#include <asm/arch/io.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/clk.h>

int usb_cpu_init(void)
{
	at91_pmc_t *pmc	= (at91_pmc_t *)AT91_PMC_BASE;

#if defined(CONFIG_AT91CAP9) || defined(CONFIG_AT91SAM9260) || \
    defined(CONFIG_AT91SAM9263) || defined(CONFIG_AT91SAM9G20) || \
    defined(CONFIG_AT91SAM9261)
	/* Enable PLLB */
	writel(get_pllb_init(), &pmc->pllbr);
	while ((readl(&pmc->sr) & AT91_PMC_LOCKB) != AT91_PMC_LOCKB)
		;
#endif

	/* Enable USB host clock. */
	writel(1 << AT91_ID_UHP, &pmc->pcer);
#ifdef CONFIG_AT91SAM9261
	writel(AT91_PMC_UHP | AT91_PMC_HCK0, &pmc->scer);
#else
	writel(AT91_PMC_UHP, &pmc->scer);
#endif

	return 0;
}

int usb_cpu_stop(void)
{
	at91_pmc_t *pmc	= (at91_pmc_t *)AT91_PMC_BASE;

	/* Disable USB host clock. */
	writel(1 << AT91_ID_UHP, &pmc->pcdr);
#ifdef CONFIG_AT91SAM9261
	writel(AT91_PMC_UHP | AT91_PMC_HCK0, &pmc->scdr);
#else
	writel(AT91_PMC_UHP, &pmc->scdr);
#endif

#if defined(CONFIG_AT91CAP9) || defined(CONFIG_AT91SAM9260) || \
    defined(CONFIG_AT91SAM9263) || defined(CONFIG_AT91SAM9G20)
	/* Disable PLLB */
	writel(0, &pmc->pllbr);
	while ((readl(&pmc->sr) & AT91_PMC_LOCKB) != 0)
		;
#endif

	return 0;
}

int usb_cpu_init_fail(void)
{
	return usb_cpu_stop();
}

#endif /* defined(CONFIG_USB_OHCI) && defined(CONFIG_SYS_USB_OHCI_CPU_INIT) */
