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
 *  Copyright 2004-2009 Analog Devices Inc.
 *                 2001 Lineo, Inc
 *                        Tony Kou
 *                 1993 Hamish Macdonald
 *
 * Licensed under the GPL-2
 */

#ifndef _BFIN_TRAPS_H
#define _BFIN_TRAPS_H

#define VEC_SYS		(0)
#define VEC_EXCPT01	(1)
#define VEC_EXCPT02	(2)
#define VEC_EXCPT03	(3)
#define VEC_EXCPT04	(4)
#define VEC_EXCPT05	(5)
#define VEC_EXCPT06	(6)
#define VEC_EXCPT07	(7)
#define VEC_EXCPT08	(8)
#define VEC_EXCPT09	(9)
#define VEC_EXCPT10	(10)
#define VEC_EXCPT11	(11)
#define VEC_EXCPT12	(12)
#define VEC_EXCPT13	(13)
#define VEC_EXCPT14	(14)
#define VEC_EXCPT15	(15)
#define VEC_STEP	(16)
#define VEC_OVFLOW	(17)
#define VEC_UNDEF_I	(33)
#define VEC_ILGAL_I	(34)
#define VEC_CPLB_VL	(35)
#define VEC_MISALI_D	(36)
#define VEC_UNCOV	(37)
#define VEC_CPLB_M	(38)
#define VEC_CPLB_MHIT	(39)
#define VEC_WATCH	(40)
#define VEC_ISTRU_VL	(41)	/*ADSP-BF535 only (MH) */
#define VEC_MISALI_I	(42)
#define VEC_CPLB_I_VL	(43)
#define VEC_CPLB_I_M	(44)
#define VEC_CPLB_I_MHIT	(45)
#define VEC_ILL_RES	(46)	/* including unvalid supervisor mode insn */
/* The hardware reserves (63) for future use - we use it to tell our
 * normal exception handling code we have a hardware error
 */
#define VEC_HWERR	(63)

#endif				/* _BFIN_TRAPS_H */
