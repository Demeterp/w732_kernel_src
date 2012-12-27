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
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <net.h>
#include "nfs.h"
#include "bootp.h"
#include "rarp.h"
#include "tftp.h"

#define TIMEOUT		5000UL	/* Milliseconds before trying BOOTP again */
#ifndef	CONFIG_NET_RETRY_COUNT
# define TIMEOUT_COUNT	5		/* # of timeouts before giving up  */
#else
# define TIMEOUT_COUNT  (CONFIG_NET_RETRY_COUNT)
#endif


int		RarpTry;

/*
 *	Handle a RARP received packet.
 */
static void
RarpHandler(uchar * dummi0, unsigned dummi1, unsigned dummi2, unsigned dummi3)
{
	char *s;
	debug("Got good RARP\n");
	if ((s = getenv("autoload")) != NULL) {
		if (*s == 'n') {
			/*
			 * Just use RARP to configure system;
			 * Do not use TFTP/NFS to to load the bootfile.
			 */
			NetState = NETLOOP_SUCCESS;
			return;
#if defined(CONFIG_CMD_NFS)
		} else if ((s != NULL) && !strcmp(s, "NFS")) {
			NfsStart();
			return;
#endif
		}
	}
	TftpStart ();
}


/*
 *	Timeout on BOOTP request.
 */
static void
RarpTimeout(void)
{
	if (RarpTry >= TIMEOUT_COUNT) {
		puts ("\nRetry count exceeded; starting again\n");
		NetStartAgain ();
	} else {
		NetSetTimeout (TIMEOUT, RarpTimeout);
		RarpRequest ();
	}
}


void
RarpRequest (void)
{
	int i;
	volatile uchar *pkt;
	ARP_t *	rarp;

	printf("RARP broadcast %d\n", ++RarpTry);
	pkt = NetTxPacket;

	pkt += NetSetEther(pkt, NetBcastAddr, PROT_RARP);

	rarp = (ARP_t *)pkt;

	rarp->ar_hrd = htons (ARP_ETHER);
	rarp->ar_pro = htons (PROT_IP);
	rarp->ar_hln = 6;
	rarp->ar_pln = 4;
	rarp->ar_op  = htons (RARPOP_REQUEST);
	memcpy (&rarp->ar_data[0],  NetOurEther, 6);	/* source ET addr */
	memcpy (&rarp->ar_data[6],  &NetOurIP,   4);	/* source IP addr */
	memcpy (&rarp->ar_data[10], NetOurEther, 6);	/* dest ET addr = source ET addr ??*/
	/* dest. IP addr set to broadcast */
	for (i = 0; i <= 3; i++) {
		rarp->ar_data[16 + i] = 0xff;
	}

	NetSendPacket(NetTxPacket, (pkt - NetTxPacket) + ARP_HDR_SIZE);

	NetSetTimeout(TIMEOUT, RarpTimeout);
	NetSetHandler(RarpHandler);
}
