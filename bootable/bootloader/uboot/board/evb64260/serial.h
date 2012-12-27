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

/* serial.h - mostly useful for DUART serial_init in serial.c */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#if 0

#define B230400         1
#define B115200         2
#define B57600          4
#define B38400          82
#define B19200          163
#define B9600           24
#define B4800           651
#define B2400           1302
#define B1200           2604
#define B600            5208
#define B300            10417
#define B150            20833
#define B110            28409
#define BDEFAULT        B115200

				/* this stuff is important to initialize
				the DUART channels */

#define	Scale		0x01L		/* distance between port addresses */
#define	COM1		0x000003f8		/* Keyboard */
#define COM2		0x000002f8		/* Host */


/* Port Definitions relative to base COM port addresses */
#define DataIn	(0x00*Scale)	/* data input port */
#define DataOut	(0x00*Scale)	/* data output port */
#define BaudLsb	(0x00*Scale)	/* baud rate divisor least significant byte */
#define BaudMsb	(0x01*Scale)	/* baud rate divisor most significant byte */
#define	Ier	(0x01*Scale)	/* interrupt enable register */
#define	Iir	(0x02*Scale)	/* interrupt identification register */
#define	Lcr	(0x03*Scale)	/* line control register */
#define	Mcr	(0x04*Scale)	/* modem control register */
#define	Lsr	(0x05*Scale)	/* line status register */
#define	Msr	(0x06*Scale)	/* modem status register */

/* Bit Definitions for above ports */
#define LcrDlab	0x80	/* b7:	 enable baud rate divisor registers */
#define	LcrDflt	0x03	/* b6-0: no parity, 1 stop, 8 data */

#define	McrRts	0x02	/* b1:	request to send (I am ready to xmit) */
#define	McrDtr	0x01	/* b0:	data terminal ready (I am alive ready to rcv) */
#define	McrDflt	(McrRts|McrDtr)

#define LsrTxD	0x6000	/* b5: transmit holding register empty (i.e. xmit OK!)*/
			/* b6: transmitter empty */
#define LsrRxD	0x0100	/* b0: received data ready (i.e. got a byte!) */

#define	MsrRi	0x0040	/* b6: ring indicator (other guy is ready to rcv) */
#define	MsrDsr	0x0020	/* b5: data set ready (other guy is alive ready to rcv */
#define	MsrCts	0x0010	/* b4: clear to send (other guy is ready to rcv) */

#define IerRda	0xf	/* b0: Enable received data available interrupt */

#endif

#endif /* __SERIAL_H__ */
