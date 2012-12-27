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
 * Driver for Blackfin on-chip ATAPI controller.
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Copyright (c) 2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef PATA_BFIN_H
#define PATA_BFIN_H

#include <asm/blackfin_local.h>

struct ata_ioports {
	unsigned long cmd_addr;
	unsigned long data_addr;
	unsigned long error_addr;
	unsigned long feature_addr;
	unsigned long nsect_addr;
	unsigned long lbal_addr;
	unsigned long lbam_addr;
	unsigned long lbah_addr;
	unsigned long device_addr;
	unsigned long status_addr;
	unsigned long command_addr;
	unsigned long altstatus_addr;
	unsigned long ctl_addr;
	unsigned long bmdma_addr;
	unsigned long scr_addr;
};

struct ata_port {
	unsigned int port_no;		/* primary=0, secondary=1       */
	struct ata_ioports ioaddr;	/* ATA cmd/ctl/dma reg blks     */
	unsigned long flag;
	unsigned int ata_mode;
	unsigned char ctl_reg;
	unsigned char last_ctl;
	unsigned char dev_mask;
};

extern block_dev_desc_t sata_dev_desc[CONFIG_SYS_SATA_MAX_DEVICE];

#define DRV_NAME		"pata-bfin"
#define DRV_VERSION		"0.9"
#define __iomem

#define ATA_REG_CTRL		0x0E
#define ATA_REG_ALTSTATUS	ATA_REG_CTRL
#define ATA_TMOUT_BOOT		30000
#define ATA_TMOUT_BOOT_QUICK	7000

#define PATA_BFIN_WAIT_TIMEOUT		10000
#define PATA_DEV_NUM_PER_PORT	2

/* These are the offset of the controller's registers */
#define ATAPI_OFFSET_CONTROL		0x00
#define ATAPI_OFFSET_STATUS		0x04
#define ATAPI_OFFSET_DEV_ADDR		0x08
#define ATAPI_OFFSET_DEV_TXBUF		0x0c
#define ATAPI_OFFSET_DEV_RXBUF		0x10
#define ATAPI_OFFSET_INT_MASK		0x14
#define ATAPI_OFFSET_INT_STATUS		0x18
#define ATAPI_OFFSET_XFER_LEN		0x1c
#define ATAPI_OFFSET_LINE_STATUS	0x20
#define ATAPI_OFFSET_SM_STATE		0x24
#define ATAPI_OFFSET_TERMINATE		0x28
#define ATAPI_OFFSET_PIO_TFRCNT		0x2c
#define ATAPI_OFFSET_DMA_TFRCNT		0x30
#define ATAPI_OFFSET_UMAIN_TFRCNT	0x34
#define ATAPI_OFFSET_UDMAOUT_TFRCNT	0x38
#define ATAPI_OFFSET_REG_TIM_0		0x40
#define ATAPI_OFFSET_PIO_TIM_0		0x44
#define ATAPI_OFFSET_PIO_TIM_1		0x48
#define ATAPI_OFFSET_MULTI_TIM_0	0x50
#define ATAPI_OFFSET_MULTI_TIM_1	0x54
#define ATAPI_OFFSET_MULTI_TIM_2	0x58
#define ATAPI_OFFSET_ULTRA_TIM_0	0x60
#define ATAPI_OFFSET_ULTRA_TIM_1	0x64
#define ATAPI_OFFSET_ULTRA_TIM_2	0x68
#define ATAPI_OFFSET_ULTRA_TIM_3	0x6c


#define ATAPI_GET_CONTROL(base)\
	bfin_read16(base + ATAPI_OFFSET_CONTROL)
#define ATAPI_SET_CONTROL(base, val)\
	bfin_write16(base + ATAPI_OFFSET_CONTROL, val)
#define ATAPI_GET_STATUS(base)\
	bfin_read16(base + ATAPI_OFFSET_STATUS)
#define ATAPI_GET_DEV_ADDR(base)\
	bfin_read16(base + ATAPI_OFFSET_DEV_ADDR)
#define ATAPI_SET_DEV_ADDR(base, val)\
	bfin_write16(base + ATAPI_OFFSET_DEV_ADDR, val)
#define ATAPI_GET_DEV_TXBUF(base)\
	bfin_read16(base + ATAPI_OFFSET_DEV_TXBUF)
#define ATAPI_SET_DEV_TXBUF(base, val)\
	bfin_write16(base + ATAPI_OFFSET_DEV_TXBUF, val)
#define ATAPI_GET_DEV_RXBUF(base)\
	bfin_read16(base + ATAPI_OFFSET_DEV_RXBUF)
#define ATAPI_SET_DEV_RXBUF(base, val)\
	bfin_write16(base + ATAPI_OFFSET_DEV_RXBUF, val)
#define ATAPI_GET_INT_MASK(base)\
	bfin_read16(base + ATAPI_OFFSET_INT_MASK)
