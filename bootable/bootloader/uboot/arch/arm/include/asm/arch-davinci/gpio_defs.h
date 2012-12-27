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
 * Copyright (C) 2009 Texas Instruments Incorporated
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
#ifndef _GPIO_DEFS_H_
#define _GPIO_DEFS_H_

#ifndef CONFIG_SOC_DA8XX
#define DAVINCI_GPIO_BINTEN	0x01C67008
#define DAVINCI_GPIO_BANK01	0x01C67010
#define DAVINCI_GPIO_BANK23	0x01C67038
#define DAVINCI_GPIO_BANK45	0x01C67060
#define DAVINCI_GPIO_BANK67	0x01C67088

#else /* CONFIG_SOC_DA8XX */
#define DAVINCI_GPIO_BINTEN	0x01E26008
#define DAVINCI_GPIO_BANK01	0x01E26010
#define DAVINCI_GPIO_BANK23	0x01E26038
#define DAVINCI_GPIO_BANK45	0x01E26060
#define DAVINCI_GPIO_BANK67	0x01E26088
#endif /* CONFIG_SOC_DA8XX */

struct davinci_gpio {
	unsigned int dir;
	unsigned int out_data;
	unsigned int set_data;
	unsigned int clr_data;
	unsigned int in_data;
	unsigned int set_rising;
	unsigned int clr_rising;
	unsigned int set_falling;
	unsigned int clr_falling;
	unsigned int intstat;
};

struct davinci_gpio_bank {
	int num_gpio;
	unsigned int irq_num;
	unsigned int irq_mask;
	unsigned long *in_use;
	unsigned long base;
};

#define davinci_gpio_bank01 ((struct davinci_gpio *)DAVINCI_GPIO_BANK01)
#define davinci_gpio_bank23 ((struct davinci_gpio *)DAVINCI_GPIO_BANK23)
#define davinci_gpio_bank45 ((struct davinci_gpio *)DAVINCI_GPIO_BANK45)
#define davinci_gpio_bank67 ((struct davinci_gpio *)DAVINCI_GPIO_BANK67)

#endif
