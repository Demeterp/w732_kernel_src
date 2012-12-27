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
 * (C) Copyright 2000
 * Sangmoon Kim, Etin Systems. dogoil@etinsys.com.
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
#include <mpc824x.h>
#include <net.h>
#include <pci.h>
#include <i2c.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard (void)
{
	/*TODO: Check processor type */

	puts (	"Board: Debris "
#ifdef CONFIG_MPC8240
		"8240"
#endif
#ifdef CONFIG_MPC8245
		"8245"
#endif
		" ##Test not implemented yet##\n");
	return 0;
}

#if 0	/* NOT USED */
int checkflash (void)
{
	/* TODO: XXX XXX XXX */
	printf ("## Test not implemented yet ##\n");

	return (0);
}
#endif

phys_size_t initdram (int board_type)
{
	int m, row, col, bank, i;
	unsigned long start, end;
	uint32_t mccr1;
	uint32_t mear1 = 0, emear1 = 0, msar1 = 0, emsar1 = 0;
	uint32_t mear2 = 0, emear2 = 0, msar2 = 0, emsar2 = 0;
	uint8_t mber = 0;

	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);

	if (i2c_reg_read (0x50, 2) != 0x04) return 0;	/* Memory type */
	m = i2c_reg_read (0x50, 5);	/* # of physical banks */
	row = i2c_reg_read (0x50, 3);	/* # of rows */
	col = i2c_reg_read (0x50, 4);	/* # of columns */
	bank = i2c_reg_read (0x50, 17);	/* # of logical banks */

	CONFIG_READ_WORD(MCCR1, mccr1);
	mccr1 &= 0xffff0000;

	start = CONFIG_SYS_SDRAM_BASE;
	end = start + (1 << (col + row + 3) ) * bank - 1;

	for (i = 0; i < m; i++) {
		mccr1 |= ((row == 13)? 2 : (bank == 4)? 0 : 3) << i * 2;
		if (i < 4) {
			msar1  |= ((start >> 20) & 0xff) << i * 8;
			emsar1 |= ((start >> 28) & 0xff) << i * 8;
			mear1  |= ((end >> 20) & 0xff) << i * 8;
			emear1 |= ((end >> 28) & 0xff) << i * 8;
		} else {
			msar2  |= ((start >> 20) & 0xff) << (i-4) * 8;
			emsar2 |= ((start >> 28) & 0xff) << (i-4) * 8;
			mear2  |= ((end >> 20) & 0xff) << (i-4) * 8;
			emear2 |= ((end >> 28) & 0xff) << (i-4) * 8;
		}
		mber |= 1 << i;
		start += (1 << (col + row + 3) ) * bank;
		end += (1 << (col + row + 3) ) * bank;
	}
	for (; i < 8; i++) {
		if (i < 4) {
			msar1  |= 0xff << i * 8;
			emsar1 |= 0x30 << i * 8;
			mear1  |= 0xff << i * 8;
			emear1 |= 0x30 << i * 8;
		} else {
			msar2  |= 0xff << (i-4) * 8;
			emsar2 |= 0x30 << (i-4) * 8;
			mear2  |= 0xff << (i-4) * 8;
			emear2 |= 0x30 << (i-4) * 8;
		}
	}

	CONFIG_WRITE_WORD(MCCR1, mccr1);
	CONFIG_WRITE_WORD(MSAR1, msar1);
	CONFIG_WRITE_WORD(EMSAR1, emsar1);
	CONFIG_WRITE_WORD(MEAR1, mear1);
	CONFIG_WRITE_WORD(EMEAR1, emear1);
	CONFIG_WRITE_WORD(MSAR2, msar2);
	CONFIG_WRITE_WORD(EMSAR2, emsar2);
	CONFIG_WRITE_WORD(MEAR2, mear2);
	CONFIG_WRITE_WORD(EMEAR2, emear2);
	CONFIG_WRITE_BYTE(MBER, mber);

	return (1 << (col + row + 3) ) * bank * m;
}

/*
 * Initialize PCI Devices, report devices found.
 */
#ifndef CONFIG_PCI_PNP
static struct pci_config_table pci_debris_config_table[] = {
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, 0x0f, PCI_ANY_ID,
	  pci_cfgfunc_config_device, { PCI_ENET0_IOADDR,
				       PCI_ENET0_MEMADDR,
				       PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, 0x10, PCI_ANY_ID,
	  pci_cfgfunc_config_device, { PCI_ENET1_IOADDR,
				       PCI_ENET1_MEMADDR,
				       PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER }},
	{ }
};
#endif

struct pci_controller hose = {
#ifndef CONFIG_PCI_PNP
	config_table: pci_debris_config_table,
#endif
};

void pci_init_board(void)
{
	pci_mpc824x_init(&hose);
}

void *nvram_read(void *dest, const long src, size_t count)
{
	volatile uchar *d = (volatile uchar*) dest;
	volatile uchar *s = (volatile uchar*) src;
	while(count--) {
		*d++ = *s++;
		asm volatile("sync");
	}
	return dest;
}

void nvram_write(long dest, const void *src, size_t count)
{
	volatile uchar *d = (volatile uchar*)dest;
	volatile uchar *s = (volatile uchar*)src;
	while(count--) {
		*d++ = *s++;
		asm volatile("sync");
	}
}

int misc_init_r(void)
{
	uchar ethaddr[6];

	if (eth_getenv_enetaddr("ethaddr", ethaddr))
		/* Write ethernet addr in NVRAM for VxWorks */
		nvram_write(CONFIG_ENV_ADDR + CONFIG_SYS_NVRAM_VXWORKS_OFFS,
				ethaddr, 6);

	return 0;
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}
