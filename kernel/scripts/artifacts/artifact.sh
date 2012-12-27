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


function Usage
{
    cat<<HELP
Usage: kernel/scripts/artifacts/`basename $0` scope project
Description:
    project
        project represent the correct project name
    scope
        scope represent the chose scope,for example:oversea
Example:
   kernel/scripts/artifacts/`basename $0` oversea oppo
   kernel/scripts/artifacts/`basename $0` other generic
Attention:
    this tool need 2 arguments
    for emulator,please use generic for the project argument
HELP
}

while getopts "h" OPT;do
case $OPT in
h) Usage;exit 1 ;;
?) Usage;exit 1 ;;
esac
done
shift `expr $OPTIND - 1`
if [ $# != 2 ];then
    Usage
    exit 1
fi

arguments=(oversea other)
flag=
for arg in ${arguments};do
    if [ "$1" = "${arg}" ];then
        flag=true
        break
    fi
done
if [ -z "$flag" ];then
    echo "Error!the first argument must in \"${arguments[@]}\""
    exit 1
fi

prjConfig="../mediatek/config/$2/ProjectConfig.mk"
if [ ! -e "${prjConfig}" ]; then
    echo "Error!the argument $2 is not a correct project name!"
    echo "please enter -h for help infomation!"
    exit 1
fi

outdir=../vendor/mediatek/$2/artifacts/kernel
outdir_obj=$outdir/obj
if [[ $1 == "other" ]]; then
  mkdir -p arch/arm/mach-mt3351
  touch arch/arm/mach-mt3351/Kconfig
  touch arch/arm/mach-mt3351/Makefile
  mkdir -p drivers/actuator
  touch drivers/actuator/Kconfig
  touch drivers/actuator/Makefile
  mkdir -p drivers/meta
  touch drivers/meta/Kconfig
  touch drivers/meta/Makefile
  mkdir -p drivers/monitor
  touch drivers/monitor/Kconfig
  touch drivers/monitor/Makefile
fi 
rm -rf $outdir_obj
mkdir -p $outdir_obj

cp -f ../mediatek/platform/mt6516/kernel/core/ccci*o $outdir_obj
cp -f ../mediatek/platform/mt6516/kernel/core/prebuilt/ccci_fs_utils.o $outdir_obj
cp -f ../mediatek/platform/mt6516/kernel/core/MT6516_PM_api.o $outdir_obj
cp -f ../mediatek/platform/mt6516/kernel/core/mt6516_intr.o $outdir_obj
cp -f ../mediatek/platform/mt6516/kernel/core/MT6516_sleep.o $outdir_obj
#touch ../mediatek/platform/mt6516/kernel/core/aed.o
#cp -f ../mediatek/source/kernel/drivers/aee/aed.o $outdir_obj
if [ -e ../mediatek/source/kernel/drivers/net/mt592x/wlan/gl_sec.o ]; then
  cp -f ../mediatek/source/kernel/drivers/net/mt592x/wlan/gl_sec.o $outdir_obj
fi
if [[ $1 == "other" ]]; then
  cp -f arch/arm/mach-mt6516/core.o $outdir_obj
  cp -f arch/arm/mach-mt6516/pwm.o $outdir_obj
  cp -f arch/arm/mach-mt6516/dma.o $outdir_obj
  cp -f arch/arm/mach-mt6516/mt6516_pll.o $outdir_obj
  cp -f arch/arm/mach-mt6516/MT6516_sleep.o $outdir_obj
  cp -f arch/arm/mach-mt6516/mt6516_wdt.o $outdir_obj
  cp -f arch/arm/mach-mt6516/mt6516_timer.o $outdir_obj
  cp -f arch/arm/mach-mt6516/gpt.o $outdir_obj
  cp -f arch/arm/mach-mt6516/mt6516_IDP.o $outdir_obj
  cp -f arch/arm/mach-mt6516/mt6516_ISP.o $outdir_obj
  cp -f arch/arm/mach-mt6516/system.o $outdir_obj
  cp -f arch/arm/mach-mt6516/mt6516_busmonitor.o $outdir_obj
  cp -f arch/arm/mach-mt6516/mt6516_devs.o $outdir_obj
  cp -f drivers/char/sampletrigger.o $outdir_obj
  cp -f drivers/mmc/host/mt6516_sd.o $outdir_obj
  cp -f drivers/power/smart_battery_mt6516.o $outdir_obj
fi

for item in "$outdir_obj"/*; do
  mv ${item} ${item}.artifacts
done

cp scripts/artifacts/objects-$1.mk $outdir/objects.mk

#chmod 755 scripts/Makefile.build
#echo "include binary/Makefile" >> scripts/Makefile.build
#chmod 444 scripts/Makefile.build
