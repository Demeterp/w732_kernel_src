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

#define ENABLE_LCD_INTERRUPT 0

#include <common.h>
#include <mmc.h>
#include <part.h>
#include <fat.h>
#include <malloc.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/arch/mt65xx.h>
#include <asm/arch/mt65xx_typedefs.h>
//#include <asm/arch/mt6516_pdn_sw.h>
#include <asm/arch/mt65xx_disp_drv.h>
#include <asm/arch/mt65xx_lcd_regs.h>

#include <asm/arch/mt65xx_lcd_drv.h>
#include <asm/arch/mt65xx_dsi_drv.h>
#define LCD_OUTREG32(addr, data)	\
		{\
		OUTREG32(addr, data);}

#define LCD_OUTREG8(addr, data)	\
		{\
		OUTREG8(addr, data);}


#define LCD_OUTREG16(addr, data)	\
		{\
		OUTREG16(addr, data);}

#define LCD_MASKREG32(addr, mask, data)	\
		{\
		MASKREG32(addr, mask, data);}

PLCD_REGS const LCD_REG = (PLCD_REGS)(LCD_BASE);

static const UINT32 TO_BPP[LCD_FB_FORMAT_NUM] = {2, 3, 4};
unsigned int wait_time = 0;
typedef struct
{
    LCD_FB_FORMAT fbFormat;
    UINT32 fbPitchInBytes;
    LCD_REG_SIZE roiWndSize;
    LCD_OUTPUT_MODE outputMode;
    LCD_REGS regBackup;
} LCD_CONTEXT;

static LCD_CONTEXT _lcdContext = {0};

static bool limit_w2m_speed = false;

UINT32 const NLI_ARB_CS = 0xC100d014;
UINT32 const DPI_PAD_CON = 0xC2080900;

UINT32 const INFRA_DRV5 = 0xC0001814;
UINT32 const INFRA_DRV6 = 0xC0001818;
UINT32 const INFRA_DRV12 = 0xC0001830;
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

LCD_STATUS LCD_Init_IO_pad(LCM_PARAMS *lcm_params)
{
	ASSERT(lcm_params->dbi.port < 2);
	printf("LCD_Init_IO:0x%x, 0x%x\n",INREG32(NLI_ARB_CS),INREG32(DPI_PAD_CON));
	if(lcm_params->type == LCM_TYPE_DBI){
		MASKREG32(NLI_ARB_CS, 0xF << (lcm_params->dbi.port << 2), lcm_params->dbi.port << (lcm_params->dbi.port << 2)); // NLI_ARB chip select
		MASKREG32(NLI_ARB_CS, 0x00040000, lcm_params->io_select_mode << 18); // NLI_ARB chip select
		if(1 == lcm_params->io_select_mode){// if DBI share DPI data pin
			MASKREG32(DPI_PAD_CON, 0x00000001, 0x00000000);// DPI PAD Mode should be other mode, not DPI mode
		}
	}
	else if(lcm_params->type == LCM_TYPE_DPI){
		MASKREG32(DPI_PAD_CON, 0x00000001, 0x00000001);// DPI mode
		MASKREG32(DPI_PAD_CON, 0x00000006, (lcm_params->dpi.i2x_en << 1) | (lcm_params->dpi.i2x_edge << 2));// DPI mode
	}
	else{}//DSI do nothing

	printf("LCD_Init_IO:0x%x, 0x%x\n",INREG32(NLI_ARB_CS),INREG32(DPI_PAD_CON));
	
	return LCD_STATUS_OK;
}

LCD_STATUS LCD_Set_DrivingCurrent(LCM_PARAMS *lcm_params)
{
	printf("LCD_Init_Driving:0x%x,0x%x,0x%x\n",INREG32(INFRA_DRV12),INREG32(INFRA_DRV5),INREG32(INFRA_DRV6));
	if(1 == lcm_params->io_select_mode){// if DBI share DPI data pin
		MASKREG32(INFRA_DRV12, 0x0ffff000,(((lcm_params->dbi.io_driving_current >> 4) - 1) << 24)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 20)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 16)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 12));
		
		MASKREG32(INFRA_DRV5, 0xffff0000,(((lcm_params->dbi.io_driving_current >> 4) - 1) << 28)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 24)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 20)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 16));
	}

	MASKREG32(INFRA_DRV6, 0x000000fff,(((lcm_params->dbi.io_driving_current >> 4) - 1) << 0)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 4)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 8));

	printf("LCD_Init_Driving:0x%x,0x%x,0x%x\n",INREG32(INFRA_DRV12),INREG32(INFRA_DRV5),INREG32(INFRA_DRV6));
	
	return LCD_STATUS_OK;
}

#if ENABLE_LCD_INTERRUPT
static __tcmfunc irqreturn_t _LCD_InterruptHandler(int irq, void *dev_id)
{   
    LCD_REG_INTERRUPT status = LCD_REG->INT_STATUS;

    if (status.COMPLETED)
    {
#if defined(MTK_TVOUT_SUPPORT)
        DISP_TvOutOnLcdDone();
#endif      
        wake_up_interruptible(&_lcd_wait_queue);
    }

    if (status.SYNC)// this is TE mode 0 interrupt
    {
    }
#if 0  //TE mode 1
	if(status.HTT)
	{
	}
#endif
    //MT6573IDP_DisableDirectLink();    // FIXME

    return IRQ_HANDLED;
}
#endif


static BOOL _IsEngineBusy(void)
{
    LCD_REG_STATUS status;

    status = LCD_REG->STATUS;
    if (status.RUN || 
        status.WAIT_CMDQ ||  
        status.WAIT_HTT|| 
        status.WAIT_SYNC || 
        status.BUSY ||
        status.GMC) 
        return TRUE;
	
	LCD_OUTREG32(&LCD_REG->INT_STATUS, 0);
    return FALSE;
}


static void _WaitForEngineNotBusy(void)
{
#if ENABLE_LCD_INTERRUPT
    static const long WAIT_TIMEOUT = 2 * HZ;    // 2 sec

    if (in_interrupt())
    {
        // perform busy waiting if in interrupt context
        while(_IsEngineBusy()) {}
    }
    else
    {
        while (_IsEngineBusy())
        {
            long ret = wait_event_interruptible_timeout(_lcd_wait_queue, 
                                                        !_IsEngineBusy(),
                                                        WAIT_TIMEOUT);
            if (0 == ret) {
                printk("[WARNING] Wait for LCD engine not busy timeout!!!\n");
            }
        }
    }
#else
    while(_IsEngineBusy()) {
		udelay(50);//sleep 1ms
		wait_time++;
		if(wait_time>20000){ //timeout
			printk("[WARNING] Wait for LCD engine not busy timeout!!!\n");
			if(LCD_REG->STATUS.WAIT_SYNC){
				printk("reason is LCD can't wait TE signal!!!\n");
				LCD_TE_Enable(FALSE);
			}
			OUTREG16(&LCD_REG->START, 0);
			OUTREG16(&LCD_REG->START, 0x1);
			OUTREG16(&LCD_REG->START, 0);
			OUTREG16(&LCD_REG->START, 0x8000);
			wait_time = 0;
		}
	}
	wait_time = 0;
#endif    
}


