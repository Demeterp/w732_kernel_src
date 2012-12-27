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

 /* linux/include/asm-arm/arch-mt6573/mt6573_boot.h
  *
  *
  * Copyright (C) 2008,2009 MediaTek <www.mediatek.com>
  * Authors: Infinity Chen <infinity.chen@mediatek.com>  
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

#ifndef _MT6575_BOOT_H_
#define _MT6575_BOOT_H_

/* MT6575 boot type definitions */
typedef enum 
{
    NORMAL_BOOT = 0,
    META_BOOT = 1,
    RECOVERY_BOOT = 2,    
    SW_REBOOT = 3,
    FACTORY_BOOT = 4,
    ADVMETA_BOOT = 5,
    ATE_FACTORY_BOOT = 6,
    ALARM_BOOT = 7,
    UNKNOWN_BOOT
} BOOTMODE;

/*META COM port type*/
 typedef enum
{
    META_UNKNOWN_COM = 0,
    META_UART_COM,
    META_USB_COM
} META_COM_TYPE;


#define BOOT_DEV_NAME           "BOOT"
#define BOOT_SYSFS              "boot"
#define BOOT_SYSFS_ATTR         "boot_mode"
#define MD_SYSFS_ATTR           "md"
#define INFO_SYSFS_ATTR         "info"

/* MT6575 chip definitions */
typedef enum 
{
    CHIP_E1 = 0xca00,
    CHIP_E2 = 0xcb00,
    CHIP_E3 = 0xcc00,    
} CHIP_VER;

/* MT6575 md sw definitions */
typedef enum 
{
    MD_SW_V1 = 0xe100,
    MD_SW_V2 = 0xe201,
    MD_SW_V3 = 0xe201,    
} MD_SW_VER;

/*modem image version definitions*/
typedef enum{
	AP_IMG_INVALID = 0,
	AP_IMG_2G,
	AP_IMG_3G
}AP_IMG_TYPE;


/* this vairable will be set by mt6573_fixup */
extern BOOTMODE g_boot_mode;
extern META_COM_TYPE g_meta_com_type;
extern unsigned int g_meta_com_id;

extern BOOTMODE get_boot_mode(void);
extern bool is_meta_mode(void);
extern bool is_advanced_meta_mode(void);

extern void boot_register_md_func(ssize_t (*show)(char*), ssize_t (*store)(const char*,size_t));

extern unsigned int get_chip_code(void);
extern unsigned int get_chip_sw_ver(void);
extern unsigned int get_chip_hw_ver(void);
extern unsigned int get_chip_hw_subcode(void);
extern CHIP_VER get_chip_eco_ver(void);

extern AP_IMG_TYPE get_ap_img_ver(void);

extern int get_td_eint_info(char * eint_name, unsigned int len);
extern int get_md_gpio_info(char *gpio_name, unsigned int len);
extern int get_md_adc_info(char *adc_name, unsigned int len);

extern void AudSys_Power_On(bool on);


#endif 

