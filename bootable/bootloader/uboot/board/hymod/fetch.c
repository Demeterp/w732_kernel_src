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
 * (C) Copyright 2001
 * Murray Jensen, CSIRO-MIT, <Murray.Jensen@csiro.au>
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
#include <net.h>

/* imports from input.c */
extern int hymod_get_ethaddr (void);

int
fetch_and_parse (char *fn, ulong addr, int (*cback)(uchar *, uchar *))
{
	char *ethaddr;
	uchar *fp, *efp;
	int rc, count = 0;

	while ((ethaddr = getenv ("ethaddr")) == NULL || *ethaddr == '\0') {

		printf ("*** Ethernet address is%s not set\n",
			count == 0 ? "" : " STILL");

		if ((rc = hymod_get_ethaddr ()) < 0) {
			if (rc == -1)
				puts ("\n*** interrupted!");
			else
				puts ("\n*** timeout!");
			printf (" - fetch of '%s' aborted\n", fn);
			return (0);
		}

		count++;
	}

	copy_filename (BootFile, fn, sizeof (BootFile));
	load_addr = addr;
	NetBootFileXferSize = 0;

	if (NetLoop (TFTP) == 0) {
		printf ("tftp transfer of file '%s' failed\n", fn);
		return (0);
	}

	if (NetBootFileXferSize == 0) {
		printf ("can't determine size of file '%s'\n", fn);
		return (0);
	}

	fp = (uchar *)load_addr;
	efp = fp + NetBootFileXferSize;

	do {
		uchar *name, *value;

		if (*fp == '#' || *fp == '\n') {
			/* skip this line */
			while (fp < efp && *fp++ != '\n')
				;
			continue;
		}

		name = fp;

		while (fp < efp && *fp != '=' && *fp != '\n')
			fp++;
		if (fp >= efp)
			break;
		if (*fp == '\n') {
			fp++;
			continue;
		}
		*fp++ = '\0';

		value = fp;

		while (fp < efp && *fp != '\n')
			fp++;
		if (fp[-1] == '\r')
			fp[-1] = '\0';
		*fp++ = '\0';	/* ok if we go off the end here */

		if ((*cback)(name, value) == 0)
			return (0);

	} while (fp < efp);

	return (1);
}
