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
 * (C) Copyright 2005
 * Martin Krause TQ-Systems GmbH martin.krause@tqs.de
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

/*
 * Basic video support for SMI SM501 "Voyager" graphic controller
 */

#include <common.h>

#include <video_fb.h>
#include <sm501.h>

#define read8(ptrReg)                \
    *(volatile unsigned char *)(sm501.isaBase + ptrReg)

#define write8(ptrReg,value) \
    *(volatile unsigned char *)(sm501.isaBase + ptrReg) = value

#define read16(ptrReg) \
    (*(volatile unsigned short *)(sm501.isaBase + ptrReg))

#define write16(ptrReg,value) \
    (*(volatile unsigned short *)(sm501.isaBase + ptrReg) = value)

#define read32(ptrReg) \
    (*(volatile unsigned int *)(sm501.isaBase + ptrReg))

#define write32(ptrReg, value) \
    (*(volatile unsigned int *)(sm501.isaBase + ptrReg) = value)

GraphicDevice sm501;

/*-----------------------------------------------------------------------------
 * SmiSetRegs --
 *-----------------------------------------------------------------------------
 */
static void SmiSetRegs (void)
{
	/*
	 * The content of the chipset register depends on the board (clocks,
	 * ...)
	 */
	const SMI_REGS *preg = board_get_regs ();
	while (preg->Index) {
		write32 (preg->Index, preg->Value);
		/*
		 * Insert a delay between
		 */
		udelay (1000);
		preg ++;
	}
}

/*-----------------------------------------------------------------------------
 * video_hw_init --
 *-----------------------------------------------------------------------------
 */
void *video_hw_init (void)
{
	unsigned int *vm, i;

	memset (&sm501, 0, sizeof (GraphicDevice));

	/*
	 * Initialization of the access to the graphic chipset Retreive base
	 * address of the chipset (see board/RPXClassic/eccx.c)
	 */
	if ((sm501.isaBase = board_video_init ()) == 0) {
		return (NULL);
	}

	if ((sm501.frameAdrs = board_video_get_fb ()) == 0) {
		return (NULL);
	}

	sm501.winSizeX = board_get_width ();
	sm501.winSizeY = board_get_height ();

#if defined(CONFIG_VIDEO_SM501_8BPP)
	sm501.gdfIndex = GDF__8BIT_INDEX;
	sm501.gdfBytesPP = 1;

#elif defined(CONFIG_VIDEO_SM501_16BPP)
	sm501.gdfIndex = GDF_16BIT_565RGB;
	sm501.gdfBytesPP = 2;

#elif defined(CONFIG_VIDEO_SM501_32BPP)
	sm501.gdfIndex = GDF_32BIT_X888RGB;
	sm501.gdfBytesPP = 4;
#else
#error Unsupported SM501 BPP
#endif

	sm501.memSize = sm501.winSizeX * sm501.winSizeY * sm501.gdfBytesPP;

	/* Load Smi registers */
	SmiSetRegs ();

	/* (see board/RPXClassic/RPXClassic.c) */
	board_validate_screen (sm501.isaBase);

	/* Clear video memory */
	i = sm501.memSize/4;
	vm = (unsigned int *)sm501.frameAdrs;
	while(i--)
		*vm++ = 0;

	return (&sm501);
}

/*-----------------------------------------------------------------------------
 * video_set_lut --
 *-----------------------------------------------------------------------------
 */
void video_set_lut (
	unsigned int index,           /* color number */
	unsigned char r,              /* red */
	unsigned char g,              /* green */
	unsigned char b               /* blue */
	)
{
}
