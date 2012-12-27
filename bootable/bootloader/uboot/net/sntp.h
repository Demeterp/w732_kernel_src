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
 * (C) Masami Komiya <mkomiya@sonare.it> 2005
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#ifndef __SNTP_H__
#define __SNTP_H__

#define NTP_SERVICE_PORT	123
#define SNTP_PACKET_LEN		48


/* Leap Indicator */
#define NTP_LI_NOLEAP		0x0
#define NTP_LI_61SECS		0x1
#define NTP_LI_59SECS		0x2
#define NTP_LI_ALARM		0x3

/* Version */

#define NTP_VERSION		4

/* Mode */
#define NTP_MODE_RESERVED	0
#define NTP_MODE_SYMACTIVE	1	/* Symmetric Active */
#define NTP_MODE_SYMPASSIVE	2	/* Symmetric Passive */
#define NTP_MODE_CLIENT		3
#define NTP_MODE_SERVER		4
#define NTP_MODE_BROADCAST	5
#define NTP_MODE_NTPCTRL	6	/* Reserved for NTP control message */
#define NTP_MODE_PRIVATE	7	/* Reserved for private use */

struct sntp_pkt_t {
#if __LITTLE_ENDIAN
	uchar mode:3;
	uchar vn:3;
	uchar li:2;
#else
	uchar li:2;
	uchar vn:3;
	uchar mode:3;
#endif
	uchar stratum;
	uchar poll;
	uchar precision;
	uint root_delay;
	uint root_dispersion;
	uint reference_id;
	unsigned long long reference_timestamp;
	unsigned long long originate_timestamp;
	unsigned long long receive_timestamp;
	unsigned long long transmit_timestamp;
};

extern void	SntpStart (void);	/* Begin SNTP */

#endif /* __SNTP_H__ */
