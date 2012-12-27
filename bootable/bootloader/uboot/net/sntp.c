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
 * SNTP support driver
 *
 * Masami Komiya <mkomiya@sonare.it> 2005
 *
 */

#include <common.h>
#include <command.h>
#include <net.h>
#include <rtc.h>

#include "sntp.h"

#define SNTP_TIMEOUT 10000UL

static int SntpOurPort;

static void
SntpSend (void)
{
	struct sntp_pkt_t pkt;
	int pktlen = SNTP_PACKET_LEN;
	int sport;

	debug("%s\n", __func__);

	memset (&pkt, 0, sizeof(pkt));

	pkt.li = NTP_LI_NOLEAP;
	pkt.vn = NTP_VERSION;
	pkt.mode = NTP_MODE_CLIENT;

	memcpy ((char *)NetTxPacket + NetEthHdrSize() + IP_HDR_SIZE, (char *)&pkt, pktlen);

	SntpOurPort = 10000 + (get_timer(0) % 4096);
	sport = NTP_SERVICE_PORT;

	NetSendUDPPacket (NetServerEther, NetNtpServerIP, sport, SntpOurPort, pktlen);
}

static void
SntpTimeout (void)
{
	puts ("Timeout\n");
	NetState = NETLOOP_FAIL;
	return;
}

static void
SntpHandler (uchar *pkt, unsigned dest, unsigned src, unsigned len)
{
	struct sntp_pkt_t *rpktp = (struct sntp_pkt_t *)pkt;
	struct rtc_time tm;
	ulong seconds;

	debug("%s\n", __func__);

	if (dest != SntpOurPort) return;

	/*
	 * As the RTC's used in U-Boot sepport second resolution only
	 * we simply ignore the sub-second field.
	 */
	memcpy (&seconds, &rpktp->transmit_timestamp, sizeof(ulong));

	to_tm(ntohl(seconds) - 2208988800UL + NetTimeOffset, &tm);
#if defined(CONFIG_CMD_DATE)
	rtc_set (&tm);
#endif
	printf ("Date: %4d-%02d-%02d Time: %2d:%02d:%02d\n",
		tm.tm_year, tm.tm_mon, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec);

	NetState = NETLOOP_SUCCESS;
}

void
SntpStart (void)
{
	debug("%s\n", __func__);

	NetSetTimeout (SNTP_TIMEOUT, SntpTimeout);
	NetSetHandler(SntpHandler);
	memset (NetServerEther, 0, 6);

	SntpSend ();
}
