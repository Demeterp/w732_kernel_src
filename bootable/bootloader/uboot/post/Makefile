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
# (C) Copyright 2002-2006
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
include $(OBJTREE)/include/autoconf.mk

LIB				= libpost.a
GPLIB-$(CONFIG_HAS_POST)	+= libgenpost.a
COBJS-$(CONFIG_HAS_POST)	+= post.o tests.o

SPLIB-$(CONFIG_HAS_POST) = drivers/libpostdrivers.a
SPLIB-$(CONFIG_HAS_POST) += $(shell if [ -d lib_$(ARCH) ]; then echo \
			    "lib_$(ARCH)/libpost$(ARCH).a"; fi)
SPLIB-$(CONFIG_HAS_POST) += $(shell if [ -d lib_$(ARCH)/fpu ]; then echo \
			    "lib_$(ARCH)/fpu/libpost$(ARCH)fpu.a"; fi)
SPLIB-$(CONFIG_HAS_POST) += $(shell if [ -d cpu/$(CPU) ]; then echo \
			    "cpu/$(CPU)/libpost$(CPU).a"; fi)
SPLIB-$(CONFIG_HAS_POST) += $(shell if [ -d board/$(BOARD) ]; then echo \
			    "board/$(BOARD)/libpost$(BOARD).a"; fi)

GPLIB	:= $(addprefix $(obj),$(GPLIB-y))
SPLIB	:= $(addprefix $(obj),$(SPLIB-y))
COBJS	:= $(COBJS-y)
SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
LIB	:= $(obj)$(LIB)

all:	$(LIB)

postdeps:
	@for lib in $(SPLIB-y) ; do \
		$(MAKE) -C `dirname $$lib` all ; \
	done

# generic POST library
$(GPLIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

# specific POST libraries
$(SPLIB): $(obj).depend postdeps
	$(MAKE) -C $(dir $(subst $(obj),,$@))

# the POST lib archive
$(LIB): $(GPLIB) $(SPLIB)
	(echo create $(LIB); for lib in $(GPLIB) $(SPLIB) ; \
	 do echo addlib $$lib; done; echo save) \
	| $(AR) -M

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
