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
 * (C) Copyright 2003
 * Stefan Roese, esd gmbh germany, stefan.roese@esd-electronics.com
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

/*******************************************************/
/* file: ports.c                                       */
/* abstract:  This file contains the routines to       */
/*            output values on the JTAG ports, to read */
/*            the TDO bit, and to read a byte of data  */
/*            from the prom                            */
/*                                                     */
/*******************************************************/

#include <common.h>
#include <asm/processor.h>
#include <asm/io.h>

#include "ports.h"

static unsigned long output = 0;
static int filepos = 0;
static int oldstate = 0;
static int newstate = 0;
static int readptr = 0;

extern const unsigned char *xsvfdata;

/* if in debugging mode, then just set the variables */
void setPort(short p,short val)
{
	if (p==TMS) {
		if (val) {
			output |= JTAG_TMS;
		} else {
			output &= ~JTAG_TMS;
		}
	}
	if (p==TDI) {
		if (val) {
			output |= JTAG_TDI;
		} else {
			output &= ~JTAG_TDI;
		}
	}
	if (p==TCK) {
		if (val) {
			output |= JTAG_TCK;
		} else {
			output &= ~JTAG_TCK;
		}
		out_be32((void *)GPIO0_OR, output);
	}
}


/* toggle tck LH */
void pulseClock(void)
{
	setPort(TCK,0);  /* set the TCK port to low  */
	setPort(TCK,1);  /* set the TCK port to high */
}


/* read in a byte of data from the prom */
void readByte(unsigned char *data)
{
	/* pretend reading using a file */
	*data = xsvfdata[readptr++];
	newstate = filepos++ >> 10;
	if (newstate != oldstate) {
		printf("%4d kB\r\r\r\r", newstate);
		oldstate = newstate;
	}
}

/* read the TDO bit from port */
unsigned char readTDOBit(void)
{
	unsigned long inputs;

	inputs = in_be32((void *)GPIO0_IR);
	if (inputs & JTAG_TDO)
		return 1;
	else
		return 0;
}


/* Wait at least the specified number of microsec.                           */
/* Use a timer if possible; otherwise estimate the number of instructions    */
/* necessary to be run based on the microcontroller speed.  For this example */
/* we pulse the TCK port a number of times based on the processor speed.     */
void waitTime(long microsec)
{
	udelay(microsec); /* esd */
}
