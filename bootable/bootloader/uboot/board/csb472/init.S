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

/******************************************************************************
 *   This source code is dual-licensed.  You may use it under the terms of the
 *   GNU General Public License version 2, or under the license below.
 *
 *	 This source code has been made available to you by IBM on an AS-IS
 *	 basis.	 Anyone receiving this source is licensed under IBM
 *	 copyrights to use it in any way he or she deems fit, including
 *	 copying it, modifying it, compiling it, and redistributing it either
 *	 with or without modifications.	 No license under IBM patents or
 *	 patent applications is to be implied by the copyright license.
 *
 *	 Any user of this software should understand that IBM cannot provide
 *	 technical support for this software and will not be responsible for
 *	 any consequences resulting from the use of this software.
 *
 *	 Any person who transfers this source code or any derivative work
 *	 must include the IBM copyright notice, this paragraph, and the
 *	 preceding two paragraphs in the transferred software.
 *
 *	 COPYRIGHT   I B M   CORPORATION 1995
 *	 LICENSED MATERIAL  -  PROGRAM PROPERTY OF I B M
 *
 *****************************************************************************/
#include <config.h>
#include <ppc4xx.h>

#define _LINUX_CONFIG_H 1	/* avoid reading Linux autoconf.h file	*/

#include <ppc_asm.tmpl>
#include <ppc_defs.h>

#include <asm/cache.h>
#include <asm/mmu.h>

#define LI32(reg,val) \
	addis   reg,0,val@h;\
	ori     reg,reg,val@l

#define WDCR_EBC(reg,val) \
	addi    r4,0,reg;\
	mtdcr   EBC0_CFGADDR,r4;\
	addis   r4,0,val@h;\
	ori     r4,r4,val@l;\
	mtdcr   EBC0_CFGDATA,r4

#define WDCR_SDRAM(reg,val) \
	addi    r4,0,reg;\
	mtdcr   SDRAM0_CFGADDR,r4;\
	addis   r4,0,val@h;\
	ori     r4,r4,val@l;\
	mtdcr   SDRAM0_CFGDATA,r4

/******************************************************************************
 * Function:	ext_bus_cntlr_init
 *
 * Description:	Configures EBC Controller and a few basic chip selects.
 *
 *		CS0 is setup to get the Boot Flash out of the addresss range
 *		so that we may setup a stack.  CS7 is setup so that we can
 *		access and reset the hardware watchdog.
 *
 *		IMPORTANT: For pass1 this code must run from
 *		cache since you can not reliably change a peripheral banks
 *		timing register (pbxap) while running code from that bank.
 *		For ex., since we are running from ROM on bank 0, we can NOT
 *		execute the code that modifies bank 0 timings from ROM, so
 *		we run it from cache.
 *
 * Notes:	Does NOT use the stack.
 *****************************************************************************/
	.section ".text"
	.align	2
	.globl	ext_bus_cntlr_init
	.type	ext_bus_cntlr_init, @function
ext_bus_cntlr_init:
	mflr	r0
	/********************************************************************
	 * Prefetch entire ext_bus_cntrl_init function into the icache.
	 * This is necessary because we are going to change the same CS we
	 * are executing from.  Otherwise a CPU lockup may occur.
	 *******************************************************************/
	bl	..getAddr
..getAddr:
	mflr	r3			/* get address of ..getAddr */

	/* Calculate number of cache lines for this function */
	addi	r4, 0, (((.Lfe0 - ..getAddr) / CONFIG_SYS_CACHELINE_SIZE) + 2)
	mtctr	r4
..ebcloop:
	icbt	r0, r3			/* prefetch cache line for addr in r3*/
	addi	r3, r3, CONFIG_SYS_CACHELINE_SIZE /* move to next cache line */
	bdnz	..ebcloop		/* continue for $CTR cache lines */

	/********************************************************************
	 * Delay to ensure all accesses to ROM are complete before changing
	 * bank 0 timings. 200usec should be enough.
	 * 200,000,000 (cycles/sec) X .000200 (sec) = 0x9C40 cycles.
	 *******************************************************************/
	addis	r3, 0, 0x0
	ori	r3, r3, 0xA000		/* wait 200us from reset */
	mtctr	r3
