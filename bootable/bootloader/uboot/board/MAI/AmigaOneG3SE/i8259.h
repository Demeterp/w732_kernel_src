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
 * (C) Copyright 2002
 * John W. Linville, linville@tuxdriver.com
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#define ICW1_1  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT1_ICW1
#define ICW1_2  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT2_ICW1
#define ICW2_1  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT1_ICW2
#define ICW2_2  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT2_ICW2
#define ICW3_1  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT1_ICW3
#define ICW3_2  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT2_ICW3
#define ICW4_1  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT1_ICW4
#define ICW4_2  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT2_ICW4
#define OCW1_1  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT1_OCW1
#define OCW1_2  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT2_OCW1
#define OCW2_1  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT1_OCW2
#define OCW2_2  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT2_OCW2
#define OCW3_1  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT1_OCW3
#define OCW3_2  CONFIG_SYS_ISA_IO_BASE_ADDRESS + ISA_INT2_OCW3

#define IMR_1   OCW1_1
#define IMR_2   OCW1_2

#define ISR_1   ICW1_1
#define ISR_2   ICW1_2

#define IPL_1   ICW1_1
#define IPL_2   ICW1_2

extern void i8259_init(void);

extern int i8259_get_irq(struct pt_regs *regs);

extern void i8259_mask_and_ack(int irq);

extern void i8259_mask_irq(int irq);

extern void i8259_unmask_irq(int irq);
