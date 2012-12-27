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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <linux/string.h>
#include <linux/time.h>
#include <linux/uaccess.h>
#include <linux/fb.h>

#include <linux/debugfs.h>

#include <disp_drv_platform.h>
#include "disp_drv_log.h"

#include "lcd_drv.h"
#include "lcd_reg.h"
#include "dpi_drv.h"
#include "dpi_reg.h"
#include "dsi_drv.h"
#include "dsi_reg.h"

#if defined(MTK_TVOUT_SUPPORT)
#include "tv_out.h"
#include "tvc_drv.h"
#include "tve_drv.h"
#include "tvrot_drv.h"
#endif

#include "debug.h"

// ---------------------------------------------------------------------------
//  External variable declarations
// ---------------------------------------------------------------------------

extern long tpd_last_down_time;
extern int  tpd_start_profiling;
extern void mtkfb_log_enable(int enable);
extern void disp_log_enable(int enable);
extern void dbi_log_enable(int enable);
extern void mtkfb_m4u_switch(bool enable);
extern void mtkfb_m4u_dump(void);
extern void mtkfb_capture_fb_only(bool enable);
extern void esd_recovery_pause(BOOL en);
extern int mtkfb_set_backlight_mode(unsigned int mode);
#if defined (MTK_TVOUT_SUPPORT)
bool capture_tv_buffer = false;
#endif

extern LCM_DRIVER *lcm_drv;
// ---------------------------------------------------------------------------
//  Debug Options
// ---------------------------------------------------------------------------

static const long int DEFAULT_LOG_FPS_WND_SIZE = 30;

typedef struct {
    unsigned int en_fps_log;
    unsigned int en_touch_latency_log;
    unsigned int log_fps_wnd_size;
    unsigned int force_dis_layers;
} DBG_OPTIONS;

static DBG_OPTIONS dbg_opt = {0};

static char STR_HELP[] =
    "\n"
    "USAGE\n"
    "        echo [ACTION]... > mtkfb\n"
    "\n"
    "ACTION\n"
    "        fps:[on|off]\n"
    "             enable fps and lcd update time log\n"
    "\n"
    "        tl:[on|off]\n"
    "             enable touch latency log\n"
    "\n"
    "        layer\n"
    "             dump lcd layer information\n"
    "\n"
    "        suspend\n"
    "             enter suspend mode\n"
    "\n"
    "        resume\n"
    "             leave suspend mode\n"
    "\n"
    "        lcm:[on|off|init]\n"
    "             power on/off lcm\n"
    "\n"
    "        cabc:[ui|mov|still]\n"
    "             cabc mode, UI/Moving picture/Still picture\n"
    "\n"
    "        lcd:[on|off]\n"
    "             power on/off display engine\n"
    "\n"
    "        te:[on|off]\n"
    "             turn on/off tearing-free control\n"
    "\n"
    "        tv:[on|off]\n"
    "             turn on/off tv-out\n"
    "\n"
    "        tvsys:[ntsc|pal]\n"
    "             switch tv system\n"
    "\n"
    "        reg:[lcd|dpi|dsi|tvc|tve]\n"
    "             dump hw register values\n"
    "\n"
    "        regw:addr=val\n"
    "             write hw register\n"
    "\n"
    "        regr:addr\n"
    "             read hw register\n"
    "\n"     
    "       cpfbonly:[on|off]\n"
    "             capture UI layer only on/off\n"
    "\n"     
    "       esd:[on|off]\n"
    "             esd kthread on/off\n"
    ;


// ---------------------------------------------------------------------------
//  Information Dump Routines
// ---------------------------------------------------------------------------

static __inline int is_layer_enable(unsigned int roi_ctl, unsigned int layer)
{
    return (roi_ctl >> (31 - layer)) & 0x1;
}


static __inline const char *narrate_lcd_layer_format(unsigned int format)
{
        ///TODO: if we have a new format support, should wrap with HW feature option, such as, MTK_HW_LCD_SUPPORT_RGBXXX_LAYER_FORMAT
        switch(format)
        {
   
            case LCD_LAYER_FORMAT_RGB565    : return "RGB565";
            case LCD_LAYER_FORMAT_YUYV422   : return "YUYV422";
            case LCD_LAYER_FORMAT_RGB888    : return "RGB888";
            case LCD_LAYER_FORMAT_ARGB8888  : return "ARGB8888";
            case LCD_LAYER_FORMAT_PARGB8888 : return "PARGB8888";
            case LCD_LAYER_FORMAT_xRGB8888  : return "xRGB8888";
            default : ASSERT(0);
        }
}


