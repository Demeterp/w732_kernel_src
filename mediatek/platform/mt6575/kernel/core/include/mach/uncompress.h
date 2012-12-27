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

#ifndef __MT6575_UNCOMPRESS_H__
#define __MT6575_UNCOMPRESS_H__

#define MT6575_UART0_PHY_BASE 0xC1009000

#define MT6575_UART0_LSR *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0x14)
#define MT6575_UART0_THR *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0x0)
#define MT6575_UART0_LCR *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0xc)
#define MT6575_UART0_DLL *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0x0)
#define MT6575_UART0_DLH *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0x4)
#define MT6575_UART0_FCR *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0x8)
#define MT6575_UART0_MCR *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0x10)
#define MT6575_UART0_SPEED *(volatile unsigned char *)(MT6575_UART0_PHY_BASE+0x24)


static void arch_decomp_setup(void)
{
#if defined(CONFIG_MT6575_FPGA)
        MT6575_UART0_LCR = 0x3;
        tmp = MT6575_UART0_LCR;
        MT6575_UART0_LCR = (tmp | 0x80);
        MT6575_UART0_SPEED = 0x0;
        MT6575_UART0_DLL = 0x0E;
        MT6575_UART0_DLH = 0;
        MT6575_UART0_LCR = tmp;
        MT6575_UART0_FCR = 0x0047;
        MT6575_UART0_MCR = (0x1 | 0x2);
#endif
}

/*
 * This does not append a newline
 */
static inline void putc(int c)
{
    while (!(MT6575_UART0_LSR & 0x20));    
    MT6575_UART0_THR = c;        
}

static inline void flush(void)
{
}

/*
 * nothing to do
 */
#define arch_decomp_wdog()

#endif /* !__MT6575_UNCOMPRESS_H__ */

