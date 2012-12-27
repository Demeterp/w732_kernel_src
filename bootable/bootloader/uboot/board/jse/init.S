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

/*------------------------------------------------------------------------+ */
/* */
/*       This source code is dual-licensed.  You may use it under the terms */
/*       of the GNU General Public License version 2, or under the license  */
/*       below.                                                             */
/*                                                                          */
/*       This source code has been made available to you by IBM on an AS-IS */
/*       basis.  Anyone receiving this source is licensed under IBM */
/*       copyrights to use it in any way he or she deems fit, including */
/*       copying it, modifying it, compiling it, and redistributing it either */
/*       with or without modifications.  No license under IBM patents or */
/*       patent applications is to be implied by the copyright license. */
/* */
/*       Any user of this software should understand that IBM cannot provide */
/*       technical support for this software and will not be responsible for */
/*       any consequences resulting from the use of this software. */
/* */
/*       Any person who transfers this source code or any derivative work */
/*       must include the IBM copyright notice, this paragraph, and the */
/*       preceding two paragraphs in the transferred software. */
/* */
/*       COPYRIGHT   I B M   CORPORATION 1995 */
/*       LICENSED MATERIAL  -  PROGRAM PROPERTY OF I B M */
/*------------------------------------------------------------------------- */

/*------------------------------------------------------------------------- */
/* Function:     ext_bus_cntlr_init */
/* Description:  Initializes the External Bus Controller for the external */
/*		peripherals. IMPORTANT: For pass1 this code must run from */
/*		cache since you can not reliably change a peripheral banks */
/*		timing register (pbxap) while running code from that bank. */
/*		For ex., since we are running from ROM on bank 0, we can NOT */
/*		execute the code that modifies bank 0 timings from ROM, so */
/*		we run it from cache. */
/*                                    */
/*                                    */
/* The layout for the PEI JSE board:  */
/*	Bank 0 - Flash and SRAM       */
/*	Bank 1 - SystemACE            */
/*	Bank 2 - not used             */
/*	Bank 3 - not used             */
/*	Bank 4 - not used             */
/*	Bank 5 - not used             */
/*	Bank 6 - not used             */
/*	Bank 7 - not used             */
/*------------------------------------------------------------------------- */
#include <ppc4xx.h>

#include <ppc_asm.tmpl>
#include <ppc_defs.h>

#include <asm/cache.h>
#include <asm/mmu.h>

	.globl	ext_bus_cntlr_init
ext_bus_cntlr_init:
	mflr    r4                      /* save link register */
	bl      ..getAddr
..getAddr:
	mflr    r3                      /* get address of ..getAddr */
	mtlr    r4                      /* restore link register */
	addi    r4,0,14                 /* set ctr to 10; used to prefetch */
	mtctr   r4                      /* 10 cache lines to fit this function */
					/* in cache (gives us 8x10=80 instrctns) */
..ebcloop:
	icbt    r0,r3                   /* prefetch cache line for addr in r3 */
	addi    r3,r3,32		/* move to next cache line */
	bdnz    ..ebcloop               /* continue for 10 cache lines */

	/*----------------------------------------------------------------- */
	/* Delay to ensure all accesses to ROM are complete before changing */
	/* bank 0 timings. 200usec should be enough. */
	/*   200,000,000 (cycles/sec) X .000200 (sec) = 0x9C40 cycles */
	/*----------------------------------------------------------------- */
	addis	r3,0,0x0
	ori     r3,r3,0xA000          /* ensure 200usec have passed since reset */
	mtctr   r3
..spinlp:
	bdnz    ..spinlp                /* spin loop */

	/*----------------------------------------------------------------- */
	/* Memory Bank 0 (Flash) initialization */
	/*----------------------------------------------------------------- */

	addi    r4,0,PB1AP
	mtdcr   EBC0_CFGADDR,r4
	addis   r4,0,0x9B01
	ori     r4,r4,0x5480
	mtdcr   EBC0_CFGDATA,r4

	addi    r4,0,PB0CR
	mtdcr   EBC0_CFGADDR,r4
	addis   r4,0,0xFFF1           /* BAS=0xFFF,BS=0x0(1MB),BU=0x3(R/W), */
	ori     r4,r4,0x8000          /* BW=0x0( 8 bits) */
	mtdcr   EBC0_CFGDATA,r4

	blr
