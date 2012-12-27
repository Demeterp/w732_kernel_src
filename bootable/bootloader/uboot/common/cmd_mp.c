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
 * Copyright 2008-2009 Freescale Semiconductor, Inc.
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

int
cpu_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long cpuid;

	if (argc < 3) {
		cmd_usage(cmdtp);
		return 1;
	}

	cpuid = simple_strtoul(argv[1], NULL, 10);
	if (cpuid >= cpu_numcores()) {
		printf ("Core num: %lu is out of range[0..%d]\n",
				cpuid, cpu_numcores() - 1);
		return 1;
	}


	if (argc == 3) {
		if (strncmp(argv[2], "reset", 5) == 0) {
			cpu_reset(cpuid);
		} else if (strncmp(argv[2], "status", 6) == 0) {
			cpu_status(cpuid);
		} else if (strncmp(argv[2], "disable", 7) == 0) {
			return cpu_disable(cpuid);
		} else {
			cmd_usage(cmdtp);
			return 1;
		}
		return 0;
	}

	/* 4 or greater, make sure its release */
	if (strncmp(argv[2], "release", 7) != 0) {
		cmd_usage(cmdtp);
		return 1;
	}

	if (cpu_release(cpuid, argc - 3, argv + 3)) {
		cmd_usage(cmdtp);
		return 1;
	}

	return 0;
}

#ifdef CONFIG_PPC
#define CPU_ARCH_HELP \
	"                         [args] : <pir> <r3> <r6>\n" \
	"                                   pir - processor id (if writeable)\n" \
	"                                    r3 - value for gpr 3\n" \
	"                                    r6 - value for gpr 6\n" \
	"\n" \
	"     Use '-' for any arg if you want the default value.\n" \
	"     Default for r3 is <num> and r6 is 0\n" \
	"\n" \
	"     When cpu <num> is released r4 and r5 = 0.\n" \
	"     r7 will contain the size of the initial mapped area"
#endif

U_BOOT_CMD(
	cpu, CONFIG_SYS_MAXARGS, 1, cpu_cmd,
	"Multiprocessor CPU boot manipulation and release",
	    "<num> reset                 - Reset cpu <num>\n"
	"cpu <num> status                - Status of cpu <num>\n"
	"cpu <num> disable               - Disable cpu <num>\n"
	"cpu <num> release <addr> [args] - Release cpu <num> at <addr> with [args]"
#ifdef CPU_ARCH_HELP
	"\n"
	CPU_ARCH_HELP
#endif
);
