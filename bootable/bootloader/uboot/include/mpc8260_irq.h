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

#ifndef _MPC8260_IRQ_H
#define _MPC8260_IRQ_H

/****************************************************************************/
/* most of this was ripped out of include/asm-ppc/irq.h from the Linux/PPC  */
/* source. There was no copyright information in the file.		    */

/*
 * this is the # irq's for all ppc arch's (pmac/chrp/prep)
 * so it is the max of them all
 *
 * [let's just worry about 8260 for now - mjj]
 */
#define NR_IRQS			64

/* The 8260 has an internal interrupt controller with a maximum of
 * 64 IRQs.  We will use NR_IRQs from above since it is large enough.
 * Don't be confused by the 8260 documentation where they list an
 * "interrupt number" and "interrupt vector".  We are only interested
 * in the interrupt vector.  There are "reserved" holes where the
 * vector number increases, but the interrupt number in the table does not.
 * (Document errata updates have fixed this...make sure you have up to
 * date processor documentation -- Dan).
 */
#define NR_SIU_INTS	64

/* There are many more than these, we will add them as we need them.
*/
#define	SIU_INT_SMC1		((uint)0x04)
#define	SIU_INT_SMC2		((uint)0x05)
#define	SIU_INT_IRQ1		((uint)0x13)
#define	SIU_INT_IRQ2		((uint)0x14)
#define	SIU_INT_IRQ3		((uint)0x15)
#define	SIU_INT_IRQ4		((uint)0x16)
#define	SIU_INT_IRQ5		((uint)0x17)
#define	SIU_INT_IRQ6		((uint)0x18)
#define	SIU_INT_IRQ7		((uint)0x19)
#define	SIU_INT_FCC1		((uint)0x20)
#define	SIU_INT_FCC2		((uint)0x21)
#define	SIU_INT_FCC3		((uint)0x22)
#define	SIU_INT_SCC1		((uint)0x28)
#define	SIU_INT_SCC2		((uint)0x29)
#define	SIU_INT_SCC3		((uint)0x2a)
#define	SIU_INT_SCC4		((uint)0x2b)

#define NR_MASK_WORDS	((NR_IRQS + 31) / 32)

#endif /* _MPC8260_IRQ_H */
