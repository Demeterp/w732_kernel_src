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
 * (C) Copyright 2003, Psyent Corporation <www.psyent.com>
 * Scott McNutt <smcnutt@psyent.com>
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

#ifndef __NIOS_H__
#define __NIOS_H__

/*------------------------------------------------------------------------
 * Control registers -- use with wrctl() & rdctl()
 *----------------------------------------------------------------------*/
#define CTL_STATUS	0x00		/* Processor status		*/
#define CTL_ISTATUS	0x01		/* Saved status (exception)	*/
#define CTL_WVALID	0x02		/* Valid window limit		*/
#define CTL_ICACHE	0x05		/* I-cache line-invalidate	*/
#define CTL_CPU_ID	0x06		/* CPU version id		*/
#define CTL_DCACHE	0x07		/* D-cache line-invalidate	*/
#define CTL_CLR_IE	0x08		/* Interrupt clear (disable)	*/
#define CTL_SET_IE	0x09		/* Interrupt set (enable)	*/

/*------------------------------------------------------------------------
 * Access to control regs
 *----------------------------------------------------------------------*/
#define _str_(s)	#s

#define rdctl(reg)\
	({unsigned int val;\
	asm volatile( "pfx " _str_(reg) "\n\t rdctl %0"\
		: "=r" (val) ); val;})

#define wrctl(reg,val)\
	asm volatile( "pfx " _str_(reg) "\n\t wrctl %0 \n\t nop"\
		: : "r" (val))

/*------------------------------------------------------------------------
 * Control reg bit masks
 *----------------------------------------------------------------------*/
#define STATUS_DC	(1<<17)		/* Data cache enable		*/
#define STATUS_IC	(1<<16)		/* Instruction cache enable	*/
#define STATUS_IE	(1<<15)		/* Interrupt enable		*/
#define STATUS_IPRI	(0x3f<<9)	/* Interrupt priority		*/
#define STATUS_CWP	(0x1f<<4)	/* Current window pointer	*/
#define STATUS_N	(1<<3)		/* Condition code: negative	*/
#define STATUS_V	(1<<2)		/* Condition code: overflow	*/
#define STATUS_Z	(1<<1)		/* Condition code: zero		*/
#define STATUS_C	(1<<0)		/* Condition code: carry/borrow */

static inline unsigned ipri( unsigned prio )
{
	unsigned tmp;
	unsigned status = rdctl(CTL_STATUS);
	prio = (prio << 9) & STATUS_IPRI;
	tmp = (status & ~STATUS_IPRI) | prio;
	wrctl(CTL_STATUS,tmp);
	return( (status & STATUS_IPRI) >> 9);
}


#endif /* __NIOS_H__ */
