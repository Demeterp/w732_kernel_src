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
#include <asm/arch/mt65xx_lcd_regs.h>
#include <asm/arch/mt65xx_dpi_drv.h>
#include <asm/arch/mt65xx_dpi_regs.h>
#include <asm/arch/mt65xx_dsi_drv.h>
#include <asm/arch/mt65xx_dsi_regs.h>

#include <asm/arch/boot_mode.h>

#include "lcm_drv.h"

extern PLCD_REGS const LCD_REG;

static UINT32 dsiTmpBufBpp = 0;

// ---------------------------------------------------------------------------
//  Private Variables
// ---------------------------------------------------------------------------

extern LCM_DRIVER *lcm_drv;
extern LCM_PARAMS *lcm_params;

typedef struct
{
    UINT32 pa;
    UINT32 pitchInBytes;
} TempBuffer;

static TempBuffer s_tmpBuffers[3];

// ---------------------------------------------------------------------------
//  Private Functions
// ---------------------------------------------------------------------------


extern BOOTMODE g_boot_mode;
static void init_lcd_te_control(void)
{
    const LCM_DBI_PARAMS *dbi = &(lcm_params->dbi);

    /* The board may not connect to LCM in META test mode,
       force disalbe TE to avoid blocked in LCD controller
    */
	// but for uboot, the boot mode selection is done after lcd init, so we have to disable te always in uboot.
    LCD_CHECK_RET(LCD_TE_Enable(FALSE));
//    return;
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

__inline DPI_FB_FORMAT get_dsi_tmp_buffer_format(void)
{
    switch(lcm_params->dsi.data_format.format)
    {
    case LCM_DSI_FORMAT_RGB565 : return 0;
    case LCM_DSI_FORMAT_RGB666 :
    case LCM_DSI_FORMAT_RGB888 : return 1;
    default : ASSERT(0);
    }
    return 1;
}


__inline UINT32 get_dsi_tmp_buffer_bpp(void)
{
    static const UINT32 TO_BPP[] = {2, 3};
    
    return TO_BPP[get_dsi_tmp_buffer_format()];
}


__inline LCD_FB_FORMAT get_lcd_tmp_buffer_format(void)
{
    static const UINT32 TO_LCD_FORMAT[] = {
        LCD_FB_FORMAT_RGB565,
        LCD_FB_FORMAT_RGB888
    };
    
    return TO_LCD_FORMAT[get_dsi_tmp_buffer_format()];
}

static __inline LCD_IF_WIDTH to_lcd_if_width(LCM_DBI_DATA_WIDTH data_width)
{
    switch(data_width)
    {
    case LCM_DBI_DATA_WIDTH_8BITS  : return LCD_IF_WIDTH_8_BITS;
    case LCM_DBI_DATA_WIDTH_9BITS  : return LCD_IF_WIDTH_9_BITS;
    case LCM_DBI_DATA_WIDTH_16BITS : return LCD_IF_WIDTH_16_BITS;
    case LCM_DBI_DATA_WIDTH_18BITS : return LCD_IF_WIDTH_18_BITS;
    case LCM_DBI_DATA_WIDTH_24BITS : return LCD_IF_WIDTH_24_BITS;
    default : ASSERT(0);
    }
    return LCD_IF_WIDTH_18_BITS;
}

static BOOL disp_drv_dsi_init_context(void)
{
    if (lcm_drv != NULL && lcm_params != NULL){
		dsiTmpBufBpp=get_dsi_tmp_buffer_bpp(); 
		return TRUE;
	}

    if (NULL == lcm_drv) {
        return FALSE;
    }

    lcm_drv->get_params(lcm_params);

	dsiTmpBufBpp=get_dsi_tmp_buffer_bpp();
    
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
    disp_drv_dsi_init_context();

    return 
    DISP_GetScreenWidth() *
    DISP_GetScreenHeight() *          
    dsiTmpBufBpp *
    lcm_params->dsi.intermediat_buffer_num;
}


static UINT32 get_assert_layer_size(void)
{
    return DAL_GetLayerSize();
}


static void init_intermediate_buffers(UINT32 fbPhysAddr)
{
    UINT32 tmpFbStartPA = fbPhysAddr + get_fb_size();

	printk("[DISP] uboot - init_intermediate_buffers \n");
	printk("[DISP] uboot - tmpFbStartPA=%x, fbPhysAddr=%x \n", tmpFbStartPA, fbPhysAddr);

    UINT32 tmpFbPitchInBytes = DISP_GetScreenWidth() * dsiTmpBufBpp;
    UINT32 tmpFbSizeInBytes  = tmpFbPitchInBytes * DISP_GetScreenHeight();

    UINT32 i;
    
    for (i = 0; i < lcm_params->dsi.intermediat_buffer_num; ++ i)
    {
        TempBuffer *b = &s_tmpBuffers[i];

        // clean the intermediate buffers as black to prevent from noise display
        memset(tmpFbStartPA, 0, tmpFbSizeInBytes);
        
        b->pitchInBytes = tmpFbPitchInBytes;
        b->pa = tmpFbStartPA;
        ASSERT((tmpFbStartPA & 0x7) == 0);  // check if 8-byte-aligned
        tmpFbStartPA += tmpFbSizeInBytes;

		printk("[DISP] uboot - s_tmpBuffers[%d], addr = %x, size = %x \n", i, s_tmpBuffers[i].pa, tmpFbSizeInBytes);
    }
}


static void init_assertion_layer(UINT32 fbVA, UINT32 fbPA)
{
	UINT32 offset;

    disp_drv_dsi_init_context();

	if(lcm_params->dsi.mode == CMD_MODE)
		offset = get_fb_size();
	else
		offset = get_fb_size() + get_intermediate_buffer_size();

    DAL_STATUS ret = DAL_Init(fbVA + offset, fbPA + offset);
    ASSERT(DAL_STATUS_OK == ret);
}


static void init_lcd(void)
{
	UINT32 i;		

	// Config LCD Controller
	LCD_CHECK_RET(LCD_LayerEnable(LCD_LAYER_ALL, FALSE));
	LCD_CHECK_RET(LCD_LayerSetTriggerMode(LCD_LAYER_ALL, LCD_SW_TRIGGER));
	LCD_CHECK_RET(LCD_EnableHwTrigger(FALSE));

	LCD_CHECK_RET(LCD_SetBackgroundColor(0));

	if(lcm_params->dsi.mode == CMD_MODE)
		LCD_CHECK_RET(LCD_SetRoiWindow(0, 0, lcm_params->width, lcm_params->height));
	else
		LCD_CHECK_RET(LCD_SetRoiWindow(0, 0, DISP_GetScreenWidth(), DISP_GetScreenHeight()));

	if(lcm_params->dsi.mode == CMD_MODE) {
		LCD_CHECK_RET(LCD_ConfigDSIIfFormat(lcm_params->dsi.data_format.color_order,
								lcm_params->dsi.data_format.trans_seq,
								lcm_params->dsi.data_format.padding,
								lcm_params->dsi.data_format.format,
								lcm_params->dsi.packet_size,
								TRUE));

		LCD_CHECK_RET(LCD_SetOutputMode(LCD_OUTPUT_TO_LCM));
		LCD_CHECK_RET(LCD_WaitDPIIndication(FALSE));
	    LCD_CHECK_RET(LCD_FBEnable(LCD_FB_0, FALSE));
	    LCD_CHECK_RET(LCD_FBEnable(LCD_FB_1, FALSE));
	    LCD_CHECK_RET(LCD_FBEnable(LCD_FB_2, FALSE));
	} else {
		LCD_CHECK_RET(LCD_FBSetFormat(get_lcd_tmp_buffer_format()));
		LCD_CHECK_RET(LCD_FBSetPitch(s_tmpBuffers[0].pitchInBytes));
		LCD_CHECK_RET(LCD_FBSetStartCoord(0, 0));

		for (i = 0; i < lcm_params->dsi.intermediat_buffer_num; ++ i)
		{
			LCD_CHECK_RET(LCD_FBSetAddress(LCD_FB_0 + i, s_tmpBuffers[i].pa));
			LCD_CHECK_RET(LCD_FBEnable(LCD_FB_0 + i, TRUE));
		}
	
		LCD_CHECK_RET(LCD_SetOutputMode(LCD_OUTPUT_TO_MEM));
		LCD_CHECK_RET(LCD_WaitDPIIndication(TRUE));
	}
}


static void init_dpi(BOOL isDpiPoweredOn)
{
    const LCM_DSI_PARAMS *dsi = &(lcm_params->dsi);
    UINT32 i;
	DPI_REG_CNTL tmp_reg;

    DPI_CHECK_RET(DPI_Init(isDpiPoweredOn));

    DPI_CHECK_RET(DPI_EnableSeqOutput(FALSE));

    DPI_CHECK_RET(DPI_FBSetSize(DISP_GetScreenWidth(), DISP_GetScreenHeight()));
    
    for (i = 0; i < dsi->intermediat_buffer_num; ++ i)
    {
        DPI_CHECK_RET(DPI_FBSetAddress(DPI_FB_0 + i, s_tmpBuffers[i].pa));
        DPI_CHECK_RET(DPI_FBSetPitch(DPI_FB_0 + i, s_tmpBuffers[i].pitchInBytes));
        DPI_CHECK_RET(DPI_FBEnable(DPI_FB_0 + i, TRUE));
    }
    DPI_CHECK_RET(DPI_FBSetFormat(get_dsi_tmp_buffer_format()));
    DPI_CHECK_RET(DPI_FBSyncFlipWithLCD(TRUE));

    if (LCM_COLOR_ORDER_BGR == dsi->data_format.color_order) {
        DPI_CHECK_RET(DPI_SetRGBOrder(DPI_RGB_ORDER_RGB, DPI_RGB_ORDER_BGR));
    } else {
        DPI_CHECK_RET(DPI_SetRGBOrder(DPI_RGB_ORDER_RGB, DPI_RGB_ORDER_RGB));
    }

	DPI_CHECK_RET(DPI_SetDSIMode(TRUE));

}


static void DSI_Config_VDO_Timing(void)
{
	unsigned int line_byte;
	unsigned int horizontal_sync_active_byte;
	unsigned int horizontal_backporch_byte;
	unsigned int horizontal_frontporch_byte;
	unsigned int rgb_byte;

	OUTREG32(&DSI_REG->DSI_VSA_NL, lcm_params->dsi.vertical_sync_active);
	OUTREG32(&DSI_REG->DSI_VBP_NL, lcm_params->dsi.vertical_backporch);
	OUTREG32(&DSI_REG->DSI_VFP_NL, lcm_params->dsi.vertical_frontporch);
	OUTREG32(&DSI_REG->DSI_VACT_NL, lcm_params->dsi.vertical_active_line);

	line_byte							= 	(lcm_params->dsi.horizontal_sync_active \
											+ lcm_params->dsi.horizontal_backporch \
											+ lcm_params->dsi.horizontal_frontporch \
											+ lcm_params->dsi.horizontal_active_pixel) * dsiTmpBufBpp;
	
	horizontal_sync_active_byte 		= 	(lcm_params->dsi.horizontal_sync_active * dsiTmpBufBpp - 4);

	if (lcm_params->dsi.mode == SYNC_EVENT_VDO_MODE)
		horizontal_backporch_byte		= 	((lcm_params->dsi.horizontal_backporch + lcm_params->dsi.horizontal_sync_active)* dsiTmpBufBpp - 4);
	else
		horizontal_backporch_byte		= 	(lcm_params->dsi.horizontal_backporch * dsiTmpBufBpp - 4);
	
	horizontal_frontporch_byte			= 	(lcm_params->dsi.horizontal_frontporch * dsiTmpBufBpp - 6);
	rgb_byte							= 	(lcm_params->dsi.horizontal_active_pixel * dsiTmpBufBpp + 6);					
	
	OUTREG32(&DSI_REG->DSI_LINE_NB, line_byte);
	OUTREG32(&DSI_REG->DSI_HSA_NB, horizontal_sync_active_byte);
	OUTREG32(&DSI_REG->DSI_HBP_NB, horizontal_backporch_byte);
	OUTREG32(&DSI_REG->DSI_HFP_NB, horizontal_frontporch_byte);
	OUTREG32(&DSI_REG->DSI_RGB_NB, rgb_byte);

	OUTREG32(&DSI_REG->DSI_HSA_WC, (horizontal_sync_active_byte-6));
	OUTREG32(&DSI_REG->DSI_HBP_WC, (horizontal_backporch_byte-6));
	OUTREG32(&DSI_REG->DSI_HFP_WC, (horizontal_frontporch_byte-6));	

	printk("[DISP] uboot - video timing, mode = %d \n", lcm_params->dsi.mode);
	printk("[DISP] uboot - VSA : %d \n", DSI_REG->DSI_VSA_NL);
	printk("[DISP] uboot - VBP : %d \n", DSI_REG->DSI_VBP_NL);
	printk("[DISP] uboot - VFP : %d \n", DSI_REG->DSI_VFP_NL);
	printk("[DISP] uboot - VACT: %d \n", DSI_REG->DSI_VACT_NL);

	printk("[DISP] uboot - HLB : %d \n", DSI_REG->DSI_LINE_NB);
	printk("[DISP] uboot - HSA : %d \n", DSI_REG->DSI_HSA_NB);
	printk("[DISP] uboot - HBP : %d \n", DSI_REG->DSI_HBP_NB);
	printk("[DISP] uboot - HFP : %d \n", DSI_REG->DSI_HFP_NB);
	printk("[DISP] uboot - RGB : %d \n", DSI_REG->DSI_RGB_NB);
	
}


void init_dsi(void)
{
	DSI_TXRX_CTRL_REG tmp_reg1;
	DSI_PSCTRL_REG tmp_reg2;

	DSI_CHECK_RET(DSI_Init());

	tmp_reg1=DSI_REG->DSI_TXRX_CTRL;
	tmp_reg1.CKSM_EN=1;
	tmp_reg1.ECC_EN=1;
	tmp_reg1.LANE_NUM=lcm_params->dsi.LANE_NUM;
	tmp_reg1.VC_NUM=0;	
	OUTREG32(&DSI_REG->DSI_TXRX_CTRL, AS_UINT32(&tmp_reg1));	

	//initialize DSI_PHY
    DSI_PHY_TIMCONFIG(lcm_params);
	DSI_PHY_clk_setting(lcm_params->dsi.pll_div1, lcm_params->dsi.pll_div2, lcm_params->dsi.LANE_NUM);

	if(lcm_params->dsi.mode != CMD_MODE)
	{
		DSI_Config_VDO_Timing();

		tmp_reg2.DSI_PS_SEL=lcm_params->dsi.PS;
		tmp_reg2.DSI_PS_WC=lcm_params->width * dsiTmpBufBpp;

		OUTREG32(&DSI_REG->DSI_PSCTRL, AS_UINT32(&tmp_reg2));	
	}
	
	// FIX ME
		*(volatile unsigned int *) (INFRA_SYS_CFG_BASE+0x890) |= 0x00000100;	// enable MIPI TX IO	
	
}


// ---------------------------------------------------------------------------
//  DBI Display Driver Public Functions
// ---------------------------------------------------------------------------
static void init_io_pad(void)
{
   LCD_CHECK_RET(LCD_Init_IO_pad(lcm_params));
}

static DISP_STATUS dsi_init(UINT32 fbVA, UINT32 fbPA, BOOL isLcmInited)
{
    if (!disp_drv_dsi_init_context()) 
        return DISP_STATUS_NOT_IMPLEMENTED;

		if(lcm_params->dsi.mode == CMD_MODE) {
	
			init_lcd();
			init_dsi();
	
			init_assertion_layer(fbVA, fbPA);					
			
			if (NULL != lcm_drv->init && !isLcmInited) {
				lcm_drv->init();
			}

			DSI_clk_HS_mode(0);

			DSI_SetMode(lcm_params->dsi.mode);
	
			//init_lcd_te_control();
		}
		else {
	
			init_intermediate_buffers(fbPA);
			
		    init_lcd();
			init_dpi(isLcmInited);
			init_dsi();
	
			init_assertion_layer(fbVA, fbPA);
	
			if (NULL != lcm_drv->init && !isLcmInited) {
				lcm_drv->init();
			}
	
			DSI_clk_HS_mode(0);
			
			DSI_SetMode(lcm_params->dsi.mode);
		}

    return DISP_STATUS_OK;
}

static DISP_STATUS dsi_enable_power(BOOL enable)
{
	disp_drv_dsi_init_context();
	
	if(lcm_params->dsi.mode == CMD_MODE) {

		if (enable) {
			// FIX ME
			// Switch bus to MIPI TX.
			*(volatile unsigned int *) (INFRA_SYS_CFG_BASE+0x890) |= 0x00000100;	// enable MIPI TX IO	
			DSI_PHY_clk_setting(lcm_params->dsi.pll_div1, lcm_params->dsi.pll_div2, lcm_params->dsi.LANE_NUM);
			DSI_CHECK_RET(DSI_PowerOn());
			DSI_WaitForNotBusy();		
			DSI_clk_HS_mode(0);
			DSI_clk_ULP_mode(0);			
			DSI_lane0_ULP_mode(0);	
			LCD_CHECK_RET(LCD_PowerOn());
		    init_lcd_te_control();
		} else {
			LCD_CHECK_RET(LCD_TE_Enable(FALSE));
			LCD_CHECK_RET(LCD_PowerOff());
			DSI_clk_HS_mode(0);
			DSI_lane0_ULP_mode(1);
			DSI_clk_ULP_mode(1);
			DSI_CHECK_RET(DSI_PowerOff());
			DSI_PHY_clk_switch(0);
			// FIX ME
			// Switch bus to GPIO, then power level will be decided by GPIO setting.
			*(volatile unsigned int *) (INFRA_SYS_CFG_BASE+0x890) &= ~0x00000100;	// disable MIPI TX IO	
		}
	} else {

	    if (enable) {
			// FIX ME
			// Switch bus to MIPI TX.
			*(volatile unsigned int *) (INFRA_SYS_CFG_BASE+0x890) |= 0x00000100;	// enable MIPI TX IO	
			DSI_PHY_clk_switch(1);
			DSI_PHY_clk_setting(lcm_params->dsi.pll_div1, lcm_params->dsi.pll_div2, lcm_params->dsi.LANE_NUM);
	        DSI_CHECK_RET(DSI_PowerOn());			
			DSI_clk_ULP_mode(0);			
			DSI_lane0_ULP_mode(0);
			DSI_clk_HS_mode(0);	
			
	        DPI_CHECK_RET(DPI_PowerOn());
	        LCD_CHECK_RET(LCD_PowerOn());

	    } else {

	        LCD_CHECK_RET(LCD_PowerOff());		
	        DPI_CHECK_RET(DPI_PowerOff());

			DSI_lane0_ULP_mode(1);
			DSI_clk_ULP_mode(1);	
	        DSI_CHECK_RET(DSI_PowerOff());

			DSI_PHY_clk_switch(0);
			// FIX ME
			// Switch bus to GPIO, then power level will be decided by GPIO setting.
			*(volatile unsigned int *) (INFRA_SYS_CFG_BASE+0x890) &= ~0x00000100;	// disable MIPI TX IO			
	    }
	}
	
    return DISP_STATUS_OK;
}

static DISP_STATUS dsi_set_fb_addr(UINT32 fbPhysAddr)
{
    LCD_CHECK_RET(LCD_LayerSetAddress(FB_LAYER, fbPhysAddr));

    return DISP_STATUS_OK;
}


static UINT32 dsi_get_vram_size(void)
{
    UINT32 vramSize;

    disp_drv_dsi_init_context();

	if(lcm_params->dsi.mode == CMD_MODE) {
		vramSize = get_fb_size() + DAL_GetLayerSize();
	} else {
		vramSize = get_fb_size() + DAL_GetLayerSize() + get_intermediate_buffer_size();
	}
    
    return vramSize;
}

static PANEL_COLOR_FORMAT dsi_get_panel_color_format(void)
{
    disp_drv_dsi_init_context();

	{

	    switch(lcm_params->dsi.data_format.format)
	    {
		    case LCM_DSI_FORMAT_RGB565 : return PANEL_COLOR_FORMAT_RGB565;
		    case LCM_DSI_FORMAT_RGB666 : return PANEL_COLOR_FORMAT_RGB666;
		    case LCM_DSI_FORMAT_RGB888 : return PANEL_COLOR_FORMAT_RGB888;
		    default : ASSERT(0);
	    }
		
	}
}

const DISP_DRIVER *DISP_GetDriverDSI()
{
    static const DISP_DRIVER DSI_DISP_DRV =
    {
        .init                   = dsi_init,
        .enable_power           = dsi_enable_power,
        .set_fb_addr            = dsi_set_fb_addr,
        .get_vram_size          = dsi_get_vram_size,
        .get_panel_color_format = dsi_get_panel_color_format
    };

    return &DSI_DISP_DRV;
}

