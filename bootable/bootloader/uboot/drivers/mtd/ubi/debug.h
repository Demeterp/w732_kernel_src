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
 * Copyright (c) International Business Machines Corp., 2006
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Author: Artem Bityutskiy (Битюцкий Артём)
 */

#ifndef __UBI_DEBUG_H__
#define __UBI_DEBUG_H__

#ifdef CONFIG_MTD_UBI_DEBUG
#ifdef UBI_LINUX
#include <linux/random.h>
#endif

#define ubi_assert(expr)  BUG_ON(!(expr))
#define dbg_err(fmt, ...) ubi_err(fmt, ##__VA_ARGS__)
#else
#define ubi_assert(expr)  ({})
#define dbg_err(fmt, ...) ({})
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_DISABLE_BGT
#define DBG_DISABLE_BGT 1
#else
#define DBG_DISABLE_BGT 0
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_MSG
/* Generic debugging message */
#define dbg_msg(fmt, ...)                                    \
	printk(KERN_DEBUG "UBI DBG: %s: " fmt "\n", \
	       __FUNCTION__, ##__VA_ARGS__)

#define ubi_dbg_dump_stack() dump_stack()

struct ubi_ec_hdr;
struct ubi_vid_hdr;
struct ubi_volume;
struct ubi_vtbl_record;
struct ubi_scan_volume;
struct ubi_scan_leb;
struct ubi_mkvol_req;

void ubi_dbg_dump_ec_hdr(const struct ubi_ec_hdr *ec_hdr);
void ubi_dbg_dump_vid_hdr(const struct ubi_vid_hdr *vid_hdr);
void ubi_dbg_dump_vol_info(const struct ubi_volume *vol);
void ubi_dbg_dump_vtbl_record(const struct ubi_vtbl_record *r, int idx);
void ubi_dbg_dump_sv(const struct ubi_scan_volume *sv);
void ubi_dbg_dump_seb(const struct ubi_scan_leb *seb, int type);
void ubi_dbg_dump_mkvol_req(const struct ubi_mkvol_req *req);

#else

#define dbg_msg(fmt, ...)    ({})
#define ubi_dbg_dump_stack() ({})
#define ubi_dbg_dump_ec_hdr(ec_hdr)      ({})
#define ubi_dbg_dump_vid_hdr(vid_hdr)    ({})
#define ubi_dbg_dump_vol_info(vol)       ({})
#define ubi_dbg_dump_vtbl_record(r, idx) ({})
#define ubi_dbg_dump_sv(sv)              ({})
#define ubi_dbg_dump_seb(seb, type)      ({})
#define ubi_dbg_dump_mkvol_req(req)      ({})

#endif /* CONFIG_MTD_UBI_DEBUG_MSG */

#ifdef CONFIG_MTD_UBI_DEBUG_MSG_EBA
/* Messages from the eraseblock association unit */
#define dbg_eba(fmt, ...) dbg_msg(fmt, ##__VA_ARGS__)
#else
#define dbg_eba(fmt, ...) ({})
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_MSG_WL
/* Messages from the wear-leveling unit */
#define dbg_wl(fmt, ...) dbg_msg(fmt, ##__VA_ARGS__)
#else
#define dbg_wl(fmt, ...) ({})
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_MSG_IO
/* Messages from the input/output unit */
#define dbg_io(fmt, ...) dbg_msg(fmt, ##__VA_ARGS__)
#else
#define dbg_io(fmt, ...) ({})
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_MSG_BLD
/* Initialization and build messages */
#define dbg_bld(fmt, ...) dbg_msg(fmt, ##__VA_ARGS__)
#else
#define dbg_bld(fmt, ...) ({})
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_EMULATE_BITFLIPS
/**
 * ubi_dbg_is_bitflip - if it is time to emulate a bit-flip.
 *
 * Returns non-zero if a bit-flip should be emulated, otherwise returns zero.
 */
static inline int ubi_dbg_is_bitflip(void)
{
	return !(random32() % 200);
}
#else
#define ubi_dbg_is_bitflip() 0
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_EMULATE_WRITE_FAILURES
/**
 * ubi_dbg_is_write_failure - if it is time to emulate a write failure.
 *
 * Returns non-zero if a write failure should be emulated, otherwise returns
 * zero.
 */
static inline int ubi_dbg_is_write_failure(void)
{
	return !(random32() % 500);
}
#else
#define ubi_dbg_is_write_failure() 0
#endif

#ifdef CONFIG_MTD_UBI_DEBUG_EMULATE_ERASE_FAILURES
/**
 * ubi_dbg_is_erase_failure - if its time to emulate an erase failure.
 *
 * Returns non-zero if an erase failure should be emulated, otherwise returns
 * zero.
 */
static inline int ubi_dbg_is_erase_failure(void)
{
		return !(random32() % 400);
}
#else
#define ubi_dbg_is_erase_failure() 0
#endif

#endif /* !__UBI_DEBUG_H__ */
