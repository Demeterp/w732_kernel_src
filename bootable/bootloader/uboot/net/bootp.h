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
 *	Copied from LiMon - BOOTP.
 *
 *	Copyright 1994, 1995, 2000 Neil Russell.
 *	(See License)
 *	Copyright 2000 Paolo Scaffardi
 */

#ifndef __BOOTP_H__
#define __BOOTP_H__

#ifndef __NET_H__
#include	<net.h>
#endif /* __NET_H__ */

/**********************************************************************/

/*
 *	BOOTP header.
 */
#if defined(CONFIG_CMD_DHCP)
#define OPT_SIZE 312	/* Minimum DHCP Options size per RFC2131 - results in 576 byte pkt */
#else
#define OPT_SIZE 64
#endif

typedef struct
{
	uchar		bp_op;		/* Operation				*/
# define OP_BOOTREQUEST	1
# define OP_BOOTREPLY	2
	uchar		bp_htype;	/* Hardware type			*/
# define HWT_ETHER	1
	uchar		bp_hlen;	/* Hardware address length		*/
# define HWL_ETHER	6
	uchar		bp_hops;	/* Hop count (gateway thing)		*/
	ulong		bp_id;		/* Transaction ID			*/
	ushort		bp_secs;	/* Seconds since boot			*/
	ushort		bp_spare1;	/* Alignment				*/
	IPaddr_t	bp_ciaddr;	/* Client IP address			*/
	IPaddr_t	bp_yiaddr;	/* Your (client) IP address		*/
	IPaddr_t	bp_siaddr;	/* Server IP address			*/
	IPaddr_t	bp_giaddr;	/* Gateway IP address			*/
	uchar		bp_chaddr[16];	/* Client hardware address		*/
	char		bp_sname[64];	/* Server host name			*/
	char		bp_file[128];	/* Boot file name			*/
	char		bp_vend[OPT_SIZE];	/* Vendor information			*/
}	Bootp_t;

#define BOOTP_HDR_SIZE	sizeof (Bootp_t)
#define BOOTP_SIZE	(ETHER_HDR_SIZE + IP_HDR_SIZE + BOOTP_HDR_SIZE)

/**********************************************************************/
/*
 *	Global functions and variables.
 */

/* bootp.c */
extern ulong	BootpID;		/* ID of cur BOOTP request		*/
extern char	BootFile[128];		/* Boot file name			*/
extern int	BootpTry;
#ifdef CONFIG_BOOTP_RANDOM_DELAY
extern ulong	seed1, seed2;		/* seed for random BOOTP delay		*/
#endif


/* Send a BOOTP request */
extern void	BootpRequest (void);

/****************** DHCP Support *********************/
extern void DhcpRequest(void);

/* DHCP States */
typedef enum { INIT,
	       INIT_REBOOT,
	       REBOOTING,
	       SELECTING,
	       REQUESTING,
	       REBINDING,
	       BOUND,
	       RENEWING } dhcp_state_t;

#define DHCP_DISCOVER 1
#define DHCP_OFFER    2
#define DHCP_REQUEST  3
#define DHCP_DECLINE  4
#define DHCP_ACK      5
#define DHCP_NAK      6
#define DHCP_RELEASE  7

#define SELECT_TIMEOUT 3000UL	/* Milliseconds to wait for offers */

/**********************************************************************/

#endif /* __BOOTP_H__ */
