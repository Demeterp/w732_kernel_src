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

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/semaphore.h>

#include <linux/disp_assert_layer.h>
#include "disp_drv.h"
#include "disp_drv_platform.h"
#include "disp_drv_log.h"

#include "lcd_drv.h"
#include "dpi_drv.h"
#include "dsi_reg.h"
#include "dsi_drv.h"

#include "lcm_drv.h"

//PDSI_PHY_REGS const DSI_PHY_REG = (PDSI_PHY_REGS)(DSI_PHY_BASE);

// ---------------------------------------------------------------------------
//  Private Variables
// ---------------------------------------------------------------------------

extern LCM_DRIVER *lcm_drv;
extern LCM_PARAMS *lcm_params;

static DPI_FB_FORMAT dpiTmpBufFormat = DPI_FB_FORMAT_RGB888;
static LCD_FB_FORMAT lcdTmpBufFormat = LCD_FB_FORMAT_RGB888;
static UINT32 dpiTmpBufBpp = 0;
extern UINT32 dpi_buffer_bpp; 
typedef struct
{
    UINT32 pa;
    UINT32 pitchInBytes;
} TempBuffer;

static TempBuffer s_tmpBuffers[3];


// ---------------------------------------------------------------------------
//  Private Functions
// ---------------------------------------------------------------------------

static __inline DPI_FB_FORMAT get_tmp_buffer_dpi_format(void)
{
    switch(lcm_params->dpi.format)
    {
    case LCM_DPI_FORMAT_RGB565 : return DPI_FB_FORMAT_RGB565;
    case LCM_DPI_FORMAT_RGB666 :
    case LCM_DPI_FORMAT_RGB888 : return DPI_FB_FORMAT_RGB888;
    default : ASSERT(0);
    }
    return DPI_FB_FORMAT_RGB888;
}


static __inline UINT32 get_tmp_buffer_bpp(void)
{
    static const UINT32 TO_BPP[] = {2, 3};
 	dpi_buffer_bpp = TO_BPP[dpiTmpBufFormat];
    return TO_BPP[dpiTmpBufFormat];
}


static __inline LCD_FB_FORMAT get_tmp_buffer_lcd_format(void)
{
    static const UINT32 TO_LCD_FORMAT[] = {
        LCD_FB_FORMAT_RGB565,
        LCD_FB_FORMAT_RGB888
    };
    
    return TO_LCD_FORMAT[dpiTmpBufFormat];
}


