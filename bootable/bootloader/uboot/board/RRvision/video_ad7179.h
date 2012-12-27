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
 * (C) Copyright 2003 Wolfgang Grandegger <wg@denx.de>
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

#define VIDEO_ENCODER_NAME	"Analog Devices AD7179"

#define VIDEO_ENCODER_I2C_RATE	100000	/* Max rate is 100Khz	*/
#define VIDEO_ENCODER_CB_Y_CR_Y		/* Use CB Y CR Y format...	*/

#define VIDEO_MODE_YUYV		/* The only mode supported by this encoder	*/
#undef	VIDEO_MODE_RGB
#define VIDEO_MODE_BPP		16

#ifdef	VIDEO_MODE_PAL
#define VIDEO_ACTIVE_COLS	720
#define VIDEO_ACTIVE_ROWS	576
#define VIDEO_VISIBLE_COLS	640
#define VIDEO_VISIBLE_ROWS	480
#else
#error "NTSC mode is not supported"
#endif

static unsigned char video_encoder_data[] = {
				0x05, /* Mode Register 0 */
				0x11, /* Mode Register 1 */
				0x20, /* Mode Register 2 */
				0x0C, /* Mode Register 3 */
				0x01, /* Mode Register 4 */
				0x00, /* Reserved */
				0x00, /* Reserved */
				0x04, /* Timing Register 0 */
				0x00, /* Timing Register 1 */
				0xCB, /* Subcarrier Frequency Register 0 */
				0x0A, /* Subcarrier Frequency Register 1 */
				0x09, /* Subcarrier Frequency Register 2 */
				0x2A, /* Subcarrier Frequency Register 3 */
				0x00, /* Subcarrier Phase */
				0x00, /* Closed Captioning Ext Reg 0 */
				0x00, /* Closed Captioning Ext Reg 1 */
				0x00, /* Closed Captioning Reg 0 */
				0x00, /* Closed Captioning Reg 1 */
				0x00, /* Pedestal Control Reg 0 */
				0x00, /* Pedestal Control Reg 1 */
				0x00, /* Pedestal Control Reg 2 */
				0x00, /* Pedestal Control Reg 3 */
				0x00, /* CGMS_WSS Reg 0 */
				0x00, /* CGMS_WSS Reg 0 */
				0x00, /* CGMS_WSS Reg 0 */
				0x00  /* Teletext Req. Control Reg */
} ;
