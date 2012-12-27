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
 * (C) Copyright 2008 Semihalf
 *
 * (C) Copyright 2000-2004
 * DENX Software Engineering
 * Wolfgang Denk, wd@denx.de
 *
 * Updated-by: Prafulla Wadaskar <prafulla@marvell.com>
 *		default_image specific code abstracted from mkimage.c
 *		some functions added to address abstraction
 *
 * All rights reserved.
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

#include "mkimage.h"
#include <image.h>
#include <u-boot/crc.h>

static image_header_t header;

static int image_check_image_types (uint8_t type)
{
	if ((type > IH_TYPE_INVALID) && (type < IH_TYPE_FLATDT))
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

static int image_check_params (struct mkimage_params *params)
{
	return	((params->dflag && (params->fflag || params->lflag)) ||
		(params->fflag && (params->dflag || params->lflag)) ||
		(params->lflag && (params->dflag || params->fflag)));
}

static int image_verify_header (unsigned char *ptr, int image_size,
			struct mkimage_params *params)
{
	uint32_t len;
	const unsigned char *data;
	uint32_t checksum;
	image_header_t header;
	image_header_t *hdr = &header;

	/*
	 * create copy of header so that we can blank out the
	 * checksum field for checking - this can't be done
	 * on the PROT_READ mapped data.
	 */
	memcpy (hdr, ptr, sizeof(image_header_t));

	if (be32_to_cpu(hdr->ih_magic) != IH_MAGIC) {
		fprintf (stderr,
			"%s: Bad Magic Number: \"%s\" is no valid image\n",
			params->cmdname, params->imagefile);
		return -FDT_ERR_BADMAGIC;
	}

	data = (const unsigned char *)hdr;
	len  = sizeof(image_header_t);

	checksum = be32_to_cpu(hdr->ih_hcrc);
	hdr->ih_hcrc = cpu_to_be32(0);	/* clear for re-calculation */

	if (crc32 (0, data, len) != checksum) {
		fprintf (stderr,
			"%s: ERROR: \"%s\" has bad header checksum!\n",
			params->cmdname, params->imagefile);
		return -FDT_ERR_BADSTATE;
	}

	data = (const unsigned char *)ptr + sizeof(image_header_t);
	len  = image_size - sizeof(image_header_t) ;

	checksum = be32_to_cpu(hdr->ih_dcrc);
	if (crc32 (0, data, len) != checksum) {
		fprintf (stderr,
			"%s: ERROR: \"%s\" has corrupted data!\n",
			params->cmdname, params->imagefile);
		return -FDT_ERR_BADSTRUCTURE;
	}
	return 0;
}

static void image_set_header (void *ptr, struct stat *sbuf, int ifd,
				struct mkimage_params *params)
{
	uint32_t checksum;

	image_header_t * hdr = (image_header_t *)ptr;

	checksum = crc32 (0,
			(const unsigned char *)(ptr +
				sizeof(image_header_t)),
			sbuf->st_size - sizeof(image_header_t));

	/* Build new header */
	image_set_magic (hdr, IH_MAGIC);
	image_set_time (hdr, sbuf->st_mtime);
	image_set_size (hdr, sbuf->st_size - sizeof(image_header_t));
	image_set_load (hdr, params->addr);
	image_set_ep (hdr, params->ep);
	image_set_dcrc (hdr, checksum);
	image_set_os (hdr, params->os);
	image_set_arch (hdr, params->arch);
	image_set_type (hdr, params->type);
	image_set_comp (hdr, params->comp);

	image_set_name (hdr, params->imagename);

	checksum = crc32 (0, (const unsigned char *)hdr,
				sizeof(image_header_t));

	image_set_hcrc (hdr, checksum);
}

/*
 * Default image type parameters definition
 */
static struct image_type_params defimage_params = {
	.name = "Default Image support",
	.header_size = sizeof(image_header_t),
	.hdr = (void*)&header,
	.check_image_type = image_check_image_types,
	.verify_header = image_verify_header,
	.print_header = image_print_contents,
	.set_header = image_set_header,
	.check_params = image_check_params,
};

void init_default_image_type (void)
{
	mkimage_register (&defimage_params);
}