static BOOL disp_drv_dpi_init_context(void)
{
	dpiTmpBufFormat = get_tmp_buffer_dpi_format();
    lcdTmpBufFormat = get_tmp_buffer_lcd_format();
    dpiTmpBufBpp    = get_tmp_buffer_bpp();

    if (lcm_drv != NULL && lcm_params!= NULL) 
        return TRUE;
    else 
        printk("%s, lcm_drv=0x%08x, lcm_params=0x%08x\n", __func__, (unsigned int)lcm_drv, (unsigned int)lcm_params);

    if (NULL == lcm_drv) {
		printk("%s, lcm_drv is NULL\n", __func__);

        return FALSE;
    }

 

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


static UINT32 get_intermediate_buffer_size(void)
{
    UINT32 size = 0;
    UINT32 framePixels = 0;

    disp_drv_dpi_init_context();

    framePixels = DISP_GetScreenWidth() * DISP_GetScreenHeight();

    size = framePixels * dpiTmpBufBpp * 
           lcm_params->dpi.intermediat_buffer_num;

    return size;
}


static UINT32 get_assert_layer_size(void)
{
    return DAL_GetLayerSize();
}


static void init_intermediate_buffers(UINT32 fbPhysAddr)
{
    UINT32 tmpFbStartPA = fbPhysAddr + get_fb_size();

    UINT32 tmpFbPitchInBytes = DISP_GetScreenWidth() * dpiTmpBufBpp;
    UINT32 tmpFbSizeInBytes  = tmpFbPitchInBytes * DISP_GetScreenHeight();

    UINT32 i;
    
    for (i = 0; i < lcm_params->dpi.intermediat_buffer_num; ++ i)
    {
        TempBuffer *b = &s_tmpBuffers[i];
        
        b->pitchInBytes = tmpFbPitchInBytes;
        b->pa = tmpFbStartPA;
        ASSERT((tmpFbStartPA & 0x7) == 0);  // check if 8-byte-aligned
        tmpFbStartPA += tmpFbSizeInBytes;
    }

}


static void init_assertion_layer(UINT32 fbVA, UINT32 fbPA)
{
    UINT32 offset = get_fb_size() + get_intermediate_buffer_size();
    DAL_STATUS ret = DAL_Init(fbVA + offset, fbPA + offset);
    ASSERT(DAL_STATUS_OK == ret);
}


static void init_mipi_pll(void)
{
    DPI_CHECK_RET(DPI_Init_PLL(lcm_params->dpi.mipi_pll_clk_ref,
	           lcm_params->dpi.mipi_pll_clk_div1,lcm_params->dpi.mipi_pll_clk_div2));
}


static void init_io_pad(void)
{
    LCD_CHECK_RET(LCD_Init_IO_pad(lcm_params));
    
}

static void init_io_driving_current(void)
{
	DPI_CHECK_RET(DPI_Set_DrivingCurrent(lcm_params));
}

static void init_lcd(void)
{
    UINT32 i;

    LCD_CHECK_RET(LCD_LayerEnable(LCD_LAYER_ALL, FALSE));
    LCD_CHECK_RET(LCD_LayerSetTriggerMode(LCD_LAYER_ALL, LCD_SW_TRIGGER));
    LCD_CHECK_RET(LCD_EnableHwTrigger(FALSE));

    LCD_CHECK_RET(LCD_SetBackgroundColor(0));
    LCD_CHECK_RET(LCD_SetRoiWindow(0, 0, DISP_GetScreenWidth(), DISP_GetScreenHeight()));

    LCD_CHECK_RET(LCD_FBSetFormat(lcdTmpBufFormat));
    LCD_CHECK_RET(LCD_FBSetPitch(s_tmpBuffers[0].pitchInBytes));
    LCD_CHECK_RET(LCD_FBSetStartCoord(0, 0));

    for (i = 0; i < lcm_params->dpi.intermediat_buffer_num; ++ i)
    {
        LCD_CHECK_RET(LCD_FBSetAddress(LCD_FB_0 + i, s_tmpBuffers[i].pa));
        LCD_CHECK_RET(LCD_FBEnable(LCD_FB_0 + i, TRUE));
    }
    
    LCD_CHECK_RET(LCD_SetOutputMode(LCD_OUTPUT_TO_MEM));
    /**
       "LCD Delay Enable" function should be used when there is only
       single buffer between LCD and DPI.
       Double buffer even triple buffer need not enable it.
    */
    LCD_CHECK_RET(LCD_WaitDPIIndication(TRUE));

}


static void init_dpi(BOOL isDpiPoweredOn)
{
    const LCM_DPI_PARAMS *dpi = &(lcm_params->dpi);
    UINT32 i;

    DPI_CHECK_RET(DPI_Init(isDpiPoweredOn));

    DPI_CHECK_RET(DPI_EnableSeqOutput(FALSE));

    DPI_CHECK_RET(DPI_ConfigPixelClk((DPI_POLARITY)dpi->clk_pol,
                                     dpi->dpi_clk_div, dpi->dpi_clk_duty));

    DPI_CHECK_RET(DPI_ConfigDataEnable((DPI_POLARITY)dpi->de_pol));

    DPI_CHECK_RET(DPI_ConfigHsync((DPI_POLARITY)dpi->hsync_pol,
                                  dpi->hsync_pulse_width,
                                  dpi->hsync_back_porch,
                                  dpi->hsync_front_porch));

    DPI_CHECK_RET(DPI_ConfigVsync((DPI_POLARITY)dpi->vsync_pol,
                                  dpi->vsync_pulse_width,
                                  dpi->vsync_back_porch,
                                  dpi->vsync_front_porch));

    DPI_CHECK_RET(DPI_FBSetSize(DISP_GetScreenWidth(), DISP_GetScreenHeight()));
    
    for (i = 0; i < dpi->intermediat_buffer_num; ++ i)
    {
        DPI_CHECK_RET(DPI_FBSetAddress(DPI_FB_0 + i, s_tmpBuffers[i].pa));
        DPI_CHECK_RET(DPI_FBSetPitch(DPI_FB_0 + i, s_tmpBuffers[i].pitchInBytes));
        DPI_CHECK_RET(DPI_FBEnable(DPI_FB_0 + i, TRUE));
    }
    DPI_CHECK_RET(DPI_FBSetFormat(dpiTmpBufFormat));
    DPI_CHECK_RET(DPI_FBSyncFlipWithLCD(TRUE));

    if (LCM_COLOR_ORDER_BGR == dpi->rgb_order) {
        DPI_CHECK_RET(DPI_SetRGBOrder(DPI_RGB_ORDER_RGB, DPI_RGB_ORDER_BGR));
    } else {
        DPI_CHECK_RET(DPI_SetRGBOrder(DPI_RGB_ORDER_RGB, DPI_RGB_ORDER_RGB));
    }

    DPI_CHECK_RET(DPI_EnableClk());
}


// ---------------------------------------------------------------------------
//  DPI Display Driver Public Functions
// ---------------------------------------------------------------------------

static DISP_STATUS dpi_init(UINT32 fbVA, UINT32 fbPA, BOOL isLcmInited)
{
    if (!disp_drv_dpi_init_context()) 
        return DISP_STATUS_NOT_IMPLEMENTED;

    init_intermediate_buffers(fbPA);

    init_mipi_pll();
    init_io_pad();
	init_io_driving_current();

    init_lcd();
    init_dpi(isLcmInited);

    init_assertion_layer(fbVA, fbPA);

    if (NULL != lcm_drv->init && !isLcmInited) {
        lcm_drv->init();
    }
	DSI_PowerOn();
	DSI_PowerOff();
    return DISP_STATUS_OK;
}


static DISP_STATUS dpi_enable_power(BOOL enable)
{
    if (enable) {
        DPI_CHECK_RET(DPI_PowerOn());

        init_mipi_pll();//for MT6573 and later chip, Must re-init mipi pll for dpi, because pll register have located in
		                //MMSYS1 except MT6516
        init_io_pad();
        LCD_CHECK_RET(LCD_PowerOn());
        DPI_CHECK_RET(DPI_EnableClk());
    } else {
        DPI_CHECK_RET(DPI_DisableClk());
		DPI_CHECK_RET(DPI_PowerOff());
        LCD_CHECK_RET(LCD_PowerOff());
		DPI_mipi_switch(false);
    }
    return DISP_STATUS_OK;
}


static DISP_STATUS dpi_set_fb_addr(UINT32 fbPhysAddr)
{
    LCD_CHECK_RET(LCD_LayerSetAddress(FB_LAYER, fbPhysAddr));

    return DISP_STATUS_OK;
}

static DISP_STATUS dpi_update_screen(void)
{

    LCD_CHECK_RET(LCD_StartTransfer(FALSE));

    return DISP_STATUS_OK;
}


static UINT32 dpi_get_vram_size(void)
{
    return get_fb_size() +
           get_intermediate_buffer_size() +
           get_assert_layer_size();
}


static PANEL_COLOR_FORMAT dpi_get_panel_color_format(void)
{
    disp_drv_dpi_init_context();

    switch(lcm_params->dpi.format)
    {
    case LCM_DPI_FORMAT_RGB565 : return PANEL_COLOR_FORMAT_RGB565;
    case LCM_DPI_FORMAT_RGB666 : return PANEL_COLOR_FORMAT_RGB666;
    case LCM_DPI_FORMAT_RGB888 : return PANEL_COLOR_FORMAT_RGB888;
    default : ASSERT(0);
    }
    return PANEL_COLOR_FORMAT_RGB888;
}



DISP_STATUS dpi_capture_framebuffer(UINT32 pvbuf, UINT32 bpp)
{
    LCD_CHECK_RET(DPI_Capture_Framebuffer(pvbuf, bpp));

	return DISP_STATUS_OK;	
}


#define MIN(x,y) ((x)>(y)?(y):(x))
static UINT32 dpi_get_dithering_bpp(void)
{
	return MIN(get_tmp_buffer_bpp() * 8,PANEL_COLOR_FORMAT_TO_BPP(dpi_get_panel_color_format()));
}

const DISP_DRIVER *DISP_GetDriverDPI()
{
    static const DISP_DRIVER DPI_DISP_DRV =
    {
        .init                   = dpi_init,
        .enable_power           = dpi_enable_power,
        .set_fb_addr            = dpi_set_fb_addr,
        .update_screen          = dpi_update_screen,
        
        .get_vram_size          = dpi_get_vram_size,
        .get_panel_color_format = dpi_get_panel_color_format,
        .get_dithering_bpp		= dpi_get_dithering_bpp,
		.capture_framebuffer	= dpi_capture_framebuffer, 
    };

    return &DPI_DISP_DRV;
}


