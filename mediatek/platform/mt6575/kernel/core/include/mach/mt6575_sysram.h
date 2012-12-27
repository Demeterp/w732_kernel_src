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

#ifndef _MT6575SYSRAM_H
#define _MT6575SYSRAM_H

#include <linux/ioctl.h>

#define SYSRAM_DEV_NAME         "mt6575-SYSRAM"
#define MT6575_SYSRAMMAGICNO    'p'

typedef enum {
    ESysramUser_Begin   = 0,
    ESysramUser_AUDIO   = ESysramUser_Begin, 
    //ESysramUser_DESCRIPTOR, 
    ESysramUser_DESC_VDO_ROT0,
    ESysramUser_DESC_VDO_ROT1,
    ESysramUser_DESC_RGB_ROT0,
    ESysramUser_DESC_RGB_ROT1,
    ESysramUser_DESC_RGB_ROT2,
    ESysramUser_DESC_RDMA0,
    ESysramUser_DESC_RDMA1,
    //ESysramUser_DESC_OVL,
    ESysramUser_FLICKER, 
    ESysramUser_SHADING, 
    ESysramUser_DEFECT, 
    ESysramUser_LCE0,
    ESysramUser_LCE1,
    ESysramUser_FD, 
    ESysramUser_PCA, 
    ESysramUser_EIS, 
    ESysramUser_VIDEO, 
    ESysramUser_TVOUT, 
    ESysramUser_TV_ROT, 
    ESysramUser_JPEG_DMA, 
    ESysramUser_JPEG_CODEC, 
    ESysramUser_BRZ, 
    ESysramUser_VRZ0, 
    ESysramUser_VRZ1, 
    ESysramUser_RGB_ROT0, 
    ESysramUser_RGB_ROT1, 
    ESysramUser_RGB_ROT2, 
    ESysramUser_VDO_ROT0, 
    ESysramUser_VDO_ROT1, 
    ESysramUser_VDO_ROT1_SUB,
    ESysramUser_RDMA0, 
    //ESysramUser_RDMA1, 
    ESysramUser_CNT, 
    ESysramUser_None    = ((unsigned int)-1)
} ESysramUser_T;

//IOCTRL(inode * ,file * ,cmd * ,arg * )
typedef struct {
    unsigned long u4Alignment;  // In : alignment in bytes
    unsigned long u4Size;       // In : Size in bytes
    ESysramUser_T u4Owner;      // In : Owner
    unsigned long u4Addr;       // In/Out : address
    unsigned long u4TimeoutInMS;// In : millisecond
} stSysramParam;

/*******************************************************************************
* IOCTL commnad
********************************************************************************/
//Lock User
#define SYSRAM_X_USRALLOC_TIMEOUT   _IOWR(MT6575_SYSRAMMAGICNO, 0, stSysramParam)
#define SYSRAM_X_USRALLOC           _IOWR(MT6575_SYSRAMMAGICNO, 1, stSysramParam)

//Unlock User
#define SYSRAM_S_USRFREE            _IOW(MT6575_SYSRAMMAGICNO, 2, stSysramParam)

//Dump memory layout
#define SYSRAM_T_DUMPLAYOUT         _IO(MT6575_SYSRAMMAGICNO, 3)

//
#define SYSRAM_S_SWITCH_BANK        _IOW(MT6575_SYSRAMMAGICNO, 4, bool)

unsigned long SYSRAM_TVOUT_ALLOC_TIMEOUT(unsigned long const u4Size, unsigned long u4Alignment, unsigned long const u4TimeoutInMS);
void SYSRAM_TVOUT_FREE(void);
unsigned long SYSRAM_TV_ROT_ALLOC_TIMEOUT(unsigned long const u4Size, unsigned long u4Alignment, unsigned long const u4TimeoutInMS);
void SYSRAM_TV_ROT_FREE(void);
unsigned long SYSRAM_AUDIO_ALLOC_TIMEOUT(unsigned long const u4Size, unsigned long u4Alignment, unsigned long const u4TimeoutInMS);
void SYSRAM_AUDIO_FREE(void);
unsigned long SYSRAM_VIDEO_ALLOC_TIMEOUT(unsigned long const u4Size, unsigned long u4Alignment, unsigned long const u4TimeoutInMS);
void SYSRAM_VIDEO_FREE(void);
void SYSRAM_MFG_SWITCH_BANK(bool Enable);


#endif  //_MT6575SYSRAM_H

