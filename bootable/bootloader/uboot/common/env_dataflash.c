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

/* LowLevel function for DataFlash environment support
 * Author : Gilles Gastaldi (Atmel)
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
#include <environment.h>
#include <linux/stddef.h>
#include <dataflash.h>

DECLARE_GLOBAL_DATA_PTR;

env_t *env_ptr = NULL;

char * env_name_spec = "dataflash";

extern int read_dataflash (unsigned long addr, unsigned long size, char
*result);
extern int write_dataflash (unsigned long addr_dest, unsigned long addr_src,
		     unsigned long size);
extern int AT91F_DataflashInit (void);
extern uchar default_environment[];


uchar env_get_char_spec (int index)
{
	uchar c;
	read_dataflash(CONFIG_ENV_ADDR + index + offsetof(env_t,data),
	1, (char *)&c);
	return (c);
}

void env_relocate_spec (void)
{
	read_dataflash(CONFIG_ENV_ADDR, CONFIG_ENV_SIZE, (char *)env_ptr);
}

int saveenv(void)
{
	/* env must be copied to do not alter env structure in memory*/
	unsigned char temp[CONFIG_ENV_SIZE];
	memcpy(temp, env_ptr, CONFIG_ENV_SIZE);
	return write_dataflash(CONFIG_ENV_ADDR, (unsigned long)temp, CONFIG_ENV_SIZE);
}

/************************************************************************
 * Initialize Environment use
 *
 * We are still running from ROM, so data use is limited
 * Use a (moderately small) buffer on the stack
 */
int env_init(void)
{
	ulong crc, len, new;
	unsigned off;
	uchar buf[64];
	if (gd->env_valid == 0){
		AT91F_DataflashInit();	/* prepare for DATAFLASH read/write */

		/* read old CRC */
		read_dataflash(CONFIG_ENV_ADDR + offsetof(env_t, crc),
			sizeof(ulong), (char *)&crc);
		new = 0;
		len = ENV_SIZE;
		off = offsetof(env_t,data);
		while (len > 0) {
			int n = (len > sizeof(buf)) ? sizeof(buf) : len;
			read_dataflash(CONFIG_ENV_ADDR + off, n, (char *)buf);
			new = crc32 (new, buf, n);
			len -= n;
			off += n;
		}
		if (crc == new) {
			gd->env_addr  = offsetof(env_t,data);
			gd->env_valid = 1;
		} else {
			gd->env_addr  = (ulong)&default_environment[0];
			gd->env_valid = 0;
		}
	}

	return (0);
}
