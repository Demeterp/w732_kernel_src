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



include $(TOPDIR)/config.mk
include $(TOPDIR)/onenand_ipl/board/$(BOARDDIR)/config.mk

LDSCRIPT= $(TOPDIR)/onenand_ipl/board/$(BOARDDIR)/u-boot.onenand.lds
LDFLAGS	= -Bstatic -T $(onenandobj)u-boot.lds -Ttext $(TEXT_BASE) $(PLATFORM_LDFLAGS)
AFLAGS	+= -DCONFIG_PRELOADER -DCONFIG_ONENAND_IPL
CFLAGS	+= -DCONFIG_PRELOADER -DCONFIG_ONENAND_IPL
OBJCFLAGS += --gap-fill=0x00

SOBJS	:= low_levelinit.o
SOBJS	+= start.o
COBJS	:= apollon.o
COBJS	+= onenand_read.o
COBJS	+= onenand_boot.o

SRCS	:= $(addprefix $(obj),$(SOBJS:.o=.S) $(COBJS:.o=.c))
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
__OBJS	:= $(SOBJS) $(COBJS)
LNDIR	:= $(OBJTREE)/onenand_ipl/board/$(BOARDDIR)

onenandobj	:= $(OBJTREE)/onenand_ipl/

ALL	= $(onenandobj)onenand-ipl $(onenandobj)onenand-ipl.bin $(onenandobj)onenand-ipl-2k.bin $(onenandobj)onenand-ipl-4k.bin

all:	$(obj).depend $(ALL)

$(onenandobj)onenand-ipl-2k.bin:	$(onenandobj)onenand-ipl
	$(OBJCOPY) ${OBJCFLAGS} --pad-to=0x800 -O binary $< $@

$(onenandobj)onenand-ipl-4k.bin:	$(onenandobj)onenand-ipl
	$(OBJCOPY) ${OBJCFLAGS} --pad-to=0x1000 -O binary $< $@

$(onenandobj)onenand-ipl.bin:	$(onenandobj)onenand-ipl
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(onenandobj)onenand-ipl:	$(OBJS) $(onenandobj)u-boot.lds
	cd $(LNDIR) && $(LD) $(LDFLAGS) $$UNDEF_SYM $(__OBJS) \
		-Map $@.map -o $@

$(onenandobj)u-boot.lds:	$(LDSCRIPT)
	$(CPP) $(CPPFLAGS) $(LDPPFLAGS) -ansi -D__ASSEMBLY__ -P - <$^ >$@

# create symbolic links from common files

# from cpu directory
$(obj)start.S:
	@rm -f $@
	ln -s $(SRCTREE)/$(CPUDIR)/start.S $@

# from onenand_ipl directory
$(obj)onenand_ipl.h:
	@rm -f $@
	ln -s $(SRCTREE)/onenand_ipl/onenand_ipl.h $@

$(obj)onenand_boot.c:	$(obj)onenand_ipl.h
	@rm -f $@
	ln -s $(SRCTREE)/onenand_ipl/onenand_boot.c $@

$(obj)onenand_read.c:	$(obj)onenand_ipl.h
	@rm -f $@
	ln -s $(SRCTREE)/onenand_ipl/onenand_read.c $@

ifneq ($(OBJTREE), $(SRCTREE))
$(obj)apollon.c:
	@rm -f $@
	ln -s $(SRCTREE)/onenand_ipl/board/$(BOARDDIR)/apollon.c $@

$(obj)low_levelinit.S:
	@rm -f $@
	ln -s $(SRCTREE)/onenand_ipl/board/$(BOARDDIR)/low_levelinit.S $@
endif

#########################################################################

$(obj)%.o:	$(obj)%.S
	$(CC) $(AFLAGS) -c -o $@ $<

$(obj)%.o:	$(obj)$.c
	$(CC) $(CFLAGS) -c -o $@ $<

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
