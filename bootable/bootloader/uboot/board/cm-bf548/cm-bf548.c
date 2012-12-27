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
 * U-boot - main board file
 *
 * Copyright (c) 2005-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <netdev.h>
#include <asm/blackfin.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	printf("Board: Bluetechnix CM-BF548 board\n");
	printf("       Support: http://www.bluetechnix.at/\n");
	return 0;
}

int board_early_init_f(void)
{
	/* Port H: PH8 - PH13 == A4 - A9
	 * address lines of the parallel asynchronous memory interface
	 */

	/************************************************
	* configure GPIO 				*
	* set port H function enable register		*
	*  configure PH8-PH13 as peripheral (not GPIO) 	*
	*************************************************/
	bfin_write_PORTH_FER(0x3F03);

	/************************************************
	* set port H MUX to configure PH8-PH13		*
	*  1st Function (MUX = 00) (bits 16-27 == 0)	*
	*  Set to address signals A4-A9 		*
	*************************************************/
	bfin_write_PORTH_MUX(0);

	/************************************************
	* set port H direction register			*
	*  enable PH8-PH13 as outputs			*
	*************************************************/
	bfin_write_PORTH_DIR_SET(0x3F00);

	/* Port I: PI0 - PH14 == A10 - A24
	 * address lines of the parallel asynchronous memory interface
	 */

	/************************************************
	* set port I function enable register		*
	*  configure PI0-PI14 as peripheral (not GPIO) 	*
	*************************************************/
	bfin_write_PORTI_FER(0x7fff);

	/**************************************************
	* set PORT I MUX to configure PI14-PI0 as	  *
	* 1st Function (MUX=00) - address signals A10-A24 *
	***************************************************/
	bfin_write_PORTI_MUX(0);

	/****************************************
	* set PORT I direction register		*
	*  enable PI0 - PI14 as outputs		*
	*****************************************/
	bfin_write_PORTI_DIR_SET(0x7fff);

	return 0;
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, CONFIG_SMC911X_BASE);
#endif
	return rc;
}
