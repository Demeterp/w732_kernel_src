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
 *  Copyright (C) 2005 Sandburst Corporation
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Ported from i2c driver for ppc4xx by AS HARNOIS by
 * Travis B. Sawyer
 * Sandburst Corporation
 */
#include <common.h>
#include <ppc4xx.h>
#include <4xx_i2c.h>
#include <i2c.h>

#ifdef CONFIG_HARD_I2C

#define I2C_BUS1_BASE_ADDR (CONFIG_SYS_PERIPHERAL_BASE + 0x00000500)
#define	   I2C_REGISTERS_BUS1_BASE_ADDRESS I2C_BUS1_BASE_ADDR
#define    IIC_MDBUF1	(&i2c->mdbuf)
#define    IIC_SDBUF1	(&i2c->sdbuf)
#define    IIC_LMADR1	(&i2c->lmadr)
#define    IIC_HMADR1	(&i2c->hmadr)
#define    IIC_CNTL1	(&i2c->cntl)
#define    IIC_MDCNTL1	(&i2c->mdcntl)
#define    IIC_STS1	(&i2c->sts)
#define    IIC_EXTSTS1	(&i2c->extsts)
#define    IIC_LSADR1	(&i2c->lsadr)
#define    IIC_HSADR1	(&i2c->hsadr)
#define    IIC_CLKDIV1	(&i2c->clkdiv)
#define    IIC_INTRMSK1	(&i2c->intrmsk)
#define    IIC_XFRCNT1	(&i2c->xfrcnt)
#define    IIC_XTCNTLSS1	(&i2c->xtcntlss)
#define    IIC_DIRECTCNTL1 (&i2c->directcntl)

void i2c1_init (int speed, int slaveadd);
int i2c_probe1 (uchar chip);
int i2c_read1 (uchar chip, uint addr, int alen, uchar * buffer, int len);
int i2c_write1 (uchar chip, uint addr, int alen, uchar * buffer, int len);
uchar i2c_reg_read1(uchar i2c_addr, uchar reg);
void i2c_reg_write1(uchar i2c_addr, uchar reg, uchar val);

#endif	/* CONFIG_HARD_I2C */
