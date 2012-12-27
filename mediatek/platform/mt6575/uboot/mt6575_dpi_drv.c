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

#define ENABLE_DPI_INTERRUPT        0
#define ENABLE_DPI_REFRESH_RATE_LOG 0

#if ENABLE_DPI_REFRESH_RATE_LOG && !ENABLE_DPI_INTERRUPT
#error "ENABLE_DPI_REFRESH_RATE_LOG should be also ENABLE_DPI_INTERRUPT"
#endif

#include <common.h>
#include <mmc.h>
#include <part.h>
#include <fat.h>
#include <malloc.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/arch/mt65xx.h>
#include <asm/arch/mt65xx_typedefs.h>
#include <asm/arch/mt65xx_disp_drv.h>
#include <asm/arch/mt65xx_dpi_drv.h>
#include <asm/arch/mt65xx_lcd_drv.h>
#include <asm/arch/mt65xx_dpi_regs.h>
#include <asm/arch/mt65xx_dsi_regs.h>
#include <asm/arch/mt6575_gpio.h>

#if ENABLE_DPI_INTERRUPT
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <mach/irqs.h>
#endif

PDPI_REGS const DPI_REG = (PDPI_REGS)(DPI_BASE);

PDSI_PHY_REGS const DSI_PHY_REG_DPI = (PDSI_PHY_REGS)(MIPI_CONFG_BASE + 0x800);
UINT32 const PLL_SOURCE = APMIXED_BASE + 0x44;
static BOOL s_isDpiPowerOn = FALSE;
static DPI_REGS regBackup;

#define DPI_REG_OFFSET(r)       offsetof(DPI_REGS, r)
#define REG_ADDR(base, offset)  (((BYTE *)(base)) + (offset))

UINT32 const INFRA_DRV_CON1 = 0xC0001804;
UINT32 const INFRA_DRV_CON2 = 0xC0001808;
UINT32 const INFRA_DRV_CON12 = 0xC0001830;

const UINT32 BACKUP_DPI_REG_OFFSETS[] =
{
    DPI_REG_OFFSET(INT_ENABLE),
    DPI_REG_OFFSET(SIZE),
    DPI_REG_OFFSET(CLK_CNTL),
//    DPI_REG_OFFSET(DITHER),

    DPI_REG_OFFSET(FB[0].ADDR),
    DPI_REG_OFFSET(FB[0].STEP),
    DPI_REG_OFFSET(FB[1].ADDR),
    DPI_REG_OFFSET(FB[1].STEP),
    DPI_REG_OFFSET(FB[2].ADDR),
    DPI_REG_OFFSET(FB[2].STEP),

    DPI_REG_OFFSET(OVL_CON),
//    DPI_REG_OFFSET(FBCD_LINE_W),
    DPI_REG_OFFSET(FIFO_TH),
    DPI_REG_OFFSET(FIFO_INC),
//    DPI_REG_OFFSET(FIFO_TH_MAX),
    DPI_REG_OFFSET(REQ_TH),

    DPI_REG_OFFSET(TGEN_HCNTL),
    DPI_REG_OFFSET(TGEN_VCNTL),

    DPI_REG_OFFSET(CUSR_CNTL),
    DPI_REG_OFFSET(CUSR_COORD),
    DPI_REG_OFFSET(CUSR_ADDR),
	DPI_REG_OFFSET(GMC_CNTL),
    DPI_REG_OFFSET(TMODE),

    DPI_REG_OFFSET(CNTL),
};

static void _BackupDPIRegisters(void)
{
    DPI_REGS *reg = &regBackup;
    UINT32 i;
    
    for (i = 0; i < ARY_SIZE(BACKUP_DPI_REG_OFFSETS); ++ i)
    {
        OUTREG32(REG_ADDR(reg, BACKUP_DPI_REG_OFFSETS[i]),
                 AS_UINT32(REG_ADDR(DPI_REG, BACKUP_DPI_REG_OFFSETS[i])));
    }
}

static void _RestoreDPIRegisters(void)
{
    DPI_REGS *reg = &regBackup;
    UINT32 i;

    for (i = 0; i < ARY_SIZE(BACKUP_DPI_REG_OFFSETS); ++ i)
    {
        OUTREG32(REG_ADDR(DPI_REG, BACKUP_DPI_REG_OFFSETS[i]),
                 AS_UINT32(REG_ADDR(reg, BACKUP_DPI_REG_OFFSETS[i])));
    }
}

