# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

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


## Android Makefile for kernel module
## by Kirby.Wu, 20090729, mediatek.inc
##
## this android makefile is (currently) used to build all kernel modules 
## and put them into android platform output directory. 
##
## to build kernel modules into system.img,
##   config build/target/board/<target>/BoardConfig.mk:
##     KERNEL_CONFIG_FILE := <your desired config file> # use .config if omit
##     TARGET_MODULES     := true                       # do make modules
##

#ifeq ($(MTK_PROJECT), gw616)
#  KERNEL_CONFIG_FILE := config-mt6516-phone
#else
#  ifeq ($(MTK_PROJECT), ds269)
#    KERNEL_CONFIG_FILE := config-mt6516-gemini
#  else
#    ifeq ($(MTK_PROJECT), oppo)
#      KERNEL_CONFIG_FILE := config-mt6516-oppo
#    else
#      ifeq ($(MTK_PROJECT), mt6516_evb)
#        KERNEL_CONFIG_FILE := config-mt6516-evb
#      else
#        ifeq ($(MTK_PROJECT), mt6573_evb)
#          KERNEL_CONFIG_FILE := config-mt6573-evb
#        else
#          ifeq ($(MTK_PROJECT), zte73v1)
#            KERNEL_CONFIG_FILE := config-mt6573-zte73v1
#          else
#            KERNEL_CONFIG_FILE := config-mt6516-$(MTK_PROJECT)
#          endif
#        endif
#      endif
#    endif
#  endif
#endif

$(info using $(KERNEL_CONFIG_FILE) .... )
ifeq ($(TARGET_KMODULES),true)
ALL_PREBUILT += $(TARGET_OUT)/lib/modules/modules.order
$(BUILT_SYSTEMIMAGE): kernel_modules
$(TARGET_OUT)/lib/modules/modules.order: kernel_modules
kernel_modules:
	@echo "building linux kernel modules..."
#ifneq (,$(KERNEL_CONFIG_FILE))
#	@cat kernel/$(KERNEL_CONFIG_FILE) > kernel/.config
#endif
	make MTK_PROJECT=$(MTK_PROJECT) -C  kernel modules
	INSTALL_MOD_STRIP=1 MTK_PROJECT=$(MTK_PROJECT) INSTALL_MOD_PATH=../$(TARGET_OUT) INSTALL_MOD_DIR=../$(TARGET_OUT) make -C kernel android_modules_install
endif
