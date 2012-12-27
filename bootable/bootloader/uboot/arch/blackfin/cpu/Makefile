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
# U-boot - Makefile
#
# Copyright (c) 2005-2008 Analog Device Inc.
#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# Licensed under the GPL-2 or later.
#

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(CPU).a

EXTRA    :=
CEXTRA   := initcode.o
SEXTRA   := start.o
SOBJS    := interrupt.o cache.o
COBJS-y  += cpu.o
COBJS-y  += interrupts.o
COBJS-$(CONFIG_JTAG_CONSOLE) += jtag-console.o
COBJS-y  += os_log.o
COBJS-y  += reset.o
COBJS-y  += serial.o
COBJS-y  += traps.o
COBJS-$(CONFIG_HW_WATCHDOG)  += watchdog.o

ifeq ($(CONFIG_BFIN_BOOT_MODE),BFIN_BOOT_BYPASS)
COBJS-y  += initcode.o
endif

SRCS     := $(SEXTRA:.o=.S) $(SOBJS:.o=.S) $(COBJS-y:.o=.c)
OBJS     := $(addprefix $(obj),$(COBJS-y) $(SOBJS))
EXTRA    := $(addprefix $(obj),$(EXTRA))
CEXTRA   := $(addprefix $(obj),$(CEXTRA))
SEXTRA   := $(addprefix $(obj),$(SEXTRA))

all:	$(obj).depend $(LIB) $(obj).depend $(EXTRA) $(CEXTRA) $(SEXTRA) check_initcode

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(OBJS): $(obj)bootrom-asm-offsets.h
$(obj)bootrom-asm-offsets.c: bootrom-asm-offsets.c.in bootrom-asm-offsets.awk
	echo '#include <asm/mach-common/bits/bootrom.h>' | $(CPP) $(CPPFLAGS) - | gawk -f ./bootrom-asm-offsets.awk > $@.tmp
	mv $@.tmp $@
$(obj)bootrom-asm-offsets.s: $(obj)bootrom-asm-offsets.c
	$(CC) $(CFLAGS) -S $^ -o $@.tmp
	mv $@.tmp $@
$(obj)bootrom-asm-offsets.h: $(obj)bootrom-asm-offsets.s
	sed -ne "/^->/{s:^->\([^ ]*\) [\$$#]*\([^ ]*\) \(.*\):#define \1 \2 /* \3 */:; s:->::; p;}" $^ > $@

# make sure our initcode (which goes into LDR) does not
# have relocs or external references
$(obj)initcode.o: CFLAGS += -fno-function-sections -fno-data-sections
READINIT = env LC_ALL=C $(CROSS_COMPILE)readelf -s $<
check_initcode: $(obj)initcode.o
ifneq ($(CONFIG_BFIN_BOOT_MODE),BFIN_BOOT_BYPASS)
	@if $(READINIT) | grep '\<GLOBAL\>.*\<UND\>' ; then \
		echo "$< contains external references!" 1>&2 ; \
		exit 1 ; \
	fi
endif

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
