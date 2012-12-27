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
 * Copyright (c) 2009 Wind River Systems, Inc.
 * Tom Rix <Tom.Rix@windriver.com>
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
 *
 */

#ifndef ZOOM2_SERIAL_H
#define ZOOM2_SERIAL_H

extern int zoom2_debug_board_connected (void);

#define SERIAL_TL16CP754C_BASE	0x10000000	/* Zoom2 Serial chip address */

#define QUAD_BASE_0	SERIAL_TL16CP754C_BASE
#define QUAD_BASE_1	(SERIAL_TL16CP754C_BASE + 0x100)
#define QUAD_BASE_2	(SERIAL_TL16CP754C_BASE + 0x200)
#define QUAD_BASE_3	(SERIAL_TL16CP754C_BASE + 0x300)

#define S(a) #a
#define N(a) S(quad##a)
#define U(a) S(UART##a)

#define QUAD_INIT(n)				\
int quad_init_##n(void)				\
{						\
	return quad_init_dev(QUAD_BASE_##n);	\
}						\
void quad_setbrg_##n(void)			\
{						\
	quad_setbrg_dev(QUAD_BASE_##n);		\
}						\
void quad_putc_##n(const char c)		\
{						\
	quad_putc_dev(QUAD_BASE_##n, c);	\
}						\
void quad_puts_##n(const char *s)		\
{						\
	quad_puts_dev(QUAD_BASE_##n, s);	\
}						\
int quad_getc_##n(void)				\
{						\
	return quad_getc_dev(QUAD_BASE_##n);	\
}						\
int quad_tstc_##n(void)				\
{						\
	return quad_tstc_dev(QUAD_BASE_##n);	\
}						\
struct serial_device zoom2_serial_device##n =	\
{						\
	N(n),					\
	U(n),					\
	quad_init_##n,				\
	NULL,					\
	quad_setbrg_##n,			\
	quad_getc_##n,				\
	quad_tstc_##n,				\
	quad_putc_##n,				\
	quad_puts_##n,				\
};

#endif /* ZOOM2_SERIAL_H */
