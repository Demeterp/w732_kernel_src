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
 * needed for arch/arm/cpu/arm_cortexa8/mx51/lowlevel_init.S
 *
 * These should be auto-generated
 */
/* CCM */
#define CLKCTL_CCR              0x00
#define CLKCTL_CCDR             0x04
#define CLKCTL_CSR              0x08
#define CLKCTL_CCSR             0x0C
#define CLKCTL_CACRR            0x10
#define CLKCTL_CBCDR            0x14
#define CLKCTL_CBCMR            0x18
#define CLKCTL_CSCMR1           0x1C
#define CLKCTL_CSCMR2           0x20
#define CLKCTL_CSCDR1           0x24
#define CLKCTL_CS1CDR           0x28
#define CLKCTL_CS2CDR           0x2C
#define CLKCTL_CDCDR            0x30
#define CLKCTL_CHSCCDR          0x34
#define CLKCTL_CSCDR2           0x38
#define CLKCTL_CSCDR3           0x3C
#define CLKCTL_CSCDR4           0x40
#define CLKCTL_CWDR             0x44
#define CLKCTL_CDHIPR           0x48
#define CLKCTL_CDCR             0x4C
#define CLKCTL_CTOR             0x50
#define CLKCTL_CLPCR            0x54
#define CLKCTL_CISR             0x58
#define CLKCTL_CIMR             0x5C
#define CLKCTL_CCOSR            0x60
#define CLKCTL_CGPR             0x64
#define CLKCTL_CCGR0            0x68
#define CLKCTL_CCGR1            0x6C
#define CLKCTL_CCGR2            0x70
#define CLKCTL_CCGR3            0x74
#define CLKCTL_CCGR4            0x78
#define CLKCTL_CCGR5            0x7C
#define CLKCTL_CCGR6            0x80
#define CLKCTL_CMEOR            0x84

/* DPLL */
#define PLL_DP_CTL	0x00
#define PLL_DP_CONFIG	0x04
#define PLL_DP_OP	0x08
#define PLL_DP_MFD	0x0C
#define PLL_DP_MFN	0x10
#define PLL_DP_HFS_OP	0x1C
#define PLL_DP_HFS_MFD	0x20
#define PLL_DP_HFS_MFN	0x24
