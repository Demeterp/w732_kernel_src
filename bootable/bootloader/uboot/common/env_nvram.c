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
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>

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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * 09-18-2001 Andreas Heppel, Sysgo RTS GmbH <aheppel@sysgo.de>
 *
 * It might not be possible in all cases to use 'memcpy()' to copy
 * the environment to NVRAM, as the NVRAM might not be mapped into
 * the memory space. (I.e. this is the case for the BAB750). In those
 * cases it might be possible to access the NVRAM using a different
 * method. For example, the RTC on the BAB750 is accessible in IO
 * space using its address and data registers. To enable usage of
 * NVRAM in those cases I invented the functions 'nvram_read()' and
 * 'nvram_write()', which will be activated upon the configuration
 * #define CONFIG_SYS_NVRAM_ACCESS_ROUTINE. Note, that those functions are
 * strongly dependent on the used HW, and must be redefined for each
 * board that wants to use them.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <linux/stddef.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SYS_NVRAM_ACCESS_ROUTINE
extern void *nvram_read(void *dest, const long src, size_t count);
extern void nvram_write(long dest, const void *src, size_t count);
env_t *env_ptr = NULL;
#else
env_t *env_ptr = (env_t *)CONFIG_ENV_ADDR;
#endif

char * env_name_spec = "NVRAM";

extern uchar default_environment[];

#ifdef CONFIG_AMIGAONEG3SE
uchar env_get_char_spec (int index)
{
#ifdef CONFIG_SYS_NVRAM_ACCESS_ROUTINE
	uchar c;

	nvram_read(&c, CONFIG_ENV_ADDR+index, 1);

	return c;
#else
	uchar retval;
	enable_nvram();
	retval = *((uchar *)(gd->env_addr + index));
	disable_nvram();
	return retval;
#endif
}
#else
uchar env_get_char_spec (int index)
{
#ifdef CONFIG_SYS_NVRAM_ACCESS_ROUTINE
	uchar c;

	nvram_read(&c, CONFIG_ENV_ADDR+index, 1);

	return c;
#else
	return *((uchar *)(gd->env_addr + index));
#endif
}
#endif

void env_relocate_spec (void)
{
#if defined(CONFIG_SYS_NVRAM_ACCESS_ROUTINE)
	nvram_read(env_ptr, CONFIG_ENV_ADDR, CONFIG_ENV_SIZE);
#else
	memcpy (env_ptr, (void*)CONFIG_ENV_ADDR, CONFIG_ENV_SIZE);
#endif
}

int saveenv (void)
{
	int rcode = 0;
#ifdef CONFIG_AMIGAONEG3SE
	enable_nvram();
#endif
#ifdef CONFIG_SYS_NVRAM_ACCESS_ROUTINE
	nvram_write(CONFIG_ENV_ADDR, env_ptr, CONFIG_ENV_SIZE);
#else
	if (memcpy ((char *)CONFIG_ENV_ADDR, env_ptr, CONFIG_ENV_SIZE) == NULL)
		    rcode = 1 ;
#endif
#ifdef CONFIG_AMIGAONEG3SE
	udelay(10000);
	disable_nvram();
#endif
	return rcode;
}


/************************************************************************
 * Initialize Environment use
 *
 * We are still running from ROM, so data use is limited
 */
int env_init (void)
{
#ifdef CONFIG_AMIGAONEG3SE
	enable_nvram();
#endif
#if defined(CONFIG_SYS_NVRAM_ACCESS_ROUTINE)
	ulong crc;
	uchar data[ENV_SIZE];
	nvram_read (&crc, CONFIG_ENV_ADDR, sizeof(ulong));
	nvram_read (data, CONFIG_ENV_ADDR+sizeof(ulong), ENV_SIZE);

	if (crc32(0, data, ENV_SIZE) == crc) {
		gd->env_addr  = (ulong)CONFIG_ENV_ADDR + sizeof(long);
#else
	if (crc32(0, env_ptr->data, ENV_SIZE) == env_ptr->crc) {
		gd->env_addr  = (ulong)&(env_ptr->data);
#endif
		gd->env_valid = 1;
	} else {
		gd->env_addr  = (ulong)&default_environment[0];
		gd->env_valid = 0;
	}
#ifdef CONFIG_AMIGAONEG3SE
	disable_nvram();
#endif
	return (0);
}
