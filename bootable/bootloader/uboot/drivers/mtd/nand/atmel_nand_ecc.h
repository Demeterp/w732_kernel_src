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
 * Error Corrected Code Controller (ECC) - System peripherals regsters.
 * Based on AT91SAM9260 datasheet revision B.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#ifndef ATMEL_NAND_ECC_H
#define ATMEL_NAND_ECC_H

#define ATMEL_ECC_CR		0x00			/* Control register */
#define		ATMEL_ECC_RST		(1 << 0)		/* Reset parity */

#define ATMEL_ECC_MR		0x04			/* Mode register */
#define		ATMEL_ECC_PAGESIZE	(3 << 0)		/* Page Size */
#define			ATMEL_ECC_PAGESIZE_528		(0)
#define			ATMEL_ECC_PAGESIZE_1056		(1)
#define			ATMEL_ECC_PAGESIZE_2112		(2)
#define			ATMEL_ECC_PAGESIZE_4224		(3)

#define ATMEL_ECC_SR		0x08			/* Status register */
#define		ATMEL_ECC_RECERR		(1 << 0)		/* Recoverable Error */
#define		ATMEL_ECC_ECCERR		(1 << 1)		/* ECC Single Bit Error */
#define		ATMEL_ECC_MULERR		(1 << 2)		/* Multiple Errors */

#define ATMEL_ECC_PR		0x0c			/* Parity register */
#define		ATMEL_ECC_BITADDR	(0xf << 0)		/* Bit Error Address */
#define		ATMEL_ECC_WORDADDR	(0xfff << 4)		/* Word Error Address */

#define ATMEL_ECC_NPR		0x10			/* NParity register */
#define		ATMEL_ECC_NPARITY	(0xffff << 0)		/* NParity */

#endif
