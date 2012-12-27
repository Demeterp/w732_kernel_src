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
 * (C) Copyright 2007 Michal Simek
 * (C) Copyright 2004 Atmark Techno, Inc.
 *
 * Michal  SIMEK <monstr@monstr.eu>
 * Yasushi SHOJI <yashi@atmark-techno.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>

DECLARE_GLOBAL_DATA_PTR;

int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
{
	/* First parameter is mapped to $r5 for kernel boot args */
	void	(*theKernel) (char *, ulong, ulong);
	char	*commandline = getenv ("bootargs");
	ulong	rd_data_start, rd_data_end;

	if ((flag != 0) && (flag != BOOTM_STATE_OS_GO))
		return 1;

	int	ret;

	char	*of_flat_tree = NULL;
#if defined(CONFIG_OF_LIBFDT)
	ulong	of_size = 0;

	/* find flattened device tree */
	ret = boot_get_fdt (flag, argc, argv, images, &of_flat_tree, &of_size);
	if (ret)
		return 1;
#endif

	theKernel = (void (*)(char *, ulong, ulong))images->ep;

	/* find ramdisk */
	ret = boot_get_ramdisk (argc, argv, images, IH_ARCH_MICROBLAZE,
			&rd_data_start, &rd_data_end);
	if (ret)
		return 1;

	show_boot_progress (15);

	if (!(ulong) of_flat_tree)
		of_flat_tree = (char *)simple_strtoul (argv[3], NULL, 16);

#ifdef DEBUG
	printf ("## Transferring control to Linux (at address 0x%08lx) " \
				"ramdisk 0x%08lx, FDT 0x%08lx...\n",
		(ulong) theKernel, rd_data_start, (ulong) of_flat_tree);
#endif

#ifdef XILINX_USE_DCACHE
#ifdef XILINX_DCACHE_BYTE_SIZE
	flush_cache(0, XILINX_DCACHE_BYTE_SIZE);
#else
#warning please rebuild BSPs and update configuration
	flush_cache(0, 32768);
#endif
#endif
	/*
	 * Linux Kernel Parameters (passing device tree):
	 * r5: pointer to command line
	 * r6: pointer to ramdisk
	 * r7: pointer to the fdt, followed by the board info data
	 */
	theKernel (commandline, rd_data_start, (ulong) of_flat_tree);
	/* does not return */

	return 1;
}
