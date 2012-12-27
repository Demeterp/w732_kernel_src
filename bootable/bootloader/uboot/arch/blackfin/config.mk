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
# (C) Copyright 2000-2002
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

CROSS_COMPILE ?= bfin-uclinux-

STANDALONE_LOAD_ADDR = 0x1000 -m elf32bfin

CONFIG_BFIN_CPU := $(strip $(subst ",,$(CONFIG_BFIN_CPU)))
CONFIG_BFIN_BOOT_MODE := $(strip $(subst ",,$(CONFIG_BFIN_BOOT_MODE)))
CONFIG_ENV_OFFSET := $(strip $(subst ",,$(CONFIG_ENV_OFFSET)))
CONFIG_ENV_SIZE := $(strip $(subst ",,$(CONFIG_ENV_SIZE)))

PLATFORM_RELFLAGS += -ffixed-P3 -fomit-frame-pointer -mno-fdpic
PLATFORM_CPPFLAGS += -DCONFIG_BLACKFIN

LDFLAGS += --gc-sections -m elf32bfin
PLATFORM_RELFLAGS += -ffunction-sections -fdata-sections

ifneq (,$(CONFIG_BFIN_CPU))
PLATFORM_RELFLAGS += -mcpu=$(CONFIG_BFIN_CPU)
endif

ifneq ($(CONFIG_BFIN_BOOT_MODE),BFIN_BOOT_BYPASS)
ALL += $(obj)u-boot.ldr
endif
ifeq ($(CONFIG_ENV_IS_EMBEDDED_IN_LDR),y)
CREATE_LDR_ENV = $(obj)tools/envcrc --binary > $(obj)env-ldr.o
else
CREATE_LDR_ENV =
endif

SYM_PREFIX = _

LDR_FLAGS-y :=
LDR_FLAGS-$(CONFIG_BFIN_BOOTROM_USES_EVT1) += -J

LDR_FLAGS += --bmode $(subst BFIN_BOOT_,,$(CONFIG_BFIN_BOOT_MODE))
LDR_FLAGS += --use-vmas
LDR_FLAGS += --initcode $(obj)$(CPUDIR)/initcode.o
ifneq ($(CONFIG_BFIN_BOOT_MODE),BFIN_BOOT_UART)
LDR_FLAGS-$(CONFIG_ENV_IS_EMBEDDED_IN_LDR) += \
	--punchit $$(($(CONFIG_ENV_OFFSET))):$$(($(CONFIG_ENV_SIZE))):$(obj)env-ldr.o
endif
ifneq (,$(findstring s,$(MAKEFLAGS)))
LDR_FLAGS += --quiet
endif

LDR_FLAGS += $(LDR_FLAGS-y)

ifeq ($(wildcard $(TOPDIR)/board/$(BOARD)/u-boot.lds*),)
LDSCRIPT = $(obj)arch/$(ARCH)/lib/u-boot.lds.S
endif
