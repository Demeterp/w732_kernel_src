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
 * config-pre.h - common defines for Blackfin boards in config.h
 *
 * Copyright (c) 2007-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __ASM_BLACKFIN_CONFIG_PRE_H__
#define __ASM_BLACKFIN_CONFIG_PRE_H__

/* Misc helper functions */
#define XMK_STR(x) #x
#define MK_STR(x) XMK_STR(x)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Bootmode defines -- your config needs to select this via CONFIG_BFIN_BOOT_MODE.
 * Depending on your cpu, some of these may not be valid, check your HRM.
 * The actual values here are meaningless as long as they're unique.
 */
#define BFIN_BOOT_BYPASS      1       /* bypass bootrom */
#define BFIN_BOOT_PARA        2       /* boot ldr out of parallel flash */
#define BFIN_BOOT_SPI_MASTER  3       /* boot ldr out of serial flash */
#define BFIN_BOOT_SPI_SLAVE   4       /* boot ldr as spi slave */
#define BFIN_BOOT_TWI_MASTER  5       /* boot ldr over twi device */
#define BFIN_BOOT_TWI_SLAVE   6       /* boot ldr over twi slave */
#define BFIN_BOOT_UART        7       /* boot ldr over uart */
#define BFIN_BOOT_IDLE        8       /* do nothing, just idle */
#define BFIN_BOOT_FIFO        9       /* boot ldr out of FIFO */
#define BFIN_BOOT_MEM         10      /* boot ldr out of memory (warmboot) */
#define BFIN_BOOT_16HOST_DMA  11      /* boot ldr from 16-bit host dma */
#define BFIN_BOOT_8HOST_DMA   12      /* boot ldr from 8-bit host dma */
#define BFIN_BOOT_NAND        13      /* boot ldr from nand flash */

#ifndef __ASSEMBLY__
static inline const char *get_bfin_boot_mode(int bfin_boot)
{
	switch (bfin_boot) {
	case BFIN_BOOT_BYPASS:     return "bypass";
	case BFIN_BOOT_PARA:       return "parallel flash";
	case BFIN_BOOT_SPI_MASTER: return "spi flash";
	case BFIN_BOOT_SPI_SLAVE:  return "spi slave";
	case BFIN_BOOT_TWI_MASTER: return "i2c flash";
	case BFIN_BOOT_TWI_SLAVE:  return "i2c slave";
	case BFIN_BOOT_UART:       return "uart";
	case BFIN_BOOT_IDLE:       return "idle";
	case BFIN_BOOT_FIFO:       return "fifo";
	case BFIN_BOOT_MEM:        return "memory";
	case BFIN_BOOT_16HOST_DMA: return "16bit dma";
	case BFIN_BOOT_8HOST_DMA:  return "8bit dma";
	case BFIN_BOOT_NAND:       return "nand flash";
	default:                   return "INVALID";
	}
}
#endif

/* Most bootroms allow for EVT1 redirection */
#if ((defined(__ADSPBF531__) || defined(__ADSPBF532__) || defined(__ADSPBF533__)) \
     && __SILICON_REVISION__ < 3) || defined(__ADSPBF561__)
# undef CONFIG_BFIN_BOOTROM_USES_EVT1
#else
# define CONFIG_BFIN_BOOTROM_USES_EVT1
#endif

/* Define the default SPI CS used when booting out of SPI */
#if defined(__ADSPBF531__) || defined(__ADSPBF532__) || defined(__ADSPBF533__) || \
    defined(__ADSPBF538__) || defined(__ADSPBF539__) || defined(__ADSPBF561__) || \
    defined(__ADSPBF51x__)
# define BFIN_BOOT_SPI_SSEL 2
#else
# define BFIN_BOOT_SPI_SSEL 1
#endif

/* There is no Blackfin/NetBSD port */
#undef CONFIG_BOOTM_NETBSD

/* We rarely use interrupts, so favor throughput over latency */
#define CONFIG_BFIN_INS_LOWOVERHEAD

#endif
