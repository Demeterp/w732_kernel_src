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
 * (C) Copyright 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Heungjun Kim <riverful.kim@samsung.com>
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
 *
 */

#ifndef __ASM_ARM_ARCH_CLOCK_H_
#define __ASM_ARM_ARCH_CLOCK_H_

#ifndef __ASSEMBLY__
struct s5pc100_clock {
	unsigned int	apll_lock;
	unsigned int	mpll_lock;
	unsigned int	epll_lock;
	unsigned int	hpll_lock;
	unsigned char	res1[0xf0];
	unsigned int	apll_con;
	unsigned int	mpll_con;
	unsigned int	epll_con;
	unsigned int	hpll_con;
	unsigned char	res2[0xf0];
	unsigned int	src0;
	unsigned int	src1;
	unsigned int	src2;
	unsigned int	src3;
	unsigned char	res3[0xf0];
	unsigned int	div0;
	unsigned int	div1;
	unsigned int	div2;
	unsigned int	div3;
	unsigned int	div4;
	unsigned char	res4[0x1ec];
	unsigned int	gate_d00;
	unsigned int	gate_d01;
	unsigned int	gate_d02;
	unsigned char	res5[0x54];
	unsigned int	gate_sclk0;
	unsigned int	gate_sclk1;
};

struct s5pc110_clock {
	unsigned int	apll_lock;
	unsigned char	res1[0x4];
	unsigned int	mpll_lock;
	unsigned char	res2[0x4];
	unsigned int	epll_lock;
	unsigned char	res3[0xc];
	unsigned int	vpll_lock;
	unsigned char	res4[0xdc];
	unsigned int	apll_con;
	unsigned char	res5[0x4];
	unsigned int	mpll_con;
	unsigned char	res6[0x4];
	unsigned int	epll_con;
	unsigned char	res7[0xc];
	unsigned int	vpll_con;
	unsigned char	res8[0xdc];
	unsigned int	src0;
	unsigned int	src1;
	unsigned int	src2;
	unsigned int	src3;
	unsigned char	res9[0xf0];
	unsigned int	div0;
	unsigned int	div1;
	unsigned int	div2;
	unsigned int	div3;
	unsigned int	div4;
	unsigned char	res10[0x1ec];
	unsigned int	gate_d00;
	unsigned int	gate_d01;
	unsigned int	gate_d02;
	unsigned char	res11[0x54];
	unsigned int	gate_sclk0;
	unsigned int	gate_sclk1;
};
#endif

#endif