static void dump_lcd_layer_info(void)
{
    unsigned int roi_ctl = AS_UINT32(&LCD_REG->WROI_CONTROL);

    unsigned int i = 0;

    DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", 
           "------------------------------------------\n"
           "[mtkfb] dump lcd layer information\n"
           "------------------------------------------\n");

    for (i = 0; i < 6; ++ i)
    {
        unsigned int layer_en = is_layer_enable(roi_ctl, i);
        
        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "Layer[%d] is %s\n", i, layer_en ? "enabled" : "disabled");
        if (!layer_en) continue;

        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "   size   : %d x %d\n", LCD_REG->LAYER[i].SIZE.WIDTH,
                                        LCD_REG->LAYER[i].SIZE.HEIGHT);
        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "   offset : (%d, %d)\n", LCD_REG->LAYER[i].OFFSET.X,
                                         LCD_REG->LAYER[i].OFFSET.Y);
        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "   format : %s\n", 
               narrate_lcd_layer_format(LCD_REG->LAYER[i].CONTROL.CLRDPT));
        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "   dlink  : %d\n", 
               (AS_UINT32(&LCD_REG->WROI_DC) >> (31 - i)) & 0x1);
    }

    DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "\n");
}


// ---------------------------------------------------------------------------
//  FPS Log
// ---------------------------------------------------------------------------

typedef struct {
    long int current_lcd_time_us;
    long int current_te_delay_time_us;
    long int total_lcd_time_us;
    long int total_te_delay_time_us;
    long int start_time_us;
    long int trigger_lcd_time_us;
    unsigned int trigger_lcd_count;

    long int current_hdmi_time_us;
    long int total_hdmi_time_us;
    long int hdmi_start_time_us;
    long int trigger_hdmi_time_us;
    unsigned int trigger_hdmi_count;
} FPS_LOGGER;

static FPS_LOGGER fps = {0};
static FPS_LOGGER hdmi_fps = {0};

static long int get_current_time_us(void)
{
    struct timeval t;
    do_gettimeofday(&t);
    return (t.tv_sec & 0xFFF) * 1000000 + t.tv_usec;
}


static void __inline reset_fps_logger(void)
{
    memset(&fps, 0, sizeof(fps));
}

static void __inline reset_hdmi_fps_logger(void)
{
    memset(&hdmi_fps, 0, sizeof(hdmi_fps));
}

void DBG_OnTriggerLcd(void)
{
    if (!dbg_opt.en_fps_log && !dbg_opt.en_touch_latency_log) return;
    
    fps.trigger_lcd_time_us = get_current_time_us();
    if (fps.trigger_lcd_count == 0) {
        fps.start_time_us = fps.trigger_lcd_time_us;
    }
}

void DBG_OnTriggerHDMI(void)
{
    if (!dbg_opt.en_fps_log && !dbg_opt.en_touch_latency_log) return;
    
    hdmi_fps.trigger_hdmi_time_us = get_current_time_us();
    if (hdmi_fps.trigger_hdmi_count == 0) {
        hdmi_fps.hdmi_start_time_us = hdmi_fps.trigger_hdmi_time_us;
    }
}

void DBG_OnTeDelayDone(void)
{
    long int time;
    
    if (!dbg_opt.en_fps_log && !dbg_opt.en_touch_latency_log) return;

    time = get_current_time_us();
    fps.current_te_delay_time_us = (time - fps.trigger_lcd_time_us);
    fps.total_te_delay_time_us += fps.current_te_delay_time_us;
}


void DBG_OnLcdDone(void)
{
    long int time;

    if (!dbg_opt.en_fps_log && !dbg_opt.en_touch_latency_log) return;

    // deal with touch latency log

    time = get_current_time_us();
    fps.current_lcd_time_us = (time - fps.trigger_lcd_time_us);

#if 0   // FIXME
    if (dbg_opt.en_touch_latency_log && tpd_start_profiling) {

        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "Touch Latency: %ld ms\n", 
               (time - tpd_last_down_time) / 1000);

        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "LCD update time %ld ms (TE delay %ld ms + LCD %ld ms)\n",
               fps.current_lcd_time_us / 1000,
               fps.current_te_delay_time_us / 1000,
               (fps.current_lcd_time_us - fps.current_te_delay_time_us) / 1000);
        
        tpd_start_profiling = 0;
    }
