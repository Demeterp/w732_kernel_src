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
 * Ethernet:	An implementation of the Ethernet Device Driver suite for the
 *		uClinux 2.0.38 operating system. This Driver has been developed
 *		for AT75C220 board.
 *
 * NOTE:	The driver is implemented for one MAC
 *
 * Version:	@(#)at91rm9200_net.h	1.0.0	01/10/2001
 *
 * Authors:	Lineo Inc <www.lineo.com>
 *
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */

#ifndef AT91RM9200_ETHERNET
#define AT91RM9200_ETHERNET

#include <common.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>

#define FALSE 0
#define TRUE 1


#define ETHERNET_ADDRESS_SIZE           6

typedef unsigned char UCHAR;

/* Interface to drive the physical layer */
typedef struct _AT91S_PhyOps
{
	unsigned char (*Init)(AT91S_EMAC *pmac);
	unsigned int (*IsPhyConnected)(AT91S_EMAC  *pmac);
	unsigned char (*GetLinkSpeed)(AT91S_EMAC *pmac);
	unsigned char (*AutoNegotiate)(AT91S_EMAC *pmac, int *);

} AT91S_PhyOps,*AT91PS_PhyOps;


#define EMAC_DESC_DONE 0x00000001  /* ownership bit */
#define EMAC_DESC_WRAP 0x00000002  /* bit for wrap */

/******************  function prototypes **********************/

/* MII functions */
void at91rm9200_EmacEnableMDIO(AT91PS_EMAC p_mac);
void at91rm9200_EmacDisableMDIO(AT91PS_EMAC p_mac);
UCHAR at91rm9200_EmacReadPhy(AT91PS_EMAC p_mac, unsigned char RegisterAddress, unsigned short *pInput);
UCHAR at91rm9200_EmacWritePhy(AT91PS_EMAC p_mac, unsigned char RegisterAddress, unsigned short *pOutput);
void at91rm9200_GetPhyInterface(AT91PS_PhyOps p_phyops);

#endif /* AT91RM9200_ETHERNET */