static void _ResetBackupedDPIRegisterValues(void)
{
    DPI_REGS *regs = &regBackup;
    memset((void*)regs, 0, sizeof(DPI_REGS));

    OUTREG32(&regs->CLK_CNTL, 0x00000101);
    OUTREG32(&regs->REQ_TH, 0x00000200);
}


#if ENABLE_DPI_REFRESH_RATE_LOG
static void _DPI_LogRefreshRate(DPI_REG_INTERRUPT status)
{
    static unsigned long prevUs = 0xFFFFFFFF;

    if (status.VSYNC)
    {
        struct timeval curr;
        do_gettimeofday(&curr);

        if (prevUs < curr.tv_usec)
        {
            printk("[DPI][Debug] Receive 1 vsync in %lu us\n", 
                   curr.tv_usec - prevUs);
        }
        prevUs = curr.tv_usec;
    }
}
#else
#define _DPI_LogRefreshRate(x)  do {} while(0)
#endif

#if ENABLE_DPI_INTERRUPT
static __tcmfunc irqreturn_t _DPI_InterruptHandler(int irq, void *dev_id)
{   
    static int counter = 0;
    DPI_REG_INTERRUPT status = DPI_REG->INT_STATUS;
    if (status.FIFO_EMPTY) ++ counter;

    if (status.VSYNC && counter) {
        printk("[Error] DPI FIFO is empty, "
               "received %d times interrupt !!!\n", counter);
        counter = 0;
    }

    _DPI_LogRefreshRate(status);

    return IRQ_HANDLED;
}
#endif


DPI_STATUS DPI_Init(BOOL isDpiPoweredOn)
{
    DPI_REG_CNTL cntl;

    if (isDpiPoweredOn) {
        _BackupDPIRegisters();
        s_isDpiPowerOn = TRUE;
    } else {
        _ResetBackupedDPIRegisterValues();
        DPI_PowerOn();
    }

    // Always enable frame shift protection and recovery
    cntl = DPI_REG->CNTL;
    cntl.FS_PROT_EN = 1;
    cntl.FS_RC_EN = 1;

    // Enable adaptive FIFO high/low threshold control
    cntl.ADP_EN = 1;
     
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    // Config ultra high threshold water mark
    {
        DPI_REG_FIFO_TH th = DPI_REG->FIFO_TH;

        th.LOW = 400;
        th.HIGH = 400;

        OUTREG32(&DPI_REG->FIFO_TH, AS_UINT32(&th));
        DPI_REG->FIFO_INC.FIFO_TH_INC = 8;
		DPI_REG->FIFO_INC.FIFO_TH_DEC = 8;
    }

#if ENABLE_DPI_INTERRUPT
    if (request_irq(MT6575_DPI_IRQ_ID,
        _DPI_InterruptHandler, IRQF_TRIGGER_LOW, MTKFB_DRIVER, NULL) < 0)
    {
        printk("[DPI][ERROR] fail to request DPI irq\n"); 
        return DPI_STATUS_ERROR;
    }

    {
        DPI_REG_INTERRUPT enInt = DPI_REG->INT_ENABLE;
        enInt.FIFO_EMPTY = 1;
        enInt.VSYNC = 1;
        OUTREG32(&DPI_REG->INT_ENABLE, AS_UINT32(&enInt));
    }
#endif
    OUTREG32(&DPI_REG->GMC_CNTL, 0x41);
	LCD_W2M_NeedLimiteSpeed(TRUE);
    return DPI_STATUS_OK;
}


DPI_STATUS DPI_Deinit(void)
{
    DPI_DisableClk();
    DPI_PowerOff();

    return DPI_STATUS_OK;
}

