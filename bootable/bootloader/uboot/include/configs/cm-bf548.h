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
 * U-boot - Configuration file for cm-bf548 board
 */

#ifndef __CONFIG_CM_BF548_H__
#define __CONFIG_CM_BF548_H__

#include <asm/config-pre.h>


/*
 * Processor Settings
 */
#define CONFIG_BFIN_CPU             bf548-0.0
#define CONFIG_BFIN_BOOT_MODE       BFIN_BOOT_PARA


/*
 * Clock Settings
 *	CCLK = (CLKIN * VCO_MULT) / CCLK_DIV
 *	SCLK = (CLKIN * VCO_MULT) / SCLK_DIV
 */
/* CONFIG_CLKIN_HZ is any value in Hz					*/
#define CONFIG_CLKIN_HZ			25000000
/* CLKIN_HALF controls the DF bit in PLL_CTL      0 = CLKIN		*/
/*                                                1 = CLKIN / 2		*/
#define CONFIG_CLKIN_HALF		0
/* PLL_BYPASS controls the BYPASS bit in PLL_CTL  0 = do not bypass	*/
/*                                                1 = bypass PLL	*/
#define CONFIG_PLL_BYPASS		0
/* VCO_MULT controls the MSEL (multiplier) bits in PLL_CTL		*/
/* Values can range from 0-63 (where 0 means 64)			*/
#define CONFIG_VCO_MULT			21
/* CCLK_DIV controls the core clock divider				*/
/* Values can be 1, 2, 4, or 8 ONLY					*/
#define CONFIG_CCLK_DIV			1
/* SCLK_DIV controls the system clock divider				*/
/* Values can range from 1-15						*/
#define CONFIG_SCLK_DIV			4

/* Decrease core voltage */
#define CONFIG_VR_CTL_VAL (VLEV_115 | GAIN_20 | FREQ_1000)


/*
 * Memory Settings
 */
#define CONFIG_MEM_ADD_WDTH	10
#define CONFIG_MEM_SIZE		64

#define CONFIG_EBIU_DDRCTL0_VAL	0x218A83FE
#define CONFIG_EBIU_DDRCTL1_VAL	0x20022222
#define CONFIG_EBIU_DDRCTL2_VAL	0x00000021

/* Default bank mapping:
 *	Async Bank 0 - 32MB Burst Flash
 *	Async Bank 1 - Ethernet
 *	Async Bank 2 - Nothing
 *	Async Bank 3 - Nothing
 */
#define CONFIG_EBIU_AMGCTL_VAL	0xFF
#define CONFIG_EBIU_AMBCTL0_VAL	0x7BB07BB0
#define CONFIG_EBIU_AMBCTL1_VAL	0xFFC27BB0
#define CONFIG_EBIU_FCTL_VAL	(BCLK_4)
#define CONFIG_EBIU_MODE_VAL	(B0MODE_FLASH)

#define CONFIG_SYS_MONITOR_LEN	(384 * 1024)
#define CONFIG_SYS_MALLOC_LEN	(640 * 1024)


/*
 * Network Settings
 */
#define ADI_CMDS_NETWORK	1
#define CONFIG_NET_MULTI
#define CONFIG_SMC911X	1
#define CONFIG_SMC911X_BASE	0x24000000
#define CONFIG_SMC911X_16_BIT
#define CONFIG_HOSTNAME		cm-bf548
/* Uncomment next line to use fixed MAC address */
/* #define CONFIG_ETHADDR	02:80:ad:24:31:91 */


/*
 * Flash Settings
 */
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_BASE		0x20000000
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_FLASH_PROTECTION
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_MAX_FLASH_SECT	259


/*
 * Env Storage Settings
 */
#define CONFIG_ENV_IS_IN_FLASH	1
#define CONFIG_ENV_ADDR		0x20008000
#define CONFIG_ENV_OFFSET	0x8000
#define CONFIG_ENV_SIZE		0x8000
#define CONFIG_ENV_IS_EMBEDDED_IN_LDR


/*
 * I2C Settings
 */
#define CONFIG_BFIN_TWI_I2C	1
#define CONFIG_HARD_I2C		1
#define CONFIG_SYS_I2C_SPEED	50000
#define CONFIG_SYS_I2C_SLAVE	0


/*
 * Misc Settings
 */
#define CONFIG_BAUDRATE		115200
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_RTC_BFIN
#define CONFIG_UART_CONSOLE	1
#define CONFIG_BOOTCOMMAND	"run flashboot"
#define FLASHBOOT_ENV_SETTINGS	"flashboot=bootm 0x20040000\0"

#ifndef __ADSPBF542__
/* Don't waste time transferring a logo over the UART */
# if (CONFIG_BFIN_BOOT_MODE != BFIN_BOOT_UART)
#  define CONFIG_VIDEO
# endif
# define CONFIG_DEB_DMA_URGENT
#endif

/* Define if want to do post memory test */
#undef CONFIG_POST
#ifdef CONFIG_POST
#define FLASH_START_POST_BLOCK 11       /* Should > = 11 */
#define FLASH_END_POST_BLOCK   71       /* Should < = 71 */
#endif


/*
 * Pull in common ADI header for remaining command/environment setup
 */
#include <configs/bfin_adi_common.h>

#endif
