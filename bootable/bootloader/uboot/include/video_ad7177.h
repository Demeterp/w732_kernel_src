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
 * (C) Copyright 2000
 * Paolo Scaffardi, AIRVENT SAM s.p.a - RIMINI(ITALY), arsenio@tin.it
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _VIDEO_AD7177_H_
#define _VIDEO_AD7177_H_

/* #define VIDEO_DEBUG_DISABLE_COLORS	0 */

#define VIDEO_ENCODER_NAME	"Analog Devices AD7177"

#define VIDEO_ENCODER_I2C_RATE	100000	/* Max rate is 100 kHz		*/
#define VIDEO_ENCODER_CB_Y_CR_Y		/* Use CB Y CR Y format...	*/

#define VIDEO_MODE_YUYV		/* The only mode supported by this encoder */
#undef	VIDEO_MODE_RGB
#define VIDEO_MODE_BPP		16

#ifdef	VIDEO_MODE_PAL
#define VIDEO_ACTIVE_COLS	720
#define VIDEO_ACTIVE_ROWS	576
#define VIDEO_VISIBLE_COLS	640
#define VIDEO_VISIBLE_ROWS	480
#endif

#ifdef	VIDEO_MODE_NTSC
#define VIDEO_ACTIVE_COLS	720
#define VIDEO_ACTIVE_ROWS	525
#define VIDEO_VISIBLE_COLS	640
#define VIDEO_VISIBLE_ROWS	400
#endif

static unsigned char
    video_encoder_data[] = {
#ifdef VIDEO_MODE_NTSC
					0x04, /* Mode Register 0	*/
#ifdef VIDEO_DEBUG_COLORBARS
					0xc2,
#else
					0x42, /* Mode Register 1	*/
#endif	/* VIDEO_DEBUG_COLORBARS */
					0x16, /* Subcarrier Freq 0	*/
					0x7c, /* Subcarrier Freq 1	*/
					0xf0, /* Subcarrier Freq 2	*/
					0x21, /* Subcarrier Freq 3	*/
					0x00, /* Subcarrier phase	*/
					0x02, /* Timing Register 0	*/
					0x00, /* Extended Captioning 0	*/
					0x00, /* Extended Captioning 1	*/
					0x00, /* Closed Captioning 0	*/
					0x00, /* Closed Captioning 1	*/
					0x00, /* Timing Register 1	*/
					0x08, /* Mode Register 2	*/
					0x00, /* Pedestal Register 0	*/
					0x00, /* Pedestal Register 1	*/
					0x00, /* Pedestal Register 2	*/
					0x00, /* Pedestal Register 3	*/
					0x08, /* Mode Register 3	*/

#endif	/* VIDEO_MODE_NTSC */

#ifdef VIDEO_MODE_PAL
#ifdef VIDEO_MODE_RGB_OUT

					0x69, /* Mode Register 0	*/
#ifdef VIDEO_DEBUG_COLORBARS
					0xc0, /* Mode Register 1 (c0)	*/
#else
					0x40, /* Mode Register 1 (c0)	*/
#endif	/* VIDEO_DEBUG_COLORBARS */
					0xcb, /* Subcarrier Freq 0	*/
					0x8a, /* Subcarrier Freq 1	*/
					0x09, /* Subcarrier Freq 2	*/
					0x2a, /* Subcarrier Freq 3	*/
					0x00, /* Subcarrier phase	*/
					0x02, /* Timing Register 0	*/
					0x00, /* Extended Captioning 0	*/
					0x00, /* Extended Captioning 1	*/
					0x00, /* Closed Captioning 0	*/
					0x00, /* Closed Captioning 1	*/
					0x00, /* Timing Register 1	*/
					0x28, /* Mode Register 2	*/
					0x00, /* Pedestal Register 0	*/
					0x00, /* Pedestal Register 1	*/
					0x00, /* Pedestal Register 2	*/
					0x00, /* Pedestal Register 3	*/
					0x08, /* Mode Register 3	*/

#else	/* ! VIDEO_MODE_RGB_OUT */

					0x09, /* Mode Register 0 (was 01) */
#ifdef VIDEO_DEBUG_COLORBARS
					0xd8, /*			*/
#else
					0x59, /* Mode Register 1 (was 58) */
#endif	/* VIDEO_DEBUG_COLORBARS */
					0xcb, /* Subcarrier Freq 0	*/
					0x8a, /* Subcarrier Freq 1	*/
					0x09, /* Subcarrier Freq 2	*/
					0x2a, /* Subcarrier Freq 3	*/
					0x00, /* Subcarrier phase	*/
					0x02, /* Timing Register 0 (was a) */
					0x00, /* Extended Captioning 0	*/
					0x00, /* Extended Captioning 1	*/
					0x00, /* Closed Captioning 0	*/
					0x00, /* Closed Captioning 1	*/
					0x00, /* Timing Register 1	*/
#ifdef VIDEO_DEBUG_LOWPOWER
#ifdef VIDEO_DEBUG_DISABLE_COLORS
					0x98, /* Mode Register 2	*/
#else
					0x88, /* Mode Register 2	*/
#endif	/* VIDEO_DEBUG_DISABLE_COLORS */
#else	/* ! VIDEO_DEBUG_LOWPOWER */
#ifdef VIDEO_DEBUG_DISABLE_COLORS
					0x18, /* Mode Register 2	*/
#else
					0x08, /* Mode Register 2	*/
#endif	/* VIDEO_DEBUG_DISABLE_COLORS */
#endif	/* VIDEO_DEBUG_LOWPOWER */
					0x00, /* Pedestal Register 0	*/
					0x00, /* Pedestal Register 1	*/
					0x00, /* Pedestal Register 2	*/
					0x00, /* Pedestal Register 3	*/
					0x08  /* Mode Register 3	*/
#endif	/* VIDEO_MODE_RGB_OUT */
#endif	/* VIDEO_MODE_PAL */
    } ;

#endif	/* _VIDEO_AD7177_H_ */