..spinlp:
	bdnz	..spinlp		/* spin loop */

	/********************************************************************
	 * SETUP CPC0_CR0
	 *******************************************************************/
	LI32(r4, 0x00c01030)
	mtdcr	CPC0_CR0, r4

	/********************************************************************
	 * Setup CPC0_CR1: Change PCIINT signal to PerWE
	 *******************************************************************/
	mfdcr	r4, CPC0_CR1
	ori	r4, r4, 0x4000
	mtdcr	CPC0_CR1, r4

	/********************************************************************
	 * Setup External Bus Controller (EBC).
	 *******************************************************************/
	WDCR_EBC(EBC0_CFG, 0xd84c0000)
	/********************************************************************
	 * Memory Bank 0 (Intel 28F640J3 Flash) initialization
	 *******************************************************************/
	/*WDCR_EBC(PB1AP, 0x03055200)*/
	/*WDCR_EBC(PB1AP, 0x04055200)*/
	WDCR_EBC(PB1AP, 0x08055200)
	WDCR_EBC(PB0CR, 0xff87a000)
	/********************************************************************
	 * Memory Bank 3 (Xilinx XC95144 CPLD) initialization
	 *******************************************************************/
	/*WDCR_EBC(PB3AP, 0x07869200)*/
	WDCR_EBC(PB3AP, 0x04055200)
	WDCR_EBC(PB3CR, 0xf081c000)
	/********************************************************************
	 * Memory Bank 1,2,4-7 (Unused) initialization
	 *******************************************************************/
	WDCR_EBC(PB1AP, 0)
	WDCR_EBC(PB1CR, 0)
	WDCR_EBC(PB2AP, 0)
	WDCR_EBC(PB2CR, 0)
	WDCR_EBC(PB4AP, 0)
	WDCR_EBC(PB4CR, 0)
	WDCR_EBC(PB5AP, 0)
	WDCR_EBC(PB5CR, 0)
	WDCR_EBC(PB6AP, 0)
	WDCR_EBC(PB6CR, 0)
	WDCR_EBC(PB7AP, 0)
	WDCR_EBC(PB7CR, 0)

	/* We are all done */
	mtlr	r0			/* Restore link register */
	blr				/* Return to calling function */
.Lfe0:	.size	ext_bus_cntlr_init,.Lfe0-ext_bus_cntlr_init
/* end ext_bus_cntlr_init() */

/******************************************************************************
 * Function:	sdram_init
 *
 * Description:	Configures SDRAM memory banks.
 *
 * Notes:	Does NOT use the stack.
 *****************************************************************************/
	.section ".text"
	.align	2
	.globl	sdram_init
	.type	sdram_init, @function
sdram_init:

	/*
	 * Disable memory controller to allow
	 * values to be changed.
	 */
	WDCR_SDRAM(SDRAM0_CFG, 0x00000000)

	/*
	 * Configure Memory Banks
	 */
	WDCR_SDRAM(SDRAM0_B0CR, 0x00062001)
	WDCR_SDRAM(SDRAM0_B1CR, 0x00000000)
	WDCR_SDRAM(SDRAM0_B2CR, 0x00000000)
	WDCR_SDRAM(SDRAM0_B3CR, 0x00000000)

	/*
	 * Set up SDTR1 (SDRAM Timing Register)
	 */
	WDCR_SDRAM(SDRAM0_TR, 0x00854009)

	/*
	 * Set RTR (Refresh Timing Register)
	 */
	WDCR_SDRAM(SDRAM0_RTR,   0x10000000)
	/* WDCR_SDRAM(SDRAM0_RTR,   0x05f00000) */

	/********************************************************************
	 * Delay to ensure 200usec have elapsed since reset. Assume worst
	 * case that the core is running 200Mhz:
	 *	  200,000,000 (cycles/sec) X .000200 (sec) = 0x9C40 cycles
	 *******************************************************************/
	addis   r3, 0, 0x0000
	ori     r3, r3, 0xA000		/* Wait >200us from reset */
	mtctr   r3
..spinlp2:
	bdnz    ..spinlp2		/* spin loop */

	/********************************************************************
	 * Set memory controller options reg, MCOPT1.
	 *******************************************************************/
	WDCR_SDRAM(SDRAM0_CFG,0x80800000)

..sdri_done:
	blr				/* Return to calling function */
.Lfe1:	.size	sdram_init,.Lfe1-sdram_init
/* end sdram_init() */
