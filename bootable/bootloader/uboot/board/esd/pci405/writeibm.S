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

/*------------------------------------------------------------------------------+ */
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
/*------------------------------------------------------------------------------- */

/*----------------------------------------------------------------------------- */
/* Function:     ext_bus_cntlr_init */
/* Description:  Initializes the External Bus Controller for the external */
/*		peripherals. IMPORTANT: For pass1 this code must run from */
/*		cache since you can not reliably change a peripheral banks */
/*		timing register (pbxap) while running code from that bank. */
/*		For ex., since we are running from ROM on bank 0, we can NOT */
/*		execute the code that modifies bank 0 timings from ROM, so */
/*		we run it from cache. */
/*	Bank 0 - Flash and SRAM */
/*	Bank 1 - NVRAM/RTC */
/*	Bank 2 - Keyboard/Mouse controller */
/*	Bank 3 - IR controller */
/*	Bank 4 - not used */
/*	Bank 5 - not used */
/*	Bank 6 - not used */
/*	Bank 7 - FPGA registers */
/*----------------------------------------------------------------------------- */
#include <ppc4xx.h>

#include <ppc_asm.tmpl>
#include <ppc_defs.h>

#include <asm/cache.h>
#include <asm/mmu.h>


	.globl	write_without_sync
write_without_sync:
		/*
		 * Write one values to host via pci busmastering
		 * ptr = 0xc0000000 -> 0x01000000 (PCI)
		 * *ptr = 0x01234567;
		 */
	addi    r31,0,0
	lis     r31,0xc000

start1:
	lis     r0,0x0123
	ori     r0,r0,0x4567
	stw     r0,0(r31)

		/*
		 * Read one value back
		 * ptr = (volatile unsigned long *)addr;
		 * val = *ptr;
		 */

	lwz     r0,0(r31)

		/*
		 * One pci config write
		 * ibmPciConfigWrite(0x2e, 2, 0x1234);
		 */
		/* subsystem id */

	li      r4,0x002C
	oris    r4,r4,0x8000
	lis     r3,0xEEC0
	stwbrx  r4,0,r3

	li      r5,0x1234
	ori     r3,r3,0x4
	stwbrx  r5,0,r3

	b       start1

	blr	/* never reached !!!! */

	.globl	write_with_sync
write_with_sync:
		/*
		 * Write one values to host via pci busmastering
		 * ptr = 0xc0000000 -> 0x01000000 (PCI)
		 * *ptr = 0x01234567;
		 */
	addi    r31,0,0
	lis     r31,0xc000

start2:
	lis     r0,0x0123
	ori     r0,r0,0x4567
	stw     r0,0(r31)

		/*
		 * Read one value back
		 * ptr = (volatile unsigned long *)addr;
		 * val = *ptr;
		 */

	lwz     r0,0(r31)

		/*
		 * One pci config write
		 * ibmPciConfigWrite(0x2e, 2, 0x1234);
		 */
		/* subsystem id */

	li      r4,0x002C
	oris    r4,r4,0x8000
	lis     r3,0xEEC0
	stwbrx  r4,0,r3
	sync

	li      r5,0x1234
	ori     r3,r3,0x4
	stwbrx  r5,0,r3
	sync

	b       start2

	blr	/* never reached !!!! */

	.globl	write_with_less_sync
write_with_less_sync:
		/*
		 * Write one values to host via pci busmastering
		 * ptr = 0xc0000000 -> 0x01000000 (PCI)
		 * *ptr = 0x01234567;
		 */
	addi    r31,0,0
	lis     r31,0xc000

start2b:
	lis     r0,0x0123
	ori     r0,r0,0x4567
	stw     r0,0(r31)

		/*
		 * Read one value back
		 * ptr = (volatile unsigned long *)addr;
		 * val = *ptr;
		 */

	lwz     r0,0(r31)

		/*
		 * One pci config write
		 * ibmPciConfigWrite(0x2e, 2, 0x1234);
		 */
		/* subsystem id */

	li      r4,0x002C
	oris    r4,r4,0x8000
	lis     r3,0xEEC0
	stwbrx  r4,0,r3
	sync

	li      r5,0x1234
	ori     r3,r3,0x4
	stwbrx  r5,0,r3
/*        sync */

	b       start2b

	blr	/* never reached !!!! */

	.globl	write_with_more_sync
write_with_more_sync:
		/*
		 * Write one values to host via pci busmastering
		 * ptr = 0xc0000000 -> 0x01000000 (PCI)
		 * *ptr = 0x01234567;
		 */
	addi    r31,0,0
	lis     r31,0xc000

start3:
	lis     r0,0x0123
	ori     r0,r0,0x4567
	stw     r0,0(r31)
	sync

		/*
		 * Read one value back
		 * ptr = (volatile unsigned long *)addr;
		 * val = *ptr;
		 */

	lwz     r0,0(r31)
	sync

		/*
		 * One pci config write
		 * ibmPciConfigWrite(0x2e, 2, 0x1234);
		 */
		/* subsystem id (PCIC0_SBSYSVID)*/

	li      r4,0x002C
	oris    r4,r4,0x8000
	lis     r3,0xEEC0
	stwbrx  r4,0,r3
	sync

	li      r5,0x1234
	ori     r3,r3,0x4
	stwbrx  r5,0,r3
	sync

	b       start3

	blr	/* never reached !!!! */
