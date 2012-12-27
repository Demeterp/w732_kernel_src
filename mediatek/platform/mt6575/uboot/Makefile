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

LIB	= $(obj)lib$(SOC).a

COBJS := mt6575.o \
	mt6575_board.o \
	mt65xx_leds.o \
	boot_mode.o \
	load_image.o \
	addr_trans.o \
	mmc_core.o \
	mmc_test.o \
	msdc_utils.o \
	msdc.o \
	mtk_serial.o \
	mtk_timer.o \
	mt6575_nand.o \
	mt6575_meta.o \
	mt6575_key.o \
	mt6575_rtc.o \
	mt6575_recovery.o \
	mt6575_factory.o \
	mt6575_auxadc.o\
	mtk_wdt.o\
	mt6575_gpio.o \
	mt6575_gpio_init.o \
	mt6575_pwm.o \
	mt6575_i2c.o \
    mt6575_disp_drv.o \
 	mt6575_disp_assert_layer.o \
    mt6575_disp_drv_dbi.o \
   	mt6575_lcd_drv.o \
  mt6575_disp_drv_dsi.o \
 	mt6575_dsi_drv.o \
        mt6575_disp_drv_dpi.o \
 	mt6575_dpi_drv.o \
    mt6575_logo.o \
    mt6575_pll.o \
  bmt.o \
  partition_mt.o \
  mt6575_sleep.o \
  irq.o
  
ifeq ($(MTK_NCP1851_SUPPORT),yes) 
    COBJS += mt6575_bat_tbl.o mt6575_i2c0.o ncp1851.o
else
    ifeq ($(MTK_FAN5405_SUPPORT),yes)
        COBJS += mt6575_bat_fan5405.o mt6575_i2c0.o fan5405.o
    else
        COBJS += mt6575_bat.o
    endif
endif

ifeq ($(CUSTOM_KERNEL_TOUCHPANEL),GT827)
COBJS += mt6575_pmic6329_tbl.o
else
COBJS += mt6575_pmic6329.o
endif

###	custom/$(MTK_PROJECT)/mt65xx_partition.o \
			
SOBJS	:= lowlevel_init.o

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
SOBJS	:= $(addprefix $(obj),$(SOBJS))

$(LIB):	$(obj).depend $(OBJS) $(SOBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS) $(SOBJS)

clean:
	rm -f $(SOBJS) $(OBJS)

distclean:	clean
	rm -f $(LIB) core *.bak $(obj).depend

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