DPI_STATUS DPI_Init_PLL(unsigned int mipi_pll_clk_ref,unsigned int mipi_pll_clk_div1,unsigned int mipi_pll_clk_div2)
{
#if 1
    MIPITX_CFG0_REG con0 = DSI_PHY_REG_DPI->MIPITX_CON0;
    MIPITX_CFG1_REG con1 = DSI_PHY_REG_DPI->MIPITX_CON1;
//	mt_gpio_dump();
	OUTREG16(0xc2080858, 0x8000);
	OUTREG16(0xc20a3824, 0x4008);
	MASKREG16(0xc20a380c, 0x000c, 0x0000); //default value is 0x7008, but should be 0x7000
	MASKREG16(PLL_SOURCE, 0x0010, 0x0010);

    con1.RG_PLL_DIV1 = mipi_pll_clk_div1;
    con1.RG_PLL_DIV2 = mipi_pll_clk_div2;

	con0.PLL_CLKR_EN = 1;
	con0.PLL_EN = 1;
	con0.RG_DPI_EN = 1;

    // Set to DSI_PHY_REG
    
    OUTREG32(&DSI_PHY_REG_DPI->MIPITX_CON0, AS_UINT32(&con0));
    OUTREG32(&DSI_PHY_REG_DPI->MIPITX_CON1, AS_UINT32(&con1));
#endif
	return DPI_STATUS_OK;
}

DPI_STATUS DPI_Set_DrivingCurrent(LCM_PARAMS *lcm_params)
{
	printf("dpi set driving:0x%x,0x%x,0x%x\n", INREG32(INFRA_DRV_CON1),INREG32(INFRA_DRV_CON2),INREG32(INFRA_DRV_CON12));
	/*set DPI data & control current*/
	MASKREG32(INFRA_DRV_CON12, 0x0ffff000,(((lcm_params->dpi.io_driving_current >> 4) - 1) << 24)|
										  (((lcm_params->dpi.io_driving_current >> 4) - 1) << 20)|
										  (((lcm_params->dpi.io_driving_current >> 4) - 1) << 16)|
										  (((lcm_params->dpi.io_driving_current >> 4) - 1) << 12));
	/*set LCD SIF current*/
	MASKREG32(INFRA_DRV_CON1, 0xffff0000,(((lcm_params->dbi.io_driving_current >> 4) - 1) << 28)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 24)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 20)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 16));
	MASKREG32(INFRA_DRV_CON2, 0x000000ff,(((lcm_params->dbi.io_driving_current >> 4) - 1) << 0)|
										  (((lcm_params->dbi.io_driving_current >> 4) - 1) << 4));
	printf("dpi set driving:0x%x,0x%x,0x%x\n", INREG32(INFRA_DRV_CON1),INREG32(INFRA_DRV_CON2),INREG32(INFRA_DRV_CON12));
	return DPI_STATUS_OK;
}

