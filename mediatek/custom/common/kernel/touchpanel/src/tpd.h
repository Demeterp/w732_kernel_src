/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/proc_fs.h> 
#ifdef MT6575
    //#include <mach/mt6573_pll.h>
    #include <mach/mt6575_gpio.h>
    #include <mach/mt6575_devs.h>
    #include <mach/mt6575_reg_base.h>
    #include <mach/mt6575_typedefs.h>
#endif
#ifdef MT6573
    #include <mach/mt6573_pll.h>
    #include <mach/mt6573_gpio.h>
    #include <mach/mt6573_devs.h>
    #include <mach/mt6573_reg_base.h>
    #include <mach/mt6573_typedefs.h>
#endif
#ifdef MT6516
    #include <mach/mt6516_pll.h>
    #include <mach/mt6516_gpio.h>
    #include <mach/mt6516_devs.h>
    #include <mach/mt6516_boot.h>
    #include <mach/mt6516_reg_base.h>
    #include <mach/mt6516_typedefs.h>
#endif
#include <mach/board.h>
#include <mach/irqs.h>
#include <asm/io.h>
#include <linux/platform_device.h>
#include <linux/autoconf.h>
#include <linux/kobject.h>
#include <linux/earlysuspend.h>
//#include "tpd_custom.h"

/* debug macros */
////#define TPD_DEBUG
#define TPD_DEBUG_CODE
//#define TPD_DEBUG_TRACK
#define TPD_DMESG(a,arg...) printk(TPD_DEVICE ": " a,##arg)
#if defined(TPD_DEBUG)
#undef TPD_DEBUG
#define TPD_DEBUG(a,arg...) printk(TPD_DEVICE ": " a,##arg)
#else
#define TPD_DEBUG(arg...) 
#endif

/* register, address, configurations */
#define TPD_DEVICE            "mtk-tpd"
#define TPD_X                  0
#define TPD_Y                  1
#define TPD_Z1                 2
#define TPD_Z2                 3
#define TP_DELAY              (2*HZ/100)
#define TP_DRV_MAX_COUNT          (20)
#define TPD_WARP_CNT          (4)
#define TPD_VIRTUAL_KEY_MAX   (10)

/* various mode */
#define TPD_MODE_NORMAL        0
#define TPD_MODE_KEYPAD        1
#define TPD_MODE_SW 2
#define TPD_MODE_FAV_SW 3
#define TPD_MODE_FAV_HW 4
#define TPD_MODE_RAW_DATA 5
#undef TPD_RES_X
#undef TPD_RES_Y
extern int TPD_RES_X;
extern int TPD_RES_Y;
extern int tpd_load_status ; //0: failed, 1: sucess
extern int tpd_mode;
extern int tpd_mode_axis;
extern int tpd_mode_min;
extern int tpd_mode_max;
extern int tpd_mode_keypad_tolerance;
extern int tpd_em_debounce_time;
extern int tpd_em_debounce_time0;
extern int tpd_em_debounce_time1;
extern int tpd_em_asamp;
extern int tpd_em_auto_time_interval;
extern int tpd_em_sample_cnt;
extern int tpd_calmat[];
extern int tpd_def_calmat[];
extern int tpd_calmat[];
extern int tpd_def_calmat[];
extern int TPD_DO_WARP;
extern int tpd_wb_start[];
extern int tpd_wb_end[];

struct tpd_device
{
    struct input_dev *dev;
    struct input_dev *kpd;
    struct timer_list timer;
    struct tasklet_struct tasklet;
    int btn_state;
};

struct tpd_driver_t
{
		char *tpd_device_name;
		int (*tpd_local_init)(void);
 		void (*suspend)(struct early_suspend *h);
 		void (*resume)(struct early_suspend *h);
 		int tpd_have_button;
};

#if 1 //#ifdef TPD_HAVE_BUTTON
void tpd_button(unsigned int x, unsigned int y, unsigned int down);
void tpd_button_init(void);
ssize_t tpd_virtual_key(char *buf);
//#ifndef TPD_BUTTON_HEIGHT
//#define TPD_BUTTON_HEIGHT TPD_RES_Y
//#endif
#endif

#if 0
void tpd_adc_init(void);
void tpd_set_debounce_time(int debounce_time);
void tpd_set_spl_number(int spl_num);
u16 tpd_read(int position);
u16 tpd_read_adc(u16 pos);
u16 tpd_read_status(void);
#endif

extern int tpd_driver_add(struct tpd_driver_t *tpd_drv);
extern int tpd_driver_remove(struct tpd_driver_t *tpd_drv);
void tpd_button_setting(int keycnt, void *keys, void *keys_dim);
extern int tpd_em_spl_num;
extern int tpd_em_pressure_threshold;

#ifdef TPD_DEBUG_CODE
#include "tpd_debug.h"
#endif

#ifdef TPD_HAVE_CALIBRATION
#include "tpd_calibrate.h"
#endif

#include "tpd_default.h"

/* switch touch panel into different mode */
void _tpd_switch_single_mode(void);
void _tpd_switch_multiple_mode(void);
void _tpd_switch_sleep_mode(void);
void _tpd_switch_normal_mode(void);   
