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
 * (C) Copyright 2001
 * Josh Huber <huber@mclx.com>, Mission Critical Linux, Inc.
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

/*
 * eth.h - header file for the polled mode GT ethernet driver
 */

#ifndef __GT6426x_ETH_H__
#define __GT6426x_ETH_H__

#include <asm/types.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <common.h>

typedef struct eth0_tx_desc_struct {
	volatile __u32 bytecount_reserved;
	volatile __u32 command_status;
	volatile struct eth0_tx_desc_struct * next_desc;
	/* Note - the following will not work for 64 bit addressing */
	volatile unsigned char * buff_pointer;
} __attribute__ ((packed)) eth0_tx_desc_single;

typedef struct eth0_rx_desc_struct {
  volatile __u32 buff_size_byte_count;
  volatile __u32 command_status;
  volatile struct eth0_rx_desc_struct * next_desc;
  volatile unsigned char * buff_pointer;
} __attribute__ ((packed)) eth0_rx_desc_single;

#define NT 20 /* Number of Transmit buffers */
#define NR 20 /* Number of Receive buffers */
#define MAX_BUFF_SIZE (1536+2*CACHE_LINE_SIZE) /* 1600 */
#define ETHERNET_PORTS_DIFFERENCE_OFFSETS 0x400

unsigned long TDN_ETH0 , RDN_ETH0; /* Rx/Tx current Descriptor Number*/
unsigned int EVB64260_ETH0_irq;

#define CLOSED 0
#define OPENED 1

#define PORT_ETH0 0

extern eth0_tx_desc_single *eth0_tx_desc;
extern eth0_rx_desc_single *eth0_rx_desc;
extern char *eth0_tx_buffer;
extern char *eth0_rx_buffer[NR];
extern char *eth_data;

extern int gt6426x_eth_poll(void *v);
extern int gt6426x_eth_transmit(void *v, volatile char *p, unsigned int s);
extern void gt6426x_eth_disable(void *v);
extern int gt6426x_eth_probe(void *v, bd_t *bis);

#endif  /* __GT64260x_ETH_H__ */
