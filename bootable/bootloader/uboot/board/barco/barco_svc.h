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

/********************************************************************
 *
 * Unless otherwise specified, Copyright (C) 2004-2005 Barco Control Rooms
 *
 * $Source: /home/services/cvs/firmware/ppc/u-boot-1.1.2/board/barco/barco_svc.h,v $
 * $Revision: 1.2 $
 * $Author: mleeman $
 * $Date: 2005/02/21 12:48:58 $
 *
 * Last ChangeLog Entry
 * $Log: barco_svc.h,v $
 * Revision 1.2  2005/02/21 12:48:58  mleeman
 * update of copyright years (feedback wd)
 *
 * Revision 1.1  2005/02/14 09:31:07  mleeman
 * renaming of files
 *
 * Revision 1.1  2005/02/14 09:23:46  mleeman
 * - moved 'barcohydra' directory to a more generic barco; since we will be
 *   supporting and adding multiple boards
 *
 * Revision 1.1  2005/02/08 15:40:19  mleeman
 * modified and added platform files
 *
 * Revision 1.2  2005/01/25 08:05:04  mleeman
 * more cleanup of the code
 *
 * Revision 1.1  2004/07/20 08:49:55  mleeman
 * Working version of the default and nfs kernel booting.
 *
 *
 *******************************************************************/

#ifndef _LOCAL_BARCOHYDRA_H_
#define _LOCAL_BARCOHYDRA_H_

#include <flash.h>
#include <asm/io.h>

/* Defines for the barcohydra board */
#ifndef CONFIG_SYS_FLASH_ERASE_SECTOR_LENGTH
#define CONFIG_SYS_FLASH_ERASE_SECTOR_LENGTH (0x10000)
#endif

#ifndef CONFIG_SYS_DEFAULT_KERNEL_ADDRESS
#define CONFIG_SYS_DEFAULT_KERNEL_ADDRESS (CONFIG_SYS_FLASH_BASE + 0x30000)
#endif

#ifndef CONFIG_SYS_WORKING_KERNEL_ADDRESS
#define CONFIG_SYS_WORKING_KERNEL_ADDRESS (0xFFE00000)
#endif


typedef struct SBootInfo {
	unsigned int address;
	unsigned int size;
	unsigned char state;
}TSBootInfo;

/* barcohydra.c */
int checkboard(void);
phys_size_t initdram(int board_type);
void pci_init_board(void);
void check_flash(void);
int write_flash(char *addr, char value);
TSBootInfo* find_boot_info(void);
void final_boot(void);
#endif
