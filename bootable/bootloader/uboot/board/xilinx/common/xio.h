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
 * xio.h
 *
 * Defines XIo functions for Xilinx OCP in terms of Linux primitives
 *
 * Author: MontaVista Software, Inc.
 *         source@mvista.com
 *
 * Copyright 2002 MontaVista Software Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef XIO_H
#define XIO_H

#include "xbasic_types.h"
#include <asm/io.h>

typedef u32 XIo_Address;

extern inline u8
XIo_In8(XIo_Address InAddress)
{
	return (u8) in_8((volatile unsigned char *) InAddress);
}
extern inline u16
XIo_In16(XIo_Address InAddress)
{
	return (u16) in_be16((volatile unsigned short *) InAddress);
}
extern inline u32
XIo_In32(XIo_Address InAddress)
{
	return (u32) in_be32((volatile unsigned *) InAddress);
}
extern inline void
XIo_Out8(XIo_Address OutAddress, u8 Value)
{
	out_8((volatile unsigned char *) OutAddress, Value);
}
extern inline void
XIo_Out16(XIo_Address OutAddress, u16 Value)
{
	out_be16((volatile unsigned short *) OutAddress, Value);
}
extern inline void
XIo_Out32(XIo_Address OutAddress, u32 Value)
{
	out_be32((volatile unsigned *) OutAddress, Value);
}

#define XIo_ToLittleEndian16(s,d) (*(u16*)(d) = cpu_to_le16((u16)(s)))
#define XIo_ToLittleEndian32(s,d) (*(u32*)(d) = cpu_to_le32((u32)(s)))
#define XIo_ToBigEndian16(s,d) (*(u16*)(d) = cpu_to_be16((u16)(s)))
#define XIo_ToBigEndian32(s,d) (*(u32*)(d) = cpu_to_be32((u32)(s)))

#define XIo_FromLittleEndian16(s,d) (*(u16*)(d) = le16_to_cpu((u16)(s)))
#define XIo_FromLittleEndian32(s,d) (*(u32*)(d) = le32_to_cpu((u32)(s)))
#define XIo_FromBigEndian16(s,d) (*(u16*)(d) = be16_to_cpu((u16)(s)))
#define XIo_FromBigEndian32(s,d) (*(u32*)(d) = be32_to_cpu((u32)(s)))

#endif				/* XIO_H */
