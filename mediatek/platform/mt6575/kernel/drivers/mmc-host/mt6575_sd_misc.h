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

#ifndef MT6575_SD_MISC_H
#define MT6575_SD_MISC_H

#include <linux/bitops.h>
#include <linux/mmc/host.h>

#include <mach/mt6575_reg_base.h>


struct msdc_ioctl{
	int  opcode;
	int  host_num;
	int  iswrite;
	int  trans_type;
	u32  total_size;
	u32  address;
	u32* buffer;
	int  cmd_pu_driving;
	int  cmd_pd_driving;
	int  dat_pu_driving;
	int  dat_pd_driving;
	int  clk_pu_driving;
	int  clk_pd_driving;
	int  clock_freq;
    int  partition;
	int  result;
};


#define MSDC_DRIVING_SETTING              (0)
#define MSDC_CLOCK_FREQUENCY              (1)
#define MSDC_SINGLE_READ_WRITE            (2)
#define MSDC_MULTIPLE_READ_WRITE          (3)
#define MSDC_GET_CID                      (4)
#define MSDC_GET_CSD                      (5)
#define MSDC_GET_EXCSD                    (6)
#define MSDC_ERASE_PARTITION              (7)
#ifdef MTK_SD_REINIT_SUPPORT
#define MSDC_REINIT_SDCARD            _IOW('r', 9, int)
#endif
#define MSDC_CARD_DUNM_FUNC               (0xff)

typedef enum {
    USER_PARTITION = 0,
    BOOT_PARTITION_1,
    BOOT_PARTITION_2,
    RPMB_PARTITION,
    GP_PARTITION_1,
    GP_PARTITION_2,
    GP_PARTITION_3,
    GP_PARTITION_4,
}PARTITON_ACCESS_T;


#endif /* end of MT6575_SD_MISC_H */
