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
 * (C) Copyright 2002 ELTEC Elektronik AG
 * Frank Gottschling <fgottschling@eltec.de>
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

/*
 * Local network srom writing for first time run
 */

/* includes */
#include <common.h>
#include <pci.h>
#include <net.h>
#include "srom.h"

extern int eepro100_write_eeprom (struct eth_device *dev,
				  int location, int addr_len,
				  unsigned short data);

/*----------------------------------------------------------------------------*/

unsigned short eepro100_srom_checksum (unsigned short *sromdata)
{
	unsigned short sum = 0;
	unsigned int i;

	for (i = 0; i < (EE_SIZE - 1); i++) {
		sum += sromdata[i];
	}
	return (EE_CHECKSUM - sum);
}

/*----------------------------------------------------------------------------*/

int eepro100_srom_store (unsigned short *source)
{
	int count;
	struct eth_device onboard_dev;

	/* get onboard network iobase */
	pci_read_config_dword (PCI_BDF (0, 0x10, 0), PCI_BASE_ADDRESS_0,
			       (unsigned int *) &onboard_dev.iobase);
	onboard_dev.iobase &= ~0xf;

	source[63] = eepro100_srom_checksum (source);

	for (count = 0; count < EE_SIZE; count++) {
		if (eepro100_write_eeprom ((struct eth_device *) &onboard_dev,
					   count, EE_ADDR_BITS,
					   SROM_SHORT (source)) == -1) {
			return -1;
		}
		source++;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/

#ifdef EEPRO100_SROM_CHECK

extern int read_eeprom (struct eth_device *dev, int location, int addr_len);

void eepro100_srom_load (unsigned short *destination)
{
	int count;
	struct eth_device onboard_dev;

#ifdef DEBUG
	int lr = 0;

	printf ("eepro100_srom_download:\n");
#endif

	/* get onboard network iobase */
	pci_read_config_dword (PCI_BDF (0, 0x10, 0), PCI_BASE_ADDRESS_0,
			       &onboard_dev.iobase);
	onboard_dev.iobase &= ~0xf;

	memset (destination, 0x65, 128);

	for (count = 0; count < 0x40; count++) {
		*destination++ = read_eeprom ((struct eth_device *) &onboard_dev,
					      count, EE_ADDR_BITS);
#ifdef DEBUG
		printf ("%04x ", *(destination - 1));
		if (lr++ == 7) {
			printf ("\n");
			lr = 0;
		}
#endif
	}
}
#endif /* EEPRO100_SROM_CHECK */

/*----------------------------------------------------------------------------*/
