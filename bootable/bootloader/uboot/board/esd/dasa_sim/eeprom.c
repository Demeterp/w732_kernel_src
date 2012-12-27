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
 * (C) Copyright 2001
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
 *
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>

#define EEPROM_CAP              0x50000358
#define EEPROM_DATA             0x5000035c


unsigned int eepromReadLong(int offs)
{
  unsigned int value;
  unsigned short ret;
  int count;

  out_be16((void *)EEPROM_CAP, offs);

  count = 0;

  for (;;)
    {
      count++;
      ret = in_be16((void *)EEPROM_CAP);

      if ((ret & 0x8000) != 0)
	break;
    }

  value = in_be32((void *)EEPROM_DATA);

  return value;
}


unsigned char eepromReadByte(int offs)
{
  unsigned int valueLong;
  unsigned char *ptr;

  valueLong = eepromReadLong(offs & ~3);
  ptr = (unsigned char *)&valueLong;

  return ptr[offs & 3];
}


void eepromWriteLong(int offs, unsigned int value)
{
  unsigned short ret;
  int count;

  count = 0;

  out_be32((void *)EEPROM_DATA, value);
  out_be16((void *)EEPROM_CAP, 0x8000 + offs);

  for (;;)
    {
      count++;
      ret = in_be16((void *)EEPROM_CAP);

      if ((ret & 0x8000) == 0)
	break;
    }
}


void eepromWriteByte(int offs, unsigned char valueByte)
{
  unsigned int valueLong;
  unsigned char *ptr;

  valueLong = eepromReadLong(offs & ~3);
  ptr = (unsigned char *)&valueLong;

  ptr[offs & 3] = valueByte;

  eepromWriteLong(offs & ~3, valueLong);
}


void i2c_read (uchar *addr, int alen, uchar *buffer, int len)
{
  int i;
  int len2, ptr;

  /*  printf("\naddr=%x alen=%x buffer=%x len=%x", addr[0], addr[1], *(short *)addr, alen, buffer, len); /###* test-only */

  ptr = *(short *)addr;

  /*
   * Read till lword boundary
   */
  len2 = 4 - (*(short *)addr & 0x0003);
  for (i=0; i<len2; i++)
    {
      *buffer++ = eepromReadByte(ptr++);
    }

  /*
   * Read all lwords
   */
  len2 = (len - len2) >> 2;
  for (i=0; i<len2; i++)
    {
      *(unsigned int *)buffer = eepromReadLong(ptr);
      buffer += 4;
      ptr += 4;
    }

  /*
   * Read last bytes
   */
  len2 = (*(short *)addr + len) & 0x0003;
  for (i=0; i<len2; i++)
    {
      *buffer++ = eepromReadByte(ptr++);
    }
}

void i2c_write (uchar *addr, int alen, uchar *buffer, int len)
{
  int i;
  int len2, ptr;

  /*  printf("\naddr=%x alen=%x buffer=%x len=%x", addr[0], addr[1], *(short *)addr, alen, buffer, len); /###* test-only */

  ptr = *(short *)addr;

  /*
   * Write till lword boundary
   */
  len2 = 4 - (*(short *)addr & 0x0003);
  for (i=0; i<len2; i++)
    {
      eepromWriteByte(ptr++, *buffer++);
    }

  /*
   * Write all lwords
   */
  len2 = (len - len2) >> 2;
  for (i=0; i<len2; i++)
    {
      eepromWriteLong(ptr, *(unsigned int *)buffer);
      buffer += 4;
      ptr += 4;
    }

  /*
   * Write last bytes
   */
  len2 = (*(short *)addr + len) & 0x0003;
  for (i=0; i<len2; i++)
    {
      eepromWriteByte(ptr++, *buffer++);
    }
}
