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
 * hwctl.c
 *
 * LinkStation HW Control Driver
 *
 * Copyright (C) 2001-2004  BUFFALO INC.
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License (GPL), incorporated herein by reference.
 * Drivers based on or derived from this code fall under the GPL and must
 * retain the authorship, copyright and license notice.  This file is not
 * a complete program and may only be used when the entire operating
 * system is licensed under the GPL.
 *
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/io.h>

#define mdelay(n)	udelay((n)*1000)

#define AVR_PORT CONFIG_SYS_NS16550_COM2

/* 2005.5.10 BUFFALO add */
/*--------------------------------------------------------------*/
static inline void miconCntl_SendUart(unsigned char dat)
{
	out_8((unsigned char *)AVR_PORT, dat);
	mdelay(1);
}

/*--------------------------------------------------------------*/
void miconCntl_SendCmd(unsigned char dat)
{
	int i;

	for (i=0; i<4; i++){
		miconCntl_SendUart(dat);
	}
}

/*--------------------------------------------------------------*/
void miconCntl_FanLow(void)
{
#ifdef CONFIG_HTGL
	miconCntl_SendCmd(0x5C);
#endif
}

/*--------------------------------------------------------------*/
void miconCntl_FanHigh(void)
{
#ifdef CONFIG_HTGL
	miconCntl_SendCmd(0x5D);
#endif
}

/*--------------------------------------------------------------*/
/* 1000Mbps */
void miconCntl_Eth1000M(int up)
{
#ifdef CONFIG_HTGL
	if (up)
		miconCntl_SendCmd(0x93);
	else
		miconCntl_SendCmd(0x92);
#else
	if (up)
		miconCntl_SendCmd(0x5D);
	else
		miconCntl_SendCmd(0x5C);
#endif
}

/*--------------------------------------------------------------*/
/* 100Mbps */
void miconCntl_Eth100M(int up)
{
#ifdef CONFIG_HTGL
	if (up)
		miconCntl_SendCmd(0x91);
	else
		miconCntl_SendCmd(0x90);
#else
	if (up)
		miconCntl_SendCmd(0x5C);
#endif
}

/*--------------------------------------------------------------*/
/* 10Mbps */
void miconCntl_Eth10M(int up)
{
#ifdef CONFIG_HTGL
	if (up)
		miconCntl_SendCmd(0x8F);
	else
		miconCntl_SendCmd(0x8E);
#else
	if (up)
		miconCntl_SendCmd(0x5C);
#endif
}

/*--------------------------------------------------------------*/
/*  */
void miconCntl_5f(void)
{
	miconCntl_SendCmd(0x5F);
	mdelay(100);
}

/*--------------------------------------------------------------*/
/* "reboot start" signal */
void miconCntl_Reboot(void)
{
	miconCntl_SendCmd(0x43);
}

/*--------------------------------------------------------------*/
/* Disable watchdog timer */
void miconCntl_DisWDT(void)
{
	miconCntl_SendCmd(0x41); /* A */
	miconCntl_SendCmd(0x46); /* F */
	miconCntl_SendCmd(0x4A); /* J */
	miconCntl_SendCmd(0x3E); /* > */
	miconCntl_SendCmd(0x56); /* V */
	miconCntl_SendCmd(0x3E); /* > */
	miconCntl_SendCmd(0x5A); /* Z */
	miconCntl_SendCmd(0x56); /* V */
	miconCntl_SendCmd(0x4B); /* K */
}
