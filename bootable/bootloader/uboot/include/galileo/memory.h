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

/* Memory.h - Memory mappings and remapping functions declarations */

/* Copyright - Galileo technology. */

#ifndef __INCmemoryh
#define __INCmemoryh

/* includes */

#include "core.h"

/* defines */

#define DONT_MODIFY     0xffffffff
#define PARITY_SUPPORT  0x40000000

#define _8BIT           0x00000000
#define _16BIT          0x00100000
#define _32BIT          0x00200000
#define _64BIT          0x00300000

/* typedefs */

 typedef struct deviceParam
{                               /* boundary values  */
    unsigned int    turnOff;    /* 0x0 - 0xf        */
    unsigned int    acc2First;  /* 0x0 - 0x1f       */
    unsigned int    acc2Next;   /* 0x0 - 0x1f       */
    unsigned int    ale2Wr;     /* 0x0 - 0xf        */
    unsigned int    wrLow;      /* 0x0 - 0xf        */
    unsigned int    wrHigh;     /* 0x0 - 0xf        */
    unsigned int    deviceWidth;    /* in Bytes  */
} DEVICE_PARAM;

typedef enum __memBank{BANK0,BANK1,BANK2,BANK3} MEMORY_BANK;
typedef enum __memDevice{DEVICE0,DEVICE1,DEVICE2,DEVICE3,BOOT_DEVICE} DEVICE;

typedef enum __memoryProtectRegion{MEM_REGION0,MEM_REGION1,MEM_REGION2,       \
				   MEM_REGION3,MEM_REGION4,MEM_REGION5,       \
				   MEM_REGION6,MEM_REGION7}                   \
				   MEMORY_PROTECT_REGION;
typedef enum __memoryAccess{MEM_ACCESS_ALLOWED,MEM_ACCESS_FORBIDEN}           \
			    MEMORY_ACCESS;
typedef enum __memoryWrite{MEM_WRITE_ALLOWED,MEM_WRITE_FORBIDEN}              \
			   MEMORY_ACCESS_WRITE;
typedef enum __memoryCacheProtect{MEM_CACHE_ALLOWED,MEM_CACHE_FORBIDEN}       \
				  MEMORY_CACHE_PROTECT;
typedef enum __memorySnoopType{MEM_NO_SNOOP,MEM_SNOOP_WT,MEM_SNOOP_WB}        \
			       MEMORY_SNOOP_TYPE;
typedef enum __memorySnoopRegion{MEM_SNOOP_REGION0,MEM_SNOOP_REGION1,         \
				 MEM_SNOOP_REGION2,MEM_SNOOP_REGION3}         \
				 MEMORY_SNOOP_REGION;

/* functions */
unsigned int memoryGetBankBaseAddress(MEMORY_BANK bank);
unsigned int memoryGetDeviceBaseAddress(DEVICE device);
unsigned int memoryGetBankSize(MEMORY_BANK bank);
unsigned int memoryGetDeviceSize(DEVICE device);
unsigned int memoryGetDeviceWidth(DEVICE device);

/* when given base Address and size Set new WINDOW for SCS_X. (X = 0,1,2 or 3*/
bool memoryMapBank(MEMORY_BANK bank, unsigned int bankBase,unsigned int bankLength);
bool memoryMapDeviceSpace(DEVICE device, unsigned int deviceBase,unsigned int deviceLength);

/* Change the Internal Register Base Address to a new given Address. */
bool memoryMapInternalRegistersSpace(unsigned int internalRegBase);
/* returns internal Register Space Base Address. */
unsigned int memoryGetInternalRegistersSpace(void);
/* Configurate the protection feature to a given space. */
bool memorySetProtectRegion(MEMORY_PROTECT_REGION region,
			    MEMORY_ACCESS memoryAccess,
			    MEMORY_ACCESS_WRITE memoryWrite,
			    MEMORY_CACHE_PROTECT cacheProtection,
			    unsigned int baseAddress,
			    unsigned int regionLength);
/* Configurate the snoop feature to a given space. */
bool memorySetRegionSnoopMode(MEMORY_SNOOP_REGION region,
			      MEMORY_SNOOP_TYPE snoopType,
			      unsigned int baseAddress,
			      unsigned int regionLength);

bool memoryRemapAddress(unsigned int remapReg, unsigned int remapValue);
bool memoryGetDeviceParam(DEVICE_PARAM *deviceParam, DEVICE deviceNum);
bool memorySetDeviceParam(DEVICE_PARAM *deviceParam, DEVICE deviceNum);
#endif  /* __INCmemoryh */