static void _BackupLCDRegisters(void)
{
//    memcpy((void*)&(_lcdContext.regBackup), (void*)LCD_REG, sizeof(LCD_REGS));
	LCD_REGS *regs = &(_lcdContext.regBackup);
    UINT32 i;
    LCD_OUTREG32(&regs->INT_ENABLE, AS_UINT32(&LCD_REG->INT_ENABLE));
    LCD_OUTREG32(&regs->SERIAL_CFG, AS_UINT32(&LCD_REG->SERIAL_CFG));

	for(i = 0; i < ARY_SIZE(LCD_REG->SIF_TIMING); ++i)
    {
        LCD_OUTREG32(&regs->SIF_TIMING[i], AS_UINT32(&LCD_REG->SIF_TIMING[i]));
    }

    for(i = 0; i < ARY_SIZE(LCD_REG->PARALLEL_CFG); ++i)
    {
        LCD_OUTREG32(&regs->PARALLEL_CFG[i], AS_UINT32(&LCD_REG->PARALLEL_CFG[i]));
    }

    LCD_OUTREG32(&regs->TEARING_CFG, AS_UINT32(&LCD_REG->TEARING_CFG));
    LCD_OUTREG32(&regs->PARALLEL_DW, AS_UINT32(&LCD_REG->PARALLEL_DW));
	LCD_OUTREG32(&regs->CALC_HTT, AS_UINT32(&LCD_REG->CALC_HTT));
	LCD_OUTREG32(&regs->SYNC_LCM_SIZE, AS_UINT32(&LCD_REG->SYNC_LCM_SIZE));
	LCD_OUTREG32(&regs->SYNC_CNT, AS_UINT32(&LCD_REG->SYNC_CNT));
	LCD_OUTREG32(&regs->GMC_CON, AS_UINT32(&LCD_REG->GMC_CON));

    for(i = 0; i < ARY_SIZE(LCD_REG->WROI_W2M_ADDR); ++i)
    {
        LCD_OUTREG32(&regs->WROI_W2M_ADDR[i], AS_UINT32(&LCD_REG->WROI_W2M_ADDR[i]));
    }

	LCD_OUTREG32(&regs->W2M_PITCH, AS_UINT32(&LCD_REG->W2M_PITCH));
    LCD_OUTREG32(&regs->WROI_W2M_OFFSET, AS_UINT32(&LCD_REG->WROI_W2M_OFFSET));
    LCD_OUTREG32(&regs->WROI_W2M_CONTROL, AS_UINT32(&LCD_REG->WROI_W2M_CONTROL));
    LCD_OUTREG32(&regs->WROI_CONTROL, AS_UINT32(&LCD_REG->WROI_CONTROL));
    LCD_OUTREG32(&regs->WROI_OFFSET, AS_UINT32(&LCD_REG->WROI_OFFSET));
    LCD_OUTREG32(&regs->WROI_CMD_ADDR, AS_UINT32(&LCD_REG->WROI_CMD_ADDR));
    LCD_OUTREG32(&regs->WROI_DATA_ADDR, AS_UINT32(&LCD_REG->WROI_DATA_ADDR));
    LCD_OUTREG32(&regs->WROI_SIZE, AS_UINT32(&LCD_REG->WROI_SIZE));
    LCD_OUTREG32(&regs->WROI_HW_REFRESH, AS_UINT32(&LCD_REG->WROI_HW_REFRESH));
    LCD_OUTREG32(&regs->WROI_DC, AS_UINT32(&LCD_REG->WROI_DC));
    LCD_OUTREG32(&regs->WROI_BG_COLOR, AS_UINT32(&LCD_REG->WROI_BG_COLOR));
    LCD_OUTREG32(&regs->DS_DSI_CON, AS_UINT32(&LCD_REG->DS_DSI_CON));

    for(i = 0; i < ARY_SIZE(LCD_REG->LAYER); ++i)
    {
        LCD_OUTREG32(&regs->LAYER[i].CONTROL, AS_UINT32(&LCD_REG->LAYER[i].CONTROL));
        LCD_OUTREG32(&regs->LAYER[i].COLORKEY, AS_UINT32(&LCD_REG->LAYER[i].COLORKEY));
        LCD_OUTREG32(&regs->LAYER[i].OFFSET, AS_UINT32(&LCD_REG->LAYER[i].OFFSET));
        LCD_OUTREG32(&regs->LAYER[i].ADDRESS, AS_UINT32(&LCD_REG->LAYER[i].ADDRESS));
        LCD_OUTREG32(&regs->LAYER[i].SIZE, AS_UINT32(&LCD_REG->LAYER[i].SIZE));
        LCD_OUTREG32(&regs->LAYER[i].SCRL_OFFSET, AS_UINT32(&LCD_REG->LAYER[i].SCRL_OFFSET));
        LCD_OUTREG32(&regs->LAYER[i].WINDOW_OFFSET, AS_UINT32(&LCD_REG->LAYER[i].WINDOW_OFFSET));
        LCD_OUTREG32(&regs->LAYER[i].WINDOW_PITCH, AS_UINT32(&LCD_REG->LAYER[i].WINDOW_PITCH));
        LCD_OUTREG32(&regs->LAYER[i].DB_ADD, AS_UINT32(&LCD_REG->LAYER[i].DB_ADD));
    }

	LCD_OUTREG32(&regs->DITHER_CON, AS_UINT32(&LCD_REG->DITHER_CON));
    
	for(i = 0; i < ARY_SIZE(LCD_REG->COEF_ROW); ++i)
    {
        LCD_OUTREG32(&regs->COEF_ROW[i], AS_UINT32(&LCD_REG->COEF_ROW[i]));
    }

    for(i = 0; i < ARY_SIZE(LCD_REG->GAMMA); ++i)
    {
        LCD_OUTREG32(&regs->GAMMA[i], AS_UINT32(&LCD_REG->GAMMA[i]));
    }
}


