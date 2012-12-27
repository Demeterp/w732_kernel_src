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

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include "partition_define.h"


/*=======================================================================*/
/* NAND PARTITION Mapping                                                  */
/*=======================================================================*/
//#ifdef CONFIG_MTD_PARTITIONS

static struct mtd_partition g_pasStatic_Partition[] = {
    { 
        .name   = "preloader",
        .offset = 0x0,
        .size   = PART_SIZE_PRELOADER,
        .mask_flags = MTD_WRITEABLE,
    },
    { 
	.name   = "dsp_bl",
	.offset = MTDPART_OFS_APPEND,
	.size   = PART_SIZE_DSP_BL,
	.mask_flags = MTD_WRITEABLE,
    },
    { 
        .name   = "nvram",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_NVRAM,
    },
    { 
        .name   = "seccnfg",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_SECCFG,
        //.mask_flags = MTD_WRITEABLE,        
    },          
    { 
        .name   = "uboot",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_UBOOT,
        .mask_flags = MTD_WRITEABLE,
    },     
    { 
        .name   = "boot",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_BOOTIMG,        
    },
    { 
        .name   = "recovery",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_RECOVERY,
    },
    { 
        .name   = "secstatic",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_SEC_RO,
    },      
    { 
        .name   = "misc",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_MISC,
    },
    { 
        .name   = "logo",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_LOGO,
        //.mask_flags = MTD_WRITEABLE,
    },    
    { 
        .name   = "expdb",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_EXPDB,
    },   
    { 
        .name   = "system",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_ANDROID,
    },
    { 
        .name   = "cache",
        .offset = MTDPART_OFS_APPEND,
        .size   = PART_SIZE_CACHE,
    },     
    { 
        .name   = "userdata",
        .offset = MTDPART_OFS_APPEND,
        .size   = MTDPART_SIZ_FULL,
    },   
};

#define NUM_PARTITIONS ARRAY_SIZE(g_pasStatic_Partition)
extern int part_num;	// = NUM_PARTITIONS;
//#endif
