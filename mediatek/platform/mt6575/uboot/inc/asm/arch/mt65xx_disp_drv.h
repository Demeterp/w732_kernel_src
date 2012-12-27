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

#ifndef __mt6573_DISP_DRV_H__
#define __mt6573_DISP_DRV_H__

#include <common.h>
#include <asm/arch/mt65xx_typedefs.h>
#include "lcm_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
//  UBoot Display Utility Macros
// ---------------------------------------------------------------------------

#define AS_UINT32(x)    (*(unsigned int *)(x))

#define mt6573_DBG  1

#if mt6573_DBG

#define ASSERT(expr)                                                    \
    do {                                                                \
        if(!(expr)) {                                                   \
    		printf("<ASSERT> %s:line %d %s\n",                          \
    		    __FILE__,__LINE__,(int)(#expr));                        \
    		while (1);                                                  \
        }                                                               \
    } while(0);

#define NOT_IMPLEMENTED()                                               \
    do {                                                                \
        printf("<NOT_IMPLEMENTED> %s:line %d\n", __FILE__, __LINE__);   \
        while (1);                                                      \
    } while(0);

#else   // !mt6573_DBG

    #define ASSERT(expr)
    #define NOT_IMPLEMENTED()

#endif  // end of mt6573_DBG

// ---------------------------------------------------------------------------

#define NOT_REFERENCED(x) {(x) = (x);}

#define printk  printf

#define DISP_CHECK_RET(expr)            \
    do {                                \
        DISP_STATUS ret = (expr);       \
        ret = ret;                      \
        ASSERT(DISP_STATUS_OK == ret);  \
    } while (0)

// ---------------------------------------------------------------------------

typedef enum
{	
   DISP_STATUS_OK = 0,

   DISP_STATUS_NOT_IMPLEMENTED,
   DISP_STATUS_ALREADY_SET,
   DISP_STATUS_ERROR,
} DISP_STATUS;


typedef enum {
   DISP_STATE_IDLE = 0,
   DISP_STATE_BUSY,
} DISP_STATE;


#define MAKE_PANEL_COLOR_FORMAT(R, G, B) ((R << 16) | (G << 8) | B)

typedef enum {
    PANEL_COLOR_FORMAT_RGB332 = MAKE_PANEL_COLOR_FORMAT(3, 3, 2),
    PANEL_COLOR_FORMAT_RGB444 = MAKE_PANEL_COLOR_FORMAT(4, 4, 4),
    PANEL_COLOR_FORMAT_RGB565 = MAKE_PANEL_COLOR_FORMAT(5, 6, 5),
    PANEL_COLOR_FORMAT_RGB666 = MAKE_PANEL_COLOR_FORMAT(6, 6, 6),
    PANEL_COLOR_FORMAT_RGB888 = MAKE_PANEL_COLOR_FORMAT(8, 8, 8),
} PANEL_COLOR_FORMAT;


typedef enum {
   DISP_VERTICAL_PROG_BAR = 0,
   DISP_HORIZONTAL_PROG_BAR,
} DISP_PROG_BAR_DIRECT;


// ---------------------------------------------------------------------------
//  UBoot Display Public Functions
// ---------------------------------------------------------------------------

DISP_STATUS DISP_Init(UINT32 fbVA, UINT32 fbPA, BOOL isLcmInited);
DISP_STATUS DISP_Deinit(void);
DISP_STATUS DISP_PowerEnable(BOOL enable);
DISP_STATUS DISP_PanelEnable(BOOL enable);
DISP_STATUS DISP_SetFrameBufferAddr(UINT32 fbPhysAddr);
DISP_STATUS DISP_EnterOverlayMode(void);
DISP_STATUS DISP_LeaveOverlayMode(void);
DISP_STATUS DISP_EnableDirectLinkMode(UINT32 layer);
DISP_STATUS DISP_DisableDirectLinkMode(UINT32 layer);
DISP_STATUS DISP_UpdateScreen(UINT32 x, UINT32 y, UINT32 width, UINT32 height);

// Retrieve Information
UINT32 DISP_GetScreenWidth(void);
UINT32 DISP_GetScreenHeight(void);
UINT32 DISP_GetScreenBpp(void);
UINT32 DISP_GetPages(void);
UINT32 DISP_GetVRamSize(void);
PANEL_COLOR_FORMAT DISP_GetPanelColorFormat(void);
DISP_STATUS DISP_SetBacklight(UINT32 level);


// ---------------------------------------------------------------------------
//  UBoot Display Driver
// ---------------------------------------------------------------------------

typedef struct
{
    DISP_STATUS (*init)(UINT32 fbVA, UINT32 fbPA, BOOL isLcmInited);
    DISP_STATUS (*enable_power)(BOOL enable);
    DISP_STATUS (*set_fb_addr)(UINT32 fbPhysAddr);

    UINT32 (*get_vram_size)(void);
    PANEL_COLOR_FORMAT (*get_panel_color_format)(void);
} DISP_DRIVER;


const DISP_DRIVER *DISP_GetDriverDBI(void);
const DISP_DRIVER *DISP_GetDriverDPI(void);
const DISP_DRIVER *DISP_GetDriverDSI(void);


   

// ---------------------------------------------------------------------------
//  UBoot Display Export Functions
// ---------------------------------------------------------------------------

UINT32 mt65xx_disp_get_vram_size(void);
void   mt65xx_disp_init(void *lcdbase);
void   mt65xx_disp_power(BOOL on);
void   mt65xx_disp_update(UINT32 x, UINT32 y, UINT32 width, UINT32 height);
void   mt65xx_disp_wait_idle(void);
UINT32 mt65xx_disp_get_lcd_time(void);

BOOL DISP_DetectDevice(void);

// -- Utility Functions for Customization --

void*  mt65xx_get_logo_db_addr(void);
void*  mt65xx_get_fb_addr(void);
UINT32 mt65xx_get_fb_size(void);

void mt65xx_disp_fill_rect(UINT32 left, UINT32 top,
                           UINT32 right, UINT32 bottom,
                           UINT32 color);

void mt65xx_disp_draw_prog_bar(DISP_PROG_BAR_DIRECT direct,
                               UINT32 left, UINT32 top,
                               UINT32 right, UINT32 bottom,
                               UINT32 fgColor, UINT32 bgColor,
                               UINT32 start_div, UINT32 total_div,
                               UINT32 occupied_div);

#ifdef __cplusplus
}
#endif

#endif // __mt65xx_DISP_DRV_H__
