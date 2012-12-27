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
 * (C) Copyright 2010 Samsung Electronics
 * Naveen Krishna Ch <ch.naveen@samsung.com>
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
 *
 * Note: This file contains the register description for Memory subsystem
 * 	 (SROM, NAND Flash, OneNand, DDR, OneDRAM) on S5PC1XX.
 *
 * 	 Only SROMC is defined as of now
 */

#ifndef __ASM_ARCH_SMC_H_
#define __ASM_ARCH_SMC_H_

#define SMC_DATA16_WIDTH(x)    (1<<((x*4)+0))
#define SMC_BYTE_ADDR_MODE(x)  (1<<((x*4)+1))  /* 0-> Half-word base address*/
						/* 1-> Byte base address*/
#define SMC_WAIT_ENABLE(x)     (1<<((x*4)+2))
#define SMC_BYTE_ENABLE(x)     (1<<((x*4)+3))

#define SMC_BC_TACS(x) (x << 28) /* 0clk     address set-up */
#define SMC_BC_TCOS(x) (x << 24) /* 4clk     chip selection set-up */
#define SMC_BC_TACC(x) (x << 16) /* 14clk    access cycle */
#define SMC_BC_TCOH(x) (x << 12) /* 1clk     chip selection hold */
#define SMC_BC_TAH(x)  (x << 8)  /* 4clk     address holding time */
#define SMC_BC_TACP(x) (x << 4)  /* 6clk     page mode access cycle */
#define SMC_BC_PMC(x)  (x << 0)  /* normal(1data)page mode configuration */

#ifndef __ASSEMBLY__
struct s5pc1xx_smc {
	unsigned int	bw;
	unsigned int	bc[6];
};
#endif	/* __ASSEMBLY__ */

/* Configure the Band Width and Bank Control Regs for required SROMC Bank */
void s5pc1xx_config_sromc(u32 srom_bank, u32 smc_bw_conf, u32 smc_bc_conf);

#endif /* __ASM_ARCH_SMC_H_ */
