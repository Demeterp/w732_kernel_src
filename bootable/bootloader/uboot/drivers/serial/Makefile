# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


#
# (C) Copyright 2006-2009
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libserial.a

COBJS-$(CONFIG_ALTERA_UART) += altera_uart.o
COBJS-$(CONFIG_ALTERA_JTAG_UART) += altera_jtag_uart.o
COBJS-$(CONFIG_ARM_DCC) += arm_dcc.o
COBJS-$(CONFIG_AT91RM9200_USART) += at91rm9200_usart.o
COBJS-$(CONFIG_ATMEL_USART) += atmel_usart.o
COBJS-$(CONFIG_MCFUART) += mcfuart.o
COBJS-$(CONFIG_NS9750_UART) += ns9750_serial.o
COBJS-$(CONFIG_OPENCORES_YANU) += opencores_yanu.o
COBJS-$(CONFIG_SYS_NS16550) += ns16550.o
COBJS-$(CONFIG_DRIVER_S3C4510_UART) += s3c4510b_uart.o
COBJS-$(CONFIG_S3C64XX) += s3c64xx.o
COBJS-$(CONFIG_S5PC1XX) += serial_s5p.o
COBJS-$(CONFIG_SYS_NS16550_SERIAL) += serial.o
COBJS-$(CONFIG_CLPS7111_SERIAL) += serial_clps7111.o
COBJS-$(CONFIG_IMX_SERIAL) += serial_imx.o
COBJS-$(CONFIG_IXP_SERIAL) += serial_ixp.o
COBJS-$(CONFIG_KS8695_SERIAL) += serial_ks8695.o
COBJS-$(CONFIG_LPC2292_SERIAL) += serial_lpc2292.o
COBJS-$(CONFIG_LH7A40X_SERIAL) += serial_lh7a40x.o
COBJS-$(CONFIG_MAX3100_SERIAL) += serial_max3100.o
COBJS-$(CONFIG_MXC_UART) += serial_mxc.o
COBJS-$(CONFIG_NETARM_SERIAL) += serial_netarm.o
COBJS-$(CONFIG_PL010_SERIAL) += serial_pl01x.o
COBJS-$(CONFIG_PL011_SERIAL) += serial_pl01x.o
COBJS-$(CONFIG_PXA_SERIAL) += serial_pxa.o
COBJS-$(CONFIG_SA1100_SERIAL) += serial_sa1100.o
COBJS-$(CONFIG_S3C24X0_SERIAL) += serial_s3c24x0.o
COBJS-$(CONFIG_S3C44B0_SERIAL) += serial_s3c44b0.o
COBJS-$(CONFIG_XILINX_UARTLITE) += serial_xuartlite.o
COBJS-$(CONFIG_SCIF_CONSOLE) += serial_sh.o
COBJS-$(CONFIG_USB_TTY) += usbtty.o

COBJS	:= $(sort $(COBJS-y))
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
