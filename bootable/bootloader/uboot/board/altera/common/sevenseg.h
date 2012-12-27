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
 * (C) Copyright 2003, Li-Pro.Net <www.li-pro.net>
 * Stephan Linz <linz@li-pro.net>
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
 *
 * common/sevenseg.h
 *
 * NIOS PIO based seven segment led support functions
 */

#ifndef __DK1S10_SEVENSEG_H__
#define __DK1S10_SEVENSEG_H__

#ifdef	CONFIG_SEVENSEG

/*
 *  15                    8 7      0
 * |-----------------------|--------|
 * |   controll value      |  value |
 * ----------------------------------
 */
#define	SEVENSEG_RAW		(int)(0)	/* write out byte value (hex) */
#define	SEVENSEG_OFF		(int)( 1 << 8)	/* display switch off */
#define	SEVENSEG_SET_DPL	(int)( 2 << 8)	/* set dp low  nibble */
#define	SEVENSEG_SET_DPH	(int)( 3 << 8)	/* set dp high nibble */
#define	SEVENSEG_RES_DPL	(int)( 4 << 8)	/* reset dp low  nibble */
#define	SEVENSEG_RES_DPH	(int)( 5 << 8)	/* reset dp high nibble */
#define	SEVENSEG_TOG_DPL	(int)( 6 << 8)	/* toggle dp low  nibble */
#define	SEVENSEG_TOG_DPH	(int)( 7 << 8)	/* toggle dp high nibble */
#define	SEVENSEG_LO		(int)( 8 << 8)	/* write out low nibble only */
#define	SEVENSEG_HI		(int)( 9 << 8)	/* write out high nibble only */
#define	SEVENSEG_STR		(int)(10 << 8)	/* write out a string */

#define	SEVENSEG_MASK_VAL	(0xff)		/* only used by SEVENSEG_RAW */
#define	SEVENSEG_MASK_CTRL	(~SEVENSEG_MASK_VAL)

#ifdef	SEVENSEG_DIGIT_HI_LO_EQUAL

#define	SEVENSEG_DIGITS_0	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_F	)
#define	SEVENSEG_DIGITS_1	(	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	)
#define	SEVENSEG_DIGITS_2	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_3	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_4	(	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_5	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_6	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_7	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	)
#define	SEVENSEG_DIGITS_8	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_9	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_A	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_B	(	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_C	(	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_D	(	SEVENSEG_DIGIT_B	\
				|	SEVENSEG_DIGIT_C	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_E	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_D	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)
#define	SEVENSEG_DIGITS_F	(	SEVENSEG_DIGIT_A	\
				|	SEVENSEG_DIGIT_E	\
				|	SEVENSEG_DIGIT_F	\
				|	SEVENSEG_DIGIT_G	)

#else	/* !SEVENSEG_DIGIT_HI_LO_EQUAL */
#error SEVENSEG: different pin asssignments not supported
#endif

void sevenseg_set(int value);

#endif	/* CONFIG_SEVENSEG */

#endif	/* __DK1S10_SEVENSEG_H__ */
