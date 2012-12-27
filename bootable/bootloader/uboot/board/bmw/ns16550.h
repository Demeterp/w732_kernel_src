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
 * NS16550 Serial Port
 * originally from linux source (arch/powerpc/boot/ns16550.h)
 * modified slightly to
 * have addresses as offsets from CONFIG_SYS_ISA_BASE
 * added a few more definitions
 * added prototypes for ns16550.c
 * reduced no of com ports to 2
 * modifications (c) Rob Taylor, Flying Pig Systems. 2000.
 * further modified to support the 8245 duart
 * modifications (c) Paul Jimenez, Musenki, Inc. 2001.
 */


struct NS16550 {
	unsigned char rbrthrdlb;	/* 0 */
	unsigned char ierdmb;		/* 1 */
	unsigned char iirfcrafr;	/* 2 */
	unsigned char lcr;		/* 3 */
	unsigned char mcr;		/* 4 */
	unsigned char lsr;		/* 5 */
	unsigned char msr;		/* 6 */
	unsigned char scr;		/* 7 */
	unsigned char reserved[2];	/* 8 & 9 */
	unsigned char dsr;		/* 10 */
	unsigned char dcr;		/* 11 */
};


#define rbr rbrthrdlb
#define thr rbrthrdlb
#define dll rbrthrdlb
#define ier ierdmb
#define dlm ierdmb
#define iir iirfcrafr
#define fcr iirfcrafr
#define afr iirfcrafr

#define FCR_FIFO_EN     0x01	/*fifo enable */
#define FCR_RXSR        0x02	/*reciever soft reset */
#define FCR_TXSR        0x04	/*transmitter soft reset */
#define FCR_DMS		0x08	/* DMA Mode Select */

#define MCR_RTS         0x02	/* Readyu to Send */
#define MCR_LOOP	0x10	/* Local loopback mode enable */
/* #define MCR_DTR         0x01    noton 8245 duart */
/* #define MCR_DMA_EN      0x04    noton 8245 duart */
/* #define MCR_TX_DFR      0x08    noton 8245 duart */

#define LCR_WLS_MSK 0x03	/* character length slect mask */
#define LCR_WLS_5   0x00	/* 5 bit character length */
#define LCR_WLS_6   0x01	/* 6 bit character length */
#define LCR_WLS_7   0x02	/* 7 bit character length */
#define LCR_WLS_8   0x03	/* 8 bit character length */
#define LCR_STB     0x04	/* Number of stop Bits, off = 1, on = 1.5 or 2) */
#define LCR_PEN     0x08	/* Parity eneble */
#define LCR_EPS     0x10	/* Even Parity Select */
#define LCR_STKP    0x20	/* Stick Parity */
#define LCR_SBRK    0x40	/* Set Break */
#define LCR_BKSE    0x80	/* Bank select enable - aka DLAB on 8245 */

#define LSR_DR      0x01	/* Data ready */
#define LSR_OE      0x02	/* Overrun */
#define LSR_PE      0x04	/* Parity error */
#define LSR_FE      0x08	/* Framing error */
#define LSR_BI      0x10	/* Break */
#define LSR_THRE    0x20	/* Xmit holding register empty */
#define LSR_TEMT    0x40	/* Xmitter empty */
#define LSR_ERR     0x80	/* Error */

/* useful defaults for LCR*/
#define LCR_8N1     0x03


volatile struct NS16550 *NS16550_init (int chan, int baud_divisor);
void NS16550_putc (volatile struct NS16550 *com_port, unsigned char c);
unsigned char NS16550_getc (volatile struct NS16550 *com_port);
int NS16550_tstc (volatile struct NS16550 *com_port);
void NS16550_reinit (volatile struct NS16550 *com_port, int baud_divisor);
