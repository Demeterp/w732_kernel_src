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
 * (C) Copyright 2000, 2001, 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <mpc8xx.h>

/*-----------------------------------------------------------------------
 * Process Hardware Information Block:
 *
 * If we boot on a system fresh from factory, check if the Hardware
 * Information Block exists and save the information it contains.
 *
 * The TQM8xxL / TQM82xx Hardware Information Block is defined as
 * follows:
 * - located in first flash bank
 * - starts at offset 0x0003FFC0
 * - size 0x00000040
 *
 * Internal structure:
 * - sequence of ASCII character strings
 * - fields separated by a single space character (0x20)
 * - last field terminated by NUL character (0x00)
 * - remaining space filled with NUL characters (0x00)
 *
 * Fields in Hardware Information Block:
 * 1) Module Type
 * 2) Serial Number
 * 3) First MAC Address
 * 4) Number of additional MAC addresses
 */

void load_sernum_ethaddr (void)
{
	unsigned char *hwi;
	unsigned char  serial [CONFIG_SYS_HWINFO_SIZE];
	unsigned char  ethaddr[CONFIG_SYS_HWINFO_SIZE];
	unsigned short ih, is, ie, part;

	hwi = (unsigned char *)(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_HWINFO_OFFSET);
	ih = is = ie = 0;

	if (*((unsigned long *)hwi) != (unsigned long)CONFIG_SYS_HWINFO_MAGIC) {
		return;
	}

	part = 1;

	/* copy serial # / MAC address */
	while ((hwi[ih] != '\0') && (ih < CONFIG_SYS_HWINFO_SIZE)) {
		if (hwi[ih] < ' ' || hwi[ih] > '~') { /* ASCII strings! */
			return;
		}
		switch (part) {
		default:		/* Copy serial # */
			if (hwi[ih] == ' ') {
				++part;
			}
			serial[is++] = hwi[ih];
			break;
		case 3:			/* Copy MAC address */
			if (hwi[ih] == ' ') {
				++part;
				break;
			}
			ethaddr[ie++] = hwi[ih];
			if ((ie % 3) == 2)
				ethaddr[ie++] = ':';
			break;
		}
		++ih;
	}
	serial[is]  = '\0';
	if (ie && ethaddr[ie-1] == ':')
		--ie;
	ethaddr[ie] = '\0';

	/* set serial# and ethaddr if not yet defined */
	if (getenv("serial#") == NULL) {
		setenv ((char *)"serial#", (char *)serial);
	}

	if (getenv("ethaddr") == NULL) {
		setenv ((char *)"ethaddr", (char *)ethaddr);
	}
}