#endif

    if (!dbg_opt.en_fps_log) return;

    // deal with fps log
        
    fps.total_lcd_time_us += fps.current_lcd_time_us;
    ++ fps.trigger_lcd_count;

    if (fps.trigger_lcd_count >= dbg_opt.log_fps_wnd_size) {

        long int f = fps.trigger_lcd_count * 100 * 1000 * 1000 
                     / (time - fps.start_time_us);

        long int update = fps.total_lcd_time_us * 100 
                          / (1000 * fps.trigger_lcd_count);

        long int te = fps.total_te_delay_time_us * 100 
                      / (1000 * fps.trigger_lcd_count);

        long int lcd = (fps.total_lcd_time_us - fps.total_te_delay_time_us) * 100
                       / (1000 * fps.trigger_lcd_count);

        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "MTKFB FPS: %ld.%02ld, Avg. update time: %ld.%02ld ms "
               "(TE delay %ld.%02ld ms, LCD %ld.%02ld ms)\n",
               f / 100, f % 100,
               update / 100, update % 100,
               te / 100, te % 100,
               lcd / 100, lcd % 100);
        reset_fps_logger();
    }
}

void DBG_OnHDMIDone(void)
{
    long int time;

    if (!dbg_opt.en_fps_log && !dbg_opt.en_touch_latency_log) return;

    // deal with touch latency log

    time = get_current_time_us();
    hdmi_fps.current_hdmi_time_us = (time - hdmi_fps.trigger_hdmi_time_us);


    if (!dbg_opt.en_fps_log) return;

    // deal with fps log
        
    hdmi_fps.total_hdmi_time_us += hdmi_fps.current_hdmi_time_us;
    ++ hdmi_fps.trigger_hdmi_count;

    if (hdmi_fps.trigger_hdmi_count >= dbg_opt.log_fps_wnd_size) {

        long int f = hdmi_fps.trigger_hdmi_count * 100 * 1000 * 1000 
                     / (time - hdmi_fps.hdmi_start_time_us);

        long int update = hdmi_fps.total_hdmi_time_us * 100 
                          / (1000 * hdmi_fps.trigger_hdmi_count);

        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "[HDMI] FPS: %ld.%02ld, Avg. update time: %ld.%02ld ms\n",
               f / 100, f % 100,
               update / 100, update % 100);
        
        reset_hdmi_fps_logger();
    }
}

// ---------------------------------------------------------------------------
//  Command Processor
// ---------------------------------------------------------------------------

