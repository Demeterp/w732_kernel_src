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
 * (C) Copyright 2007
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
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
#include <command.h>
#include <i2c.h>

static u8 boot_267_nor[] = {
	0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x00,
	0x14, 0xc0, 0x36, 0xcc, 0x00, 0x0c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static u8 boot_267_nand[] = {
	0xd0, 0x38, 0xc3, 0x50, 0x13, 0x88, 0x8e, 0x00,
	0x14, 0xc0, 0x36, 0xcc, 0x00, 0x0c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static int do_bootstrap(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	u8 chip;
	u8 *buf;
	int cpu_freq;

	if (argc < 3) {
		cmd_usage(cmdtp);
		return 1;
	}

	cpu_freq = simple_strtol(argv[1], NULL, 10);
	if (cpu_freq != 267) {
		printf("Unsupported cpu-frequency - only 267 supported\n");
		return 1;
	}

	/* use 0x50 as I2C EEPROM address for now */
	chip = 0x50;

	if ((strcmp(argv[2], "nor") != 0) &&
	    (strcmp(argv[2], "nand") != 0)) {
		printf("Unsupported boot-device - only nor|nand support\n");
		return 1;
	}

	if (strcmp(argv[2], "nand") == 0) {
		switch (cpu_freq) {
		case 267:
			buf = boot_267_nand;
			break;
		default:
			break;
		}
	} else {
		switch (cpu_freq) {
		case 267:
			buf = boot_267_nor;
			break;
		default:
			break;
		}
	}

	if (i2c_write(chip, 0, 1, buf, 16) != 0)
		printf("Error writing to EEPROM at address 0x%x\n", chip);
	udelay(CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS * 1000);
	if (i2c_write(chip, 0x10, 1, buf+16, 4) != 0)
		printf("Error2 writing to EEPROM at address 0x%x\n", chip);

	printf("Done\n");
	printf("Please power-cycle the board for the changes to take effect\n");

	return 0;
}

U_BOOT_CMD(
	bootstrap,	3,	0,	do_bootstrap,
	"program the I2C bootstrap EEPROM",
	"<cpu-freq> <nor|nand> - program the I2C bootstrap EEPROM"
);