static void _RestoreLCDRegisters(void)
{
    LCD_REGS *regs = &(_lcdContext.regBackup);
    UINT32 i;

    LCD_OUTREG32(&LCD_REG->INT_ENABLE, AS_UINT32(&regs->INT_ENABLE));
    LCD_OUTREG32(&LCD_REG->SERIAL_CFG, AS_UINT32(&regs->SERIAL_CFG));

	for(i = 0; i < ARY_SIZE(LCD_REG->SIF_TIMING); ++i)
    {
        LCD_OUTREG32(&LCD_REG->SIF_TIMING[i], AS_UINT32(&regs->SIF_TIMING[i]));
    }

    for(i = 0; i < ARY_SIZE(LCD_REG->PARALLEL_CFG); ++i)
    {
        LCD_OUTREG32(&LCD_REG->PARALLEL_CFG[i], AS_UINT32(&regs->PARALLEL_CFG[i]));
    }

    LCD_OUTREG32(&LCD_REG->TEARING_CFG, AS_UINT32(&regs->TEARING_CFG));
    LCD_OUTREG32(&LCD_REG->PARALLEL_DW, AS_UINT32(&regs->PARALLEL_DW));
	LCD_OUTREG32(&LCD_REG->CALC_HTT, AS_UINT32(&regs->CALC_HTT));
	LCD_OUTREG32(&LCD_REG->SYNC_LCM_SIZE, AS_UINT32(&regs->SYNC_LCM_SIZE));
	LCD_OUTREG32(&LCD_REG->SYNC_CNT, AS_UINT32(&regs->SYNC_CNT));
	LCD_OUTREG32(&LCD_REG->GMC_CON, AS_UINT32(&regs->GMC_CON));

    for(i = 0; i < ARY_SIZE(LCD_REG->WROI_W2M_ADDR); ++i)
    {
        LCD_OUTREG32(&LCD_REG->WROI_W2M_ADDR[i], AS_UINT32(&regs->WROI_W2M_ADDR[i]));
    }
	
	LCD_OUTREG32(&LCD_REG->W2M_PITCH, AS_UINT32(&regs->W2M_PITCH));
    LCD_OUTREG32(&LCD_REG->WROI_W2M_OFFSET, AS_UINT32(&regs->WROI_W2M_OFFSET));
    LCD_OUTREG32(&LCD_REG->WROI_W2M_CONTROL, AS_UINT32(&regs->WROI_W2M_CONTROL));
    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&regs->WROI_CONTROL));
    LCD_OUTREG32(&LCD_REG->WROI_OFFSET, AS_UINT32(&regs->WROI_OFFSET));
    LCD_OUTREG32(&LCD_REG->WROI_CMD_ADDR, AS_UINT32(&regs->WROI_CMD_ADDR));
    LCD_OUTREG32(&LCD_REG->WROI_DATA_ADDR, AS_UINT32(&regs->WROI_DATA_ADDR));
    LCD_OUTREG32(&LCD_REG->WROI_SIZE, AS_UINT32(&regs->WROI_SIZE));
    LCD_OUTREG32(&LCD_REG->WROI_HW_REFRESH, AS_UINT32(&regs->WROI_HW_REFRESH));
    LCD_OUTREG32(&LCD_REG->WROI_DC, AS_UINT32(&regs->WROI_DC));
    LCD_OUTREG32(&LCD_REG->WROI_BG_COLOR, AS_UINT32(&regs->WROI_BG_COLOR));
    LCD_OUTREG32(&LCD_REG->DS_DSI_CON, AS_UINT32(&regs->DS_DSI_CON));

    for(i = 0; i < ARY_SIZE(LCD_REG->LAYER); ++i)
    {
        LCD_OUTREG32(&LCD_REG->LAYER[i].CONTROL, AS_UINT32(&regs->LAYER[i].CONTROL));
        LCD_OUTREG32(&LCD_REG->LAYER[i].COLORKEY, AS_UINT32(&regs->LAYER[i].COLORKEY));
        LCD_OUTREG32(&LCD_REG->LAYER[i].OFFSET, AS_UINT32(&regs->LAYER[i].OFFSET));
        LCD_OUTREG32(&LCD_REG->LAYER[i].ADDRESS, AS_UINT32(&regs->LAYER[i].ADDRESS));
        LCD_OUTREG32(&LCD_REG->LAYER[i].SIZE, AS_UINT32(&regs->LAYER[i].SIZE));
        LCD_OUTREG32(&LCD_REG->LAYER[i].SCRL_OFFSET, AS_UINT32(&regs->LAYER[i].SCRL_OFFSET));
        LCD_OUTREG32(&LCD_REG->LAYER[i].WINDOW_OFFSET, AS_UINT32(&regs->LAYER[i].WINDOW_OFFSET));
        LCD_OUTREG32(&LCD_REG->LAYER[i].WINDOW_PITCH, AS_UINT32(&regs->LAYER[i].WINDOW_PITCH));
        LCD_OUTREG32(&LCD_REG->LAYER[i].DB_ADD, AS_UINT32(&regs->LAYER[i].DB_ADD));
    }

	LCD_OUTREG32(&LCD_REG->DITHER_CON, AS_UINT32(&regs->DITHER_CON));

    for(i = 0; i < ARY_SIZE(LCD_REG->COEF_ROW); ++i)
    {
        LCD_OUTREG32(&LCD_REG->COEF_ROW[i], AS_UINT32(&regs->COEF_ROW[i]));
    }

    for(i = 0; i < ARY_SIZE(LCD_REG->GAMMA); ++i)
    {
        LCD_OUTREG32(&LCD_REG->GAMMA[i], AS_UINT32(&regs->GAMMA[i]));
    }
}


static void _ResetBackupedLCDRegisterValues(void)
{
    LCD_REGS *regs = &_lcdContext.regBackup;
    memset((void*)regs, 0, sizeof(LCD_REGS));

    LCD_OUTREG32(&regs->SERIAL_CFG, 0x00003000);
    LCD_OUTREG32(&regs->PARALLEL_CFG[0], 0x00FC0000);
    LCD_OUTREG32(&regs->PARALLEL_CFG[1], 0x00300000);
    LCD_OUTREG32(&regs->PARALLEL_CFG[2], 0x00300000);
}


// ---------------------------------------------------------------------------
//  LCD Controller API Implementations
// ---------------------------------------------------------------------------

