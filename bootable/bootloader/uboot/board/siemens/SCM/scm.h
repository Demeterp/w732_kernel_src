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

#ifndef __SCM_H
#define __SCM_H

/*----------------*/
/* CAN Structures */
/*----------------*/

/* Message */
struct can_msg {
    uchar	ctrl_0;
    uchar	ctrl_1;
    uchar	arbit_0;
    uchar	arbit_1;
    uchar	arbit_2;
    uchar	arbit_3;
    uchar	config;
    uchar	data[8];
} __attribute__ ((packed));

typedef struct can_msg can_msg_t;

/* CAN Register */
typedef struct can_reg {
    uchar	ctrl;
    uchar	status;
    uchar	cpu_interface;
    uchar	resv0;
    ushort	high_speed_rd;
    ushort	gbl_mask_std;
    uint	gbl_mask_extd;
    uint	msg15_mask;
    can_msg_t	msg1;
    uchar	clkout;
    can_msg_t	msg2;
    uchar	bus_config;
    can_msg_t	msg3;
    uchar	bit_timing_0;
    can_msg_t	msg4;
    uchar	bit_timing_1;
    can_msg_t	msg5;
    uchar	interrupt;
    can_msg_t	msg6;
    uchar	resv1;
    can_msg_t	msg7;
    uchar	resv2;
    can_msg_t	msg8;
    uchar	resv3;
    can_msg_t	msg9;
    uchar	p1conf;
    can_msg_t	msg10;
    uchar	p2conf;
    can_msg_t	msg11;
    uchar	p1in;
    can_msg_t	msg12;
    uchar	p2in;
    can_msg_t	msg13;
    uchar	p1out;
    can_msg_t	msg14;
    uchar	p2out;
    can_msg_t	msg15;
    uchar	ser_res_addr;
    uchar	resv_cs[0x8000-0x100];	/* 0x8000 is the min size for CS */
} can_reg_t;


#endif /* __SCM_H */
