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

/* alps/ALPS_SW/TRUNK/MAIN/alps/kernel/arch/arm/mach-mt6516/include/mach/fm.h
 *
 * (C) Copyright 2009 
 * MediaTek <www.MediaTek.com>
 * William Chung <William.Chung@MediaTek.com>
 *
 * MT6516 AR10x0 FM Radio Driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __MTGPIO_H__
#define __MTGPIO_H__

#include <linux/ioctl.h>

#define GPIO_IOC_MAGIC 0x90

#define GPIO_IOCQMODE           _IOR(GPIO_IOC_MAGIC, 0x01, uint32_t)
#define GPIO_IOCTMODE0          _IOW(GPIO_IOC_MAGIC, 0x02, uint32_t)
#define GPIO_IOCTMODE1          _IOW(GPIO_IOC_MAGIC, 0x03, uint32_t)
#define GPIO_IOCTMODE2          _IOW(GPIO_IOC_MAGIC, 0x04, uint32_t)
#define GPIO_IOCTMODE3          _IOW(GPIO_IOC_MAGIC, 0x05, uint32_t)
#define GPIO_IOCQDIR            _IOR(GPIO_IOC_MAGIC, 0x06, uint32_t)
#define GPIO_IOCSDIRIN          _IOW(GPIO_IOC_MAGIC, 0x07, uint32_t)
#define GPIO_IOCSDIROUT         _IOW(GPIO_IOC_MAGIC, 0x08, uint32_t)
#define GPIO_IOCQPULLEN         _IOR(GPIO_IOC_MAGIC, 0x09, uint32_t)
#define GPIO_IOCSPULLENABLE     _IOW(GPIO_IOC_MAGIC, 0x0A, uint32_t)
#define GPIO_IOCSPULLDISABLE    _IOW(GPIO_IOC_MAGIC, 0x0B, uint32_t)
#define GPIO_IOCQPULL           _IOR(GPIO_IOC_MAGIC, 0x0C, uint32_t)
#define GPIO_IOCSPULLDOWN       _IOW(GPIO_IOC_MAGIC, 0x0D, uint32_t)
#define GPIO_IOCSPULLUP         _IOW(GPIO_IOC_MAGIC, 0x0E, uint32_t)
#define GPIO_IOCQINV            _IOR(GPIO_IOC_MAGIC, 0x0F, uint32_t)
#define GPIO_IOCSINVENABLE      _IOW(GPIO_IOC_MAGIC, 0x10, uint32_t)
#define GPIO_IOCSINVDISABLE     _IOW(GPIO_IOC_MAGIC, 0x11, uint32_t)
#define GPIO_IOCQDATAIN         _IOR(GPIO_IOC_MAGIC, 0x12, uint32_t)
#define GPIO_IOCQDATAOUT        _IOR(GPIO_IOC_MAGIC, 0x13, uint32_t)
#define GPIO_IOCSDATALOW        _IOW(GPIO_IOC_MAGIC, 0x14, uint32_t)
#define GPIO_IOCSDATAHIGH       _IOW(GPIO_IOC_MAGIC, 0x15, uint32_t)


#endif // __MTGPIO_H__
