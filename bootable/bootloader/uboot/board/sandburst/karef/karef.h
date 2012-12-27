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

#ifndef __KAREF_H__
#define __KAREF_H__
/*
 * (C) Copyright 2005
 * Sandburst Corporation
 * Travis B. Sawyer
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* Ka Reference Design OFEM FPGA Registers & definitions */
#include "hal_ka_sc_auto.h"
#include "hal_ka_of_auto.h"

typedef struct karef_board_id_s {
	const char name[40];
} KAREF_BOARD_ID_ST, *KAREF_BOARD_ID_PST;

/* SCAN FPGA */
typedef struct karef_fpga_regs_s
{
    volatile unsigned long revision_ul;       /* Read Only  */
    volatile unsigned long reset_ul;          /* Read/Write */
    volatile unsigned long interrupt_ul;      /* Read Only  */
    volatile unsigned long mask_ul;           /* Read/Write */
    volatile unsigned long scratch_ul;        /* Read/Write */
    volatile unsigned long scrmask_ul;        /* Read/Write */
    volatile unsigned long status_ul;         /* Read Only  */
    volatile unsigned long control_ul;        /* Read/Write */
    volatile unsigned long boardinfo_ul;      /* Read Only  */
    volatile unsigned long scan_from0_ul;     /* Read Only  */
    volatile unsigned long scan_from1_ul;     /* Read Only  */
    volatile unsigned long scan_to0_ul;       /* Read/Write */
    volatile unsigned long scan_to1_ul;       /* Read/Write */
    volatile unsigned long scan_control_ul;   /* Read/Write */
    volatile unsigned long pll_control_ul;    /* Read/Write */
    volatile unsigned long core_clock_cnt_ul; /* Read/Write */
    volatile unsigned long dr_clock_cnt_ul;   /* Read/Write */
    volatile unsigned long spi_clock_cnt_ul;  /* Read/Write */
    volatile unsigned long brdout_data_ul;    /* Read/Write */
    volatile unsigned long brdout_enable_ul;  /* Read/Write */
    volatile unsigned long brdin_data_ul;     /* Read Only  */
    volatile unsigned long misc_ul;           /* Read/Write */
} __attribute__((packed)) KAREF_FPGA_REGS_ST , * KAREF_FPGA_REGS_PST;

/* OFEM FPGA */
typedef struct ofem_fpga_regs_s
{
    volatile unsigned long revision_ul;       /* Read Only  */
    volatile unsigned long reset_ul;          /* Read/Write */
    volatile unsigned long interrupt_ul;      /* Read Only  */
    volatile unsigned long mask_ul;           /* Read/Write */
    volatile unsigned long scratch_ul;        /* Read/Write */
    volatile unsigned long scrmask_ul;        /* Read/Write */
    volatile unsigned long control_ul;        /* Read/Write */
    volatile unsigned long mac_flow_ctrl_ul;  /* Read/Write */
} __attribute__((packed)) OFEM_FPGA_REGS_ST , * OFEM_FPGA_REGS_PST;


#endif /* __KAREF_H__ */
