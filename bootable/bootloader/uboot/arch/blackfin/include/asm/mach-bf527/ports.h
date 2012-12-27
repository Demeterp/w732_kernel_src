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
 * Port Masks
 */

#ifndef __BFIN_PERIPHERAL_PORT__
#define __BFIN_PERIPHERAL_PORT__

/* PORTx_MUX Masks */
#define PORT_x_MUX_0_MASK	0x0003
#define PORT_x_MUX_1_MASK	0x000C
#define PORT_x_MUX_2_MASK	0x0030
#define PORT_x_MUX_3_MASK	0x00C0
#define PORT_x_MUX_4_MASK	0x0300
#define PORT_x_MUX_5_MASK	0x0C00
#define PORT_x_MUX_6_MASK	0x3000
#define PORT_x_MUX_7_MASK	0xC000

#define PORT_x_MUX_FUNC_1	(0x0)
#define PORT_x_MUX_FUNC_2	(0x1)
#define PORT_x_MUX_FUNC_3	(0x2)
#define PORT_x_MUX_FUNC_4	(0x3)
#define PORT_x_MUX_0_FUNC_1	(PORT_x_MUX_FUNC_1 << 0)
#define PORT_x_MUX_0_FUNC_2	(PORT_x_MUX_FUNC_2 << 0)
#define PORT_x_MUX_0_FUNC_3	(PORT_x_MUX_FUNC_3 << 0)
#define PORT_x_MUX_0_FUNC_4	(PORT_x_MUX_FUNC_4 << 0)
#define PORT_x_MUX_1_FUNC_1	(PORT_x_MUX_FUNC_1 << 2)
#define PORT_x_MUX_1_FUNC_2	(PORT_x_MUX_FUNC_2 << 2)
#define PORT_x_MUX_1_FUNC_3	(PORT_x_MUX_FUNC_3 << 2)
#define PORT_x_MUX_1_FUNC_4	(PORT_x_MUX_FUNC_4 << 2)
#define PORT_x_MUX_2_FUNC_1	(PORT_x_MUX_FUNC_1 << 4)
#define PORT_x_MUX_2_FUNC_2	(PORT_x_MUX_FUNC_2 << 4)
#define PORT_x_MUX_2_FUNC_3	(PORT_x_MUX_FUNC_3 << 4)
#define PORT_x_MUX_2_FUNC_4	(PORT_x_MUX_FUNC_4 << 4)
#define PORT_x_MUX_3_FUNC_1	(PORT_x_MUX_FUNC_1 << 6)
#define PORT_x_MUX_3_FUNC_2	(PORT_x_MUX_FUNC_2 << 6)
#define PORT_x_MUX_3_FUNC_3	(PORT_x_MUX_FUNC_3 << 6)
#define PORT_x_MUX_3_FUNC_4	(PORT_x_MUX_FUNC_4 << 6)
#define PORT_x_MUX_4_FUNC_1	(PORT_x_MUX_FUNC_1 << 8)
#define PORT_x_MUX_4_FUNC_2	(PORT_x_MUX_FUNC_2 << 8)
#define PORT_x_MUX_4_FUNC_3	(PORT_x_MUX_FUNC_3 << 8)
#define PORT_x_MUX_4_FUNC_4	(PORT_x_MUX_FUNC_4 << 8)
#define PORT_x_MUX_5_FUNC_1	(PORT_x_MUX_FUNC_1 << 10)
#define PORT_x_MUX_5_FUNC_2	(PORT_x_MUX_FUNC_2 << 10)
#define PORT_x_MUX_5_FUNC_3	(PORT_x_MUX_FUNC_3 << 10)
#define PORT_x_MUX_5_FUNC_4	(PORT_x_MUX_FUNC_4 << 10)
#define PORT_x_MUX_6_FUNC_1	(PORT_x_MUX_FUNC_1 << 12)
#define PORT_x_MUX_6_FUNC_2	(PORT_x_MUX_FUNC_2 << 12)
#define PORT_x_MUX_6_FUNC_3	(PORT_x_MUX_FUNC_3 << 12)
#define PORT_x_MUX_6_FUNC_4	(PORT_x_MUX_FUNC_4 << 12)
#define PORT_x_MUX_7_FUNC_1	(PORT_x_MUX_FUNC_1 << 14)
#define PORT_x_MUX_7_FUNC_2	(PORT_x_MUX_FUNC_2 << 14)
#define PORT_x_MUX_7_FUNC_3	(PORT_x_MUX_FUNC_3 << 14)
#define PORT_x_MUX_7_FUNC_4	(PORT_x_MUX_FUNC_4 << 14)

#include "../mach-common/bits/ports-f.h"
#include "../mach-common/bits/ports-g.h"
#include "../mach-common/bits/ports-h.h"
#include "../mach-common/bits/ports-j.h"

#endif
