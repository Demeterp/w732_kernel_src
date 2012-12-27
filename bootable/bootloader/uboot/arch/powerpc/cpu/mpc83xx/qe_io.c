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
 * Copyright (C) 2006 Freescale Semiconductor, Inc.
 *
 * Dave Liu <daveliu@freescale.com>
 * based on source code of Shlomi Gridish
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

#include "common.h"
#include "asm/errno.h"
#include "asm/io.h"
#include "asm/immap_83xx.h"

#define	NUM_OF_PINS	32
void qe_config_iopin(u8 port, u8 pin, int dir, int open_drain, int assign)
{
	u32			pin_2bit_mask;
	u32			pin_2bit_dir;
	u32			pin_2bit_assign;
	u32			pin_1bit_mask;
	u32			tmp_val;
	volatile immap_t	*im = (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile qepio83xx_t	*par_io = (volatile qepio83xx_t *)&im->qepio;

	/* Caculate pin location and 2bit mask and dir */
	pin_2bit_mask = (u32)(0x3 << (NUM_OF_PINS-(pin%(NUM_OF_PINS/2)+1)*2));
	pin_2bit_dir = (u32)(dir << (NUM_OF_PINS-(pin%(NUM_OF_PINS/2)+1)*2));

	/* Setup the direction */
	tmp_val = (pin > (NUM_OF_PINS/2) - 1) ? \
		in_be32(&par_io->ioport[port].dir2) :
		in_be32(&par_io->ioport[port].dir1);

	if (pin > (NUM_OF_PINS/2) -1) {
		out_be32(&par_io->ioport[port].dir2, ~pin_2bit_mask & tmp_val);
		out_be32(&par_io->ioport[port].dir2, pin_2bit_dir | tmp_val);
	} else {
		out_be32(&par_io->ioport[port].dir1, ~pin_2bit_mask & tmp_val);
		out_be32(&par_io->ioport[port].dir1, pin_2bit_dir | tmp_val);
	}

	/* Calculate pin location for 1bit mask */
	pin_1bit_mask = (u32)(1 << (NUM_OF_PINS - (pin+1)));

	/* Setup the open drain */
	tmp_val = in_be32(&par_io->ioport[port].podr);
	if (open_drain) {
		out_be32(&par_io->ioport[port].podr, pin_1bit_mask | tmp_val);
	} else {
		out_be32(&par_io->ioport[port].podr, ~pin_1bit_mask & tmp_val);
	}

	/* Setup the assignment */
	tmp_val = (pin > (NUM_OF_PINS/2) - 1) ?
		in_be32(&par_io->ioport[port].ppar2):
		in_be32(&par_io->ioport[port].ppar1);
	pin_2bit_assign = (u32)(assign
				<< (NUM_OF_PINS - (pin%(NUM_OF_PINS/2)+1)*2));

	/* Clear and set 2 bits mask */
	if (pin > (NUM_OF_PINS/2) - 1) {
		out_be32(&par_io->ioport[port].ppar2, ~pin_2bit_mask & tmp_val);
		out_be32(&par_io->ioport[port].ppar2, pin_2bit_assign | tmp_val);
	} else {
		out_be32(&par_io->ioport[port].ppar1, ~pin_2bit_mask & tmp_val);
		out_be32(&par_io->ioport[port].ppar1, pin_2bit_assign | tmp_val);
	}
}
