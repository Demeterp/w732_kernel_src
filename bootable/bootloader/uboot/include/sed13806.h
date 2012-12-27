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
 * (C) Copyright 2002
 * Stäubli Faverges - <www.staubli.com>
 * Pierre AUBERT  p.aubert@staubli.com
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
/* Video support for Epson SED13806 chipset                                  */


#ifndef _SED13806_H_
#define _SED13806_H_


/* General definitions                                                       */
#define FRAME_BUFFER_OFFSET        0x200000     /* Frame buffer offset */
#define TOTAL_SPACE_SIZE           0x400000

#define DEFAULT_VIDEO_MEMORY_SIZE  0x140000     /* Video Memory Size */

#define HWCURSORSIZE		   1024     /* Size of memory reserved
						   for HW cursor*/

/* Offset of chipset registers                                               */
#define	BLT_CTRL0	(0x0100)
#define	BLT_CTRL1	(0x0101)
#define BLT_ROP		(0x0102)
#define	BLT_OP		(0x0103)
#define BLT_SRC_ADDR0	(0x0104)
#define	BLT_SRC_ADDR1	(0x0105)
#define	BLT_SRC_ADDR2	(0x0106)
#define	BLT_DST_ADDR0	(0x0108)
#define BLT_DST_ADDR1	(0x0109)
#define	BLT_DST_ADDR2	(0x010A)
#define BLT_MEM_OFF0	(0x010C)
#define BLT_MEM_OFF1	(0x010D)
#define BLT_WIDTH0	(0x0110)
#define BLT_WIDTH1	(0x0111)
#define BLT_HEIGHT0	(0x0112)
#define BLT_HEIGHT1	(0x0113)
#define	BLT_BGCOLOR0	(0x0114)
#define	BLT_BGCOLOR1	(0x0115)
#define	BLT_FGCOLOR0	(0x0118)
#define BLT_FGCOLOR1	(0x0119)

#define BLT_REG         (0x100000)

/* Lookup table registers                                                    */
#define REG_LUT_ADDR 0x1e2
#define REG_LUT_DATA 0x1e4

/* Cursor/Ink registers                                                      */
#define LCD_CURSOR_CNTL         (0x0070)
#define LCD_CURSOR_START        (0x0071)
#define LCD_CURSOR_XL           (0x0072)
#define LCD_CURSOR_XM           (0x0073)
#define LCD_CURSOR_YL           (0x0074)
#define LCD_CURSOR_YM           (0x0075)
#define LCD_CURSOR_COL0_B       (0x0076)
#define LCD_CURSOR_COL0_G       (0x0077)
#define LCD_CURSOR_COL0_R       (0x0078)
#define LCD_CURSOR_COL1_B       (0x007A)
#define LCD_CURSOR_COL1_G       (0x007B)
#define LCD_CURSOR_COL1_R       (0x007C)
#define LCD_CURSOR_FIFO         (0x007E)

typedef struct
{
    unsigned short      Index;
    unsigned char       Value;
} S1D_REGS;


/* Board specific functions                                                  */
unsigned int board_video_init (void);
void board_validate_screen (unsigned int base);
const S1D_REGS *board_get_regs (void);
int board_get_width (void);
int board_get_height (void);

#endif /* _SED13806_H_ */