LCD_STATUS LCD_Init(void)
{
    LCD_STATUS ret = LCD_STATUS_OK;

    memset(&_lcdContext, 0, sizeof(_lcdContext));

    // LCD controller would NOT reset register as default values
    // Do it by SW here
    //
    _ResetBackupedLCDRegisterValues();

    ret = LCD_PowerOn();
	LCD_OUTREG32(&LCD_REG->SYNC_LCM_SIZE, 0x00010001);
	LCD_OUTREG32(&LCD_REG->SYNC_CNT, 0x1);
	LCD_REG->INT_ENABLE.SYNC = 1;
    ASSERT(ret == LCD_STATUS_OK);
	LCD_OUTREG32(&LCD_REG->GMC_CON, 0x4);
#if ENABLE_LCD_INTERRUPT
    if (request_irq(MT6575_LCD_IRQ_ID,
        _LCD_InterruptHandler, MT65xx_POLARITY_LOW, MTKFB_DRIVER, NULL) < 0)
    {
        printk("[LCD][ERROR] fail to request LCD irq\n"); 
        return LCD_STATUS_ERROR;
    }

    init_waitqueue_head(&_lcd_wait_queue);
    LCD_REG->INT_ENABLE.COMPLETED = 1;
	LCD_REG->INT_ENABLE.REG_COMPLETED = 1;
	LCD_REG->INT_ENABLE.CMDQ_COMPLETED = 1;
	LCD_REG->INT_ENABLE.HTT = 1;
    LCD_REG->INT_ENABLE.SYNC = 1;
#endif


    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_Deinit(void)
{
    LCD_STATUS ret = LCD_PowerOff();

    ASSERT(ret == LCD_STATUS_OK);

    return LCD_STATUS_OK;
}

static BOOL s_isLcdPowerOn = FALSE;

LCD_STATUS LCD_PowerOn(void)
{
#if 1
    if (!s_isLcdPowerOn)
    {
#if 1   // FIXME
		LCD_MASKREG32(0xC0001040, 0x02, 0x0);
		LCD_MASKREG32(0xC2080020, 0x80, 0x80);
		LCD_MASKREG32(0xC2080028, 0x00000280, 0x00000280);
		LCD_MASKREG32(0xC2080024, 0x00000020, 0x00000020);
		printf("0x%8x,0x%8x,0x%8x\n", INREG32(0xC2080000), INREG32(0xC2080004), INREG32(0xC2080008));
#endif        
        _RestoreLCDRegisters();
        s_isLcdPowerOn = TRUE;
    }

#endif
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_PowerOff(void)
{
    if (s_isLcdPowerOn)
    {
        int ret = 1;
        _WaitForEngineNotBusy();
        _BackupLCDRegisters();
#if 1   // FIXME
		LCD_MASKREG32(0xC0001040, 0x02, 0x02);
		LCD_MASKREG32(0xC2080010, 0x80, 0x80);
		LCD_MASKREG32(0xC2080018, 0x00000280, 0x00000280);
		LCD_MASKREG32(0xC2080014, 0x00000020, 0x00000020);
		printf("0x%8x,0x%8x,0x%8x\n", INREG32(0xC2080000), INREG32(0xC2080004), INREG32(0xC2080008));
#endif        
        s_isLcdPowerOn = FALSE;
    }

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_WaitForNotBusy(void)
{
    _WaitForEngineNotBusy();
    return LCD_STATUS_OK;
}

// -------------------- LCD Controller Interface --------------------

LCD_STATUS LCD_ConfigParallelIF(LCD_IF_ID id,
                                LCD_IF_PARALLEL_BITS ifDataWidth,
                                LCD_IF_PARALLEL_CLK_DIV clkDivisor,
                                UINT32 writeSetup,
                                UINT32 writeHold,
                                UINT32 writeWait,
                                UINT32 readSetup,
								UINT32 readHold,
                                UINT32 readLatency,
                                UINT32 waitPeriod,
								UINT32 chw)
{
    ASSERT(id >= LCD_IF_PARALLEL_0 && id <= LCD_IF_PARALLEL_2);
    ASSERT(writeSetup <= 16U);
    ASSERT(writeHold <= 16U);
    ASSERT(writeWait <= 64U);
    ASSERT(readSetup <= 16U);
	ASSERT(readHold <= 16U);
    ASSERT(readLatency <= 64U);
    ASSERT(chw <= 16U);

    if (0 == writeHold)   writeHold = 1;
    if (0 == writeWait)   writeWait = 1;
    if (0 == readLatency) readLatency = 1;

    _WaitForEngineNotBusy();

    // (1) Config Data Width
    {
        LCD_REG_PCNFDW pcnfdw = LCD_REG->PARALLEL_DW;

        switch(id)
        {
        case LCD_IF_PARALLEL_0: pcnfdw.PCNF0_DW = (UINT32)ifDataWidth; pcnfdw.PCNF0_CHW = chw;break;
        case LCD_IF_PARALLEL_1: pcnfdw.PCNF1_DW = (UINT32)ifDataWidth; pcnfdw.PCNF1_CHW = chw;break;
        case LCD_IF_PARALLEL_2: pcnfdw.PCNF2_DW = (UINT32)ifDataWidth; pcnfdw.PCNF2_CHW = chw;break;
        default : ASSERT(0);
        };

        LCD_OUTREG32(&LCD_REG->PARALLEL_DW, AS_UINT32(&pcnfdw));
    }

    // (2) Config Timing
    {
        UINT32 i;
        LCD_REG_PCNF config;
        
        i = (UINT32)id - LCD_IF_PARALLEL_0;
        config = LCD_REG->PARALLEL_CFG[i];

        config.C2WS = writeSetup;
        config.C2WH = writeHold - 1;
        config.WST  = writeWait - 1;
        config.C2RS = readSetup;
        config.C2RH = readHold;
        config.RLT  = readLatency - 1;

        LCD_OUTREG32(&LCD_REG->PARALLEL_CFG[i], AS_UINT32(&config));
    }

    // (3) Config Delay Between Commands
    {
        LCD_REG_WROI_CON ctrl = LCD_REG->WROI_CONTROL;
//        ctrl.PERIOD = waitPeriod;
        LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&ctrl));
    }

    // TODO: modify this for 6573
	#warning "iopad selection should be refined"
	#if 0
    LCD_OUTREG32(MMSYS1_CONFIG_BASE+0x0400, 0x3);
	#endif
    return LCD_STATUS_OK;
}



LCD_STATUS LCD_ConfigIfFormat(LCD_IF_FMT_COLOR_ORDER order,
                              LCD_IF_FMT_TRANS_SEQ transSeq,
                              LCD_IF_FMT_PADDING padding,
                              LCD_IF_FORMAT format,
                              LCD_IF_WIDTH busWidth)
{
    LCD_REG_WROI_CON ctrl = LCD_REG->WROI_CONTROL;


    ctrl.RGB_ORDER  = order;
    ctrl.BYTE_ORDER = transSeq;
    ctrl.PADDING    = padding;
    ctrl.DATA_FMT   = (UINT32)format;
    ctrl.IF_FMT   = (UINT32)busWidth;
	ctrl.IF_24 = 0;
    if(busWidth == LCD_IF_WIDTH_24_BITS)
	{
	    ctrl.IF_24 = 1;
	}
    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&ctrl));


    return LCD_STATUS_OK;
}

LCD_STATUS LCD_ConfigSerialIF(LCD_IF_ID id,
                              LCD_IF_SERIAL_BITS bits,
                              UINT32 three_wire,
                              UINT32 sdi,
                              BOOL   first_pol,
                              BOOL   sck_def,
                              UINT32 div2,
                              UINT32 hw_cs,
                              UINT32 css,
                              UINT32 csh,
                              UINT32 rd_1st,
                              UINT32 rd_2nd,
                              UINT32 wr_1st,
                              UINT32 wr_2nd)
{
    LCD_REG_SCNF config;
	LCD_REG_SIF_TIMING sif_timing;
	unsigned int offset = 0;
	unsigned int sif_id = 0;
    
	ASSERT(id >= LCD_IF_SERIAL_0 && id <= LCD_IF_SERIAL_1);

    _WaitForEngineNotBusy();

    memset(&config, 0, sizeof(config));
	
	if(id == LCD_IF_SERIAL_1){
		offset = 8;
		sif_id = 1;
	}
	
	LCD_MASKREG32(&config, 0x07 << offset, bits << offset);
	LCD_MASKREG32(&config, 0x08 << offset, three_wire << (offset + 3));
	LCD_MASKREG32(&config, 0x10 << offset, sdi << (offset + 4));
	LCD_MASKREG32(&config, 0x20 << offset, first_pol << (offset + 5));
	LCD_MASKREG32(&config, 0x40 << offset, sck_def << (offset + 6));
	LCD_MASKREG32(&config, 0x80 << offset, div2 << (offset + 7));

	config.HW_CS = hw_cs;
//	config.SIZE_0 = bits;
    LCD_OUTREG32(&LCD_REG->SERIAL_CFG, AS_UINT32(&config));

	sif_timing.WR_2ND = wr_2nd;
	sif_timing.WR_1ST = wr_1st;
	sif_timing.RD_2ND = rd_2nd;
	sif_timing.RD_1ST = rd_1st;
	sif_timing.CSH = csh;
	sif_timing.CSS = css;
	
	LCD_OUTREG32(&LCD_REG->SIF_TIMING[sif_id], AS_UINT32(&sif_timing));

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_SetResetSignal(BOOL high)
{
    LCD_REG->RESET = high ? 1 : 0;

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_ConfigDSIIfFormat(LCD_DSI_IF_FMT_COLOR_ORDER order,
                              LCD_DSI_IF_FMT_TRANS_SEQ transSeq,
                              LCD_DSI_IF_FMT_PADDING padding,
                              LCD_DSI_IF_FORMAT format,
                              UINT32 packet_size,
                              bool DC_DSI)
{
	LCD_REG_DSI_DC config;

	config.DC_DSI		= DC_DSI;	
	config.BYTE_SWAP 	= transSeq;
	config.RGB_SWAP		= order;
	config.PAD_MSB		= padding;
	config.CLR_FMT		= format;
	config.PACKET_SIZE	= packet_size;

	OUTREG32(&LCD_REG->DS_DSI_CON, AS_UINT32(&config));

	return LCD_STATUS_OK;
}


// -------------------- Command Queue --------------------

LCD_STATUS LCD_CmdQueueEnable(BOOL enabled)
{
    LCD_REG_WROI_CON ctrl;

    _WaitForEngineNotBusy();

    ctrl = LCD_REG->WROI_CONTROL;
    ctrl.ENC = enabled ? 1 : 0;
    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&ctrl));

    return LCD_STATUS_OK;
}

