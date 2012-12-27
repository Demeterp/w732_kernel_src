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
 *	FILE		bitfield.h
 *
 *	Version		1.1
 *	Author		Copyright (c) Marc A. Viredaz, 1998
 *			DEC Western Research Laboratory, Palo Alto, CA
 *	Date		April 1998 (April 1997)
 *	System		Advanced RISC Machine (ARM)
 *	Language	C or ARM Assembly
 *	Purpose		Definition of macros to operate on bit fields.
 */


#ifndef __BITFIELD_H
#define __BITFIELD_H

#ifndef __ASSEMBLY__
#define UData(Data)	((unsigned long) (Data))
#else
#define UData(Data)	(Data)
#endif


/*
 * MACRO: Fld
 *
 * Purpose
 *    The macro "Fld" encodes a bit field, given its size and its shift value
 *    with respect to bit 0.
 *
 * Note
 *    A more intuitive way to encode bit fields would have been to use their
 *    mask. However, extracting size and shift value information from a bit
 *    field's mask is cumbersome and might break the assembler (255-character
 *    line-size limit).
 *
 * Input
 *    Size		Size of the bit field, in number of bits.
 *    Shft		Shift value of the bit field with respect to bit 0.
 *
 * Output
 *    Fld		Encoded bit field.
 */

#define Fld(Size, Shft)	(((Size) << 16) + (Shft))


/*
 * MACROS: FSize, FShft, FMsk, FAlnMsk, F1stBit
 *
 * Purpose
 *    The macros "FSize", "FShft", "FMsk", "FAlnMsk", and "F1stBit" return
 *    the size, shift value, mask, aligned mask, and first bit of a
 *    bit field.
 *
 * Input
 *    Field		Encoded bit field (using the macro "Fld").
 *
 * Output
 *    FSize		Size of the bit field, in number of bits.
 *    FShft		Shift value of the bit field with respect to bit 0.
 *    FMsk		Mask for the bit field.
 *    FAlnMsk		Mask for the bit field, aligned on bit 0.
 *    F1stBit		First bit of the bit field.
 */

#define FSize(Field)	((Field) >> 16)
#define FShft(Field)	((Field) & 0x0000FFFF)
#define FMsk(Field)	(((UData (1) << FSize (Field)) - 1) << FShft (Field))
#define FAlnMsk(Field)	((UData (1) << FSize (Field)) - 1)
#define F1stBit(Field)	(UData (1) << FShft (Field))


/*
 * MACRO: FInsrt
 *
 * Purpose
 *    The macro "FInsrt" inserts a value into a bit field by shifting the
 *    former appropriately.
 *
 * Input
 *    Value		Bit-field value.
 *    Field		Encoded bit field (using the macro "Fld").
 *
 * Output
 *    FInsrt		Bit-field value positioned appropriately.
 */

#define FInsrt(Value, Field) \
			(UData (Value) << FShft (Field))


/*
 * MACRO: FExtr
 *
 * Purpose
 *    The macro "FExtr" extracts the value of a bit field by masking and
 *    shifting it appropriately.
 *
 * Input
 *    Data		Data containing the bit-field to be extracted.
 *    Field		Encoded bit field (using the macro "Fld").
 *
 * Output
 *    FExtr		Bit-field value.
 */

#define FExtr(Data, Field) \
			((UData (Data) >> FShft (Field)) & FAlnMsk (Field))


#endif /* __BITFIELD_H */
