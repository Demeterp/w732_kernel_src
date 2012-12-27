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

#ifndef _PPC_KERNEL_MPC8349_PCI_H
#define _PPC_KERNEL_MPC8349_PCI_H


#define M8265_PCIBR0	0x101ac
#define M8265_PCIBR1	0x101b0
#define M8265_PCIMSK0	0x101c4
#define M8265_PCIMSK1	0x101c8

/* Bit definitions for PCIBR registers */

#define PCIBR_ENABLE        0x00000001

/* Bit definitions for PCIMSK registers */

#define PCIMSK_32KB         0xFFFF8000  /* Size of window, smallest */
#define PCIMSK_64KB         0xFFFF0000
#define PCIMSK_128KB        0xFFFE0000
#define PCIMSK_256KB        0xFFFC0000
#define PCIMSK_512KB        0xFFF80000
#define PCIMSK_1MB          0xFFF00000
#define PCIMSK_2MB          0xFFE00000
#define PCIMSK_4MB          0xFFC00000
#define PCIMSK_8MB          0xFF800000
#define PCIMSK_16MB         0xFF000000
#define PCIMSK_32MB         0xFE000000
#define PCIMSK_64MB         0xFC000000
#define PCIMSK_128MB        0xF8000000
#define PCIMSK_256MB        0xF0000000
#define PCIMSK_512MB        0xE0000000
#define PCIMSK_1GB          0xC0000000  /* Size of window, largest */


#define M826X_SCCR_PCI_MODE_EN 0x100


/*
 * Outbound ATU registers (3 sets). These registers control how 60x bus
 * (local) addresses are translated to PCI addresses when the MPC826x is
 * a PCI bus master (initiator).
 */

#define POTAR_REG0          0x10800     /* PCI Outbound Translation Addr registers */
#define POTAR_REG1          0x10818
#define POTAR_REG2          0x10830

#define POBAR_REG0          0x10808     /* PCI Outbound Base Addr registers */
#define POBAR_REG1          0x10820
#define POBAR_REG2          0x10838

#define POCMR_REG0          0x10810     /* PCI Outbound Comparison Mask registers */
#define POCMR_REG1          0x10828
#define POCMR_REG2          0x10840

/* Bit definitions for POMCR registers */

#define POCMR_MASK_4KB      0x000FFFFF
#define POCMR_MASK_8KB      0x000FFFFE
#define POCMR_MASK_16KB     0x000FFFFC
#define POCMR_MASK_32KB     0x000FFFF8
#define POCMR_MASK_64KB     0x000FFFF0
#define POCMR_MASK_128KB    0x000FFFE0
#define POCMR_MASK_256KB    0x000FFFC0
#define POCMR_MASK_512KB    0x000FFF80
#define POCMR_MASK_1MB      0x000FFF00
#define POCMR_MASK_2MB      0x000FFE00
#define POCMR_MASK_4MB      0x000FFC00
#define POCMR_MASK_8MB      0x000FF800
#define POCMR_MASK_16MB     0x000FF000
#define POCMR_MASK_32MB     0x000FE000
#define POCMR_MASK_64MB     0x000FC000
#define POCMR_MASK_128MB    0x000F8000
#define POCMR_MASK_256MB    0x000F0000
#define POCMR_MASK_512MB    0x000E0000
#define POCMR_MASK_1GB      0x000C0000

#define POCMR_ENABLE        0x80000000
#define POCMR_PCI_IO        0x40000000
#define POCMR_PREFETCH_EN   0x20000000
#define POCMR_PCI2          0x10000000

/* Soft PCI reset */

#define PCI_GCR_REG         0x10880

/* Bit definitions for PCI_GCR registers */

#define PCIGCR_PCI_BUS_EN   0x1

/*
 * Inbound ATU registers (2 sets). These registers control how PCI
 * addresses are translated to 60x bus (local) addresses when the
 * MPC826x is a PCI bus target.
 */

#define PITAR_REG1          0x108D0
#define PIBAR_REG1          0x108D8
#define PICMR_REG1          0x108E0
#define PITAR_REG0          0x108E8
#define PIBAR_REG0          0x108F0
#define PICMR_REG0          0x108F8

/* Bit definitions for PCI Inbound Comparison Mask registers */

#define PICMR_MASK_4KB       0x000FFFFF
#define PICMR_MASK_8KB       0x000FFFFE
#define PICMR_MASK_16KB      0x000FFFFC
#define PICMR_MASK_32KB      0x000FFFF8
#define PICMR_MASK_64KB      0x000FFFF0
#define PICMR_MASK_128KB     0x000FFFE0
#define PICMR_MASK_256KB     0x000FFFC0
#define PICMR_MASK_512KB     0x000FFF80
#define PICMR_MASK_1MB       0x000FFF00
#define PICMR_MASK_2MB       0x000FFE00
#define PICMR_MASK_4MB       0x000FFC00
#define PICMR_MASK_8MB       0x000FF800
#define PICMR_MASK_16MB      0x000FF000
#define PICMR_MASK_32MB      0x000FE000
#define PICMR_MASK_64MB      0x000FC000
#define PICMR_MASK_128MB     0x000F8000
#define PICMR_MASK_256MB     0x000F0000
#define PICMR_MASK_512MB     0x000E0000
#define PICMR_MASK_1GB       0x000C0000

#define PICMR_ENABLE         0x80000000
#define PICMR_NO_SNOOP_EN    0x40000000
#define PICMR_PREFETCH_EN    0x20000000

/* PCI error Registers */

#define	PCI_ERROR_STATUS_REG		0x10884
#define	PCI_ERROR_MASK_REG		0x10888
#define	PCI_ERROR_CONTROL_REG		0x1088C
#define PCI_ERROR_ADRS_CAPTURE_REG      0x10890
#define PCI_ERROR_DATA_CAPTURE_REG      0x10898
#define PCI_ERROR_CTRL_CAPTURE_REG      0x108A0

/* PCI error Register bit defines */

#define	PCI_ERROR_PCI_ADDR_PAR			0x00000001
#define	PCI_ERROR_PCI_DATA_PAR_WR		0x00000002
#define	PCI_ERROR_PCI_DATA_PAR_RD		0x00000004
#define	PCI_ERROR_PCI_NO_RSP			0x00000008
#define	PCI_ERROR_PCI_TAR_ABT			0x00000010
#define	PCI_ERROR_PCI_SERR			0x00000020
#define	PCI_ERROR_PCI_PERR_RD			0x00000040
#define	PCI_ERROR_PCI_PERR_WR			0x00000080
#define	PCI_ERROR_I2O_OFQO			0x00000100
#define	PCI_ERROR_I2O_IPQO			0x00000200
#define	PCI_ERROR_IRA				0x00000400
#define	PCI_ERROR_NMI				0x00000800
#define	PCI_ERROR_I2O_DBMC			0x00001000

/*
 * Register pair used to generate configuration cycles on the PCI bus
 * and access the MPC826x's own PCI configuration registers.
 */

#define PCI_CFG_ADDR_REG     0x10900
#define PCI_CFG_DATA_REG     0x10904

/* Bus parking decides where the bus control sits when idle */
/* If modifying memory controllers for PCI park on the core */

#define PPC_ACR_BUS_PARK_CORE 0x6
#define PPC_ACR_BUS_PARK_PCI  0x3

#endif /* _PPC_KERNEL_M8260_PCI_H */