/*
LCD_STATUS LCD_CmdQueueSelect(LCD_CMDQ_ID id)
{
    LCD_REG_WROI_CON ctrl;

    _WaitForEngineNotBusy();
    ctrl = LCD_REG->WROI_CONTROL;

    switch(id)
    {
    case LCD_CMDQ_0 :
    case LCD_CMDQ_1 :
        ctrl.COM_SEL = (UINT32)id;
        break;
    default : ASSERT(0);
    }

    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&ctrl));

    return LCD_STATUS_OK;
}
*/

LCD_STATUS LCD_CmdQueueSetWaitPeriod(UINT32 period)
{
    LCD_REG_WROI_CON ctrl;

    ASSERT(period < 1024);
    
    _WaitForEngineNotBusy();

    ctrl = LCD_REG->WROI_CONTROL;
//    ctrl.PERIOD = period;
    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&ctrl));

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_CmdQueueWrite(UINT32 *cmds, UINT32 cmdCount)
{
    LCD_REG_WROI_CON ctrl;
    UINT32 i;

    ASSERT(cmdCount >= 0 && cmdCount < ARY_SIZE(LCD_REG->CMDQ));
    
    _WaitForEngineNotBusy();
    ctrl = LCD_REG->WROI_CONTROL;
    ctrl.COMMAND = cmdCount - 1;
    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&ctrl));

    for (i = 0; i < cmdCount; ++ i)
    {
        LCD_REG->CMDQ[i] = cmds[i];
    }

    return LCD_STATUS_OK;
}


// -------------------- Layer Configurations --------------------

LCD_STATUS LCD_LayerEnable(LCD_LAYER_ID id, BOOL enable)
{
    LCD_REG_WROI_CON ctrl;

    _WaitForEngineNotBusy();

    ctrl = LCD_REG->WROI_CONTROL;
    
    switch(id)
    {
    case LCD_LAYER_0 : ctrl.EN0 = enable ? 1 : 0; break;
    case LCD_LAYER_1 : ctrl.EN1 = enable ? 1 : 0; break;
    case LCD_LAYER_2 : ctrl.EN2 = enable ? 1 : 0; break;
    case LCD_LAYER_3 : ctrl.EN3 = enable ? 1 : 0; break;
    case LCD_LAYER_4 : ctrl.EN4 = enable ? 1 : 0; break;
    case LCD_LAYER_5 : ctrl.EN5 = enable ? 1 : 0; break;
    case LCD_LAYER_ALL :
        LCD_MASKREG32(&ctrl, 0xFC000000, enable ? 0xFC000000 : 0);
        break;
    default : ASSERT(0);
    }

    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&ctrl));
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetAddress(LCD_LAYER_ID id, UINT32 address)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);
    ASSERT((address & 0x7) == 0);   // layer address should be 8-byte-aligned

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_REG->LAYER[id].ADDRESS = address;
    }
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetSize(LCD_LAYER_ID id, UINT32 width, UINT32 height)
{
    LCD_REG_SIZE size;
    size.WIDTH = (UINT16)width;
    size.HEIGHT = (UINT16)height;

    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_OUTREG32(&LCD_REG->LAYER[id].SIZE, AS_UINT32(&size));
    }
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetPitch(LCD_LAYER_ID id, UINT32 pitch)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_OUTREG16(&LCD_REG->LAYER[id].WINDOW_PITCH, pitch);
    }
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetOffset(LCD_LAYER_ID id, UINT32 x, UINT32 y)
{
    LCD_REG_COORD offset;
    offset.X = (UINT16)x;
    offset.Y = (UINT16)y;

    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();
    
    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_OUTREG32(&LCD_REG->LAYER[id].OFFSET, AS_UINT32(&offset));
    }
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetFormat(LCD_LAYER_ID id, LCD_LAYER_FORMAT format)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();
    
    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_REG_LAYER_CON ctrl = LCD_REG->LAYER[id].CONTROL;
        ctrl.CLRDPT = (UINT32)format;
        LCD_OUTREG32(&LCD_REG->LAYER[id].CONTROL, AS_UINT32(&ctrl));
    }
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerEnableByteSwap(LCD_LAYER_ID id, BOOL enable)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        UINT32 i;
        for (i = 0; i < ARY_SIZE(LCD_REG->LAYER); ++ i)
        {
            LCD_LayerEnableByteSwap((LCD_LAYER_ID)i, enable);
        }
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_REG_LAYER_CON ctrl = LCD_REG->LAYER[id].CONTROL;
        ctrl.SWP = enable ? 1 : 0;
        LCD_OUTREG32(&LCD_REG->LAYER[id].CONTROL, AS_UINT32(&ctrl));
    }

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetRotation(LCD_LAYER_ID id, LCD_LAYER_ROTATION rotation)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_REG_LAYER_CON ctrl = LCD_REG->LAYER[id].CONTROL;
        ctrl.ROTATE = (UINT32)rotation;
        LCD_OUTREG32(&LCD_REG->LAYER[id].CONTROL, AS_UINT32(&ctrl));
    }

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetAlphaBlending(LCD_LAYER_ID id, BOOL enable, UINT8 alpha)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_REG_LAYER_CON ctrl = LCD_REG->LAYER[id].CONTROL;
        ctrl.OPAEN = enable ? 1 : 0;
        ctrl.OPA   = alpha;
        LCD_OUTREG32(&LCD_REG->LAYER[id].CONTROL, AS_UINT32(&ctrl));
    }
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_LayerSetSourceColorKey(LCD_LAYER_ID id, BOOL enable, UINT32 colorKey)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_REG_LAYER_CON ctrl = LCD_REG->LAYER[id].CONTROL;
        ctrl.SRC_KEY_EN = enable ? 1 : 0;
        LCD_OUTREG32(&LCD_REG->LAYER[id].CONTROL, AS_UINT32(&ctrl));
        LCD_REG->LAYER[id].COLORKEY= colorKey;
    }
    
    return LCD_STATUS_OK;
}

