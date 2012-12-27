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
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>

#include "pci.h"

#include "hardware.h"
#include "pcippc2.h"

u32		pcippc2_fpga0_phys;
u32		pcippc2_fpga1_phys;

void pcippc2_fpga_init (void)
{
  pci_dev_t		bdf = pci_find_device(FPGA_VENDOR_ID, FPGA_DEVICE_ID, 0);
  unsigned int		addr;
  u16			cmd;

  if (bdf == -1)
  {
    puts("Unable to find FPGA !\n");
    hang();
  }

  pci_read_config_word(bdf, PCI_COMMAND, &cmd);
  if ((cmd & (PCI_COMMAND_MEMORY | PCI_COMMAND_IO)) != (PCI_COMMAND_MEMORY | PCI_COMMAND_IO))
  {
    puts("FPGA is not configured !\n");
    hang();
  }

  pci_read_config_dword(bdf, PCI_BASE_ADDRESS_0, &addr);
  if (addr & 0x1)
  {
      /* IO space
       */
    pcippc2_fpga0_phys = pci_io_to_phys(bdf, addr & 0xfffffffc);
  }
  else
  {
      /* Memory space
       */
    pcippc2_fpga0_phys = pci_mem_to_phys(bdf, addr & 0xfffffff0);
  }

  pci_read_config_dword(bdf, PCI_BASE_ADDRESS_1, &addr);
  if (addr & 0x1)
  {
      /* IO space
       */
    pcippc2_fpga1_phys = pci_io_to_phys(bdf, addr & 0xfffffffc);
  }
  else
  {
      /* Memory space
       */
    pcippc2_fpga1_phys = pci_mem_to_phys(bdf, addr & 0xfffffff0);
  }

    /* Interrupts are not used
     */
  out32(FPGA(INT, INTR_MASK), 0xffffffff);
  iobarrier_rw();
}
