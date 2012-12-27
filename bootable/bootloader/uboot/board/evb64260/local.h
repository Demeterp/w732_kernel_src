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
 * include/local.h - local configuration options, board specific
 */

#ifndef __LOCAL_H
#define __LOCAL_H

/*
 * High Level Configuration Options
 * (easy to change)
 */

/* This tells U-Boot that the config options are compiled in */
/* #undef ENV_IS_EMBEDDED */
/* Don't touch this! U-Boot figures this out  based on other
 * magic. */

/* Uncomment and define any of the below options */

/* #define CONFIG_750CX */ /* The 750CX doesn't support as many things in L2CR */
			/* Note: If you defined CONFIG_EVB64260_750CX this */
			/* gets defined automatically. */

/* These want string arguments */
/* #define CONFIG_BOOTARGS */
/* #define CONFIG_BOOTCOMMAND */
/* #define CONFIG_RAMBOOTCOMMAND */
/* #define CONFIG_NFSBOOTCOMMAND */
/* #define CONFIG_SYS_AUTOLOAD */
/* #define CONFIG_PREBOOT */

/* These don't */

/* #define CONFIG_BOOTDELAY  */
/* #define CONFIG_BAUDRATE */
/* #define CONFIG_LOADS_ECHO */
/* #define CONFIG_ETHADDR */
/* #define CONFIG_ETH2ADDR */
/* #define CONFIG_ETH3ADDR */
/* #define CONFIG_IPADDR */
/* #define CONFIG_SERVERIP */
/* #define CONFIG_ROOTPATH */
/* #define CONFIG_GATEWAYIP */
/* #define CONFIG_NETMASK */
/* #define CONFIG_HOSTNAME */
/* #define CONFIG_BOOTFILE */
/* #define CONFIG_LOADADDR */

/* these hardware addresses are pretty bogus, please change them to
   suit your needs */

/* first ethernet */
#define CONFIG_ETHADDR          00:11:22:33:44:55

/* next two ethernet hwaddrs */
#define CONFIG_HAS_ETH1
#define CONFIG_ETH1ADDR		00:11:22:33:44:66
#define CONFIG_HAS_ETH2
#define CONFIG_ETH2ADDR		00:11:22:33:44:77

#define CONFIG_ENV_OVERWRITE
#endif	/* __CONFIG_H */
