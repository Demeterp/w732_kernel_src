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
 * (C) Copyright 2002-2004
 * Stefan Roese, esd gmbh germany, stefan.roese@esd-electronics.com
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
#include <command.h>
#include <malloc.h>
#include <net.h>
#include <asm/io.h>
#include <pci.h>
#include <asm/4xx_pci.h>
#include <asm/processor.h>

#include "pci405.h"

#if defined(CONFIG_CMD_BSP)

extern int do_bootm (cmd_tbl_t *, int, int, char *[]);

/*
 * Command loadpci: wait for signal from host and boot image.
 */
int do_loadpci(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int *ptr = 0;
	int count = 0;
	int count2 = 0;
	int status;
	int i;
	char addr[16];
	char str[] = "\\|/-";
	char *local_args[2];

	/*
	 * Mark sync address
	 */
	ptr = 0;
	*ptr = 0xffffffff;
	puts("\nWaiting for image from pci host -");

	/*
	 * Wait for host to write the start address
	 */
	while (*ptr == 0xffffffff) {
		count++;
		if (!(count % 100)) {
			count2++;
			putc(0x08); /* backspace */
			putc(str[count2 % 4]);
		}

		/* Abort if ctrl-c was pressed */
		if (ctrlc()) {
			puts("\nAbort\n");
			return 0;
		}

		udelay(1000);
	}

	if (*ptr == PCI_RECONFIG_MAGIC) {
		/*
		 * Save own pci configuration in PRAM
		 */
		memset((char *)PCI_REGS_ADDR, 0, PCI_REGS_LEN);
		ptr = (unsigned int *)PCI_REGS_ADDR + 1;
		for (i=0; i<0x40; i+=4) {
			pci_read_config_dword(PCIDEVID_405GP, i, ptr++);
		}
		ptr = (unsigned int *)PCI_REGS_ADDR;
		*ptr = crc32(0, (uchar *)PCI_REGS_ADDR+4, PCI_REGS_LEN-4);

		printf("\nStoring PCI Configuration Regs...\n");
	} else {
		sprintf(addr, "%08x", *ptr);

		/*
		 * Boot image via bootm
		 */
		printf("\nBooting Image at addr 0x%s ...\n", addr);
		setenv("loadaddr", addr);

		local_args[0] = argv[0];
		local_args[1] = NULL;
		status = do_bootm (cmdtp, 0, 1, local_args);
	}

	return 0;
}
U_BOOT_CMD(
	loadpci,	1,	1,	do_loadpci,
	"Wait for pci-image and boot it",
	""
);
#endif
