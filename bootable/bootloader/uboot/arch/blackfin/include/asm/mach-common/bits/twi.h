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
 * TWI Masks
 */

#ifndef __BFIN_PERIPHERAL_TWI__
#define __BFIN_PERIPHERAL_TWI__

/* TWI_CLKDIV Macros (Use: *pTWI_CLKDIV = CLKLOW(x)|CLKHI(y); ) */
#define	CLKLOW(x)		((x) & 0xFF)		/* Periods Clock Is Held Low */
#define CLKHI(y)		(((y)&0xFF)<<0x8)	/* Periods Before New Clock Low */

/* TWI_PRESCALE Masks */
#define	PRESCALE		0x007F	/* SCLKs Per Internal Time Reference (10MHz) */
#define	TWI_ENA			0x0080	/* TWI Enable */
#define	SCCB			0x0200	/* SCCB Compatibility Enable */

/* TWI_SLAVE_CTL Masks */
#define	SEN			0x0001	/* Slave Enable */
#define	SADD_LEN		0x0002	/* Slave Address Length */
#define	STDVAL			0x0004	/* Slave Transmit Data Valid */
#define	TSC_NAK			0x0008	/* NAK/ACK* Generated At Conclusion Of Transfer */
#define	GEN			0x0010	/* General Call Adrress Matching Enabled */

/* TWI_SLAVE_STAT Masks */
#define	SDIR			0x0001	/* Slave Transfer Direction (Transmit/Receive*) */
#define GCALL			0x0002	/* General Call Indicator */

/* TWI_MASTER_CTRL Masks */
#define	MEN			0x0001	/* Master Mode Enable */
#define	MADD_LEN		0x0002	/* Master Address Length */
#define	MDIR			0x0004	/* Master Transmit Direction (RX/TX*) */
#define	FAST			0x0008	/* Use Fast Mode Timing Specs */
#define	STOP			0x0010	/* Issue Stop Condition */
#define	RSTART			0x0020	/* Repeat Start or Stop* At End Of Transfer */
#define	DCNT			0x3FC0	/* Data Bytes To Transfer */
#define	SDAOVR			0x4000	/* Serial Data Override */
#define	SCLOVR			0x8000	/* Serial Clock Override */

/* TWI_MASTER_STAT Masks */
#define	MPROG			0x0001	/* Master Transfer In Progress */
#define	LOSTARB			0x0002	/* Lost Arbitration Indicator (Xfer Aborted) */
#define	ANAK			0x0004	/* Address Not Acknowledged */
#define	DNAK			0x0008	/* Data Not Acknowledged */
#define	BUFRDERR		0x0010	/* Buffer Read Error */
#define	BUFWRERR		0x0020	/* Buffer Write Error */
#define	SDASEN			0x0040	/* Serial Data Sense */
#define	SCLSEN			0x0080	/* Serial Clock Sense */
#define	BUSBUSY			0x0100	/* Bus Busy Indicator */

/* TWI_INT_SRC and TWI_INT_ENABLE Masks */
#define	SINIT			0x0001	/* Slave Transfer Initiated */
#define	SCOMP			0x0002	/* Slave Transfer Complete */
#define	SERR			0x0004	/* Slave Transfer Error */
#define	SOVF			0x0008	/* Slave Overflow */
#define	MCOMP			0x0010	/* Master Transfer Complete */
#define	MERR			0x0020	/* Master Transfer Error */
#define	XMTSERV			0x0040	/* Transmit FIFO Service */
#define	RCVSERV			0x0080	/* Receive FIFO Service */

/* TWI_FIFO_CTRL Masks */
#define	XMTFLUSH		0x0001	/* Transmit Buffer Flush */
#define	RCVFLUSH		0x0002	/* Receive Buffer Flush */
#define	XMTINTLEN		0x0004	/* Transmit Buffer Interrupt Length */
#define	RCVINTLEN		0x0008	/* Receive Buffer Interrupt Length */

/* TWI_FIFO_STAT Masks */
#define	XMTSTAT			0x0003	/* Transmit FIFO Status */
#define	XMT_EMPTY		0x0000	/* Transmit FIFO Empty */
#define	XMT_HALF		0x0001	/* Transmit FIFO Has 1 Byte To Write */
#define	XMT_FULL		0x0003	/* Transmit FIFO Full (2 Bytes To Write) */

#define	RCVSTAT			0x000C	/* Receive FIFO Status */
#define	RCV_EMPTY		0x0000	/* Receive FIFO Empty */
#define	RCV_HALF		0x0004	/* Receive FIFO Has 1 Byte To Read */
#define	RCV_FULL		0x000C	/* Receive FIFO Full (2 Bytes To Read) */

#endif
