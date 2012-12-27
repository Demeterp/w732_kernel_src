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


# please see binary/Makefile for usage

obj_to_install := \
  arch/arm/mach-mt6516:core.o \
  arch/arm/mach-mt6516:pwm.o \
  arch/arm/mach-mt6516:dma.o \
  arch/arm/mach-mt6516:mt6516_pll.o \
  arch/arm/mach-mt6516:mt6516_intr.o \
  arch/arm/mach-mt6516:MT6516_PM_api.o \
  arch/arm/mach-mt6516:MT6516_sleep.o \
  arch/arm/mach-mt6516:mt6516_wdt.o \
  arch/arm/mach-mt6516:mt6516_timer.o \
  arch/arm/mach-mt6516:mt6516_devs.o \
  arch/arm/mach-mt6516:ccci.o \
  arch/arm/mach-mt6516:ccci_md.o \
  arch/arm/mach-mt6516:ccci_fs.o \
  arch/arm/mach-mt6516:ccci_tty.o \
  arch/arm/mach-mt6516:ccci_pmic.o \
  arch/arm/mach-mt6516:gpt.o \
  arch/arm/mach-mt6516:aed.o \
  arch/arm/mach-mt6516:mt6516_IDP.o \
  arch/arm/mach-mt6516:mt6516_ISP.o \
  arch/arm/mach-mt6516:system.o \
  arch/arm/mach-mt6516:mt6516_busmonitor.o \
  drivers/net/wireless/mt592x:arb_fsm.o \
  drivers/net/wireless/mt592x:gl_init.o \
  drivers/net/wireless/mt592x:hal_wlantable.o \
  drivers/net/wireless/mt592x:nic_eeprom.o \
  drivers/net/wireless/mt592x:parse.o \
  drivers/net/wireless/mt592x:rftest.o \
  drivers/net/wireless/mt592x:statistics.o \
  drivers/net/wireless/mt592x:assoc.o \
  drivers/net/wireless/mt592x:gl_kal.o \
  drivers/net/wireless/mt592x:ibss.o \
  drivers/net/wireless/mt592x:nic_hw_scan.o \
  drivers/net/wireless/mt592x:privacy.o \
  drivers/net/wireless/mt592x:roaming_fsm.o \
  drivers/net/wireless/mt592x:timer.o \
  drivers/net/wireless/mt592x:auth.o \
  drivers/net/wireless/mt592x:gl_proc.o \
  drivers/net/wireless/mt592x:join_fsm.o \
  drivers/net/wireless/mt592x:nic_hw_timer.o \
  drivers/net/wireless/mt592x:pta_fsm.o \
  drivers/net/wireless/mt592x:rsn.o \
  drivers/net/wireless/mt592x:tkip_mic.o \
  drivers/net/wireless/mt592x:bb_mt5911.o \
  drivers/net/wireless/mt592x:gl_wext.o \
  drivers/net/wireless/mt592x:mgt_buf.o \
  drivers/net/wireless/mt592x:nic_privacy.o \
  drivers/net/wireless/mt592x:pwr_mgt_fsm.o \
  drivers/net/wireless/mt592x:rx.o \
  drivers/net/wireless/mt592x:tx.o \
  drivers/net/wireless/mt592x:bb_mt5921.o \
  drivers/net/wireless/mt592x:gl_wext_priv.o \
  drivers/net/wireless/mt592x:mib.o \
  drivers/net/wireless/mt592x:nic_pta.o \
  drivers/net/wireless/mt592x:qos_enhance.o \
  drivers/net/wireless/mt592x:scan.o \
  drivers/net/wireless/mt592x:util.o \
  drivers/net/wireless/mt592x:bb_mt5922.o \
  drivers/net/wireless/mt592x:hal.o \
  drivers/net/wireless/mt592x:mt5921.o \
  drivers/net/wireless/mt592x:nic_pwr_mgt.o \
  drivers/net/wireless/mt592x:rate.o \
  drivers/net/wireless/mt592x:scan_fsm.o \
  drivers/net/wireless/mt592x:wlan_lib.o \
  drivers/net/wireless/mt592x:bss.o \
  drivers/net/wireless/mt592x:hal_hw_scan.o \
  drivers/net/wireless/mt592x:mt5922.o \
  drivers/net/wireless/mt592x:nic_rate.o \
  drivers/net/wireless/mt592x:rf_a60105.o \
  drivers/net/wireless/mt592x:sdio.o \
  drivers/net/wireless/mt592x:wlan.mod.o \
  drivers/net/wireless/mt592x:domain.o \
  drivers/net/wireless/mt592x:hal_pta.o \
  drivers/net/wireless/mt592x:mt592x.o \
  drivers/net/wireless/mt592x:nic_rx.o \
  drivers/net/wireless/mt592x:rf_al2236.o \
  drivers/net/wireless/mt592x:sec_fsm.o \
  drivers/net/wireless/mt592x:wlan_oid.o \
  drivers/net/wireless/mt592x:dump.o \
  drivers/net/wireless/mt592x:hal_pwr_mgt.o \
  drivers/net/wireless/mt592x:nic.o \
  drivers/net/wireless/mt592x:nic_tx.o \
  drivers/net/wireless/mt592x:rf_mt5921.o \
  drivers/net/wireless/mt592x:sta_record.o \
  drivers/net/wireless/mt592x:wlan_oid_rftest.o \
  drivers/char:sampletrigger.o \
  drivers/mmc/host:mt6516_sd.o \
  drivers/power:smart_battery_mt6516.o

file_to_touch := \
  arch/arm/mach-mt3351/Kconfig \
  arch/arm/mach-mt3351/Makefile \
  drivers/net/wireless/mt592x/Kconfig \
  drivers/net/wireless/mt592x/Makefile \
  drivers/actuator/Kconfig \
  drivers/actuator/Makefile \
  drivers/meta/Kconfig \
  drivers/meta/Makefile \
  drivers/monitor/Kconfig \
  drivers/monitor/Makefile

