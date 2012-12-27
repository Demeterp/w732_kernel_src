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
 * (C) Copyright 2005 REA Elektronik GmbH <www.rea.de>
 * Anders Larsen <alarsen@rea.de>
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
#include <asm/arch/AT91RM9200.h>
#include <netdev.h>
#include <asm/io.h>
#if defined(CONFIG_DRIVER_ETHER)
#include <at91rm9200_net.h>
#include <bcm5221.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */
/*
 * Miscelaneous platform dependent initialisations
 */

int board_init (void)
{
	/* Enable Ctrlc */
	console_init_f ();

	/* memory and cpu-speed are setup before relocation */
	/* so we do _nothing_ here */

	/* arch number of CSB637-Board */
	gd->bd->bi_arch_number = MACH_TYPE_CSB637;
	/* adress of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	return 0;
}

int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_SIZE;
	return 0;
}

#ifdef CONFIG_DRIVER_ETHER
#if defined(CONFIG_CMD_NET)

/*
 * Name:
 *	at91rm9200_GetPhyInterface
 * Description:
 *	Initialise the interface functions to the PHY
 * Arguments:
 *	None
 * Return value:
 *	None
 */
void at91rm9200_GetPhyInterface(AT91PS_PhyOps p_phyops)
{
	p_phyops->Init		 = bcm5221_InitPhy;
	p_phyops->IsPhyConnected = bcm5221_IsPhyConnected;
	p_phyops->GetLinkSpeed	 = bcm5221_GetLinkSpeed;
	p_phyops->AutoNegotiate	 = bcm5221_AutoNegotiate;
}

#endif
#endif	/* CONFIG_DRIVER_ETHER */

#ifdef CONFIG_DRIVER_AT91EMAC
int board_eth_init(bd_t *bis)
{
	int rc = 0;
	rc = at91emac_register(bis, 0);
	return rc;
}
#endif
