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

#ifndef __SBCOMMON_H__
#define __SBCOMMON_H__
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
#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <spd_sdram.h>
#include <i2c.h>
#include "ppc440gx_i2c.h"

/*
 * GPIO Settings
 */
/* Chassis settings */
#define SBCOMMON_GPIO_PRI_N		0x00001000  /* 0 = Chassis Master, 1 = Slave */
#define SBCOMMON_GPIO_SEC_PRES		0x00000800  /* 1 = Other board present */

/* Debug LEDs */
#define SBCOMMON_GPIO_DBGLED_0		0x00000400
#define SBCOMMON_GPIO_DBGLED_1		0x00000200
#define SBCOMMON_GPIO_DBGLED_2		0x00100000
#define SBCOMMON_GPIO_DBGLED_3		0x00000100

#define SBCOMMON_GPIO_DBGLEDS		(SBCOMMON_GPIO_DBGLED_0 | \
					 SBCOMMON_GPIO_DBGLED_1 | \
					 SBCOMMON_GPIO_DBGLED_2 | \
					 SBCOMMON_GPIO_DBGLED_3)

#define SBCOMMON_GPIO_SYS_FAULT		0x00000080
#define SBCOMMON_GPIO_SYS_OTEMP		0x00000040
#define SBCOMMON_GPIO_SYS_STATUS	0x00000020

#define SBCOMMON_GPIO_SYS_LEDS		(SBCOMMON_GPIO_SYS_STATUS)

#define SBCOMMON_GPIO_LEDS		(SBCOMMON_GPIO_DBGLED_0 | \
					 SBCOMMON_GPIO_DBGLED_1 | \
					 SBCOMMON_GPIO_DBGLED_2 | \
					 SBCOMMON_GPIO_DBGLED_3 | \
					 SBCOMMON_GPIO_SYS_STATUS)

typedef struct ppc440_gpio_regs {
	volatile unsigned long out;
	volatile unsigned long tri_state;
	volatile unsigned long dummy[4];
	volatile unsigned long open_drain;
	volatile unsigned long in;
}  __attribute__((packed)) ppc440_gpio_regs_t;

int sbcommon_get_master(void);
int sbcommon_secondary_present(void);
unsigned short sbcommon_get_serial_number(void);
void sbcommon_fans(void);
void board_get_enetaddr(int macaddr_idx, uchar *enet);

#endif /* __SBCOMMON_H__ */
