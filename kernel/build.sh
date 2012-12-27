#!/bin/bash
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

# Default settings
verfile="android.ver"
curcfg=".config"
release="n"
rebuild="n"
clean="n"
makeflags="-w"
makedefs="V=0"
makejobs=${MAKEJOBS}
curdir=`pwd`

usage() {
    echo "Usage: $0 {release|rebuild|clean|silent|verbose|single} [config-xxx]"
    echo "  config file will be generated if build with TARGET_PRODUCT"
    exit 1
}

make_clean() {
    echo "**** Cleaning ****"
    nice make ${makeflags} ${makedefs} distclean
}


# Main starts here
while test -n "$1"; do
    case "$1" in
    release)
        release="y"
    ;;
    rebuild)
        rebuild="y"
    ;;
    clean)
        clean="y"
    ;;
    silent)
        makeflags="-ws"
        makedefs="V=0"
    ;;
    verbose)
        makeflags="-w"
        makedefs="V=1"
    ;;
    single)
        makejobs=""
    ;;
    *)
        export TARGET_PRODUCT=$1
    ;;
    esac
    shift
done

source ../mediatek/build/shell.sh ../ kernel
defcfg="${MTK_ROOT_GEN_CONFIG}/kconfig"


if [ ! -z $KMOD_PATH ]; then
  if [ ! -e $KMOD_PATH ]; then
     echo "Invalid KMOD_PATH:$KMOD_PATH"
     echo "CURDIR=$curdir"
     exit 1;
  fi
fi

# clean if it is necessary
if [ "${clean}" == "y" ]; then
   if [ ! -z $KMOD_PATH ]; then
      echo "Clean kernel module PROJECT=$MTK_PROJECT PATH=$KMOD_PATH";
      make M="$KMOD_PATH" clean
      exit $?
   else
      make_clean; exit $?;
   fi
fi

if [ "${rebuild}" == "y" ]; then make_clean; fi

echo "**** Configuring / $defcfg / ****"
# select correct configuration file
make mediatek-configs

# Config DRAM size according to central Project Configuration file setting
# Todo:
# Need a robust mechanism to control Kconfig content by central Config setting
# Move below segment to a configuration file for extension
if [ "$CUSTOM_DRAM_SIZE" == "3G" ]; then
    # Config DRAM size as 3G (0x18000000).
    sed --in-place=.orig \
        -e 's/\(CONFIG_MAX_DRAM_SIZE_SUPPORT=\).*/\10x18000000/' \
        .config
else
  if [ "$CUSTOM_DRAM_SIZE" == "2G" ]; then
      # Config DRAM size as 2G (0x10000000).
      sed --in-place=.orig \
          -e 's/\(CONFIG_MAX_DRAM_SIZE_SUPPORT=\).*/\10x10000000/' \
          -e 's/\(CONFIG_RESERVED_MEM_SIZE_FOR_PMEM=\).*/\10x1700000/' \
          .config
  else
    if [ "$CUSTOM_DRAM_SIZE" == "4G" ]; then
        # Config DRAM size as 4G (0x20000000).
        sed --in-place=.orig \
            -e 's/\(CONFIG_MAX_DRAM_SIZE_SUPPORT=\).*/\10x20000000/' \
            .config
    else
      if [ "$CUSTOM_DRAM_SIZE" == "6G" ]; then
          # Config DRAM size as 6G (0x30000000).
          sed --in-place=.orig \
              -e 's/\(CONFIG_MAX_DRAM_SIZE_SUPPORT=\).*/\10x30000000/' \
              -e 's/\(CONFIG_CMDLINE=.*\)"/\1 vmalloc=280M"/' \
              -e 's/.*\(CONFIG_HIGHMEM\).*/\1=y/' \
              -e '$ a\# CONFIG_HIGHPTE is not set' \
              -e '$ a\# CONFIG_DEBUG_HIGHMEM is not set' \
              .config
      else
        if [ "$CUSTOM_DRAM_SIZE" == "8G" ]; then
            # Config DRAM size as 8G (0x40000000).
            sed --in-place=.orig \
                -e 's/\(CONFIG_MAX_DRAM_SIZE_SUPPORT=\).*/\10x40000000/' \
                -e 's/\(CONFIG_CMDLINE=.*\)"/\1 vmalloc=280M"/' \
                -e 's/.*\(CONFIG_HIGHMEM\).*/\1=y/' \
                -e '$ a\# CONFIG_HIGHPTE is not set' \
                -e '$ a\# CONFIG_DEBUG_HIGHMEM is not set' \
                .config
        fi
      fi
    fi
  fi
fi

# update configuration
nice make ${makeflags} ${makedefs} silentoldconfig

if [ ! -z $KMOD_PATH ]; then
  echo "Build kernel module PROJECT=$MTK_PROJECT PATH=$KMOD_PATH";
  make M="$KMOD_PATH" modules
  exit $?
fi

echo "**** Building ****"
make ${makeflags} ${makejobs} ${makedefs}

if [ $? -ne 0 ]; then exit 1; fi

echo "**** Successfully built kernel ****"

mkimg="${MTK_ROOT_BUILD}/tools/mkimage"
kernel_img="${curdir}/arch/arm/boot/Image"
kernel_zimg="${curdir}/arch/arm/boot/zImage"

echo "**** Generate download images ****"

if [ ! -x ${mkimg} ]; then chmod a+x ${mkimg}; fi

${mkimg} ${kernel_zimg} KERNEL > kernel_${MTK_PROJECT}.bin

copy_to_legacy_download_flash_folder   kernel_${MTK_PROJECT}.bin rootfs_${MTK_PROJECT}.bin
