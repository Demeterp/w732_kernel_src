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
 * CF IDE addon card code
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Copyright (c) 2005-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <asm/blackfin.h>

void cf_outb(unsigned char val, volatile unsigned char *addr)
{
	*(addr) = val;
	SSYNC();
}

unsigned char cf_inb(volatile unsigned char *addr)
{
	volatile unsigned char c;

	c = *(addr);
	SSYNC();

	return c;
}

void cf_insw(unsigned short *sect_buf, unsigned short *addr, int words)
{
	int i;

	for (i = 0; i < words; i++)
		*(sect_buf + i) = *(addr);
	SSYNC();
}

void cf_outsw(unsigned short *addr, unsigned short *sect_buf, int words)
{
	int i;

	for (i = 0; i < words; i++)
		*(addr) = *(sect_buf + i);
	SSYNC();
}

void cf_ide_init(void)
{
#if defined(CONFIG_BFIN_TRUE_IDE)
	/* Enable ATASEL when in True IDE mode */
	printf("Using CF True IDE Mode\n");
	cf_outb(0, (unsigned char *)CONFIG_CF_ATASEL_ENA);
	udelay(1000);
#elif defined(CONFIG_BFIN_CF_IDE)
	/* Disable ATASEL when we're in Common Memory Mode */
	printf("Using CF Common Memory Mode\n");
	cf_outb(0, (unsigned char *)CONFIG_CF_ATASEL_DIS);
	udelay(1000);
#elif defined(CONFIG_BFIN_HDD_IDE)
	printf("Using HDD IDE Mode\n");
#endif
	ide_init();
}
