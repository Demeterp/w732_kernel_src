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

/*-----------------------------------------------------------------------------+
  |   This source code is dual-licensed.  You may use it under the terms of the
  |   GNU General Public License version 2, or under the license below.
  |
  |	  This source code has been made available to you by IBM on an AS-IS
  |	  basis.  Anyone receiving this source is licensed under IBM
  |	  copyrights to use it in any way he or she deems fit, including
  |	  copying it, modifying it, compiling it, and redistributing it either
  |	  with or without modifications.  No license under IBM patents or
  |	  patent applications is to be implied by the copyright license.
  |
  |	  Any user of this software should understand that IBM cannot provide
  |	  technical support for this software and will not be responsible for
  |	  any consequences resulting from the use of this software.
  |
  |	  Any person who transfers this source code or any derivative work
  |	  must include the IBM copyright notice, this paragraph, and the
  |	  preceding two paragraphs in the transferred software.
  |
  |	  COPYRIGHT   I B M   CORPORATION 1995
  |	  LICENSED MATERIAL  -	PROGRAM PROPERTY OF I B M
  +-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------+
  |
  |  File Name:	 miiphy.c
  |
  |  Function:	 This module has utilities for accessing the MII PHY through
  |	       the EMAC3 macro.
  |
  |  Author:	 Mark Wisner
  |
  |  Change Activity-
  |
  |  Date	 Description of Change					     BY
  |  ---------	 ---------------------					     ---
  |  05-May-99	 Created						     MKW
  |  01-Jul-99	 Changed clock setting of sta_reg from 66MHz to 50MHz to
  |		 better match OPB speed. Also modified delay times.	     JWB
  |  29-Jul-99	 Added Full duplex support				     MKW
  |  24-Aug-99	 Removed printf from dp83843_duplex()			     JWB
  |  19-Jul-00	 Ported to esd cpci405					     sr
  |  23-Dec-03	 Ported from miiphy.c to 440GX Travis Sawyer		     TBS
  |		 <travis.sawyer@sandburst.com>
  |
  +-----------------------------------------------------------------------------*/

#include <common.h>
#include <miiphy.h>
#include "IxOsal.h"
#include "IxEthAcc.h"
#include "IxEthAcc_p.h"
#include "IxEthAccMac_p.h"
#include "IxEthAccMii_p.h"

/***********************************************************/
/* Dump out to the screen PHY regs			   */
/***********************************************************/

void miiphy_dump (char *devname, unsigned char addr)
{
	unsigned long i;
	unsigned short data;


	for (i = 0; i < 0x1A; i++) {
		if (miiphy_read (devname, addr, i, &data)) {
			printf ("read error for reg %lx\n", i);
			return;
		}
		printf ("Phy reg %lx ==> %4x\n", i, data);

		/* jump to the next set of regs */
		if (i == 0x07)
			i = 0x0f;

	}			/* end for loop */
}				/* end dump */


/***********************************************************/
/* (Re)start autonegotiation				   */
/***********************************************************/
int phy_setup_aneg (char *devname, unsigned char addr)
{
	unsigned short ctl, adv;

	/* Setup standard advertise */
	miiphy_read (devname, addr, PHY_ANAR, &adv);
	adv |= (PHY_ANLPAR_ACK | PHY_ANLPAR_RF | PHY_ANLPAR_T4 |
		PHY_ANLPAR_TXFD | PHY_ANLPAR_TX | PHY_ANLPAR_10FD |
		PHY_ANLPAR_10);
	miiphy_write (devname, addr, PHY_ANAR, adv);

	/* Start/Restart aneg */
	miiphy_read (devname, addr, PHY_BMCR, &ctl);
	ctl |= (PHY_BMCR_AUTON | PHY_BMCR_RST_NEG);
	miiphy_write (devname, addr, PHY_BMCR, ctl);

	return 0;
}


int npe_miiphy_read (char *devname, unsigned char addr,
		     unsigned char reg, unsigned short *value)
{
	u16 val;

	ixEthAccMiiReadRtn(addr, reg, &val);
	*value = val;

	return 0;
}				/* phy_read */


int npe_miiphy_write (char *devname, unsigned char addr,
		      unsigned char reg, unsigned short value)
{
	ixEthAccMiiWriteRtn(addr, reg, value);
	return 0;
}				/* phy_write */