DPI_STATUS DPI_PowerOn()
{
    if (!s_isDpiPowerOn)
    {
#if 1   // FIXME
        MASKREG32(0xC2080028, 0x10, 0x10);
#endif        
        _RestoreDPIRegisters();
        s_isDpiPowerOn = TRUE;
    }

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_PowerOff()
{
    if (s_isDpiPowerOn)
    {
        BOOL ret = TRUE;
        _BackupDPIRegisters();
#if 1   // FIXME
        MASKREG32(0xC2080018, 0x10, 0x10);
#endif        
        ASSERT(ret);
        s_isDpiPowerOn = FALSE;
    }

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_EnableClk()
{
	DPI_REG_EN en = DPI_REG->DPI_EN;
    en.EN = 1;
    OUTREG32(&DPI_REG->DPI_EN, AS_UINT32(&en));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_DisableClk()
{
    DPI_REG_EN en = DPI_REG->DPI_EN;
    en.EN = 0;
    OUTREG32(&DPI_REG->DPI_EN, AS_UINT32(&en));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_EnableSeqOutput(BOOL enable)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.INTF_68_EN = enable ? 1 : 0;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_SetRGBOrder(DPI_RGB_ORDER input, DPI_RGB_ORDER output)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.SRGB_ORDER = input;    // Source RGB Order
    cntl.IRGB_ORDER = output;   // Interface RGB Order
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigPixelClk(DPI_POLARITY polarity, UINT32 divisor, UINT32 duty)
{
    DPI_REG_CLKCNTL ctrl;

    ASSERT(divisor >= 2);
    ASSERT(duty > 0 && duty < divisor);
    
    ctrl.POLARITY = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    ctrl.DIVISOR = divisor - 1;
    ctrl.DUTY = duty;

    OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&ctrl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigDataEnable(DPI_POLARITY polarity)
{
    DPI_REG_TGEN_HCNTL hctrl = DPI_REG->TGEN_HCNTL;
    hctrl.DE_POL = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    OUTREG32(&DPI_REG->TGEN_HCNTL, AS_UINT32(&hctrl));
    
    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigVsync(DPI_POLARITY polarity, UINT32 pulseWidth, UINT32 backPorch,
                           UINT32 frontPorch)
{
    DPI_REG_TGEN_VCNTL ctrl;

    ctrl.VSYNC_POL = (DPI_POLARITY_RISING == polarity) ? 1 : 0;
    ctrl.VPW = pulseWidth - 1;
    ctrl.VBP = backPorch - 1;
    ctrl.VFP = frontPorch - 1;

    OUTREG32(&DPI_REG->TGEN_VCNTL, AS_UINT32(&ctrl));
    
    return DPI_STATUS_OK;
}


DPI_STATUS DPI_ConfigHsync(DPI_POLARITY polarity, UINT32 pulseWidth, UINT32 backPorch,
                           UINT32 frontPorch)
{
    DPI_REG_TGEN_HCNTL ctrl = DPI_REG->TGEN_HCNTL;

    ctrl.HSYNC_POL = (DPI_POLARITY_RISING == polarity) ? 1 : 0;
    ctrl.HPW = pulseWidth - 1;
    ctrl.HBP = backPorch - 1;
    ctrl.HFP = frontPorch - 1;

    OUTREG32(&DPI_REG->TGEN_HCNTL, AS_UINT32(&ctrl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBEnable(DPI_FB_ID id, BOOL enable)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;

    switch (id)
    {
        case DPI_FB_0:
            // do nothing
            break;

        case DPI_FB_1:
            cntl.FB1_EN = enable ? 1 : 0;
            break;

        case DPI_FB_2:
            cntl.FB2_EN = enable ? 1 : 0;
            break;

        default:
            ASSERT(0);
    }

    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBSyncFlipWithLCD(BOOL enable)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.FB_CHK_EN = enable ? 1 : 0;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_SetDSIMode(BOOL enable)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.DSI_MODE = enable ? 1 : 0;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}


BOOL DPI_IsDSIMode(void)
{

	return DPI_REG->CNTL.DSI_MODE ? TRUE : FALSE;
	
}


DPI_STATUS DPI_FBSetFormat(DPI_FB_FORMAT format)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    cntl.PXL_FMT = format;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&cntl));

    return DPI_STATUS_OK;
}

DPI_FB_FORMAT DPI_FBGetFormat(void)
{
    DPI_REG_CNTL cntl = DPI_REG->CNTL;
    return cntl.PXL_FMT;
}



DPI_STATUS DPI_FBSetSize(UINT32 width, UINT32 height)
{
    DPI_REG_SIZE size;
    size.WIDTH = width;
    size.HEIGHT = height;
    
    OUTREG32(&DPI_REG->SIZE, AS_UINT32(&size));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FBSetAddress(DPI_FB_ID id, UINT32 address)
{
    ASSERT(id < DPI_FB_NUM);
    DPI_REG->FB[id].ADDR = address;

    return DPI_STATUS_OK;
}    


DPI_STATUS DPI_FBSetPitch(DPI_FB_ID id, UINT32 pitchInByte)
{
    ASSERT(id < DPI_FB_NUM);
    DPI_REG->FB[id].STEP = pitchInByte;

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_SetFifoThreshold(UINT32 low, UINT32 high)
{
    DPI_REG_FIFO_TH th = DPI_REG->FIFO_TH;

    if (high > DPI_FIFO_TH_MAX) {
        high = DPI_FIFO_TH_MAX;
    }
    if (low > high) {
        low = high;
    }

    th.LOW = low;
    th.HIGH = high;
    OUTREG32(&DPI_REG->FIFO_TH, AS_UINT32(&th));

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_DumpRegisters(void)
{
    UINT32 i;

    printf("---------- Start dump DPI registers ----------\n");
    
    for (i = 0; i < sizeof(DPI_REGS); i += 4)
    {
        printf("DPI+%04x : 0x%08x\n", i, INREG32(DPI_BASE + i));
    }

    return DPI_STATUS_OK;
}