#define ATAPI_SET_INT_MASK(base, val)\
	bfin_write16(base + ATAPI_OFFSET_INT_MASK, val)
#define ATAPI_GET_INT_STATUS(base)\
	bfin_read16(base + ATAPI_OFFSET_INT_STATUS)
#define ATAPI_SET_INT_STATUS(base, val)\
	bfin_write16(base + ATAPI_OFFSET_INT_STATUS, val)
#define ATAPI_GET_XFER_LEN(base)\
	bfin_read16(base + ATAPI_OFFSET_XFER_LEN)
#define ATAPI_SET_XFER_LEN(base, val)\
	bfin_write16(base + ATAPI_OFFSET_XFER_LEN, val)
#define ATAPI_GET_LINE_STATUS(base)\
	bfin_read16(base + ATAPI_OFFSET_LINE_STATUS)
#define ATAPI_GET_SM_STATE(base)\
	bfin_read16(base + ATAPI_OFFSET_SM_STATE)
#define ATAPI_GET_TERMINATE(base)\
	bfin_read16(base + ATAPI_OFFSET_TERMINATE)
#define ATAPI_SET_TERMINATE(base, val)\
	bfin_write16(base + ATAPI_OFFSET_TERMINATE, val)
#define ATAPI_GET_PIO_TFRCNT(base)\
	bfin_read16(base + ATAPI_OFFSET_PIO_TFRCNT)
#define ATAPI_GET_DMA_TFRCNT(base)\
	bfin_read16(base + ATAPI_OFFSET_DMA_TFRCNT)
#define ATAPI_GET_UMAIN_TFRCNT(base)\
	bfin_read16(base + ATAPI_OFFSET_UMAIN_TFRCNT)
#define ATAPI_GET_UDMAOUT_TFRCNT(base)\
	bfin_read16(base + ATAPI_OFFSET_UDMAOUT_TFRCNT)
#define ATAPI_GET_REG_TIM_0(base)\
	bfin_read16(base + ATAPI_OFFSET_REG_TIM_0)
#define ATAPI_SET_REG_TIM_0(base, val)\
	bfin_write16(base + ATAPI_OFFSET_REG_TIM_0, val)
#define ATAPI_GET_PIO_TIM_0(base)\
	bfin_read16(base + ATAPI_OFFSET_PIO_TIM_0)
#define ATAPI_SET_PIO_TIM_0(base, val)\
	bfin_write16(base + ATAPI_OFFSET_PIO_TIM_0, val)
#define ATAPI_GET_PIO_TIM_1(base)\
	bfin_read16(base + ATAPI_OFFSET_PIO_TIM_1)
#define ATAPI_SET_PIO_TIM_1(base, val)\
	bfin_write16(base + ATAPI_OFFSET_PIO_TIM_1, val)
#define ATAPI_GET_MULTI_TIM_0(base)\
	bfin_read16(base + ATAPI_OFFSET_MULTI_TIM_0)
#define ATAPI_SET_MULTI_TIM_0(base, val)\
	bfin_write16(base + ATAPI_OFFSET_MULTI_TIM_0, val)
#define ATAPI_GET_MULTI_TIM_1(base)\
	bfin_read16(base + ATAPI_OFFSET_MULTI_TIM_1)
#define ATAPI_SET_MULTI_TIM_1(base, val)\
	bfin_write16(base + ATAPI_OFFSET_MULTI_TIM_1, val)
#define ATAPI_GET_MULTI_TIM_2(base)\
	bfin_read16(base + ATAPI_OFFSET_MULTI_TIM_2)
#define ATAPI_SET_MULTI_TIM_2(base, val)\
	bfin_write16(base + ATAPI_OFFSET_MULTI_TIM_2, val)
#define ATAPI_GET_ULTRA_TIM_0(base)\
	bfin_read16(base + ATAPI_OFFSET_ULTRA_TIM_0)
#define ATAPI_SET_ULTRA_TIM_0(base, val)\
	bfin_write16(base + ATAPI_OFFSET_ULTRA_TIM_0, val)
#define ATAPI_GET_ULTRA_TIM_1(base)\
	bfin_read16(base + ATAPI_OFFSET_ULTRA_TIM_1)
#define ATAPI_SET_ULTRA_TIM_1(base, val)\
	bfin_write16(base + ATAPI_OFFSET_ULTRA_TIM_1, val)
#define ATAPI_GET_ULTRA_TIM_2(base)\
	bfin_read16(base + ATAPI_OFFSET_ULTRA_TIM_2)
#define ATAPI_SET_ULTRA_TIM_2(base, val)\
	bfin_write16(base + ATAPI_OFFSET_ULTRA_TIM_2, val)
#define ATAPI_GET_ULTRA_TIM_3(base)\
	bfin_read16(base + ATAPI_OFFSET_ULTRA_TIM_3)
#define ATAPI_SET_ULTRA_TIM_3(base, val)\
	bfin_write16(base + ATAPI_OFFSET_ULTRA_TIM_3, val)

#endif
