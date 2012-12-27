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
 *  Header file for OneNAND support for U-Boot
 *
 *  Adaptation from kernel to U-Boot
 *
 *  Copyright (C) 2005-2007 Samsung Electronics
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __UBOOT_ONENAND_H
#define __UBOOT_ONENAND_H

#include <linux/types.h>

/* Forward declarations */
struct mtd_info;
struct mtd_oob_ops;
struct erase_info;
struct onenand_chip;

extern struct mtd_info onenand_mtd;
extern struct onenand_chip onenand_chip;

/* board */
extern void onenand_board_init(struct mtd_info *);

/* Functions */
extern void onenand_init(void);
extern int onenand_read(struct mtd_info *mtd, loff_t from, size_t len,
			size_t * retlen, u_char * buf);
extern int onenand_read_oob(struct mtd_info *mtd, loff_t from, struct mtd_oob_ops *ops);
extern int onenand_write(struct mtd_info *mtd, loff_t from, size_t len,
			 size_t * retlen, const u_char * buf);
extern int onenand_erase(struct mtd_info *mtd, struct erase_info *instr);

extern char *onenand_print_device_info(int device, int version);

extern unsigned onenand_block(struct onenand_chip *this, loff_t addr);

extern loff_t onenand_addr(struct onenand_chip *this, int block);

extern int flexonenand_region(struct mtd_info *mtd, loff_t addr);

extern int flexonenand_set_boundary(struct mtd_info *mtd, int die,
					int boundary, int lock);

/* S3C64xx */
extern void s3c64xx_onenand_init(struct mtd_info *);
extern void s3c64xx_set_width_regs(struct onenand_chip *);

#endif /* __UBOOT_ONENAND_H */
