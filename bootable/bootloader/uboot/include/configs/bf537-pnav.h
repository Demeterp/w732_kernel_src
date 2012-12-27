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
 * U-boot - Configuration file for BF537 PNAV board
 */

#ifndef __CONFIG_BF537_PNAV_H__
#define __CONFIG_BF537_PNAV_H__

#include <asm/config-pre.h>


/*
 * Processor Settings
 */
#define CONFIG_BFIN_CPU             bf537-0.2
#define CONFIG_BFIN_BOOT_MODE       BFIN_BOOT_SPI_MASTER


/*
 * Clock Settings
 *	CCLK = (CLKIN * VCO_MULT) / CCLK_DIV
 *	SCLK = (CLKIN * VCO_MULT) / SCLK_DIV
 */
/* CONFIG_CLKIN_HZ is any value in Hz					*/
#define CONFIG_CLKIN_HZ			24576000
/* CLKIN_HALF controls the DF bit in PLL_CTL      0 = CLKIN		*/
/*                                                1 = CLKIN / 2		*/
#define CONFIG_CLKIN_HALF		0
/* PLL_BYPASS controls the BYPASS bit in PLL_CTL  0 = do not bypass	*/
/*                                                1 = bypass PLL	*/
#define CONFIG_PLL_BYPASS		0
/* VCO_MULT controls the MSEL (multiplier) bits in PLL_CTL		*/
/* Values can range from 0-63 (where 0 means 64)			*/
#define CONFIG_VCO_MULT			20
/* CCLK_DIV controls the core clock divider				*/
/* Values can be 1, 2, 4, or 8 ONLY					*/
#define CONFIG_CCLK_DIV			1
/* SCLK_DIV controls the system clock divider				*/
/* Values can range from 1-15						*/
#define CONFIG_SCLK_DIV			4


/*
 * Memory Settings
 */
#define CONFIG_MEM_ADD_WDTH	10
#define CONFIG_MEM_SIZE		64

#define CONFIG_EBIU_SDRRC_VAL	0x3b7
#define CONFIG_EBIU_SDGCTL_VAL	0x9111cd

#define CONFIG_EBIU_AMGCTL_VAL	0xFF
#define CONFIG_EBIU_AMBCTL0_VAL	0x7BB033B0
#define CONFIG_EBIU_AMBCTL1_VAL	0xFFC27BB0

#define CONFIG_SYS_MONITOR_LEN		(256 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(128 * 1024)


/*
 * Network Settings
 */
#ifndef __ADSPBF534__
#define ADI_CMDS_NETWORK	1
#define CONFIG_BFIN_MAC
#define CONFIG_RMII
#define CONFIG_NET_MULTI	1
#endif
#define CONFIG_HOSTNAME		bf537-pnav
/* Uncomment next line to use fixed MAC address */
/* #define CONFIG_ETHADDR	02:80:ad:24:21:18 */


/*
 * Flash Settings
 */
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_BASE		0x20000000
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_MAX_FLASH_SECT	71


/*
 * SPI Settings
 */
#define CONFIG_BFIN_SPI
#define CONFIG_ENV_SPI_MAX_HZ	30000000
#define CONFIG_SF_DEFAULT_SPEED	30000000
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO


/*
 * Env Storage Settings
 */
#if (CONFIG_BFIN_BOOT_MODE == BFIN_BOOT_SPI_MASTER)
#define CONFIG_ENV_IS_EMBEDDED_IN_LDR
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_OFFSET	0x4000
#else
#define ENV_IS_EMBEDDED
#define CONFIG_ENV_IS_IN_FLASH	1
#define CONFIG_ENV_ADDR		0x20004000
#define CONFIG_ENV_OFFSET	0x4000
#endif
#define CONFIG_ENV_SIZE		0x1000
#define CONFIG_ENV_SECT_SIZE	0x2000
#ifdef ENV_IS_EMBEDDED
/* WARNING - the following is hand-optimized to fit within
 * the sector before the environment sector. If it throws
 * an error during compilation remove an object here to get
 * it linked after the configuration sector.
 */
# define LDS_BOARD_TEXT \
	arch/blackfin/cpu/traps.o		(.text .text.*); \
	arch/blackfin/cpu/interrupt.o	(.text .text.*); \
	arch/blackfin/cpu/serial.o		(.text .text.*); \
	common/dlmalloc.o		(.text .text.*); \
	lib/crc32.o		(.text .text.*); \
	. = DEFINED(env_offset) ? env_offset : .; \
	common/env_embedded.o		(.text .text.*);
#endif


/*
 * NAND Settings
 */
#define CONFIG_NAND_PLAT

#define CONFIG_SYS_NAND_BASE		0x20100000
#define CONFIG_SYS_MAX_NAND_DEVICE	1

#define BFIN_NAND_CLE(chip) ((unsigned long)(chip)->IO_ADDR_W | (1 << 2))
#define BFIN_NAND_ALE(chip) ((unsigned long)(chip)->IO_ADDR_W | (1 << 1))
#define BFIN_NAND_READY     PF12
#define BFIN_NAND_WRITE(addr, cmd) \
	do { \
		bfin_write8(addr, cmd); \
		SSYNC(); \
	} while (0)

#define NAND_PLAT_WRITE_CMD(chip, cmd) BFIN_NAND_WRITE(BFIN_NAND_CLE(chip), cmd)
#define NAND_PLAT_WRITE_ADR(chip, cmd) BFIN_NAND_WRITE(BFIN_NAND_ALE(chip), cmd)
#define NAND_PLAT_DEV_READY(chip)      (bfin_read_PORTHIO() & BFIN_NAND_READY)
#define NAND_PLAT_INIT() \
	do { \
		bfin_write_PORTH_FER(bfin_read_PORTH_FER() & ~BFIN_NAND_READY); \
		bfin_write_PORTHIO_DIR(bfin_read_PORTHIO_DIR() & ~BFIN_NAND_READY); \
		bfin_write_PORTHIO_INEN(bfin_read_PORTHIO_INEN() | BFIN_NAND_READY); \
	} while (0)


/*
 * I2C settings
 */
#define CONFIG_BFIN_TWI_I2C	1
#define CONFIG_HARD_I2C		1
#define CONFIG_SYS_I2C_SPEED		50000
#define CONFIG_SYS_I2C_SLAVE		0


/*
 * Misc Settings
 */
#define CONFIG_BAUDRATE		115200
#define CONFIG_MISC_INIT_R
#define CONFIG_RTC_BFIN
#define CONFIG_UART_CONSOLE	0

/* JFFS Partition offset set  */
#define CONFIG_SYS_JFFS2_FIRST_BANK	0
#define CONFIG_SYS_JFFS2_NUM_BANKS	1
/* 512k reserved for u-boot */
#define CONFIG_SYS_JFFS2_FIRST_SECTOR	15

#define CONFIG_BOOTCOMMAND	"run nandboot"
#define CONFIG_BOOTARGS_ROOT	"/dev/mtdblock1 rw rootfstype=yaffs"


/*
 * Pull in common ADI header for remaining command/environment setup
 */
#include <configs/bfin_adi_common.h>

#endif
