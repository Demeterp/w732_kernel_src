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
#ifndef INNO_SPI_H
#define INNO_SPI_H
#include "if208.h"
#define CMMB_SPI_INTERFACE_MAX_PKT_LENGTH_PER_TIMES (0x400)
#define CMMB_SPI_INTERFACE_MAX_PKT_COUNT_PER_TIMES  (0x100)

INNO_RET INNO_SPI_Init(int enable);
INNO_RET INNO_SPI_Write_One_Byte_NoCS(unsigned char data);
INNO_RET INNO_SPI_Read_One_Byte_NoCS(unsigned char *data);
INNO_RET INNO_SPI_Read_Bytes_NoCS(unsigned char *buffer, int len);

INNO_RET INNO_SPI_Write_One_Byte_NoCS(unsigned char data);
INNO_RET INNO_SPI_Read_One_Byte_NoCS(unsigned char *data); 

INNO_RET INNO_SPI_Write_cmd_rsp(unsigned char cmd,unsigned char *rsp);
INNO_RET INNO_SPI_Write_Bytes_NoCS(unsigned char *buffer, int len);

#define MTK_SPI
#ifdef MTK_SPI
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <linux/kdev_t.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/vmalloc.h>
#include <linux/device.h>
typedef enum
{
    /* Deassert mode. SPI CS pin will be pulled low and high for each byte during transmission. */
    SPI_MODE_DEASSERT,
    /* Pause mode. SPI CS pin is pulled low and keeps until specific amount of transfers have been finished. */
    SPI_MODE_PAUSE,
    /* Ultra high mode. Raise DMA priority during DMA transmission. */
    SPI_MODE_ULTRA_HIGH,
    /* Slow down mode. Slow down SPI DMA speed during DMA transmission. */
    SPI_MODE_SLOW_DOWN,
    /* Get tick delay mode. This mode is used to tuning SPI timing. */
    SPI_MODE_GET_TICK
} SPI_HAL_MODE;

typedef struct
{
    /* [IN] Specify a SPI mode. */
    SPI_HAL_MODE mode;
    /* [IN/OUT] Specify whether the mode is enabled.
       For SPI_IOCTL_SET_MODE, it is an input parameter.
       For SPI_IOCTL_GET_MODE, it is an output parameter.
    */
    bool bEnable;
    /* [IN/OUT] The parameter for the specific mode.
       The meaning of this parameter depends on the mode to be set/get.
    */
    int Param;
} SPI_MODE_T;
bool spi_mode_setting(SPI_MODE_T pSetMode);
struct INNODev_data {
	dev_t			    devt;
	spinlock_t		    spi_lock;
	struct spi_device	*spi;
	struct list_head	device_entry;

	struct mutex		buf_lock;
	unsigned char   	users;
	u8			       *tx_buf;
	u8                         *rx_buf;
};
#endif
#endif
