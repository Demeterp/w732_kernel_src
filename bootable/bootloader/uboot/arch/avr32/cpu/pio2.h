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
 * Register definitions for Parallel Input/Output Controller
 */
#ifndef __CPU_AT32AP_PIO2_H__
#define __CPU_AT32AP_PIO2_H__

/* PIO2 register offsets */
#define PIO2_PER				0x0000
#define PIO2_PDR				0x0004
#define PIO2_PSR				0x0008
#define PIO2_OER				0x0010
#define PIO2_ODR				0x0014
#define PIO2_OSR				0x0018
#define PIO2_IFER				0x0020
#define PIO2_IFDR				0x0024
#define PIO2_ISFR				0x0028
#define PIO2_SODR				0x0030
#define PIO2_CODR				0x0034
#define PIO2_ODSR				0x0038
#define PIO2_PDSR				0x003c
#define PIO2_IER				0x0040
#define PIO2_IDR				0x0044
#define PIO2_IMR				0x0048
#define PIO2_ISR				0x004c
#define PIO2_MDER				0x0050
#define PIO2_MDDR				0x0054
#define PIO2_MDSR				0x0058
#define PIO2_PUDR				0x0060
#define PIO2_PUER				0x0064
#define PIO2_PUSR				0x0068
#define PIO2_ASR				0x0070
#define PIO2_BSR				0x0074
#define PIO2_ABSR				0x0078
#define PIO2_OWER				0x00a0
#define PIO2_OWDR				0x00a4
#define PIO2_OWSR				0x00a8

/* Register access macros */
#define pio2_readl(base,reg)				\
	readl((void *)base + PIO2_##reg)
#define pio2_writel(base,reg,value)			\
	writel((value), (void *)base + PIO2_##reg)

#endif /* __CPU_AT32AP_PIO2_H__ */
