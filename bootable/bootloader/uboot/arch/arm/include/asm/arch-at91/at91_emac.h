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
 * Copyright (C) 2009 Jens Scharsig (js_at_ng@scharsoft.de)
 *
 * based on AT91RM9200 datasheet revision I (36. Ethernet MAC (EMAC))
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef AT91_H
#define AT91_H

typedef struct at91_emac {
	u32	 ctl;
	u32	 cfg;
	u32	 sr;
	u32	 tar;
	u32	 tcr;
	u32	 tsr;
	u32	 rbqp;
	u32	 reserved0;
	u32	 rsr;
	u32	 isr;
	u32	 ier;
	u32	 idr;
	u32	 imr;
	u32	 man;
	u32	 reserved1[2];
	u32	 fra;
	u32	 scol;
	u32	 mocl;
	u32	 ok;
	u32	 seqe;
	u32	 ale;
	u32	 dte;
	u32	 lcol;
	u32	 ecol;
	u32	 cse;
	u32	 tue;
	u32	 cde;
	u32	 elr;
	u32	 rjb;
	u32	 usf;
	u32	 sqee;
	u32	 drfc;
	u32	 reserved2[3];
	u32	 hsh;
	u32	 hsl;
	u32	 sh1l;
	u32	 sa1h;
	u32	 sa2l;
	u32	 sa2h;
	u32	 sa3l;
	u32	 sa3h;
	u32	 sa4l;
	u32	 sa4h;
} at91_emac_t;

#define AT91_EMAC_CTL_LB	0x0001
#define AT91_EMAC_CTL_LBL	0x0002
#define AT91_EMAC_CTL_RE	0x0004
#define AT91_EMAC_CTL_TE	0x0008
#define AT91_EMAC_CTL_MPE	0x0010
#define AT91_EMAC_CTL_CSR	0x0020
#define AT91_EMAC_CTL_ISR	0x0040
#define AT91_EMAC_CTL_WES	0x0080
#define AT91_EMAC_CTL_BP	0x1000

#define AT91_EMAC_CFG_SPD	0x0001
#define AT91_EMAC_CFG_FD	0x0002
#define AT91_EMAC_CFG_BR	0x0004
#define AT91_EMAC_CFG_CAF	0x0010
#define AT91_EMAC_CFG_NBC	0x0020
#define AT91_EMAC_CFG_MTI	0x0040
#define AT91_EMAC_CFG_UNI	0x0080
#define AT91_EMAC_CFG_BIG	0x0100
#define AT91_EMAC_CFG_EAE	0x0200
#define AT91_EMAC_CFG_CLK_MASK	0xFFFFF3FF
#define AT91_EMAC_CFG_MCLK_8	0x0000
#define AT91_EMAC_CFG_MCLK_16	0x0400
#define AT91_EMAC_CFG_MCLK_32	0x0800
#define AT91_EMAC_CFG_MCLK_64	0x0C00
#define AT91_EMAC_CFG_RTY	0x1000
#define AT91_EMAC_CFG_RMII	0x2000

#define AT91_EMAC_SR_LINK	0x0001
#define AT91_EMAC_SR_MDIO	0x0002
#define AT91_EMAC_SR_IDLE	0x0004

#define AT91_EMAC_TCR_LEN(x)	(x & 0x7FF)
#define AT91_EMAC_TCR_NCRC	0x8000

#define AT91_EMAC_TSR_OVR	0x0001
#define AT91_EMAC_TSR_COL	0x0002
#define AT91_EMAC_TSR_RLE	0x0004
#define AT91_EMAC_TSR_TXIDLE	0x0008
#define AT91_EMAC_TSR_BNQ	0x0010
#define AT91_EMAC_TSR_COMP	0x0020
#define AT91_EMAC_TSR_UND	0x0040

#define AT91_EMAC_RSR_BNA	0x0001
#define AT91_EMAC_RSR_REC	0x0002
#define AT91_EMAC_RSR_OVR	0x0004

/*  ISR, IER, IDR, IMR use the same bits */
#define AT91_EMAC_IxR_DONE	0x0001
#define AT91_EMAC_IxR_RCOM	0x0002
#define AT91_EMAC_IxR_RBNA	0x0004
#define AT91_EMAC_IxR_TOVR	0x0008
#define AT91_EMAC_IxR_TUND	0x0010
#define AT91_EMAC_IxR_RTRY	0x0020
#define AT91_EMAC_IxR_TBRE	0x0040
#define AT91_EMAC_IxR_TCOM	0x0080
#define AT91_EMAC_IxR_TIDLE	0x0100
#define AT91_EMAC_IxR_LINK	0x0200
#define AT91_EMAC_IxR_ROVR	0x0400
#define AT91_EMAC_IxR_HRESP	0x0800

#define AT91_EMAC_MAN_DATA_MASK		0xFFFF
#define AT91_EMAC_MAN_CODE_802_3	0x00020000
#define AT91_EMAC_MAN_REGA(reg)		((reg & 0x1F) << 18)
#define AT91_EMAC_MAN_PHYA(phy)		((phy & 0x1F) << 23)
#define AT91_EMAC_MAN_RW_R		0x20000000
#define AT91_EMAC_MAN_RW_W		0x10000000
#define AT91_EMAC_MAN_HIGH		0x40000000
#define AT91_EMAC_MAN_LOW		0x80000000

#endif
