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


if [[ $1 == "other" ]]; then
  rm -rf arch/arm/mach-mt3351
  rm -rf drivers/actuator
  rm -rf drivers/meta
  rm -rf drivers/monitor
  rm -f  arch/arm/mach-mt6516/include/mach/mt6516_IDP.h
  rm -f  arch/arm/mach-mt6516/include/mach/mt6516_ISP.h
  rm -f  arch/arm/mach-mt6516/include/mach/mt6516_auxadc_hw.h
  rm -f  arch/arm/mach-mt6516/include/mach/mt6516_auxadc_sw.h
  rm -f  arch/arm/mach-mt6516/include/mach/mt6516_gpt_sw.h
  rm -f  arch/arm/mach-mt6516/include/mach/mt6516_wdt.h
  rm -f  arch/arm/mach-mt6516/include/mach/mt6516_busmonitor.h

  rm -f arch/arm/mach-mt6516/core.c
  rm -f arch/arm/mach-mt6516/pwm.c
  rm -f arch/arm/mach-mt6516/dma.c
  rm -f arch/arm/mach-mt6516/mt6516_pll.c
  rm -f arch/arm/mach-mt6516/mt6516_wdt.c
  rm -f arch/arm/mach-mt6516/mt6516_timer.c
  rm -f arch/arm/mach-mt6516/gpt.c
  rm -f arch/arm/mach-mt6516/mt6516_IDP.c
  rm -f arch/arm/mach-mt6516/mt6516_ISP.c
  rm -f arch/arm/mach-mt6516/system.c
  rm -f arch/arm/mach-mt6516/mt6516_busmonitor.c
  rm -f arch/arm/mach-mt6516/mt6516_devs.c
  rm -f drivers/char/sampletrigger.c
  rm -f drivers/mmc/host/mt6516_sd.c
  rm -f drivers/power/smart_battery_mt6516.c
fi

rm -f arch/arm/mach-mt6516/MT6516_sleep.c
rm -f arch/arm/mach-mt6516/mt6516_intr.c
rm -f arch/arm/mach-mt6516/MT6516_PM_api.c
rm -f arch/arm/mach-mt6516/ccci*c
rm -f drivers/net/wireless/mt592x/*.c

rm -f arch/arm/mach-mt6516/include/mach/ccci*.h
rm -f arch/arm/mach-mt6516/include/mach/ccif.h
