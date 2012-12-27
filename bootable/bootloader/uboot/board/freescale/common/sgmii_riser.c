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
 * Freescale SGMII Riser Card
 *
 * This driver supports the SGMII Riser card found on the
 * "DS" style of development board from Freescale.
 *
 * This software may be used and distributed according to the
 * terms of the GNU Public License, Version 2, incorporated
 * herein by reference.
 *
 * Copyright 2008 Freescale Semiconductor, Inc.
 *
 */

#include <config.h>
#include <common.h>
#include <net.h>
#include <libfdt.h>
#include <tsec.h>

void fsl_sgmii_riser_init(struct tsec_info_struct *tsec_info, int num)
{
	int i;

	for (i = 0; i < num; i++)
		if (tsec_info[i].flags & TSEC_SGMII)
			tsec_info[i].phyaddr += SGMII_RISER_PHY_OFFSET;
}

void fsl_sgmii_riser_fdt_fixup(void *fdt)
{
	struct eth_device *dev;
	int node;
	int i = -1;
	int etsec_num = 0;

	node = fdt_path_offset(fdt, "/aliases");
	if (node < 0)
		return;

	while ((dev = eth_get_dev_by_index(++i)) != NULL) {
		struct tsec_private *priv;
		int enet_node;
		char enet[16];
		const u32 *phyh;
		int phynode;
		const char *model;
		const char *path;

		if (!strstr(dev->name, "eTSEC"))
			continue;

		sprintf(enet, "ethernet%d", etsec_num++);
		path = fdt_getprop(fdt, node, enet, NULL);
		if (!path) {
			debug("No alias for %s\n", enet);
			continue;
		}

		enet_node = fdt_path_offset(fdt, path);
		if (enet_node < 0)
			continue;

		model = fdt_getprop(fdt, enet_node, "model", NULL);

		/*
		 * We only want to do this to eTSECs.  On some platforms
		 * there are more than one type of gianfar-style ethernet
		 * controller, and as we are creating an implicit connection
		 * between ethernet nodes and eTSEC devices, it is best to
		 * make the connection use as much explicit information
		 * as exists.
		 */
		if (!strstr(model, "TSEC"))
			continue;

		phyh = fdt_getprop(fdt, enet_node, "phy-handle", NULL);
		if (!phyh)
			continue;

		phynode = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(*phyh));

		priv = dev->priv;

		if (priv->flags & TSEC_SGMII)
			fdt_setprop_cell(fdt, phynode, "reg", priv->phyaddr);
	}
}
