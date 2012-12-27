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
# (C) Copyright 2001-2006
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

#CFLAGS += -DDEBUG

LIB	= $(obj)librtc.a

COBJS-$(CONFIG_RTC_BFIN) += bfin_rtc.o
COBJS-y += date.o
COBJS-$(CONFIG_RTC_DS12887) += ds12887.o
COBJS-$(CONFIG_RTC_DS1302) += ds1302.o
COBJS-$(CONFIG_RTC_DS1306) += ds1306.o
COBJS-$(CONFIG_RTC_DS1307) += ds1307.o
COBJS-$(CONFIG_RTC_DS1338) += ds1307.o
COBJS-$(CONFIG_RTC_DS1337) += ds1337.o
COBJS-$(CONFIG_RTC_DS1374) += ds1374.o
COBJS-$(CONFIG_RTC_DS1556) += ds1556.o
COBJS-$(CONFIG_RTC_DS164x) += ds164x.o
COBJS-$(CONFIG_RTC_DS174x) += ds174x.o
COBJS-$(CONFIG_RTC_DS3231) += ds3231.o
COBJS-$(CONFIG_RTC_FTRTC010) += ftrtc010.o
COBJS-$(CONFIG_RTC_ISL1208) += isl1208.o
COBJS-$(CONFIG_RTC_M41T11) += m41t11.o
COBJS-$(CONFIG_RTC_M41T60) += m41t60.o
COBJS-$(CONFIG_RTC_M41T62) += m41t62.o
COBJS-$(CONFIG_RTC_M41T94) += m41t94.o
COBJS-$(CONFIG_RTC_M48T35A) += m48t35ax.o
COBJS-$(CONFIG_RTC_MAX6900) += max6900.o
COBJS-$(CONFIG_RTC_MC13783) += mc13783-rtc.o
COBJS-$(CONFIG_RTC_MC146818) += mc146818.o
COBJS-$(CONFIG_MCFRTC) += mcfrtc.o
COBJS-$(CONFIG_RTC_MK48T59) += mk48t59.o
COBJS-$(CONFIG_RTC_MPC5200) += mpc5xxx.o
COBJS-$(CONFIG_RTC_MPC8xx) += mpc8xx.o
COBJS-$(CONFIG_RTC_PCF8563) += pcf8563.o
COBJS-$(CONFIG_RTC_PL031) += pl031.o
COBJS-$(CONFIG_RTC_RS5C372A) += rs5c372.o
COBJS-$(CONFIG_RTC_RTC4543) += rtc4543.o
COBJS-$(CONFIG_RTC_RX8025) += rx8025.o
COBJS-$(CONFIG_RTC_S3C24X0) += s3c24x0_rtc.o
COBJS-$(CONFIG_RTC_S3C44B0) += s3c44b0_rtc.o
COBJS-$(CONFIG_RTC_X1205) += x1205.o

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