static void process_dbg_opt(const char *opt)
{
    if (0 == strncmp(opt, "fps:", 4))
    {
        if (0 == strncmp(opt + 4, "on", 2)) {
            dbg_opt.en_fps_log = 1;
        } else if (0 == strncmp(opt + 4, "off", 3)) {
            dbg_opt.en_fps_log = 0;
        } else {
            goto Error;
        }
        reset_fps_logger();
    }
    else if (0 == strncmp(opt, "tl:", 3))
    {
        if (0 == strncmp(opt + 3, "on", 2)) {
            dbg_opt.en_touch_latency_log = 1;
        } else if (0 == strncmp(opt + 3, "off", 3)) {
            dbg_opt.en_touch_latency_log = 0;
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "suspend", 4))
    {
        DISP_PanelEnable(FALSE);
        DISP_PowerEnable(FALSE);
    }
    else if (0 == strncmp(opt, "resume", 4))
    {
        DISP_PowerEnable(TRUE);
        DISP_PanelEnable(TRUE);
    }
    else if (0 == strncmp(opt, "lcm:", 4))
    {
        if (0 == strncmp(opt + 4, "on", 2)) {
            DISP_PanelEnable(TRUE);
        } else if (0 == strncmp(opt + 4, "off", 3)) {
            DISP_PanelEnable(FALSE);
        }
		else if (0 == strncmp(opt + 4, "init", 4)) {
			if (NULL != lcm_drv && NULL != lcm_drv->init) {
        		lcm_drv->init();
    		}
        }else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "cabc:", 5))
    {
        if (0 == strncmp(opt + 5, "ui", 2)) {
			mtkfb_set_backlight_mode(1);
        }else if (0 == strncmp(opt + 5, "mov", 3)) {
			mtkfb_set_backlight_mode(3);
        }else if (0 == strncmp(opt + 5, "still", 5)) {
			mtkfb_set_backlight_mode(2);
        }else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "lcd:", 4))
    {
        if (0 == strncmp(opt + 4, "on", 2)) {
            DISP_PowerEnable(TRUE);
        } else if (0 == strncmp(opt + 4, "off", 3)) {
            DISP_PowerEnable(FALSE);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "te:", 3))
    {
        if (0 == strncmp(opt + 3, "on", 2)) {
            LCD_TE_Enable(TRUE);
        } else if (0 == strncmp(opt + 3, "off", 3)) {
            LCD_TE_Enable(FALSE);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "layer", 5))
    {
        dump_lcd_layer_info();
    }
    else if (0 == strncmp(opt, "reg:", 4))
    {
        if (0 == strncmp(opt + 4, "lcd", 3)) {
            LCD_DumpRegisters();
        } else if (0 == strncmp(opt + 4, "dpi", 3)) {
            DPI_DumpRegisters();
        } else if (0 == strncmp(opt + 4, "dsi", 3)) {
            DSI_DumpRegisters();
#if defined(MTK_TVOUT_SUPPORT)
        } else if (0 == strncmp(opt + 4, "tvc", 3)) {
            TVC_DumpRegisters();
        } else if (0 == strncmp(opt + 4, "tve", 3)) {
            TVE_DumpRegisters();
        } else if (0 == strncmp(opt + 4, "tvr", 3)) {
            TVR_DumpRegisters();
#endif		
        } else {
            goto Error;
        }
    }

