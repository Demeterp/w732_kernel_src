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

#ifndef _MTK_ADC_HW_H
#define _MTK_ADC_HW_H

#include <mach/mt6575_reg_base.h>
#if 0
#define AUXADC_CON0                     (AUXADC_BASE + 0x000)
#define AUXADC_CON1                     (AUXADC_BASE + 0x004)
#define AUXADC_CON2                     (AUXADC_BASE + 0x008)
#define AUXADC_CON3                     (AUXADC_BASE + 0x00C)

#define AUXADC_DAT0                     (AUXADC_BASE + 0x010)
#define AUXADC_DAT1                     (AUXADC_BASE + 0x014)
#define AUXADC_DAT2                     (AUXADC_BASE + 0x018)
#define AUXADC_DAT3                     (AUXADC_BASE + 0x01C)
#define AUXADC_DAT4                     (AUXADC_BASE + 0x020)
#define AUXADC_DAT5                     (AUXADC_BASE + 0x024)
#define AUXADC_DAT6                     (AUXADC_BASE + 0x028)
#define AUXADC_DAT7                     (AUXADC_BASE + 0x02C)
#define AUXADC_DAT8                     (AUXADC_BASE + 0x030)
#define AUXADC_DAT9                     (AUXADC_BASE + 0x034)
#define AUXADC_DAT10                    (AUXADC_BASE + 0x038)
#define AUXADC_DAT11                    (AUXADC_BASE + 0x03C)
#define AUXADC_DAT12                    (AUXADC_BASE + 0x040)
#define AUXADC_DAT13                    (AUXADC_BASE + 0x044)
#endif
#define AUXADC_CON0                     (AUXADC_BASE + 0x000)
#define AUXADC_CON1                     (AUXADC_BASE + 0x004)
#define AUXADC_CON1_SET                 (AUXADC_BASE + 0x008)
#define AUXADC_CON1_CLR                 (AUXADC_BASE + 0x00C)
#define AUXADC_CON2                     (AUXADC_BASE + 0x010)
#define AUXADC_CON3                     (AUXADC_BASE + 0x014)

#define AUXADC_DAT0                     (AUXADC_BASE + 0x018)
#define AUXADC_DAT1                     (AUXADC_BASE + 0x01C)
#define AUXADC_DAT2                     (AUXADC_BASE + 0x020)
#define AUXADC_DAT3                     (AUXADC_BASE + 0x024)
#define AUXADC_DAT4                     (AUXADC_BASE + 0x028)
#define AUXADC_DAT5                     (AUXADC_BASE + 0x024+0x008)
#define AUXADC_DAT6                     (AUXADC_BASE + 0x028+0x008)
#define AUXADC_DAT7                     (AUXADC_BASE + 0x02C+0x008)
#define AUXADC_DAT8                     (AUXADC_BASE + 0x030+0x008)
#define AUXADC_DAT9                     (AUXADC_BASE + 0x034+0x008)
#define AUXADC_DAT10                    (AUXADC_BASE + 0x038+0x008)
#define AUXADC_DAT11                    (AUXADC_BASE + 0x03C+0x008)
#define AUXADC_DAT12                    (AUXADC_BASE + 0x040+0x008)
#define AUXADC_DAT13                    (AUXADC_BASE + 0x044+0x008)
#define AUXADC_DET_VOLT                 (AUXADC_BASE + 0x084)
#define AUXADC_DET_SEL                  (AUXADC_BASE + 0x088)
#define AUXADC_DET_PERIOD               (AUXADC_BASE + 0x08C)
#define AUXADC_DET_DEBT                 (AUXADC_BASE + 0x090)
#define AUXADC_MISC                     (AUXADC_BASE + 0x094)
#define AUXADC_ECC                      (AUXADC_BASE + 0x098)
#define AUXADC_SAMPLE_LIST              (AUXADC_BASE + 0x09c)
#define AUXADC_ABIST_PERIOD             (AUXADC_BASE + 0x0A0)

//-----------------------------------------------------------------------------

/*AUXADC_SYNC on AUXADC_CON0*/
#define AUXADC_SYNC_CHAN(_line)         (0x0001<<_line)   /*Time event 1*/

/*AUXADC_IMM on AUXADC_CON1*/
#define AUXADC_IMM_CHAN(_line)          (0x0001<<_line)

/*AUXADC_SYN on AUXADC_CON2*/
#define AUXADC_SYN_BIT                  (0x0001)         /*Time event 0*/

/*AUXADC_CON3*/
#define AUXADC_CON3_STATUS_MASK         (0x0001)
#define AUXADC_CON3_STATUS_OFFSET       (0)
#define AUXADC_STATUS_BUSY          	(0x01)
#define AUXADC_STATUS_IDLE          	(0x00) 

#define AUXADC_CON3_SOFT_RST_MASK       (0x0080)
#define AUXADC_CON3_SOFT_RST_OFFSET     (7)

#define AUXADC_CON3_AUTOCLR0_MASK       (0x0100)
#define AUXADC_CON3_AUTOCLR0_OFFSET     (8)

#define AUXADC_CON3_AUTOCLR1_MASK       (0x0200)
#define AUXADC_CON3_AUTOCLR1_OFFSET     (9)

#define AUXADC_CON3_PUWAIT_EN_MASK      (0x0800)
#define AUXADC_CON3_PUWAIT_EN_OFFSET    (11)

#define AUXADC_CON3_AUTOSET_MASK        (0x8000)
#define AUXADC_CON3_AUTOSET_OFFSET      (15)

#endif   /*_MTK_ADC_HW_H*/

