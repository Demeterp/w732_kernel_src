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
 * Heiko Schocher, DENX Software Engineering, <hs@denx.de>
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

#include "os_support.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "sha1.h"

int main (int argc, char **argv)
{
	unsigned char output[20];
	int i, len;

	char	*imagefile;
	char	*cmdname = *argv;
	unsigned char	*ptr;
	unsigned char	*data;
	struct stat sbuf;
	unsigned char	*ptroff;
	int	ifd;
	int	off;

	if (argc > 1) {
		imagefile = argv[1];
		ifd = open (imagefile, O_RDWR|O_BINARY);
		if (ifd < 0) {
			fprintf (stderr, "%s: Can't open %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}
		if (fstat (ifd, &sbuf) < 0) {
			fprintf (stderr, "%s: Can't stat %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}
		len = sbuf.st_size;
		ptr = (unsigned char *)mmap(0, len,
				    PROT_READ, MAP_SHARED, ifd, 0);
		if (ptr == (unsigned char *)MAP_FAILED) {
			fprintf (stderr, "%s: Can't read %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}

		/* create a copy, so we can blank out the sha1 sum */
		data = malloc (len);
		memcpy (data, ptr, len);
		off = SHA1_SUM_POS;
		ptroff = &data[len +  off];
		for (i = 0; i < SHA1_SUM_LEN; i++) {
			ptroff[i] = 0;
		}

		sha1_csum ((unsigned char *) data, len, (unsigned char *)output);

		printf ("U-Boot sum:\n");
	        for (i = 0; i < 20 ; i++) {
	            printf ("%02X ", output[i]);
	        }
	        printf ("\n");
		/* overwrite the sum in the bin file, with the actual */
		lseek (ifd, SHA1_SUM_POS, SEEK_END);
		if (write (ifd, output, SHA1_SUM_LEN) != SHA1_SUM_LEN) {
			fprintf (stderr, "%s: Can't write %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}

		free (data);
		(void) munmap((void *)ptr, len);
		(void) close (ifd);
	}

	return EXIT_SUCCESS;
}