#if defined(MTK_TVOUT_SUPPORT)
    else if (0 == strncmp(opt, "tv:", 3))
    {
        if (0 == strncmp(opt + 3, "on", 2)) {
            TVOUT_TvCablePlugIn();
        } else if (0 == strncmp(opt +3, "off", 3)) {
            TVOUT_TvCablePlugOut();
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvforce:", 8))
    {
        if (0 == strncmp(opt + 8, "on", 2)) {
            TVOUT_TvCablePlugIn_Directly();
        } else if (0 == strncmp(opt +8, "off", 3)) {
            TVOUT_TvCablePlugOut_Directly();
        } else {
            goto Error;
        }
    }
	else if (0 == strncmp(opt, "tvsys:", 6))
    {
        if (0 == strncmp(opt + 6, "ntsc", 4)) {
            TVOUT_SetTvSystem(TVOUT_SYSTEM_NTSC);
        } else if (0 == strncmp(opt + 6, "pal", 3)) {
            TVOUT_SetTvSystem(TVOUT_SYSTEM_PAL);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvrot:", 6))
    {
        if (0 == strncmp(opt + 6, "on", 2)) {
            TVOUT_SetOrientation(TVOUT_ROT_270);
        } else if (0 == strncmp(opt + 6, "off", 3)) {
            TVOUT_SetOrientation(TVOUT_ROT_0);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvcb:", 5))
    {
        if (0 == strncmp(opt + 5, "on", 2)) {
            TVOUT_EnableColorBar(TRUE);
        } else if (0 == strncmp(opt + 5, "off", 3)) {
            TVOUT_EnableColorBar(FALSE);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvvm:", 5))
    {
        if (0 == strncmp(opt + 5, "on", 2)) {
            TVOUT_DisableVideoMode(false);
        } else if (0 == strncmp(opt + 5, "off", 3)) {
            TVOUT_DisableVideoMode(true);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvlog:", 6))
    {
        if (0 == strncmp(opt + 6, "on", 2)) {
            TVOUT_EnableLog(true);
        } else if (0 == strncmp(opt + 6, "off", 3)) {
            TVOUT_EnableLog(false);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvtime:", 7))
    {
        if (0 == strncmp(opt + 7, "on", 2)) {
            TVOUT_EnableTimeProfiling(true);
        } else if (0 == strncmp(opt + 7, "off", 3)) {
            TVOUT_EnableTimeProfiling(false);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvcap:", 6))
    {
        if (0 == strncmp(opt + 6, "on", 2)) {
            capture_tv_buffer = true;
        } else if (0 == strncmp(opt + 6, "off", 3)) {
            capture_tv_buffer = false;
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvcam:", 6))
    {
        if (0 == strncmp(opt + 6, "on", 2)) {
            TVOUT_ForceClose();
        } else if (0 == strncmp(opt + 6, "off", 3)) {
            TVOUT_RestoreOpen();
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvuser:", 7))
    {
        if (0 == strncmp(opt + 7, "on", 2)) {
            TVOUT_TurnOn(true);
        } else if (0 == strncmp(opt + 7, "off", 3)) {
            TVOUT_TurnOn(false);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "tvcoffset:", 10))
    {
        char *p = (char *)opt + 10;
        unsigned long offset = simple_strtoul(p, &p, 10);
        TVC_SetCheckLineOffset(offset);
    }
#endif
    else if (0 == strncmp(opt, "regw:", 5))
    {
        char *p = (char *)opt + 5;
        unsigned long addr = simple_strtoul(p, &p, 16);
        unsigned long val  = simple_strtoul(p + 1, &p, 16);

        if (addr) {
            OUTREG32(addr, val);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "regr:", 5))
    {
        char *p = (char *)opt + 5;
        unsigned int addr = (unsigned int) simple_strtoul(p, &p, 16);

        if (addr) {
            DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "Read register 0x%08x: 0x%08x\n", addr, INREG32(addr));
        } else {
            goto Error;
        }
    }
	else if(0 == strncmp(opt, "bkl:", 4))
	{
		char *p = (char *)opt + 4;
		unsigned int level = (unsigned int) simple_strtoul(p, &p, 10);
		DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "process_dbg_opt(), set backlight level = %d\n", level);
		DISP_SetBacklight(level);
	}
	else if(0 == strncmp(opt, "dither:", 7))
	{
		unsigned lrs, lgs, lbs, dbr, dbg, dbb;
		char *p = (char *)opt + 7;
		lrs = (unsigned int) simple_strtoul(p, &p, 16);
		p++;
		lgs = (unsigned int) simple_strtoul(p, &p, 16);
		p++;
		lbs = (unsigned int) simple_strtoul(p, &p, 16);
		p++;
		dbr = (unsigned int) simple_strtoul(p, &p, 16);
		p++;
		dbg = (unsigned int) simple_strtoul(p, &p, 16);
		p++;
		dbb = (unsigned int) simple_strtoul(p, &p, 16);

		DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "process_dbg_opt(), %d %d %d %d %d %d\n", lrs, lgs, lbs, dbr, dbg, dbb);
#if 0 //defined(CONFIG_ARCH_MT6573)
		LCD_WaitForNotBusy();
		LCD_ConfigDither(lrs, lgs, lbs, dbr, dbg, dbb);
		LCD_StartTransfer(true);
#endif
	}
    else if (0 == strncmp(opt, "mtkfblog:", 9))
    {
        if (0 == strncmp(opt + 9, "on", 2)) {
            mtkfb_log_enable(true);
        } else if (0 == strncmp(opt + 9, "off", 3)) {
            mtkfb_log_enable(false);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "displog:", 8))
    {
        if (0 == strncmp(opt + 8, "on", 2)) {
            disp_log_enable(true);
        } else if (0 == strncmp(opt + 8, "off", 3)) {
            disp_log_enable(false);
        } else {
            goto Error;
        }
    }

    else if (0 == strncmp(opt, "lcdlog:", 7))
    {
        if (0 == strncmp(opt + 7, "on", 2)) {
            dbi_log_enable(true);
        } else if (0 == strncmp(opt + 7, "off", 3)) {
            dbi_log_enable(false);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "dpilog:", 7))
    {
        if (0 == strncmp(opt + 7, "on", 2)) {
            //dpi_log_enable(true);
        } else if (0 == strncmp(opt + 7, "off", 3)) {
            //dpi_log_enable(false);
        } else {
            goto Error;
        }
    }

    else if (0 == strncmp(opt, "log:", 4))
    {
        if (0 == strncmp(opt + 4, "on", 2)) {
			mtkfb_log_enable(true);
			disp_log_enable(true);

			dbi_log_enable(true);

            //dpi_log_enable(true);
        } else if (0 == strncmp(opt + 4, "off", 3)) {
            mtkfb_log_enable(false);
			disp_log_enable(false);

			dbi_log_enable(false);

			//dpi_log_enable(false);
        } else {
            goto Error;
        }
    }
    else if (0 == strncmp(opt, "update", 6))
    {
		DISP_UpdateScreen(0, 0, DISP_GetScreenWidth(), DISP_GetScreenHeight());
    }
 #if defined(MTK_M4U_SUPPORT)
    else if (0 == strncmp(opt, "m4u:", 4))
    {
        if (0 == strncmp(opt + 4, "on", 2)) {
            mtkfb_m4u_switch(true);
        } else if (0 == strncmp(opt + 4, "off", 3)) {
            mtkfb_m4u_switch(false);
        } else if (0 == strncmp(opt + 4, "dump", 4)) {
            mtkfb_m4u_dump();
        } else{
            goto Error;
        }
	}
#endif
    else if (0 == strncmp(opt, "cpfbonly:", 9))
    {
        if (0 == strncmp(opt + 9, "on", 2))
        {
            mtkfb_capture_fb_only(true);
        }
        else if (0 == strncmp(opt + 9, "off", 3))
        {
            mtkfb_capture_fb_only(false);
        }
    }
    else if (0 == strncmp(opt, "esd:", 4))
    {
        if (0 == strncmp(opt + 4, "on", 2))
        {
            esd_recovery_pause(FALSE);
        }
        else if (0 == strncmp(opt + 4, "off", 3))
        {
            esd_recovery_pause(TRUE);
        }
    }
    else
	{
		goto Error;
	}

    return;

Error:
    DISP_LOG_PRINT(ANDROID_LOG_INFO, "ERROR", "parse command error!\n\n%s", STR_HELP);
}


static void process_dbg_cmd(char *cmd)
{
    char *tok;
    
    DISP_LOG_PRINT(ANDROID_LOG_INFO, "DBG", "[mtkfb_dbg] %s\n", cmd);
    
    while ((tok = strsep(&cmd, " ")) != NULL)
    {
        process_dbg_opt(tok);
    }
}


// ---------------------------------------------------------------------------
//  Debug FileSystem Routines
// ---------------------------------------------------------------------------

struct dentry *mtkfb_dbgfs = NULL;


static ssize_t debug_open(struct inode *inode, struct file *file)
{
    file->private_data = inode->i_private;
    return 0;
}


static char debug_buffer[2048];

static ssize_t debug_read(struct file *file,
                          char __user *ubuf, size_t count, loff_t *ppos)
{
    const int debug_bufmax = sizeof(debug_buffer) - 1;
    int n = 0;

    n += scnprintf(debug_buffer + n, debug_bufmax - n, STR_HELP);
    debug_buffer[n++] = 0;

    return simple_read_from_buffer(ubuf, count, ppos, debug_buffer, n);
}


static ssize_t debug_write(struct file *file,
                           const char __user *ubuf, size_t count, loff_t *ppos)
{
    const int debug_bufmax = sizeof(debug_buffer) - 1;
	size_t ret;

	ret = count;

	if (count > debug_bufmax) 
        count = debug_bufmax;

	if (copy_from_user(&debug_buffer, ubuf, count))
		return -EFAULT;

	debug_buffer[count] = 0;

    process_dbg_cmd(debug_buffer);

    return ret;
}


static struct file_operations debug_fops = {
	.read  = debug_read,
    .write = debug_write,
	.open  = debug_open,
};


void DBG_Init(void)
{
    mtkfb_dbgfs = debugfs_create_file("mtkfb",
        S_IFREG|S_IRUGO, NULL, (void *)0, &debug_fops);

    memset(&dbg_opt, sizeof(dbg_opt), 0);
	memset(&fps, sizeof(fps), 0);
    dbg_opt.log_fps_wnd_size = DEFAULT_LOG_FPS_WND_SIZE;
}


void DBG_Deinit(void)
{
    debugfs_remove(mtkfb_dbgfs);
}

