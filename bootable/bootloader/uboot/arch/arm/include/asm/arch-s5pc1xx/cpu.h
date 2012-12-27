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

#ifndef _S5PC1XX_CPU_H
#define _S5PC1XX_CPU_H

#define S5PC1XX_ADDR_BASE	0xE0000000

#define S5PC1XX_CLOCK_BASE	0xE0100000

/* S5PC100 */
#define S5PC100_GPIO_BASE	0xE0300000
#define S5PC100_VIC0_BASE	0xE4000000
#define S5PC100_VIC1_BASE	0xE4100000
#define S5PC100_VIC2_BASE	0xE4200000
#define S5PC100_DMC_BASE	0xE6000000
#define S5PC100_SROMC_BASE	0xE7000000
#define S5PC100_ONENAND_BASE	0xE7100000
#define S5PC100_PWMTIMER_BASE	0xEA000000
#define S5PC100_WATCHDOG_BASE	0xEA200000
#define S5PC100_UART_BASE	0xEC000000

/* S5PC110 */
#define S5PC110_GPIO_BASE	0xE0200000
#define S5PC110_PWMTIMER_BASE	0xE2500000
#define S5PC110_WATCHDOG_BASE	0xE2700000
#define S5PC110_UART_BASE	0xE2900000
#define S5PC110_SROMC_BASE	0xE8000000
#define S5PC110_DMC0_BASE	0xF0000000
#define S5PC110_DMC1_BASE	0xF1400000
#define S5PC110_VIC0_BASE	0xF2000000
#define S5PC110_VIC1_BASE	0xF2100000
#define S5PC110_VIC2_BASE	0xF2200000
#define S5PC110_VIC3_BASE	0xF2300000

/* Chip ID */
#define S5PC1XX_PRO_ID		0xE0000000

#ifndef __ASSEMBLY__
/* CPU detection macros */
extern unsigned int s5pc1xx_cpu_id;

#define IS_SAMSUNG_TYPE(type, id)			\
static inline int cpu_is_##type(void)			\
{							\
	return s5pc1xx_cpu_id == id ? 1 : 0;		\
}

IS_SAMSUNG_TYPE(s5pc100, 0xc100)
IS_SAMSUNG_TYPE(s5pc110, 0xc110)
#endif

#endif	/* _S5PC1XX_CPU_H */
