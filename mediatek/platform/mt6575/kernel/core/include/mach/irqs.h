/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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

#ifndef __MT6575_IRQS_H__
#define __MT6575_IRQS_H__

#include "mt6575_irq.h"

/*
 * Define constants.
 */
#define FIQ_START 0
#define NR_IRQS NR_MT6575_IRQ_LINE
#define MT65xx_EDGE_SENSITIVE 0
#define MT65xx_LEVEL_SENSITIVE 1

#define MT65xx_POLARITY_LOW   0
#define MT65xx_POLARITY_HIGH  1


#if !defined(__ASSEMBLY__)

/*
 * Define data structures.
 */

enum {IRQ_MASK_HEADER = 0xF1F1F1F1, IRQ_MASK_FOOTER = 0xF2F2F2F2};

struct mtk_irq_mask
{
    unsigned int header;   /* for error checking */
    __u32 mask0;
    __u32 mask1;
    __u32 mask2;
    __u32 mask3;
    __u32 mask4;
    unsigned int footer;   /* for error checking */
};

/*
 * Define function prototypes.
 */

//extern void mt6575_irq_mask(struct irq_data *data);
//extern void mt6575_irq_unmask(struct irq_data *data);
//extern void mt6575_irq_ack(struct irq_data *data);
extern void mt6575_irq_set_sens(unsigned int irq, unsigned int sens);
extern void mt6575_irq_set_polarity(unsigned int irq, unsigned int polarity);
//extern int mt6575_irq_set_type(struct irq_data *data, unsigned int flow_type);
extern void mt6575_init_irq(void);
extern int mt6575_irq_is_active(const unsigned int irq);

/*
 * Define macros.
 */

#define mt65xx_irq_mask(l) mt6575_irq_mask((l))
#define mt65xx_irq_unmask(l) mt6575_irq_unmask((l))
#define mt65xx_irq_ack(l) mt6575_irq_ack((l))
#define mt65xx_irq_set_sens(l, s) mt6575_irq_set_sens((l), (s))
#define mt65xx_irq_set_polarity(l, s) mt6575_irq_set_polarity((l), (s))

#endif  /* !__ASSEMBLY__ */

#endif  /* !__MT6575_IRQS_H__ */