LCD_STATUS LCD_LayerSetDestColorKey(LCD_LAYER_ID id, BOOL enable, UINT32 colorKey)
{
    ASSERT(id < LCD_LAYER_NUM || LCD_LAYER_ALL == id);

    _WaitForEngineNotBusy();

    if (LCD_LAYER_ALL == id)
    {
        NOT_IMPLEMENTED();
    }
    else if (id < LCD_LAYER_NUM)
    {
        LCD_REG_LAYER_CON ctrl = LCD_REG->LAYER[id].CONTROL;
        ctrl.DST_KEY_EN = enable ? 1 : 0;
        LCD_OUTREG32(&LCD_REG->LAYER[id].CONTROL, AS_UINT32(&ctrl));
        LCD_REG->LAYER[id].COLORKEY = colorKey;
    }
    
    return LCD_STATUS_OK;
}

// -------------------- HW Trigger Configurations --------------------

LCD_STATUS LCD_LayerSetTriggerMode(LCD_LAYER_ID id, LCD_LAYER_TRIGGER_MODE mode)
{
    LCD_WROI_HWREF hwref;
    LCD_REG_WROI_DC dc;

    BOOL enHwTrigger = (LCD_SW_TRIGGER != mode);
    BOOL enDirectCouple = (LCD_HW_TRIGGER_DIRECT_COUPLE == mode);

    _WaitForEngineNotBusy();

    hwref = LCD_REG->WROI_HW_REFRESH;
    dc = LCD_REG->WROI_DC;
        
    switch(id)
    {
    case LCD_LAYER_0 :
        hwref.EN0 = enHwTrigger ? 1 : 0;
        dc.EN0 = enDirectCouple ? 1 : 0;
        break;
    case LCD_LAYER_1 : 
        hwref.EN1 = enHwTrigger ? 1 : 0;
        dc.EN1 = enDirectCouple ? 1 : 0;
        break;
    case LCD_LAYER_2 : 
        hwref.EN2 = enHwTrigger ? 1 : 0;
        dc.EN2 = enDirectCouple ? 1 : 0;
        break;
    case LCD_LAYER_3 : 
        hwref.EN3 = enHwTrigger ? 1 : 0;
        dc.EN3 = enDirectCouple ? 1 : 0;
        break;
    case LCD_LAYER_4 : 
        hwref.EN4 = enHwTrigger ? 1 : 0;
        dc.EN4 = enDirectCouple ? 1 : 0;
        break;
    case LCD_LAYER_5 : 
        hwref.EN5 = enHwTrigger ? 1 : 0;
        dc.EN5 = enDirectCouple ? 1 : 0;
        break;
    case LCD_LAYER_ALL :
        LCD_MASKREG32(&hwref, 0xFC000000, enHwTrigger ? 0xFC000000 : 0);
        LCD_MASKREG32(&dc, 0xFC000000, enDirectCouple ? 0xFC000000 : 0);
        break;
    default : ASSERT(0);
    }

    LCD_OUTREG32(&LCD_REG->WROI_HW_REFRESH, AS_UINT32(&hwref));
    LCD_OUTREG32(&LCD_REG->WROI_DC, AS_UINT32(&dc));

    return LCD_STATUS_OK;
}

LCD_STATUS LCD_EnableHwTrigger(BOOL enable)
{
    LCD_WROI_HWREF hwref = LCD_REG->WROI_HW_REFRESH;
    hwref.HWEN = enable ? 1 : 0;
    LCD_OUTREG32(&LCD_REG->WROI_HW_REFRESH, AS_UINT32(&hwref));

    // Disable all layers direct-couple
    //
    if (!enable && (INREG32(&LCD_REG->WROI_DC) & 0xFC000000))
    {
        LCD_MASKREG32(&LCD_REG->WROI_DC, 0xFC000000, 0);
    }

    return LCD_STATUS_OK;
}


// -------------------- ROI Window Configurations --------------------

