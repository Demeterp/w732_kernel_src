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


#
# Makefile for the linux kernel.
#

# Fix-me: marked others for early porting
obj-y := core.o irq.o system.o timer.o mt6575_devs.o mt6575_board.o mt6575_dma.o mt6575_boot.o mt6575_gpt.o clock.o mt6575_eint.o mt6575_pm_init.o mt6575_outercache.o mt6575_ca9_power.o mt6575_pwm.o sbchk_base.o
#obj-y := core.o irq.o system.o timer.o mt6575_devs.o mt6575_board.o mt6575_gpt.o clock.o mt6575_dma.o mt6575_eint.o mt6575_gpio.o mt6575_outercache.o mt6575_ca9_power.o mt6575_pwm.o
obj-y += mt_rfkill.o
#obj-y += kdump_sdhc_fs.o
obj-y += mt6575_isp.o
obj-y += mt6575_sysram.o 
obj-y += mt6575_gpio.o
obj-y += mt6575_eis.o
obj-y += mt6575_fdvt.o
obj-y += mt6575_mdp.o
obj-y += mt6575_mdp_kernel_trap.o
obj-y += mt6575_emi_bwl.o
obj-y += mt6575_res_mgr.o
ifneq ($(CONFIG_MT6575_FPGA),y)
ifneq ($(CONFIG_MT6575_ESL),y)
obj-y += mt6575_emi_mpu.o
endif
endif
#obj-y += mt6575_boot.o
obj-y += mt6575_pm_ldo.o mt6575_pmic_feature_api.o
obj-y += mt6575_auxadc.o
obj-y += mt6575_dcm.o mt6575_clock_manager.o mt6575_pm_log.o
obj-y += cpufreq-mt6575.o mt6575_thermal.o mt6575_cpe.o
obj-y += mt6575_sleep.o mt6575_sc.o
obj-y += ca9_power.o
obj-y += mt6575_wdt.o
#ifneq ($(MTK_EMMC_SUPPORT),yes)
#obj-y += sbchk_base.o
#endif
obj-$(CONFIG_MT65XX_TRACER) += armV7_perf_mon.o mt6575_emi_bm.o mt6575_mon.o
obj-y += innercache.o
obj-y += ple.o
obj-y += dram_overclock.o
obj-y += mtk_ccci_helper.o
obj-y += mt6575_dramc.o
obj-$(CONFIG_FIQ_GLUE) += mt6575_fiq.o

ifeq ($(MTK_BT_SUPPORT), yes)
# MTK COMBO chip or not
ifeq ($(CONFIG_MTK_COMBO),)
obj-y += mt_bt.o
endif
endif

ccflags-y += -I$(MTK_PATH_PLATFORM)/drivers/power \
             -I$(MTK_PATH_SOURCE)/drivers/wdk
obj-y += mt6575_dormant.o cpu_dormant.o
KBUILD_AFLAGS	:=$(KBUILD_AFLAGS:-msoft-float=-Wa,-mfpu=neon)
