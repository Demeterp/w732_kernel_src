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

#ifndef __UBOOT_SL811_H
#define __UBOOT_SL811_H

#undef SL811_DEBUG

#ifdef SL811_DEBUG
	#define PDEBUG(level, fmt, args...) \
		if (debug >= (level)) printf("[%s:%d] " fmt, \
		__PRETTY_FUNCTION__, __LINE__ , ## args)
#else
	#define PDEBUG(level, fmt, args...) do {} while(0)
#endif

/* Sl811 host control register */
#define	SL811_CTRL_A		0x00
#define	SL811_ADDR_A		0x01
#define	SL811_LEN_A		0x02
#define	SL811_STS_A		0x03	/* read	*/
#define	SL811_PIDEP_A		0x03	/* write */
#define	SL811_CNT_A		0x04	/* read	*/
#define	SL811_DEV_A		0x04	/* write */
#define	SL811_CTRL1		0x05
#define	SL811_INTR		0x06
#define	SL811_CTRL_B		0x08
#define	SL811_ADDR_B		0x09
#define	SL811_LEN_B		0x0A
#define	SL811_STS_B		0x0B	/* read	*/
#define	SL811_PIDEP_B		0x0B	/* write */
#define	SL811_CNT_B		0x0C	/* read	*/
#define	SL811_DEV_B		0x0C	/* write */
#define	SL811_INTRSTS		0x0D	/* write clears	bitwise	*/
#define	SL811_HWREV		0x0E	/* read	*/
#define	SL811_SOFLOW		0x0E	/* write */
#define	SL811_SOFCNTDIV		0x0F	/* read	*/
#define	SL811_CTRL2		0x0F	/* write */

/* USB control register bits (addr 0x00 and addr 0x08) */
#define	SL811_USB_CTRL_ARM	0x01
#define	SL811_USB_CTRL_ENABLE	0x02
#define	SL811_USB_CTRL_DIR_OUT	0x04
#define	SL811_USB_CTRL_ISO	0x10
#define	SL811_USB_CTRL_SOF	0x20
#define	SL811_USB_CTRL_TOGGLE_1	0x40
#define	SL811_USB_CTRL_PREAMBLE	0x80

/* USB status register bits (addr 0x03 and addr 0x0B) */
#define	SL811_USB_STS_ACK	0x01
#define	SL811_USB_STS_ERROR	0x02
#define	SL811_USB_STS_TIMEOUT	0x04
#define	SL811_USB_STS_TOGGLE_1	0x08
#define	SL811_USB_STS_SETUP	0x10
#define	SL811_USB_STS_OVERFLOW	0x20
#define	SL811_USB_STS_NAK	0x40
#define	SL811_USB_STS_STALL	0x80

/* Control register 1 bits (addr 0x05) */
#define	SL811_CTRL1_SOF		0x01
#define	SL811_CTRL1_RESET	0x08
#define	SL811_CTRL1_JKSTATE	0x10
#define	SL811_CTRL1_SPEED_LOW	0x20
#define	SL811_CTRL1_SUSPEND	0x40

/* Interrut enable (addr 0x06) and interrupt status register bits (addr 0x0D) */
#define	SL811_INTR_DONE_A	0x01
#define	SL811_INTR_DONE_B	0x02
#define	SL811_INTR_SOF		0x10
#define	SL811_INTR_INSRMV	0x20
#define	SL811_INTR_DETECT	0x40
#define	SL811_INTR_NOTPRESENT	0x40
#define	SL811_INTR_SPEED_FULL	0x80    /* only in status reg */

/* HW rev and SOF lo register bits (addr 0x0E) */
#define	SL811_HWR_HWREV		0xF0

/* SOF counter and control reg 2 (addr 0x0F) */
#define	SL811_CTL2_SOFHI	0x3F
#define	SL811_CTL2_DSWAP	0x40
#define	SL811_CTL2_HOST		0x80

/* Set up for 1-ms SOF time. */
#define SL811_12M_LOW		0xE0
#define SL811_12M_HI		0x2E

#define SL811_DATA_START	0x10
#define SL811_DATA_LIMIT	240

/* Requests: bRequest << 8 | bmRequestType */
#define RH_GET_STATUS           0x0080
#define RH_CLEAR_FEATURE        0x0100
#define RH_SET_FEATURE          0x0300
#define RH_SET_ADDRESS		0x0500
#define RH_GET_DESCRIPTOR	0x0680
#define RH_SET_DESCRIPTOR       0x0700
#define RH_GET_CONFIGURATION	0x0880
#define RH_SET_CONFIGURATION	0x0900
#define RH_GET_STATE            0x0280
#define RH_GET_INTERFACE        0x0A80
#define RH_SET_INTERFACE        0x0B00
#define RH_SYNC_FRAME           0x0C80


#define PIDEP(pid, ep) (((pid) & 0x0f) << 4 | (ep))

#endif	/* __UBOOT_SL811_H */
