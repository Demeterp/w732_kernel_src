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
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <i2c.h>

#if defined(CONFIG_ZEUS)

u8 buf_zeus_ce[] = {
/*00    01    02    03    04    05    06    07 */
  0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*08    09    0a    0b    0c    0d    0e    0f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*10    11    12    13    14    15    16    17 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*18    19    1a    1b    1c    1d    1e    1f */
  0x00, 0xc0, 0x50, 0x12, 0x72, 0x3e, 0x00, 0x00 };

u8 buf_zeus_pe[] = {

/* CPU_CLOCK_DIV 1    = 00
   CPU_PLB_FREQ_DIV 3 = 10
   OPB_PLB_FREQ_DIV 2 = 01
   EBC_PLB_FREQ_DIV 2 = 00
   MAL_PLB_FREQ_DIV 1 = 00
   PCI_PLB_FRQ_DIV 3  = 10
   PLL_PLLOUTA        = IS SET
   PLL_OPERATING      = IS NOT SET
   PLL_FDB_MUL 10     = 1010
   PLL_FWD_DIV_A 3    = 101
   PLL_FWD_DIV_B 3    = 101
   TUNE               = 0x2be */
/*00    01    02    03    04    05    06    07 */
  0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*08    09    0a    0b    0c    0d    0e    0f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*10    11    12    13    14    15    16    17 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*18    19    1a    1b    1c    1d    1e    1f */
  0x00, 0x60, 0x68, 0x2d, 0x42, 0xbe, 0x00, 0x00 };

static int update_boot_eeprom(void)
{
	u32 len = 0x20;
	u8 chip = CONFIG_SYS_I2C_EEPROM_ADDR;
	u8 *pbuf;
	u8 base;
	int i;

	if (in_be32((void *)GPIO0_IR) & GPIO_VAL(CONFIG_SYS_GPIO_ZEUS_PE)) {
		pbuf = buf_zeus_pe;
		base = 0x40;
	} else {
		pbuf = buf_zeus_ce;
		base = 0x00;
	}

	for (i = 0; i < len; i++, base++) {
		if (i2c_write(chip, base, 1, &pbuf[i], 1) != 0) {
			printf("i2c_write fail\n");
			return 1;
		}
		udelay(11000);
	}

	return 0;
}

int do_update_boot_eeprom(cmd_tbl_t* cmdtp, int flag, int argc, char* argv[])
{
	return update_boot_eeprom();
}

U_BOOT_CMD (
	update_boot_eeprom, 1, 1, do_update_boot_eeprom,
	"update boot eeprom content",
	""
);

#endif
