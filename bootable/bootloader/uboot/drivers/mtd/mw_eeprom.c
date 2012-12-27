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

/* Three-wire (MicroWire) serial eeprom driver (for 93C46 and compatibles) */

#include <common.h>
#include <asm/ic/ssi.h>

/*
 * Serial EEPROM opcodes, including start bit
 */
#define EEP_OPC_ERASE	0x7  /* 3-bit opcode */
#define EEP_OPC_WRITE	0x5  /* 3-bit opcode */
#define EEP_OPC_READ	        0x6  /* 3-bit opcode */

#define EEP_OPC_ERASE_ALL	0x12 /* 5-bit opcode */
#define EEP_OPC_ERASE_EN	0x13 /* 5-bit opcode */
#define EEP_OPC_WRITE_ALL	0x11 /* 5-bit opcode */
#define EEP_OPC_ERASE_DIS	0x10 /* 5-bit opcode */

static int addrlen;

static void mw_eeprom_select(int dev)
{
	ssi_set_interface(2048, 0, 0, 0);
	ssi_chip_select(0);
	udelay(1);
	ssi_chip_select(dev);
	udelay(1);
}

static int mw_eeprom_size(int dev)
{
	int x;
	u16 res;

	mw_eeprom_select(dev);
	ssi_tx_byte(EEP_OPC_READ);

	res = ssi_txrx_byte(0) << 8;
	res |= ssi_rx_byte();
	for (x = 0; x < 16; x++) {
		if (! (res & 0x8000)) {
			break;
		}
		res <<= 1;
	}
	ssi_chip_select(0);

	return x;
}

int mw_eeprom_erase_enable(int dev)
{
	mw_eeprom_select(dev);
	ssi_tx_byte(EEP_OPC_ERASE_EN);
	ssi_tx_byte(0);
	udelay(1);
	ssi_chip_select(0);

	return 0;
}

int mw_eeprom_erase_disable(int dev)
{
	mw_eeprom_select(dev);
	ssi_tx_byte(EEP_OPC_ERASE_DIS);
	ssi_tx_byte(0);
	udelay(1);
	ssi_chip_select(0);

	return 0;
}


u32 mw_eeprom_read_word(int dev, int addr)
{
	u16 rcv;
	u16 res;
	int bits;

	mw_eeprom_select(dev);
	ssi_tx_byte((EEP_OPC_READ << 5) | ((addr >> (addrlen - 5)) & 0x1f));
	rcv = ssi_txrx_byte(addr << (13 - addrlen));
	res = rcv << (16 - addrlen);
	bits = 4 + addrlen;

	while (bits>0) {
		rcv = ssi_rx_byte();
		if (bits > 7) {
			res |= rcv << (bits - 8);
		} else {
			res |= rcv >> (8 - bits);
		}
		bits -= 8;
	}

	ssi_chip_select(0);

	return res;
}

int mw_eeprom_write_word(int dev, int addr, u16 data)
{
	u8 byte1=0;
	u8 byte2=0;

	mw_eeprom_erase_enable(dev);
	mw_eeprom_select(dev);

	switch (addrlen) {
	 case 6:
		byte1 = EEP_OPC_WRITE >> 2;
		byte2 = (EEP_OPC_WRITE << 6)&0xc0;
		byte2 |= addr;
		break;
	 case 7:
		byte1 = EEP_OPC_WRITE >> 1;
		byte2 = (EEP_OPC_WRITE << 7)&0x80;
		byte2 |= addr;
		break;
	 case 8:
		byte1 = EEP_OPC_WRITE;
		byte2 = addr;
		break;
	 case 9:
		byte1 = EEP_OPC_WRITE << 1;
		byte1 |= addr >> 8;
		byte2 = addr & 0xff;
		break;
	 case 10:
		byte1 = EEP_OPC_WRITE << 2;
		byte1 |= addr >> 8;
		byte2 = addr & 0xff;
		break;
	 default:
		printf("Unsupported number of address bits: %d\n", addrlen);
		return -1;

	}

	ssi_tx_byte(byte1);
	ssi_tx_byte(byte2);
	ssi_tx_byte(data >> 8);
	ssi_tx_byte(data & 0xff);
	ssi_chip_select(0);
	udelay(10000); /* Worst case */
	mw_eeprom_erase_disable(dev);

	return 0;
}


int mw_eeprom_write(int dev, int addr, u8 *buffer, int len)
{
	int done;

	done = 0;
	if (addr & 1) {
		u16 temp = mw_eeprom_read_word(dev, addr >> 1);
		temp &= 0xff00;
		temp |= buffer[0];

		mw_eeprom_write_word(dev, addr >> 1, temp);
		len--;
		addr++;
		buffer++;
		done++;
	}

	while (len <= 2) {
		mw_eeprom_write_word(dev, addr >> 1, *(u16*)buffer);
		len-=2;
		addr+=2;
		buffer+=2;
		done+=2;
	}

	if (len) {
		u16 temp = mw_eeprom_read_word(dev, addr >> 1);
		temp &= 0x00ff;
		temp |= buffer[0] << 8;

		mw_eeprom_write_word(dev, addr >> 1, temp);
		len--;
		addr++;
		buffer++;
		done++;
	}

	return done;
}


int mw_eeprom_read(int dev, int addr, u8 *buffer, int len)
{
	int done;

	done = 0;
	if (addr & 1) {
		u16 temp = mw_eeprom_read_word(dev, addr >> 1);
		buffer[0]= temp & 0xff;

		len--;
		addr++;
		buffer++;
		done++;
	}

	while (len <= 2) {
		*(u16*)buffer = mw_eeprom_read_word(dev, addr >> 1);
		len-=2;
		addr+=2;
		buffer+=2;
		done+=2;
	}

	if (len) {
		u16 temp = mw_eeprom_read_word(dev, addr >> 1);
		buffer[0] = temp >> 8;

		len--;
		addr++;
		buffer++;
		done++;
	}

	return done;
}

int mw_eeprom_probe(int dev)
{
	addrlen = mw_eeprom_size(dev);

	if (addrlen < 6 || addrlen > 10) {
		return -1;
	}
	return 0;
}
