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
 * caddy.c -- esd VME8349 support for "missing" access modes in TSI148.
 * Copyright (c) 2009 esd gmbh.
 *
 * Reinhard Arlt <reinhard.arlt@esd-electronics.com>
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
 *
 */

#include <common.h>
#include <ioports.h>
#include <mpc83xx.h>
#include <asm/mpc8349_pci.h>
#include <pci.h>
#include <asm/mmu.h>
#include <asm/io.h>

#include "caddy.h"

static struct caddy_interface *caddy_interface;

void generate_answer(struct caddy_cmd *cmd, uint32_t status, uint32_t *result)
{
	struct caddy_answer *answer;
	uint32_t ptr;

	answer = &caddy_interface->answer[caddy_interface->answer_in];
	memset((void *)answer, 0, sizeof(struct caddy_answer));
	answer->answer = cmd->cmd;
	answer->issue = cmd->issue;
	answer->status = status;
	memcpy(answer->par, result, 5 * sizeof(result[0]));
	ptr = caddy_interface->answer_in + 1;
	ptr = ptr & (ANSWER_SIZE - 1);
	if (ptr != caddy_interface->answer_out)
		caddy_interface->answer_in = ptr;
}

int do_caddy(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long base_addr;
	uint32_t ptr;
	struct caddy_cmd *caddy_cmd;
	uint32_t result[5];
	uint16_t data16;
	uint8_t data8;
	uint32_t status;
	pci_dev_t dev;
	void *pci_ptr;

	if (argc < 2) {
		puts("Missing parameter\n");
		return 1;
	}

	base_addr = simple_strtoul(argv[1], NULL, 16);
	caddy_interface = (struct caddy_interface *) base_addr;

	memset((void *)caddy_interface, 0, sizeof(struct caddy_interface));
	memcpy((void *)&caddy_interface->magic[0], &CADDY_MAGIC, 16);

	while (ctrlc() == 0) {
		if (caddy_interface->cmd_in != caddy_interface->cmd_out) {
			memset(result, 0, 5 * sizeof(result[0]));
			status = 0;
			caddy_cmd = &caddy_interface->cmd[caddy_interface->cmd_out];
			pci_ptr = (void *)CONFIG_SYS_PCI1_IO_PHYS +
				(caddy_cmd->addr & 0x001fffff);

			switch (caddy_cmd->cmd) {
			case CADDY_CMD_IO_READ_8:
				result[0] = in_8(pci_ptr);
				break;

			case CADDY_CMD_IO_READ_16:
				result[0] = in_be16(pci_ptr);
				break;

			case CADDY_CMD_IO_READ_32:
				result[0] = in_be32(pci_ptr);
				break;

			case CADDY_CMD_IO_WRITE_8:
				data8 = caddy_cmd->par[0] & 0x000000ff;
				out_8(pci_ptr, data8);
				break;

			case CADDY_CMD_IO_WRITE_16:
				data16 = caddy_cmd->par[0] & 0x0000ffff;
				out_be16(pci_ptr, data16);
				break;

			case CADDY_CMD_IO_WRITE_32:
				out_be32(pci_ptr, caddy_cmd->par[0]);
				break;

			case CADDY_CMD_CONFIG_READ_8:
				dev = PCI_BDF(caddy_cmd->par[0],
					      caddy_cmd->par[1],
					      caddy_cmd->par[2]);
				status = pci_read_config_byte(dev,
							      caddy_cmd->addr,
							      &data8);
				result[0] = data8;
				break;

			case CADDY_CMD_CONFIG_READ_16:
				dev = PCI_BDF(caddy_cmd->par[0],
					      caddy_cmd->par[1],
					      caddy_cmd->par[2]);
				status = pci_read_config_word(dev,
							      caddy_cmd->addr,
							      &data16);
				result[0] = data16;
				break;

			case CADDY_CMD_CONFIG_READ_32:
				dev = PCI_BDF(caddy_cmd->par[0],
					      caddy_cmd->par[1],
					      caddy_cmd->par[2]);
				status = pci_read_config_dword(dev,
							       caddy_cmd->addr,
							       &result[0]);
				break;

			case CADDY_CMD_CONFIG_WRITE_8:
				dev = PCI_BDF(caddy_cmd->par[0],
					      caddy_cmd->par[1],
					      caddy_cmd->par[2]);
				data8 = caddy_cmd->par[3] & 0x000000ff;
				status = pci_write_config_byte(dev,
							       caddy_cmd->addr,
							       data8);
				break;

			case CADDY_CMD_CONFIG_WRITE_16:
				dev = PCI_BDF(caddy_cmd->par[0],
					      caddy_cmd->par[1],
					      caddy_cmd->par[2]);
				data16 = caddy_cmd->par[3] & 0x0000ffff;
				status = pci_write_config_word(dev,
							       caddy_cmd->addr,
							       data16);
				break;

			case CADDY_CMD_CONFIG_WRITE_32:
				dev = PCI_BDF(caddy_cmd->par[0],
					      caddy_cmd->par[1],
					      caddy_cmd->par[2]);
				status = pci_write_config_dword(dev,
								caddy_cmd->addr,
								caddy_cmd->par[3]);
				break;

			default:
				status = 0xffffffff;
				break;
			}

			generate_answer(caddy_cmd, status, &result[0]);

			ptr = caddy_interface->cmd_out + 1;
			ptr = ptr & (CMD_SIZE - 1);
			caddy_interface->cmd_out = ptr;
		}

		caddy_interface->heartbeat++;
	}

	return 0;
}

U_BOOT_CMD(
	caddy,	2,	0,	do_caddy,
	"Start Caddy server.",
	"Start Caddy server with Data structure a given addr\n"
	);
