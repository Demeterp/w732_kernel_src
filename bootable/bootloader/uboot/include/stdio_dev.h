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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _STDIO_DEV_H_
#define _STDIO_DEV_H_

#include <linux/list.h>

/*
 * STDIO DEVICES
 */

#define DEV_FLAGS_INPUT	 0x00000001	/* Device can be used as input	console */
#define DEV_FLAGS_OUTPUT 0x00000002	/* Device can be used as output console */
#define DEV_FLAGS_SYSTEM 0x80000000	/* Device is a system device		*/
#define DEV_EXT_VIDEO	 0x00000001	/* Video extensions supported		*/

/* Device information */
struct stdio_dev {
	int	flags;			/* Device flags: input/output/system	*/
	int	ext;			/* Supported extensions			*/
	char	name[16];		/* Device name				*/

/* GENERAL functions */

	int (*start) (void);		/* To start the device			*/
	int (*stop) (void);		/* To stop the device			*/

/* OUTPUT functions */

	void (*putc) (const char c);	/* To put a char			*/
	void (*puts) (const char *s);	/* To put a string (accelerator)	*/

/* INPUT functions */

	int (*tstc) (void);		/* To test if a char is ready...	*/
	int (*getc) (void);		/* To get that char			*/

/* Other functions */

	void *priv;			/* Private extensions			*/
	struct list_head list;
};

/*
 * VIDEO EXTENSIONS
 */
#define VIDEO_FORMAT_RGB_INDEXED	0x0000
#define VIDEO_FORMAT_RGB_DIRECTCOLOR	0x0001
#define VIDEO_FORMAT_YUYV_4_4_4		0x0010
#define VIDEO_FORMAT_YUYV_4_2_2		0x0011

typedef struct {
	void *address;			/* Address of framebuffer		*/
	ushort	width;			/* Horizontal resolution		*/
	ushort	height;			/* Vertical resolution			*/
	uchar	format;			/* Format				*/
	uchar	colors;			/* Colors number or color depth		*/
	void (*setcolreg) (int, int, int, int);
	void (*getcolreg) (int, void *);
} video_ext_t;

/*
 * VARIABLES
 */
extern struct stdio_dev *stdio_devices[];
extern char *stdio_names[MAX_FILES];

/*
 * PROTOTYPES
 */
int	stdio_register (struct stdio_dev * dev);
int	stdio_init (void);
void	stdio_print_current_devices(void);
#ifdef CONFIG_SYS_STDIO_DEREGISTER
int	stdio_deregister(char *devname);
#endif
struct list_head* stdio_get_list(void);
struct stdio_dev* stdio_get_by_name(char* name);
struct stdio_dev* stdio_clone(struct stdio_dev *dev);

#ifdef CONFIG_ARM_DCC_MULTI
int drv_arm_dcc_init(void);
#endif
#ifdef CONFIG_LCD
int	drv_lcd_init (void);
#endif
#ifdef CONFIG_VFD
int	drv_vfd_init (void);
#endif
#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)
int	drv_video_init (void);
#endif
#ifdef CONFIG_KEYBOARD
int	drv_keyboard_init (void);
#endif
#ifdef CONFIG_USB_TTY
int	drv_usbtty_init (void);
#endif
#ifdef CONFIG_NETCONSOLE
int	drv_nc_init (void);
#endif
#ifdef CONFIG_JTAG_CONSOLE
int drv_jtag_console_init (void);
#endif

#endif
