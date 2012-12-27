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
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

/*--------------------------------------------------------------------------
 *
 * Motorola S-Record Format:
 *
 * Motorola S-Records are an industry-standard format for
 * transmitting binary files to target systems and PROM
 * programmers. LSI Logic have extended this standard to include
 * an S4-record containing an address and a symbol.
 *
 * The extended S-record standard is as follows:
 *
 * S<type><length><address><data....><checksum>
 * S4<length><address><name>,<checksum>
 *
 * Where:
 *
 * type
 *     is the record type. Where:
 *
 *     0  starting record (optional)
 *     1  data record with 16-bit address
 *     2  data record with 24-bit address
 *     3  data record with 32-bit address
 *     4  symbol record (LSI extension)
 *     5  number of data records in preceeding block
 *     6  unused
 *     7  ending record for S3 records
 *     8  ending record for S2 records
 *     9  ending record for S1 records
 *
 * length
 *     is two hex characters. This defines the length of the
 *     record in bytes (not characters). It includes the address
 *     field, the data field, and the checksum field.
 *
 * address
 *     is 4, 6, or 8 characters. Corresponding to a 16-, 24-, or
 *     32-bit address. The address field for S4 records is
 *     always 32 bits.
 *
 * data
 *
 *     Are the data bytes. Each pair of hex characters represent
 *     one byte in memory.
 *
 * name
 *     Is the symbol name. The symbol is terminated by a ','.
 *
 * checksum
 *     Is the one's complement of the 8-bit checksum.
 *
 * Example
 *
 * S0030000FC
 * .
 * .
 * S325000004403C0880018D08DD900000000011000026000000003C0880012508DC50C50000B401
 * S32500000460C50100B8C50200BCC50300C0C50400C4C50500C8C50600CCC50700D0C50800D4FA
 * S32500000480C50900D8C50A00DCC50B00E0C50C00E4C50D00E8C50E00ECC50F00F0C51000F49A
 * S325000004A0C51100F8C51200FCC5130100C5140104C5150108C516010CC5170110C518011434
 * .
 * .
 * S70500000000FA
 *
 * The S0 record starts the file. The S3 records contain the
 * data. The S7 record contains the entry address and terminates
 * the download.
 *
 *--------------------------------------------------------------------------
 */

#define SREC_START	0	/* Start Record (module name)		    */
#define SREC_DATA2	1	/* Data  Record with 2 byte address	    */
#define SREC_DATA3	2	/* Data  Record with 3 byte address	    */
#define SREC_DATA4	3	/* Data  Record with 4 byte address	    */
#define SREC_COUNT	5	/* Count Record (previously transmitted)    */
#define SREC_END4	7	/* End   Record with 4 byte start address   */
#define SREC_END3	8	/* End   Record with 3 byte start address   */
#define SREC_END2	9	/* End   Record with 2 byte start address   */
#define SREC_EMPTY	10	/* Empty Record without any data	    */

#define SREC_REC_OK  SREC_EMPTY /* last code without error condition	    */

#define SREC_E_BADTYPE	-1	/* no valid S-Record		            */
#define SREC_E_NOSREC	-2	/* line format differs from s-record	    */
#define SREC_E_BADCHKS	-3	/* checksum error in an s-record line	    */

#define SREC_MAXRECLEN	(512 + 4)   /* max ASCII record length		    */
#define SREC_MAXBINLEN	255	    /* resulting binary length		    */

int srec_decode (char *input, int *count, ulong *addr, char *data);
