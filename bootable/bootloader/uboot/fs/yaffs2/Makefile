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


# Makefile for YAFFS direct test
#
#
# YAFFS: Yet another Flash File System. A NAND-flash specific file system.
#
# Copyright (C) 2003 Aleph One Ltd.
#
#
# Created by Charles Manning <charles@aleph1.co.uk>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# NB Warning this Makefile does not include header dependencies.
#
# $Id: Makefile,v 1.15 2007/07/18 19:40:38 charles Exp $

#EXTRA_COMPILE_FLAGS = -DYAFFS_IGNORE_TAGS_ECC
include $(TOPDIR)/config.mk

LIB = $(obj)libyaffs2.a

COBJS-$(CONFIG_YAFFS2) := \
	yaffscfg.o yaffs_ecc.o yaffsfs.o yaffs_guts.o yaffs_packedtags1.o \
	yaffs_tagscompat.o yaffs_packedtags2.o yaffs_tagsvalidity.o \
	yaffs_nand.o yaffs_checkptrw.o yaffs_qsort.o yaffs_mtdif.o \
	yaffs_mtdif2.o

SRCS    := $(COBJS-y:.o=.c)
OBJS    := $(addprefix $(obj),$(COBJS-y))

# -DCONFIG_YAFFS_NO_YAFFS1
CFLAGS +=    -DCONFIG_YAFFS_DIRECT -DCONFIG_YAFFS_SHORT_NAMES_IN_RAM -DCONFIG_YAFFS_YAFFS2 -DNO_Y_INLINE -DLINUX_VERSION_CODE=0x20622

all:  $(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean distclean
clean:
	rm -f $(OBJS)

distclean:  clean
	rm -f $(LIB) core *.bak .depend

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
