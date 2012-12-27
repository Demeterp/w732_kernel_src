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
 * Copyright 2007 Freescale Semiconductor, Inc.
 * York Sun <yorksun@freescale.com>
 *
 * FSL DIU Framebuffer driver
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

struct fb_var_screeninfo {
	unsigned int xres;		/* visible resolution		*/
	unsigned int yres;

	unsigned int bits_per_pixel;	/* guess what			*/

	/* Timing: All values in pixclocks, except pixclock (of course) */
	unsigned int pixclock;		/* pixel clock in ps (pico seconds) */
	unsigned int left_margin;	/* time from sync to picture	*/
	unsigned int right_margin;	/* time from picture to sync	*/
	unsigned int upper_margin;	/* time from sync to picture	*/
	unsigned int lower_margin;
	unsigned int hsync_len;		/* length of horizontal sync	*/
	unsigned int vsync_len;		/* length of vertical sync	*/
	unsigned int sync;		/* see FB_SYNC_*		*/
	unsigned int vmode;		/* see FB_VMODE_*		*/
	unsigned int rotate;		/* angle we rotate counter clockwise */
};

struct fb_info {
	struct fb_var_screeninfo var;	/* Current var */
	unsigned long smem_start;	/* Start of frame buffer mem */
					/* (physical address) */
	unsigned int smem_len;		/* Length of frame buffer mem */
	unsigned int type;		/* see FB_TYPE_*		*/
	unsigned int line_length;	/* length of a line in bytes    */

	char *screen_base;
	unsigned long screen_size;
	int logo_height;
	unsigned int logo_size;
};


extern char *fsl_fb_open(struct fb_info **info);
extern int fsl_diu_init(int xres,
			unsigned int pixel_format,
			int gamma_fix,
			unsigned char *splash_bmp);
extern void fsl_diu_clear_screen(void);
extern int fsl_diu_display_bmp(unsigned char *bmp,
			       int xoffset,
			       int yoffset,
			       int transpar);
