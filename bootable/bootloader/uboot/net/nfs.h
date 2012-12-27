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
 * (C) Masami Komiya <mkomiya@sonare.it> 2004
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#ifndef __NFS_H__
#define __NFS_H__

#define SUNRPC_PORT     111

#define PROG_PORTMAP    100000
#define PROG_NFS        100003
#define PROG_MOUNT      100005

#define MSG_CALL        0
#define MSG_REPLY       1

#define PORTMAP_GETPORT 3

#define MOUNT_ADDENTRY  1
#define MOUNT_UMOUNTALL 4

#define NFS_LOOKUP      4
#define NFS_READLINK    5
#define NFS_READ        6

#define NFS_FHSIZE      32

#define NFSERR_PERM     1
#define NFSERR_NOENT    2
#define NFSERR_ACCES    13
#define NFSERR_ISDIR    21
#define NFSERR_INVAL    22

/* Block size used for NFS read accesses.  A RPC reply packet (including  all
 * headers) must fit within a single Ethernet frame to avoid fragmentation.
 * However, if CONFIG_IP_DEFRAG is set, the config file may want to use a
 * bigger value. In any case, most NFS servers are optimized for a power of 2.
 */
#ifdef CONFIG_NFS_READ_SIZE
#define NFS_READ_SIZE CONFIG_NFS_READ_SIZE
#else
#define NFS_READ_SIZE 1024 /* biggest power of two that fits Ether frame */
#endif

#define NFS_MAXLINKDEPTH 16

struct rpc_t {
	union {
		uint8_t data[2048];
		struct {
			uint32_t id;
			uint32_t type;
			uint32_t rpcvers;
			uint32_t prog;
			uint32_t vers;
			uint32_t proc;
			uint32_t data[1];
		} call;
		struct {
			uint32_t id;
			uint32_t type;
			uint32_t rstatus;
			uint32_t verifier;
			uint32_t v2;
			uint32_t astatus;
			uint32_t data[19];
		} reply;
	} u;
};
extern void	NfsStart (void);	/* Begin NFS */


/**********************************************************************/

#endif /* __NFS_H__ */
