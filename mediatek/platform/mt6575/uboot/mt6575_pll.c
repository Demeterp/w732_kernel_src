/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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
 * (C) Copyright 2008
 * MediaTek <www.mediatek.com>
 * Infinity Chen <infinity.chen@mediatek.com>
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

#include <common.h>

#include <asm/arch/mt65xx.h>
#include <asm/arch/mt65xx_typedefs.h>

#define ARMPLL_CON0     ((volatile UINT16 *)0xC0007100)
#define MAINPLL_CON0    ((volatile UINT16 *)0xC0007120)
#define MAINPLL_CON1    ((volatile UINT16 *)0xC0007124)
#define IPLL_CON0       ((volatile UINT16 *)0xC0007140)
#define UPLL_CON0       ((volatile UINT16 *)0xC0007160)
#define MDPLL_CON0      ((volatile UINT16 *)0xC0007180)
#define TVDDS_CON2      ((volatile UINT16 *)0xC00071A8)
#define WPLL_CON0       ((volatile UINT16 *)0xC00071C0)
#define AUDPLL_CON0     ((volatile UINT16 *)0xC00071E0)
#define MEMPLL_CON0     ((volatile UINT16 *)0xC0007200)
#define MEMPLL_CON1     ((volatile UINT16 *)0xC0007204)

/* Top Clock Generator Register Definition */
#define TOP_CKMUXSEL    ((volatile UINT32 *)0xC0001000)
#define TOP_CKDIV0      ((volatile UINT32 *)0xC0001004)
#define TOP_CKDIV1      ((volatile UINT32 *)0xC0001008)
#define TOP_CKDIV23     ((volatile UINT32 *)0xC000100C)
#define TOP_DCMCTL      ((volatile UINT32 *)0xC0001010)
#define TOP_MISC        ((volatile UINT32 *)0xC0001014)
#define TOP_CA9DCMFSEL  ((volatile UINT32 *)0xC0001018)
#define TOP_CKCTL       ((volatile UINT32 *)0xC0001020)

#define WPLL197_FREQ 197000 // 3G PLL frequency is 197Mhz
#define CLKSQ_FREQ   26000  // Default frequency is 26Mhz

kal_uint32 mt6575_get_bus_freq(void)
{
    kal_uint32 fout = 0, fbdiv = 0, fbsel = 0, prediv = 0, postdiv = 0, ckdiv = 0;
    
    if ((*TOP_CKMUXSEL & 0x3) == 0x0) // Using CLKSQ
    {
        fout = CLKSQ_FREQ;
    }
    else if ((*TOP_CKMUXSEL & 0x3) == 0x2) // Using WPLL
    {
        fout = WPLL197_FREQ;
    }
    else
    {
        fbdiv = ((*MAINPLL_CON0) & 0x7F00) >> 8;
        
        fbsel = ((*MAINPLL_CON0) & 0x0030) >> 4;
        if (fbsel == 0)
            fbsel = 1;
        else if (fbsel == 1)
            fbsel = 2;
        else
            fbsel = 4;
        
        prediv = ((*MAINPLL_CON0) & 0x00C0) >> 6;
        if (prediv == 0)
            prediv = 1;
        else if (prediv == 1)
            prediv = 2;
        else
            prediv = 4;
        
        postdiv = ((*MAINPLL_CON1) & 0x0030) >> 4;
        if (postdiv == 0)
            postdiv = 1;
        else if (postdiv == 1)
            postdiv = 2;
        else
            postdiv = 4;
        
        fout = CLKSQ_FREQ * (fbdiv + 1) * fbsel / prediv / postdiv; // KHz
    }
    
    ckdiv = ((*TOP_CKDIV0) & 0x00000018) >> 3;
    
    switch (ckdiv)
    {
        case 0:
            fout = fout;     // bus clock will not be divided
            break;
        case 1:
            fout = fout / 4; // bus clock will be divided by 4
            break;
        case 2:
            fout = fout / 5; // bus clock will be divided by 5
            break;
        case 3:
            fout = fout / 6; // bus clock will be divided by 6
            break;
        default:
            break; 
    }
    
    return fout;
}