LCD_STATUS LCD_SetBackgroundColor(UINT32 bgColor)
{
    _WaitForEngineNotBusy();
    LCD_REG->WROI_BG_COLOR = bgColor;
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_SetRoiWindow(UINT32 x, UINT32 y, UINT32 width, UINT32 height)
{
    LCD_REG_COORD offset;
    LCD_REG_SIZE size;
    
    offset.X = (UINT16)x;
    offset.Y = (UINT16)y;
    size.WIDTH = (UINT16)width;
    size.HEIGHT = (UINT16)height;

    _WaitForEngineNotBusy();
    LCD_OUTREG32(&LCD_REG->WROI_OFFSET, AS_UINT32(&offset));
    LCD_OUTREG32(&LCD_REG->WROI_SIZE, AS_UINT32(&size));

    _lcdContext.roiWndSize = size;
        
    return LCD_STATUS_OK;
}

// -------------------- Output to Memory Configurations --------------------

LCD_STATUS LCD_SetOutputMode(LCD_OUTPUT_MODE mode)
{
    LCD_REG_WROI_CON    roiCtrl;
    LCD_REG_WROI_W2MCON w2mCtrl;

    _WaitForEngineNotBusy();

    roiCtrl = LCD_REG->WROI_CONTROL;
    w2mCtrl = LCD_REG->WROI_W2M_CONTROL;

    w2mCtrl.W2LCM     = (mode & LCD_OUTPUT_TO_LCM) ? 1 : 0;
    roiCtrl.W2M       = (mode & LCD_OUTPUT_TO_MEM) ? 1 : 0;
    w2mCtrl.DC_TV_ROT = (mode & LCD_OUTPUT_TO_TVROT) ? 1 : 0;

    LCD_OUTREG32(&LCD_REG->WROI_CONTROL, AS_UINT32(&roiCtrl));
    LCD_OUTREG32(&LCD_REG->WROI_W2M_CONTROL, AS_UINT32(&w2mCtrl));

    _lcdContext.outputMode = mode;

    return LCD_STATUS_OK;    
}


LCD_STATUS LCD_WaitDPIIndication(BOOL enable)
{
    LCD_REG_WROI_W2MCON w2mCtrl;

    _WaitForEngineNotBusy();

    w2mCtrl = LCD_REG->WROI_W2M_CONTROL;
    w2mCtrl.DLY_EN = enable ? 1 : 0;
    LCD_OUTREG32(&LCD_REG->WROI_W2M_CONTROL, AS_UINT32(&w2mCtrl));
    
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_FBSetFormat(LCD_FB_FORMAT format)
{
    LCD_REG_WROI_W2MCON w2mCtrl;

    _WaitForEngineNotBusy();

    w2mCtrl = LCD_REG->WROI_W2M_CONTROL;
    w2mCtrl.W2M_FMT = (UINT32)format;
    LCD_OUTREG32(&LCD_REG->WROI_W2M_CONTROL, AS_UINT32(&w2mCtrl));

    _lcdContext.fbFormat = format;

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_FBSetPitch(UINT32 pitchInByte)
{
    _lcdContext.fbPitchInBytes = pitchInByte;

    LCD_REG->W2M_PITCH = pitchInByte;
    return LCD_STATUS_OK;
}


LCD_STATUS LCD_FBEnable(LCD_FB_ID id, BOOL enable)
{
    LCD_REG_WROI_W2MCON w2mCtrl;

    _WaitForEngineNotBusy();

    w2mCtrl = LCD_REG->WROI_W2M_CONTROL;

    switch(id)
    {
    case LCD_FB_0 : // do nothing
        break;
        
    case LCD_FB_1 :
        w2mCtrl.FB1_EN = enable ? 1 : 0;
        break;

    case LCD_FB_2 :
        w2mCtrl.FB2_EN = enable ? 1 : 0;
        break;

    default:
        ASSERT(0);
    }

    LCD_OUTREG32(&LCD_REG->WROI_W2M_CONTROL, AS_UINT32(&w2mCtrl));

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_FBSetAddress(LCD_FB_ID id, UINT32 address)
{
    ASSERT(id < LCD_FB_NUM);

    _WaitForEngineNotBusy();
    LCD_REG->WROI_W2M_ADDR[id] = address;

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_FBSetStartCoord(UINT32 x, UINT32 y)
{
    LCD_REG_COORD offset;
    offset.X = (UINT16)x;
    offset.Y = (UINT16)y;

    _WaitForEngineNotBusy();

    //WDT_SW_MM_PERI_RESET(MM_PERI_LCD);
    
    LCD_OUTREG32(&LCD_REG->WROI_W2M_OFFSET, AS_UINT32(&offset));

    return LCD_STATUS_OK;
}

// -------------------- Color Matrix --------------------

LCD_STATUS LCD_EnableColorMatrix(LCD_IF_ID id, BOOL enable)
{
#warning "the color matrix is different on MT6573"
    switch(id)
    {
    case LCD_IF_PARALLEL_0 :
    case LCD_IF_PARALLEL_1 :
    case LCD_IF_PARALLEL_2 :
    {
        UINT32 i = id - LCD_IF_PARALLEL_0;
        LCD_REG_PCNF config = LCD_REG->PARALLEL_CFG[i];
        LCD_OUTREG32(&LCD_REG->PARALLEL_CFG[i], AS_UINT32(&config));
        break;
    }
    case LCD_IF_SERIAL_0 :
    case LCD_IF_SERIAL_1 :
    {
        LCD_REG_SCNF config = LCD_REG->SERIAL_CFG;

        LCD_OUTREG32(&LCD_REG->SERIAL_CFG, AS_UINT32(&config));
        break;
    }
    case LCD_IF_ALL :
    {
        LCD_EnableColorMatrix(LCD_IF_PARALLEL_0, enable);
        LCD_EnableColorMatrix(LCD_IF_PARALLEL_1, enable);
        LCD_EnableColorMatrix(LCD_IF_PARALLEL_2, enable);
        LCD_EnableColorMatrix(LCD_IF_SERIAL_0, enable);
        break;
    }
    default: ASSERT(0);
    }

    return LCD_STATUS_OK;
}

/** Input: const S2_8 mat[9], fixed ponit signed 2.8 format
           |                      |
           | mat[0] mat[1] mat[2] |
           | mat[3] mat[4] mat[5] |
           | mat[6] mat[7] mat[8] |
           |                      |
*/
LCD_STATUS LCD_SetColorMatrix(const S2_8 mat[9])
{
#warning "the color matrix is different on MT6573"
    UINT32 i, j = 0;

    for (i = 0; i < ARY_SIZE(LCD_REG->COEF_ROW); ++ i)
    {
        LCD_REG_COEF_ROW row = LCD_REG->COEF_ROW[i];
        row.COL0 = mat[j++];
        row.COL1 = mat[j++];
        row.COL2 = mat[j++];
        LCD_OUTREG32(&LCD_REG->COEF_ROW[i], AS_UINT32(&row));
    }

    return LCD_STATUS_OK;
}

// -------------------- Tearing Control --------------------

LCD_STATUS LCD_TE_Enable(BOOL enable)
{
    LCD_REG_TECON tecon = LCD_REG->TEARING_CFG;
    tecon.ENABLE = enable ? 1 : 0;
    LCD_OUTREG32(&LCD_REG->TEARING_CFG, AS_UINT32(&tecon));

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_TE_SetMode(LCD_TE_MODE mode)
{
    LCD_REG_TECON tecon = LCD_REG->TEARING_CFG;
    tecon.MODE = (LCD_TE_MODE_VSYNC_OR_HSYNC == mode) ? 1 : 0;
    LCD_OUTREG32(&LCD_REG->TEARING_CFG, AS_UINT32(&tecon));

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_TE_SetEdgePolarity(BOOL polarity)
{
    LCD_REG_TECON tecon = LCD_REG->TEARING_CFG;
    tecon.EDGE_SEL = (polarity ? 1 : 0);
    LCD_OUTREG32(&LCD_REG->TEARING_CFG, AS_UINT32(&tecon));

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_TE_ConfigVHSyncMode(UINT32 hsDelayCnt,
                                   UINT32 vsWidthCnt,
                                   LCD_TE_VS_WIDTH_CNT_DIV vsWidthCntDiv)
{
/*    LCD_REG_TECON tecon = LCD_REG->TEARING_CFG;
    tecon.HS_MCH_CNT = (hsDelayCnt ? hsDelayCnt - 1 : 0);
    tecon.VS_WLMT = (vsWidthCnt ? vsWidthCnt - 1 : 0);
    tecon.VS_CNT_DIV = vsWidthCntDiv;
    LCD_OUTREG32(&LCD_REG->TEARING_CFG, AS_UINT32(&tecon));
*/
    return LCD_STATUS_OK;
}

// -------------------- Operations --------------------

LCD_STATUS LCD_SelectWriteIF(LCD_IF_ID id)
{
//    LCD_REG_CMD_ADDR cmd_addr;
//	LCD_REG_DAT_ADDR dat_addr;
	UINT16 cmd_addr = 0;
	UINT16 dat_addr = 0;

    switch(id)
    {
    case LCD_IF_PARALLEL_0 : cmd_addr |= 0; break;
    case LCD_IF_PARALLEL_1 : cmd_addr |= (2<<4); break;
    case LCD_IF_PARALLEL_2 : cmd_addr |= (4<<4); break;
    case LCD_IF_SERIAL_0   : cmd_addr |= (8<<4); break;
    case LCD_IF_SERIAL_1   : cmd_addr |= (0xa<<4); break;
    default:
        ASSERT(0);
    }
	dat_addr = cmd_addr + (1<<4);
    LCD_OUTREG16(&LCD_REG->WROI_CMD_ADDR, cmd_addr);
    LCD_OUTREG16(&LCD_REG->WROI_DATA_ADDR, dat_addr);

    return LCD_STATUS_OK;
}


__inline static void _LCD_WriteIF(DWORD baseAddr, UINT32 value, LCD_IF_MCU_WRITE_BITS bits)
{
    switch(bits)
    {
    case LCD_IF_MCU_WRITE_8BIT :
        LCD_OUTREG8(baseAddr, value);
        break;
        
    case LCD_IF_MCU_WRITE_16BIT :
        LCD_OUTREG16(baseAddr, value);
        break;
        
    case LCD_IF_MCU_WRITE_32BIT :
        LCD_OUTREG32(baseAddr, value);
        break;

    default:
        ASSERT(0);
    }
}


LCD_STATUS LCD_WriteIF(LCD_IF_ID id, LCD_IF_A0_MODE a0,
                       UINT32 value, LCD_IF_MCU_WRITE_BITS bits)
{
    DWORD baseAddr = 0;

    switch(id)
    {
    case LCD_IF_PARALLEL_0 : baseAddr = (DWORD)&LCD_REG->PCMD0; break;
    case LCD_IF_PARALLEL_1 : baseAddr = (DWORD)&LCD_REG->PCMD1; break;
    case LCD_IF_PARALLEL_2 : baseAddr = (DWORD)&LCD_REG->PCMD2; break;
    case LCD_IF_SERIAL_0   : baseAddr = (DWORD)&LCD_REG->SCMD0; break;
    case LCD_IF_SERIAL_1   : baseAddr = (DWORD)&LCD_REG->SCMD1; break;
    default:
        ASSERT(0);
    }

    if (LCD_IF_A0_HIGH == a0)
    {
        baseAddr += LCD_A0_HIGH_OFFSET;
    }

    _LCD_WriteIF(baseAddr, value, bits);

    return LCD_STATUS_OK;
}


__inline static UINT32 _LCD_ReadIF(DWORD baseAddr, LCD_IF_MCU_WRITE_BITS bits)
{
    switch(bits)
    {
    case LCD_IF_MCU_WRITE_8BIT :
        return (UINT32)INREG8(baseAddr);
        
    case LCD_IF_MCU_WRITE_16BIT :
        return (UINT32)INREG16(baseAddr);
        
    case LCD_IF_MCU_WRITE_32BIT :
        return (UINT32)INREG32(baseAddr);

    default:
        ASSERT(0);
    }
}


LCD_STATUS LCD_ReadIF(LCD_IF_ID id, LCD_IF_A0_MODE a0,
                      UINT32 *value, LCD_IF_MCU_WRITE_BITS bits)
{
    DWORD baseAddr = 0;

    if (NULL == value) return LCD_STATUS_ERROR;

    switch(id)
    {
    case LCD_IF_PARALLEL_0 : baseAddr = (DWORD)&LCD_REG->PCMD0; break;
    case LCD_IF_PARALLEL_1 : baseAddr = (DWORD)&LCD_REG->PCMD1; break;
    case LCD_IF_PARALLEL_2 : baseAddr = (DWORD)&LCD_REG->PCMD2; break;
    case LCD_IF_SERIAL_0   : baseAddr = (DWORD)&LCD_REG->SCMD0; break;
    case LCD_IF_SERIAL_1   : baseAddr = (DWORD)&LCD_REG->SCMD1; break;
    default:
        ASSERT(0);
    }

    if (LCD_IF_A0_HIGH == a0)
    {
        baseAddr += LCD_A0_HIGH_OFFSET;
    }

    *value = _LCD_ReadIF(baseAddr, bits);

    return LCD_STATUS_OK;
}


LCD_STATUS LCD_StartTransfer(BOOL blocking)
{
    LCD_REG_SIZE mainWndSize = _lcdContext.roiWndSize;

    if (_lcdContext.outputMode & LCD_OUTPUT_TO_MEM)
    {
        UINT32 bpp = TO_BPP[_lcdContext.fbFormat];

        // Set pitch in pixel here according to frame buffer BPP
        ASSERT(_lcdContext.fbPitchInBytes);
        ASSERT(_lcdContext.fbPitchInBytes % bpp == 0);
        mainWndSize.WIDTH = (UINT16)(_lcdContext.fbPitchInBytes / bpp);
    }

    //LCD_OUTREG32(&LCD_REG->MWIN_SIZE, AS_UINT32(&mainWndSize));
	LCD_SetGMCThrottle(); 
    _WaitForEngineNotBusy();
    
	LCD_OUTREG32(&LCD_REG->START, 0);
    LCD_OUTREG32(&LCD_REG->START, (1 << 15));

    if (blocking)
    {
        _WaitForEngineNotBusy();
    }

    return LCD_STATUS_OK;
}

// -------------------- Retrieve Information --------------------

LCD_STATE  LCD_GetState(void)
{
    if (!s_isLcdPowerOn)
    {
        return LCD_STATE_POWER_OFF;
    }

    if (_IsEngineBusy())
    {
        return LCD_STATE_BUSY;
    }

    return LCD_STATE_IDLE;
}


LCD_STATUS LCD_DumpRegisters(void)
{
    UINT32 i;

    printk("---------- Start dump LCD registers ----------\n");
    
    for (i = 0; i < offsetof(LCD_REGS, COEF_ROW); i += 4)
    {
        printk("LCD+%04x : 0x%08x\n", i, INREG32(LCD_BASE + i));
    }

    return LCD_STATUS_OK;
}

LCD_STATUS LCD_W2M_NeedLimiteSpeed(BOOL enable)
{
	limit_w2m_speed = enable;
}

static bool LCD_IsLayerEnable(LCD_LAYER_ID id)
{
    LCD_REG_WROI_CON ctrl;

//    _WaitForEngineNotBusy();
	bool ret = false;
    ctrl = LCD_REG->WROI_CONTROL;
    
    switch(id)
    {
    case LCD_LAYER_0 :if(ctrl.EN0 == 1)ret = true;break;
    case LCD_LAYER_1 :if(ctrl.EN1 == 1)ret = true;break;   
	case LCD_LAYER_2 :if(ctrl.EN2 == 1)ret = true;break;   
	case LCD_LAYER_3 :if(ctrl.EN3 == 1)ret = true;break;  
	case LCD_LAYER_4 :if(ctrl.EN4 == 1)ret = true;break;   
	case LCD_LAYER_5 :if(ctrl.EN5 == 1)ret = true;break;
    default : ASSERT(0);
    }
    return ret;
}

LCD_STATUS LCD_SetGMCThrottle()
{
    UINT16 i, pitch, h;

	UINT32 total_req = 0;
	INT32 throttle_cnt = 0;

	if(!limit_w2m_speed){
		LCD_OUTREG32(&LCD_REG->GMC_CON, (throttle_cnt << 16) | (0 << 4) | 0x4);
		return LCD_STATUS_OK;
	}

	for(i=0;i<LCD_LAYER_NUM;i++){
		if(LCD_IsLayerEnable((LCD_LAYER_ID)i)){
	  	    pitch = LCD_REG->LAYER[(LCD_LAYER_ID)i].WINDOW_PITCH;
   		    h = LCD_REG->LAYER[(LCD_LAYER_ID)i].SIZE.HEIGHT;
			total_req += pitch * h;
		}
	}

	if(total_req != 0)
		throttle_cnt = 34666666/total_req - 2;//34666666 is calculated by 130MHz/60fps*16
	
	if(throttle_cnt < 0)
		throttle_cnt = 0;	
	LCD_OUTREG32(&LCD_REG->GMC_CON, (throttle_cnt << 16) | (1 << 4) | 0x4);
	
	return LCD_STATUS_OK;
}
