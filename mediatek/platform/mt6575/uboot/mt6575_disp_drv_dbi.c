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

#include <asm/arch/mt65xx_disp_drv.h>
#include <asm/arch/mt65xx_disp_assert_layer.h>
#include <asm/arch/mt65xx_lcd_drv.h>

#include <asm/arch/boot_mode.h>

#include "lcm_drv.h"


// ---------------------------------------------------------------------------
//  Private Variables
// ---------------------------------------------------------------------------

extern LCM_DRIVER *lcm_drv;
extern LCM_PARAMS *lcm_params;



// ---------------------------------------------------------------------------
//  Private Functions
// ---------------------------------------------------------------------------

static BOOL disp_drv_dbi_init_context(void)
{
    if (lcm_drv != NULL && lcm_params!= NULL) 
		return TRUE;

    if (NULL == lcm_drv) {
		printf("%s, lcm_drv is NULL\n", __func__);
        return FALSE;
    }

    lcm_drv->get_params(lcm_params);
    
    return TRUE;
}


#define ALIGN_TO(x, n)  \
	(((x) + ((n) - 1)) & ~((n) - 1))

static UINT32 get_fb_size(void)
{
    return ALIGN_TO(DISP_GetScreenWidth(), 32) * 
           ALIGN_TO(DISP_GetScreenHeight(), 32) * 
           ((DISP_GetScreenBpp() + 7) >> 3) * 
           DISP_GetPages();
}


static void init_assertion_layer(UINT32 fbVA, UINT32 fbPA)
{
    UINT32 offset = get_fb_size();
    DAL_STATUS ret = DAL_Init(fbVA + offset, fbPA + offset);
    ASSERT(DAL_STATUS_OK == ret);
}


static void init_lcd(void)
{
    // Config LCD Controller

    LCD_CHECK_RET(LCD_LayerEnable(LCD_LAYER_ALL, FALSE));
    LCD_CHECK_RET(LCD_LayerSetTriggerMode(LCD_LAYER_ALL, LCD_SW_TRIGGER));
    LCD_CHECK_RET(LCD_EnableHwTrigger(FALSE));

    LCD_CHECK_RET(LCD_SetBackgroundColor(0));
    LCD_CHECK_RET(LCD_SetRoiWindow(0, 0, lcm_params->width, lcm_params->height));

    LCD_CHECK_RET(LCD_SetOutputMode(LCD_OUTPUT_TO_LCM));
    LCD_CHECK_RET(LCD_WaitDPIIndication(FALSE));
    LCD_CHECK_RET(LCD_FBEnable(LCD_FB_0, FALSE));
    LCD_CHECK_RET(LCD_FBEnable(LCD_FB_1, FALSE));
    LCD_CHECK_RET(LCD_FBEnable(LCD_FB_2, FALSE));
}

extern BOOTMODE g_boot_mode;
static void init_lcd_te_control(void)
{
    const LCM_DBI_PARAMS *dbi = &(lcm_params->dbi);

    /* The board may not connect to LCM in META test mode,
       force disalbe TE to avoid blocked in LCD controller
    */
	// but for uboot, the boot mode selection is done after lcd init, so we have to disable te always in uboot.
	LCD_CHECK_RET(LCD_TE_Enable(FALSE));
	if(!DISP_IsLcmFound()){
		return;
	}
	printf("boot_mode = %d\n",g_boot_mode);
	if(g_boot_mode == META_BOOT){
		return;
	}

    if (LCM_DBI_TE_MODE_DISABLED == dbi->te_mode) {
        LCD_CHECK_RET(LCD_TE_Enable(FALSE));
        return;
    }

    if (LCM_DBI_TE_MODE_VSYNC_ONLY == dbi->te_mode) {
        LCD_CHECK_RET(LCD_TE_SetMode(LCD_TE_MODE_VSYNC_ONLY));
    } else if (LCM_DBI_TE_MODE_VSYNC_OR_HSYNC == dbi->te_mode) {
        LCD_CHECK_RET(LCD_TE_SetMode(LCD_TE_MODE_VSYNC_OR_HSYNC));
        LCD_CHECK_RET(LCD_TE_ConfigVHSyncMode(dbi->te_hs_delay_cnt,
                                              dbi->te_vs_width_cnt,
                     (LCD_TE_VS_WIDTH_CNT_DIV)dbi->te_vs_width_cnt_div));
    } else ASSERT(0);

    LCD_CHECK_RET(LCD_TE_SetEdgePolarity(dbi->te_edge_polarity));
    LCD_CHECK_RET(LCD_TE_Enable(TRUE));
}

static void init_io_driving_current(void)
{
	LCD_CHECK_RET(LCD_Set_DrivingCurrent(lcm_params));
}

// ---------------------------------------------------------------------------
//  DBI Display Driver Public Functions
// ---------------------------------------------------------------------------
static void init_io_pad(void)
{
   LCD_CHECK_RET(LCD_Init_IO_pad(lcm_params));
}

static DISP_STATUS dbi_init(UINT32 fbVA, UINT32 fbPA, BOOL isLcmInited)
{
    if (!disp_drv_dbi_init_context()) 
        return DISP_STATUS_NOT_IMPLEMENTED;

	init_io_pad();
	init_io_driving_current();
    
	init_lcd();
    init_assertion_layer(fbVA, fbPA);

    if (NULL != lcm_drv->init && !isLcmInited) {
        lcm_drv->init();
    }

//    init_lcd_te_control();

    return DISP_STATUS_OK;
}


static DISP_STATUS dbi_enable_power(BOOL enable)
{
    if (enable) {
        LCD_CHECK_RET(LCD_PowerOn());
		init_lcd_te_control();
    } else {
		LCD_CHECK_RET(LCD_TE_Enable(FALSE));
        LCD_CHECK_RET(LCD_PowerOff());
    }
    return DISP_STATUS_OK;
}


static DISP_STATUS dbi_set_fb_addr(UINT32 fbPhysAddr)
{
    LCD_CHECK_RET(LCD_LayerSetAddress(FB_LAYER, fbPhysAddr));

    return DISP_STATUS_OK;
}


static UINT32 dbi_get_vram_size(void)
{
    UINT32 vramSize = get_fb_size();

    // assertion layer
    vramSize += DAL_GetLayerSize();
    
    return vramSize;
}


static PANEL_COLOR_FORMAT dbi_get_panel_color_format(void)
{
    disp_drv_dbi_init_context();

    switch(lcm_params->dbi.data_format.format)
    {
    case LCM_DBI_FORMAT_RGB332 : return PANEL_COLOR_FORMAT_RGB332;
    case LCM_DBI_FORMAT_RGB444 : return PANEL_COLOR_FORMAT_RGB444;
    case LCM_DBI_FORMAT_RGB565 : return PANEL_COLOR_FORMAT_RGB565;
    case LCM_DBI_FORMAT_RGB666 : return PANEL_COLOR_FORMAT_RGB666;
    case LCM_DBI_FORMAT_RGB888 : return PANEL_COLOR_FORMAT_RGB888;
    default : ASSERT(0);
    }
    return PANEL_COLOR_FORMAT_RGB888;
}


const DISP_DRIVER *DISP_GetDriverDBI()
{
    static const DISP_DRIVER DBI_DISP_DRV =
    {
        .init                   = dbi_init,
        .enable_power           = dbi_enable_power,
        .set_fb_addr            = dbi_set_fb_addr,
        .get_vram_size          = dbi_get_vram_size,
        .get_panel_color_format = dbi_get_panel_color_format
    };

    return &DBI_DISP_DRV;
}

