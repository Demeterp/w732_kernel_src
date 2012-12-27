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
 * Daniel Engström, Omicron Ceti AB <daniel@omicron.se>.
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

/* now setup the General purpose bus to give us access to the LEDs.
 * We can then use the leds to display status information.
 */

sc520_cdp_registers:
/* size    offset        value */
.word 1 ; .word 0x040 ; .long 0x00       /* SDRAM buffer control */
.word 2 ; .word 0xc08 ; .long 0x0001     /* GP CS offset */
.word 2 ; .word 0xc09 ; .long 0x0003     /* GP CS width */
.word 2 ; .word 0xc0a ; .long 0x0001     /* GP CS width */
.word 2 ; .word 0xc0b ; .long 0x0003     /* GP RD pulse width */
.word 2 ; .word 0xc0c ; .long 0x0001     /* GP RD offse */
.word 2 ; .word 0xc0d ; .long 0x0003     /* GP WR pulse width */
.word 2 ; .word 0xc0e ; .long 0x0001     /* GP WR offset */
.word 2 ; .word 0xc2c ; .long 0x003f     /* GPIO directionreg 31-16 */
.word 2 ; .word 0xc2a ; .long 0xe000     /* GPIO directionreg 15-0 */
.word 2 ; .word 0xc22 ; .long 0xffc0     /* GPIO pin function 31-16 reg */
.word 2 ; .word 0xc20 ; .long 0x1fff     /* GPIO pin function 15-0 reg */
.word 0 ; .word 0x000 ; .long 0x00

/* board early intialization */
.globl early_board_init
early_board_init:
	movl    $sc520_cdp_registers,%esi
init_loop:
	movl    $0xfffef000,%edi     /* MMCR base to edi */
	movw	(%esi), %bx          /* load size to bx */
	cmpw	$0, %bx              /* if size is 0 we're done */
	je	done
	xorl	%edx,%edx
	movw	2(%esi), %dx        /* load MMCR offset to dx */
	addl	%edx, %edi          /* add offset to base in edi */
	movl	4(%esi), %eax       /* load value in eax */
	cmpw	$1, %bx
	je	byte                 /* byte op? */
	cmpw	$2, %bx
	je	word                 /* word op? */
	movl	%eax, (%edi)         /* must be long, then */
	jmp	next
byte:   movb    %al,(%edi)
	jmp	next
word:	movw	%ax,(%edi)
next:	addl	$8, %esi             /* advance esi */
	jmp	init_loop

	/* light all leds */
done:   movl    $0xfffefc32,%edx
	movw    $0000,(%edx)

	jmp	*%ebp		     /* return to caller */


.globl show_boot_progress_asm
show_boot_progress_asm:
	movl    $0xfffefc32,%edx
	xorw    $0xffff, %ax
	movw    %ax,(%edx)
	jmp	*%ebp
