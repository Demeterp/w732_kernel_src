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
#include <asm/arch/boot_mode.h>

#include <asm/arch/mt65xx_disp_drv.h>
#include <asm/arch/mt65xx_dsi_regs.h>
#include <asm/arch/mt65xx_lcd_regs.h>

#include <asm/arch/mt65xx_dsi_drv.h>
#include <asm/arch/mt65xx_lcd_drv.h>

/*
#define PLL_BASE			(0xF0060000)
#define DSI_PHY_BASE		(0xF0060B00)
#define DSI_BASE            	(0xF0140000)
*/

//#define PWR_OFF                 (APCONFIG_BASE + 0x0304)
//#define GRAPH1_PDN              (1 << 3)
#define G1_MEM_PDN              (APCONFIG_BASE + 0x0060)
#define G1_MEM_DSI              (1)
#define GRAPH1SYS_CG_SET        (GMC1_BASE + 0x320)
#define GRAPH1SYS_CG_CLR        (GMC1_BASE + 0x340)
#define GRAPH1SYS_CG_DSI        (1 << 28)

PDSI_REGS const DSI_REG = (PDSI_REGS)(DSI_BASE);
PDSI_PHY_REGS const DSI_PHY_REG = (PDSI_PHY_REGS)(MIPI_CONFG_BASE+0x800);
PDSI_CMDQ_REGS const DSI_CMDQ_REG = (PDSI_CMDQ_REGS)(DSI_BASE+0x180);

static bool s_isDsiPowerOn = FALSE;

typedef struct
{
	DSI_REGS regBackup;
} DSI_CONTEXT;

static DSI_CONTEXT _dsiContext = {0};

static void lcm_udelay(UINT32 us)
{
    udelay(us);
}


static void lcm_mdelay(UINT32 ms)
{
    udelay(1000 * ms);
}


static BOOL _IsEngineBusy(void)
{
	DSI_STATUS_REG status;

	status = DSI_REG->DSI_STA;
	
	if (status.BUSY || status.ERR_MSG)
	{
		printk("[DISP] DSI status BUSY: %d, ERR_MSG: %d !! \n", status.BUSY, status.ERR_MSG); 		
	
		return TRUE;
	}


	return FALSE;
}


static BOOL _IsCMDQBusy(void)
{
	DSI_INT_STATUS_REG INT_status;

	INT_status=DSI_REG->DSI_INTSTA;

	if (!INT_status.CMD_DONE)
	{
		printk("[DISP] DSI CMDQ status BUSY !! \n"); 		
	
		return TRUE;
	}

	return FALSE;
}


static void _WaitForEngineNotBusy(void)
{
	if (DSI_REG->DSI_MODE_CTRL.MODE)
		return ;

#if ENABLE_DSI_INTERRUPT
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

    while(_IsEngineBusy()) {}

#endif    
}

static void _BackupDSIRegisters(void)
{
    DSI_REGS *regs = &(_dsiContext.regBackup);
    UINT32 i;
	
    //memcpy((void*)&(_dsiContext.regBackup), (void*)DSI_BASE, sizeof(DSI_REGS));

    OUTREG32(&regs->DSI_INTEN, AS_UINT32(&DSI_REG->DSI_INTEN));
    OUTREG32(&regs->DSI_MODE_CTRL, AS_UINT32(&DSI_REG->DSI_MODE_CTRL));
    OUTREG32(&regs->DSI_TXRX_CTRL, AS_UINT32(&DSI_REG->DSI_TXRX_CTRL));
    OUTREG32(&regs->DSI_PSCTRL, AS_UINT32(&DSI_REG->DSI_PSCTRL));

    OUTREG32(&regs->DSI_VSA_NL, AS_UINT32(&DSI_REG->DSI_VSA_NL));		
    OUTREG32(&regs->DSI_VBP_NL, AS_UINT32(&DSI_REG->DSI_VBP_NL));		
    OUTREG32(&regs->DSI_VFP_NL, AS_UINT32(&DSI_REG->DSI_VFP_NL));		
    OUTREG32(&regs->DSI_VACT_NL, AS_UINT32(&DSI_REG->DSI_VACT_NL));		

    OUTREG32(&regs->DSI_LINE_NB, AS_UINT32(&DSI_REG->DSI_LINE_NB));		
    OUTREG32(&regs->DSI_HSA_NB, AS_UINT32(&DSI_REG->DSI_HSA_NB));		
    OUTREG32(&regs->DSI_HBP_NB, AS_UINT32(&DSI_REG->DSI_HBP_NB));		
    OUTREG32(&regs->DSI_HFP_NB, AS_UINT32(&DSI_REG->DSI_HFP_NB));		

    OUTREG32(&regs->DSI_RGB_NB, AS_UINT32(&DSI_REG->DSI_RGB_NB));		
    OUTREG32(&regs->DSI_HSA_WC, AS_UINT32(&DSI_REG->DSI_HSA_WC));		
    OUTREG32(&regs->DSI_HBP_WC, AS_UINT32(&DSI_REG->DSI_HBP_WC));		
    OUTREG32(&regs->DSI_HFP_WC, AS_UINT32(&DSI_REG->DSI_HFP_WC));		
	
    OUTREG32(&regs->DSI_MEM_CONTI, AS_UINT32(&DSI_REG->DSI_MEM_CONTI));

    OUTREG32(&regs->DSI_PHY_TIMECON0, AS_UINT32(&DSI_REG->DSI_PHY_TIMECON0));
    OUTREG32(&regs->DSI_PHY_TIMECON1, AS_UINT32(&DSI_REG->DSI_PHY_TIMECON1));
    OUTREG32(&regs->DSI_PHY_TIMECON2, AS_UINT32(&DSI_REG->DSI_PHY_TIMECON2));
    OUTREG32(&regs->DSI_PHY_TIMECON3, AS_UINT32(&DSI_REG->DSI_PHY_TIMECON3));
	
}

static void _RestoreDSIRegisters(void)
{
    DSI_REGS *regs = &(_dsiContext.regBackup);
    UINT32 i;

    OUTREG32(&DSI_REG->DSI_INTEN, AS_UINT32(&regs->DSI_INTEN));	
    OUTREG32(&DSI_REG->DSI_MODE_CTRL, AS_UINT32(&regs->DSI_MODE_CTRL));	
    OUTREG32(&DSI_REG->DSI_TXRX_CTRL, AS_UINT32(&regs->DSI_TXRX_CTRL));	
    OUTREG32(&DSI_REG->DSI_PSCTRL, AS_UINT32(&regs->DSI_PSCTRL));	

    OUTREG32(&DSI_REG->DSI_VSA_NL, AS_UINT32(&regs->DSI_VSA_NL));		
    OUTREG32(&DSI_REG->DSI_VBP_NL, AS_UINT32(&regs->DSI_VBP_NL));		
    OUTREG32(&DSI_REG->DSI_VFP_NL, AS_UINT32(&regs->DSI_VFP_NL));		
    OUTREG32(&DSI_REG->DSI_VACT_NL, AS_UINT32(&regs->DSI_VACT_NL));		

    OUTREG32(&DSI_REG->DSI_LINE_NB, AS_UINT32(&regs->DSI_LINE_NB));		
    OUTREG32(&DSI_REG->DSI_HSA_NB, AS_UINT32(&regs->DSI_HSA_NB));		
    OUTREG32(&DSI_REG->DSI_HBP_NB, AS_UINT32(&regs->DSI_HBP_NB));		
    OUTREG32(&DSI_REG->DSI_HFP_NB, AS_UINT32(&regs->DSI_HFP_NB));		

    OUTREG32(&DSI_REG->DSI_RGB_NB, AS_UINT32(&regs->DSI_RGB_NB));		
    OUTREG32(&DSI_REG->DSI_HSA_WC, AS_UINT32(&regs->DSI_HSA_WC));		
    OUTREG32(&DSI_REG->DSI_HBP_WC, AS_UINT32(&regs->DSI_HBP_WC));		
    OUTREG32(&DSI_REG->DSI_HFP_WC, AS_UINT32(&regs->DSI_HFP_WC));		

    OUTREG32(&DSI_REG->DSI_MEM_CONTI, AS_UINT32(&regs->DSI_MEM_CONTI));		

    OUTREG32(&DSI_REG->DSI_PHY_TIMECON0, AS_UINT32(&regs->DSI_PHY_TIMECON0));		
    OUTREG32(&DSI_REG->DSI_PHY_TIMECON1, AS_UINT32(&regs->DSI_PHY_TIMECON1));
    OUTREG32(&DSI_REG->DSI_PHY_TIMECON2, AS_UINT32(&regs->DSI_PHY_TIMECON2));		
    OUTREG32(&DSI_REG->DSI_PHY_TIMECON3, AS_UINT32(&regs->DSI_PHY_TIMECON3));		

}

static void _ResetBackupedDSIRegisterValues(void)
{
    DSI_REGS *regs = &_dsiContext.regBackup;
    memset((void*)regs, 0, sizeof(DSI_REGS));
}

DSI_STATUS DSI_Init(void)
{
    DSI_STATUS ret = DSI_STATUS_OK;

    memset(&_dsiContext, 0, sizeof(_dsiContext));

    // LCD controller would NOT reset register as default values
    // Do it by SW here
    //
    _ResetBackupedDSIRegisterValues();

    ret = DSI_PowerOn();

	OUTREG32(&DSI_REG->DSI_MEM_CONTI, (DSI_WMEM_CONTI<<16)|(DSI_RMEM_CONTI));	

    ASSERT(ret == DSI_STATUS_OK);

	//DSI_REG->DSI_INTEN.CMD_DONE=1;

#if ENABLE_DSI_INTERRUPT
    if (request_irq(MT6575_DSI_IRQ_ID,
        _DSI_InterruptHandler, IRQF_TRIGGER_LOW, MTKFB_DRIVER, NULL) < 0)
    {
        printf("DSI", "fail to request DSI irq\n"); 
        return DSI_STATUS_ERROR;
    }
	mt65xx_irq_unmask(MT6575_DSI_IRQ_ID);
    init_waitqueue_head(&_dsi_wait_queue);
    DSI_REG->DSI_INTEN.CMD_DONE=1;
    DSI_REG->DSI_INTEN.RD_RDY=1;
#endif
    
    return DSI_STATUS_OK;
}


DSI_STATUS DSI_Deinit(void)
{
    DSI_STATUS ret = DSI_PowerOff();

    ASSERT(ret == DSI_STATUS_OK);

    return DSI_STATUS_OK;
}


DSI_STATUS DSI_PowerOn(void)
{

    if (!s_isDsiPowerOn)
    {
#if 0   // FIXME
        BOOL ret = hwEnableClock(MT65XX_PDN_MM_DSI, "DSI");
        ASSERT(ret);
#else
		MASKREG32(0xC2080028, 0x00000020, 0x00000020);
		printf("[DISP] - uboot - DSI_PowerOn. 0x%8x,0x%8x,0x%8x\n", INREG32(0xC2080000), INREG32(0xC2080004), INREG32(0xC2080008));

#endif        
        _RestoreDSIRegisters();
		//_WaitForEngineNotBusy();		
        s_isDsiPowerOn = TRUE;
    }

    return DSI_STATUS_OK;
}


DSI_STATUS DSI_PowerOff(void)
{

    if (s_isDsiPowerOn)
    {
        BOOL ret = TRUE;
        //_WaitForEngineNotBusy();
        _BackupDSIRegisters();
#if 0   // FIXME
        ret = hwDisableClock(MT65XX_PDN_MM_DSI, "DSI");
        ASSERT(ret);
#else
        MASKREG32(0xC2080018, 0x00000020, 0x00000020);
		printf("[DISP] - uboot - DSI_PowerOff. 0x%8x,0x%8x,0x%8x\n", INREG32(0xC2080000), INREG32(0xC2080004), INREG32(0xC2080008));
#endif        
        s_isDsiPowerOn = FALSE;
    }
    
    return DSI_STATUS_OK;
}


DSI_STATUS DSI_WaitForNotBusy(void)
{
    _WaitForEngineNotBusy();

    return DSI_STATUS_OK;
}


DSI_STATUS DSI_EnableClk(void)
{
	_WaitForEngineNotBusy();

	DSI_REG->DSI_START.DSI_START=0;
	DSI_REG->DSI_START.DSI_START=1;

    return DSI_STATUS_OK;
}


DSI_STATUS DSI_DisableClk(void)
{
	DSI_REG->DSI_START.DSI_START=0;

    return DSI_STATUS_OK;
}


DSI_STATUS DSI_Reset(void)
{
	DSI_REG->DSI_COM_CTRL.DSI_RESET = 1;
	lcm_mdelay(5);
	DSI_REG->DSI_COM_CTRL.DSI_RESET = 0;

    return DSI_STATUS_OK;
}


DSI_STATUS DSI_SetMode(unsigned int mode)
{

	DSI_REG->DSI_MODE_CTRL.MODE = mode;

	return DSI_STATUS_OK;
}


void init_mipi_pll(void)
{
/*
    DSI_PHY_REG_ANACON0 con0 = DSI_PHY_REG->ANACON0;
    DSI_PHY_REG_ANACON1 con1 = DSI_PHY_REG->ANACON1;
    //DSI_PHY_REG_ANACON2 con2 = DSI_PHY_REG->ANACON2;


    con1.RG_PLL_DIV1 = lcm_params.dpi.mipi_pll_clk_div1;
    con1.RG_PLL_DIV2 = lcm_params.dpi.mipi_pll_clk_div2;

		con0.PLL_CLKR_EN = 1;
		con0.PLL_EN = 1;
		con0.RG_DPI_EN = 1;

    // Set to DSI_PHY_REG   
    OUTREG32(&DSI_PHY_REG->ANACON0, AS_UINT32(&con0));
    OUTREG32(&DSI_PHY_REG->ANACON1, AS_UINT32(&con1));
    //OUTREG32(&DSI_PHY_REG->ANACON2, AS_UINT32(&con2));
*/
}

void DSI_PHY_clk_setting(unsigned int div1, unsigned int div2, unsigned int lane_no)
{

	unsigned int tx_ldo_en=0x1;
	unsigned int tx_ldo_vocal=0x4;
	unsigned int rg_pll_pr_div=0;
	unsigned int rg_pll_fb_div2=0;
	unsigned int po_div2_en=0x00;

	MIPITX_CFG0_REG mipitx_con0=DSI_PHY_REG->MIPITX_CON0;
	MIPITX_CFG1_REG mipitx_con1=DSI_PHY_REG->MIPITX_CON1;
	MIPITX_CFG3_REG mipitx_con3=DSI_PHY_REG->MIPITX_CON3;
	MIPITX_CFG8_REG mipitx_con8=DSI_PHY_REG->MIPITX_CON8;
	MIPITX_CFG9_REG mipitx_con9=DSI_PHY_REG->MIPITX_CON9;
	
	if (div1>0x1e) //vco high speed mode
	{
		rg_pll_pr_div=1;
		rg_pll_fb_div2=1;
	}
	else
	{
		rg_pll_pr_div=0;
		rg_pll_fb_div2=0;
	}
	
	//mipi TX/PLL enable
	mipitx_con0.PLL_EN=1;
	mipitx_con0.PLL_CLKR_EN=1;
	mipitx_con0.RG_LNT_LPTX_BIAS_EN=1;
	if(get_chip_eco_ver()==CHIP_E1)
		mipitx_con0.RG_LNT_HSTX_EDGE_SEL=1;
	else
		mipitx_con0.RG_LNT_HSTX_EDGE_SEL=0;
	mipitx_con0.RG_DSI_PHY_CK_PSEL=0;
	mipitx_con0.RG_DPI_EN=0;
	mipitx_con0.RG_LNT_HSTX_BIAS_EN=1;
	mipitx_con0.RG_LPTX_SWBYPASS=0;
	mipitx_con0.RG_PRBS_EN=0;
	mipitx_con0.RG_DPI_CKSEL=3;
	mipitx_con0.RG_CLK_SEL=0;
	
	// mipitx pll setting
	mipitx_con1.RG_PLL_DIV1=(div1&0x3F);
	mipitx_con1.RG_PLL_DIV2=(div2&0xF);
	mipitx_con1.RG_DSI_CK_SEL=(lane_no-1);
	mipitx_con1.RG_DSI_CK_DSEL=0;
	
	//mipitx pll pdiv2/prdiv
	mipitx_con3.PG_PLL_PRDIV=rg_pll_pr_div;
	mipitx_con3.RG_PLL_VCOCAL_CKCTL=0;
	mipitx_con3.RG_PLL_FBSEL=rg_pll_fb_div2;
	mipitx_con3.RG_PLL_CDIV=0;
	mipitx_con3.RG_PLL_ACCEN=0;
	//mipitx_con3.RG_PLL_AUTOK_LOAD=0;
	//mipitx_con3.RG_PLL_LOAD_RSTB=0;
	//mipitx_con3.RG_PLL_AUTOK_EN=0;
	mipitx_con3.RG_PLL_PODIV2=po_div2_en;

	mipitx_con8.RG_LNTC_HS_CZ=8;
	mipitx_con8.RG_LNT0_HS_CZ=8;
	mipitx_con8.RG_LNT1_HS_CZ=8;
	mipitx_con8.RG_LNT_CIRE=2;

	//unsigned int mipitx_con9=(tx_ldo_vocal<<0x0b)|(tx_ldo_en<<0x03);
	mipitx_con9.RG_TXLDO_FBCAL=0;
	mipitx_con9.RG_TXLDO_EN=tx_ldo_en;
	mipitx_con9.RG_TXLDO_IBCAL=0;
	mipitx_con9.RG_TXLDO_OCCAL=0;
	mipitx_con9.RG_TXLDO_PDDOS_EN=0;
	mipitx_con9.RG_TXLDO_VOCAL=tx_ldo_vocal;
		
	DSI_PHY_REG->MIPITX_CON0=mipitx_con0;
	DSI_PHY_REG->MIPITX_CON1=mipitx_con1;
	DSI_PHY_REG->MIPITX_CON3=mipitx_con3;	
	DSI_PHY_REG->MIPITX_CON8=mipitx_con8;
	DSI_PHY_REG->MIPITX_CON9=mipitx_con9;

	// wait pll stable
	mdelay(1);
}


void DSI_PHY_clk_switch(bool on)
{
	MIPITX_CFG0_REG mipitx_con0=DSI_PHY_REG->MIPITX_CON0;

	if(on)
		mipitx_con0.PLL_EN=1;
	else
		mipitx_con0.PLL_EN=0;

	DSI_PHY_REG->MIPITX_CON0=mipitx_con0;	
}


void DSI_PHY_TIMCONFIG(LCM_PARAMS *lcm_params)
{
	DSI_PHY_TIMCON0_REG timcon0;
	DSI_PHY_TIMCON1_REG timcon1;	
	DSI_PHY_TIMCON2_REG timcon2;
	DSI_PHY_TIMCON3_REG timcon3;
	unsigned int div1 = lcm_params->dsi.pll_div1;
	unsigned int div2 = lcm_params->dsi.pll_div2;
	unsigned int lane_no = lcm_params->dsi.LANE_NUM;
	
	unsigned int div2_real=div2 ? div2*0x02 : 0x1;
	unsigned int cycle_time = (8 * 1000 * div2_real)/ (26 * (div1+0x01));
	unsigned int ui = (1000 * div2_real)/ (26 * (div1+0x01)) + 1;
	unsigned int hs_trail_m, hs_trail_n;

	printk("[DISP] - uboot - DSI_PHY_TIMCONFIG, Cycle Time = %d(ns), Unit Interval = %d(ns). div1 = %d, div2 = %d, lane# = %d \n", cycle_time, ui, div1, div2, lane_no); 		

	#define NS_TO_CYCLE(n, c)	((n) / c + (( (n) % c) ? 1 : 0))

	hs_trail_m=lane_no;
	hs_trail_n= (lcm_params->dsi.HS_TRAIL == 0) ? NS_TO_CYCLE(((lane_no * 4 * ui) + 60), cycle_time) : lcm_params->dsi.HS_TRAIL;

	// +3 is recommended from designer becauase of HW latency
	timcon0.HS_TRAIL	= ((hs_trail_m > hs_trail_n) ? hs_trail_m : hs_trail_n) + 3;
	timcon0.HS_ZERO 	= (lcm_params->dsi.HS_ZERO == 0) ? NS_TO_CYCLE((105 + 6 * ui), cycle_time) : lcm_params->dsi.HS_ZERO;
	timcon0.HS_PRPR 	= (lcm_params->dsi.HS_PRPR == 0) ? NS_TO_CYCLE((40 + 4 * ui), cycle_time) : lcm_params->dsi.HS_PRPR;
	// HS_PRPR can't be 1.
	if (timcon0.HS_PRPR < 2)
		timcon0.HS_PRPR = 2;
	timcon0.LPX 		= (lcm_params->dsi.LPX == 0) ? NS_TO_CYCLE((50+75)/2, cycle_time) : lcm_params->dsi.LPX;

	timcon1.TA_SACK 	= (lcm_params->dsi.TA_SACK == 0) ? 1 : lcm_params->dsi.TA_SACK;
	timcon1.TA_GET 		= (lcm_params->dsi.TA_GET == 0) ? (5 * timcon0.LPX) : lcm_params->dsi.TA_GET;
	timcon1.TA_SURE 	= (lcm_params->dsi.TA_SURE == 0) ? (3 * timcon0.LPX / 2) : lcm_params->dsi.TA_SURE;
	timcon1.TA_GO 		= (lcm_params->dsi.TA_GO == 0) ? (4 * timcon0.LPX) : lcm_params->dsi.TA_GO;


	timcon2.CLK_TRAIL 	= (lcm_params->dsi.CLK_TRAIL == 0) ? NS_TO_CYCLE(60, cycle_time) : lcm_params->dsi.CLK_TRAIL;
	// CLK_TRAIL can't be 1.
	if (timcon2.CLK_TRAIL < 2)
		timcon2.CLK_TRAIL = 2;
	timcon2.CLK_ZERO 	= (lcm_params->dsi.CLK_ZERO == 0) ? NS_TO_CYCLE((300 - 38), cycle_time) : lcm_params->dsi.CLK_ZERO;
	timcon2.LPX_WAIT 	= (lcm_params->dsi.LPX_WAIT == 0) ? 1 : lcm_params->dsi.LPX_WAIT;
	timcon2.CONT_DET 	= lcm_params->dsi.CONT_DET;

	timcon3.CLK_HS_PRPR	= (lcm_params->dsi.CLK_HS_PRPR == 0) ? NS_TO_CYCLE((38 + 95) / 2, cycle_time) : lcm_params->dsi.CLK_HS_PRPR;


	printk("[DISP] - uboot - DSI_PHY_TIMCONFIG, HS_TRAIL = %d, HS_ZERO = %d, HS_PRPR = %d, LPX = %d, TA_SACK = %d, TA_GET = %d, TA_SURE = %d, TA_GO = %d, CLK_TRAIL = %d, CLK_ZERO = %d, CLK_HS_PRPR = %d \n", \
			timcon0.HS_TRAIL, timcon0.HS_ZERO, timcon0.HS_PRPR, timcon0.LPX, timcon1.TA_SACK, timcon1.TA_GET, timcon1.TA_SURE, timcon1.TA_GO, timcon2.CLK_TRAIL, timcon2.CLK_ZERO, timcon3.CLK_HS_PRPR);		

	DSI_REG->DSI_PHY_TIMECON0=timcon0;
	DSI_REG->DSI_PHY_TIMECON1=timcon1;
	DSI_REG->DSI_PHY_TIMECON2=timcon2;
	DSI_REG->DSI_PHY_TIMECON3=timcon3;
	
}



void DSI_clk_ULP_mode(bool enter)
{
	DSI_PHY_LCCON_REG tmp_reg1;
	//DSI_PHY_REG_ANACON0	tmp_reg2;

	tmp_reg1=DSI_REG->DSI_PHY_LCCON;
	//tmp_reg2=DSI_PHY_REG->ANACON0;

	if(enter) {

		tmp_reg1.LC_HS_TX_EN=0;
		OUTREG32(&DSI_REG->DSI_PHY_LCCON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
		tmp_reg1.LC_ULPM_EN=1;
		OUTREG32(&DSI_REG->DSI_PHY_LCCON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
		//tmp_reg2.PLL_EN=0;
		//OUTREG32(&DSI_PHY_REG->ANACON0, AS_UINT32(&tmp_reg2));

	}
	else {

		//tmp_reg2.PLL_EN=1;
		//OUTREG32(&DSI_PHY_REG->ANACON0, AS_UINT32(&tmp_reg2));
		lcm_mdelay(1);
		tmp_reg1.LC_ULPM_EN=0;
		OUTREG32(&DSI_REG->DSI_PHY_LCCON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
		tmp_reg1.LC_WAKEUP_EN=1;
		OUTREG32(&DSI_REG->DSI_PHY_LCCON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
		tmp_reg1.LC_WAKEUP_EN=0;
		OUTREG32(&DSI_REG->DSI_PHY_LCCON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);

	}
}


void DSI_clk_HS_mode(bool enter)
{
	DSI_PHY_LCCON_REG tmp_reg1 = DSI_REG->DSI_PHY_LCCON;

	if(enter && !DSI_clk_HS_state()) {
		tmp_reg1.LC_HS_TX_EN=1;
		OUTREG32(&DSI_REG->DSI_PHY_LCCON, AS_UINT32(&tmp_reg1));
		//lcm_mdelay(1);
	}
	else if (!enter && DSI_clk_HS_state()) {
		tmp_reg1.LC_HS_TX_EN=0;
		OUTREG32(&DSI_REG->DSI_PHY_LCCON, AS_UINT32(&tmp_reg1));
		//lcm_mdelay(1);

	}
}	


bool DSI_clk_HS_state(void)
{
	return DSI_REG->DSI_PHY_LCCON.LC_HS_TX_EN ? TRUE : FALSE;
}


void DSI_lane0_ULP_mode(bool enter)
{
	DSI_PHY_LD0CON_REG tmp_reg1;

	tmp_reg1=DSI_REG->DSI_PHY_LD0CON;

	if(enter) {
		// suspend
		tmp_reg1.L0_HS_TX_EN=0;
		OUTREG32(&DSI_REG->DSI_PHY_LD0CON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
		tmp_reg1.L0_ULPM_EN=1;
		OUTREG32(&DSI_REG->DSI_PHY_LD0CON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
	}
	else {
		// resume
		tmp_reg1.L0_ULPM_EN=0;
		OUTREG32(&DSI_REG->DSI_PHY_LD0CON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
		tmp_reg1.L0_WAKEUP_EN=1;
		OUTREG32(&DSI_REG->DSI_PHY_LD0CON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
		tmp_reg1.L0_WAKEUP_EN=0;
		OUTREG32(&DSI_REG->DSI_PHY_LD0CON, AS_UINT32(&tmp_reg1));
		lcm_mdelay(1);
	}
}


void DSI_set_cmdq_V2(unsigned cmd, unsigned char count, unsigned char *para_list, unsigned char force_update)
{
	UINT32 i, layer, layer_state, lane_num;
	UINT32 goto_addr, mask_para, set_para;
	UINT32 fbPhysAddr, fbVirAddr;
	DSI_T0_INS t0;	
	DSI_T1_INS t1;	
	DSI_T2_INS t2;	

	_WaitForEngineNotBusy();

	if (count > 59)
	{
		UINT32 pixel = count/3 + ((count%3) ? 1 : 0);
		
		LCD_REG_LAYER 	fb_layer_info;
		LCD_REG_DSI_DC	dsi_info;

		// backup layer state.
		layer_state = AS_UINT32(&LCD_REG->WROI_CONTROL) & 0xFC000000;

		// backup FB layer info.
		memcpy(&fb_layer_info, &LCD_REG->LAYER[FB_LAYER], sizeof(LCD_REG_LAYER));

		// backup LCD-DSI I/F configuration.
		dsi_info = LCD_REG->DS_DSI_CON;

		// backup lane number.
		lane_num = DSI_REG->DSI_TXRX_CTRL.LANE_NUM;

		// HW limitation.
		// LP type-1 command can't go with 2 lanes. So we must switch to lane mode.
		DSI_REG->DSI_TXRX_CTRL.LANE_NUM = 1;
		DSI_PHY_REG->MIPITX_CON1.RG_DSI_CK_SEL = 0;

		// Modify LCD-DSI configuration
		LCD_REG->DS_DSI_CON.DC_DSI = TRUE;
		// Let LSB of RGB(BGR in buffer) first.
		LCD_REG->DS_DSI_CON.RGB_SWAP = LCD_DSI_IF_FMT_COLOR_ORDER_BGR;
		// Let parameters be in unit of byte.
		LCD_REG->DS_DSI_CON.CLR_FMT = LCD_DSI_IF_FORMAT_RGB888;
		// HW limitation
		// It makes package numbers > 1.
		LCD_REG->DS_DSI_CON.PACKET_SIZE = 30;

		// Start of Enable only one layer (FB layer) to push data to DSI
		LCD_CHECK_RET(LCD_LayerEnable(LCD_LAYER_ALL, FALSE));
		LCD_CHECK_RET(LCD_LayerEnable(FB_LAYER, TRUE));
		LCD_CHECK_RET(LCD_SetRoiWindow(0, 0, pixel, 1));
		LCD_CHECK_RET(LCD_SetBackgroundColor(0));

		LCD_REG->LAYER[FB_LAYER].ADDRESS = para_list;
		
		LCD_CHECK_RET(LCD_LayerSetFormat(FB_LAYER, LCD_LAYER_FORMAT_RGB888));
		LCD_CHECK_RET(LCD_LayerSetPitch(FB_LAYER, pixel*3));
		LCD_CHECK_RET(LCD_LayerSetOffset(FB_LAYER, 0, 0));
		LCD_CHECK_RET(LCD_LayerSetSize(FB_LAYER, pixel, 1));
		// End of Enable only one layer (FB layer) to push data to DSI

		t1.CONFG = 1;
		t1.Data_ID = DSI_DCS_LONG_PACKET_ID;
		t1.mem_start0 = (cmd&0xFF);
		t1.mem_start1 = (cmd>>8);

		OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(&t1));
		OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);	

		printk("[DISP] - uboot - DSI_set_cmdq_V2. command(0x%x) parameter count = %d > 59, pixel = %d \n", cmd, count, pixel);
		printk("[DISP] - uboot - command queue only support 16 x 4 bytes. Header used 4 byte. DCS used 1 byte. If parameter > 59 byte, work around by Type-1 command. \n");
		printk("para_list[%d] = {", count);
	    for (i = 0; i < count; i++)
	        printk("0x%02x, ", para_list[i]);
		printk("} \n");

		if(force_update)
		{
			LCD_CHECK_RET(LCD_StartTransfer(FALSE));		
			DSI_EnableClk();
		}

		_WaitForEngineNotBusy();

		// restore FB layer info.
		memcpy(&LCD_REG->LAYER[FB_LAYER], &fb_layer_info, sizeof(LCD_REG_LAYER));

		// restore LCD-DSI I/F configuration.
		LCD_REG->DS_DSI_CON = dsi_info;

		// restore lane number.
		DSI_REG->DSI_TXRX_CTRL.LANE_NUM = lane_num;
		DSI_PHY_REG->MIPITX_CON1.RG_DSI_CK_SEL = (lane_num - 1);
		
		// restore layer state.
		for(layer=LCD_LAYER_0; layer<LCD_LAYER_NUM; layer++)
		{
			if(layer_state&(0x80000000>>layer))
				LCD_CHECK_RET(LCD_LayerEnable(layer, TRUE));
			else
				LCD_CHECK_RET(LCD_LayerEnable(layer, FALSE));
		}
		
	}
	else
	{
		if (cmd < 0xB0)
		{
			if (count > 1)
			{
				t2.CONFG = 2;
				t2.Data_ID = DSI_DCS_LONG_PACKET_ID;
				t2.WC16 = count+1;

				OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(&t2));

				goto_addr = (void*)(&DSI_CMDQ_REG->data0[1].byte0);
				mask_para = (0xFF<<((goto_addr&0x3)*8));
				set_para = (cmd<<((goto_addr&0x3)*8));
				MASKREG32(goto_addr&(~0x3), mask_para, set_para);
				
				for(i=0; i<count; i++)
				{
					goto_addr = (void*)(&DSI_CMDQ_REG->data0[1].byte1) + i;
					mask_para = (0xFF<<((goto_addr&0x3)*8));
					set_para = (para_list[i]<<((goto_addr&0x3)*8));
					MASKREG32(goto_addr&(~0x3), mask_para, set_para);			
				}

				OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 2+(count)/4);			
			}
			else
			{
				t0.CONFG = 0;
				t0.Data0 = cmd;
				if (count)
				{
					t0.Data_ID = DSI_DCS_SHORT_PACKET_ID_1;
					t0.Data1 = para_list[0];
				}
				else
				{
					t0.Data_ID = DSI_DCS_SHORT_PACKET_ID_0;
					t0.Data1 = 0;
				}
				OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(&t0));
				OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);
			}
		}
		else
		{
			if (count > 1)
			{
				t2.CONFG = 2;
				t2.Data_ID = DSI_GERNERIC_LONG_PACKET_ID;
				t2.WC16 = count+1;

				OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(&t2));

				goto_addr = (void*)(&DSI_CMDQ_REG->data0[1].byte0);
				mask_para = (0xFF<<((goto_addr&0x3)*8));
				set_para = (cmd<<((goto_addr&0x3)*8));
				MASKREG32(goto_addr&(~0x3), mask_para, set_para);
				
				for(i=0; i<count; i++)
				{
					goto_addr = (void*)(&DSI_CMDQ_REG->data0[1].byte1) + i;
					mask_para = (0xFF<<((goto_addr&0x3)*8));
					set_para = (para_list[i]<<((goto_addr&0x3)*8));
					MASKREG32(goto_addr&(~0x3), mask_para, set_para);			
				}

				OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 2+(count)/4);

			}
			else
			{
				t0.CONFG = 0;
				t0.Data0 = cmd;
				if (count)
				{
					t0.Data_ID = DSI_GERNERIC_SHORT_PACKET_ID_2;
					t0.Data1 = para_list[0];
				}
				else
				{
					t0.Data_ID = DSI_GERNERIC_SHORT_PACKET_ID_1;
					t0.Data1 = 0;
				}
				OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(&t0));
				OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);
			}
		}

	    for (i = 0; i < AS_UINT32(&DSI_REG->DSI_CMDQ_SIZE); i++)
	        printk("[DISP] - uboot - DSI_set_cmdq_V2. DSI_CMDQ+%04x : 0x%08x\n", i*4, INREG32(DSI_BASE + 0x180 + i*4));

		if(force_update)
			DSI_EnableClk();
	}

}


void DSI_set_cmdq(unsigned int *pdata, unsigned int queue_size, bool force_update)
{
	UINT32 i;

	ASSERT(queue_size<=16);
	
	_WaitForEngineNotBusy();
	
	for(i=0; i<queue_size; i++)
		OUTREG32(&DSI_CMDQ_REG->data0[i], AS_UINT32((pdata+i)));

	OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, queue_size);

    for (i = 0; i < queue_size; i++)
        printk("[DISP] - uboot - DSI_set_cmdq. DSI_CMDQ+%04x : 0x%08x\n", i*4, INREG32(DSI_BASE + 0x180 + i*4));

	if(force_update)
		DSI_EnableClk();
	
}


DSI_STATUS DSI_Write_T0_INS(DSI_T0_INS *t0)
{
    OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(t0));	

	OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);
	OUTREG32(&DSI_REG->DSI_START, 0);
	OUTREG32(&DSI_REG->DSI_START, 1);
}


DSI_STATUS DSI_Write_T1_INS(DSI_T1_INS *t1)
{
    OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(t1));	

	OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);
	OUTREG32(&DSI_REG->DSI_START, 0);
	OUTREG32(&DSI_REG->DSI_START, 1);
}


DSI_STATUS DSI_Write_T2_INS(DSI_T2_INS *t2)
{
	unsigned int i;
	
	OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(t2));

	for(i=0;i<((t2->WC16-1)>>2)+1;i++)
	    OUTREG32(&DSI_CMDQ_REG->data0[1+i], AS_UINT32((t2->pdata+i)));

	OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, (((t2->WC16-1)>>2)+2));
	OUTREG32(&DSI_REG->DSI_START, 0);
	OUTREG32(&DSI_REG->DSI_START, 1);
}


DSI_STATUS DSI_Write_T3_INS(DSI_T3_INS *t3)
{
    OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(t3));	

	OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);
	OUTREG32(&DSI_REG->DSI_START, 0);
	OUTREG32(&DSI_REG->DSI_START, 1);
}


void DSI_write_lcm_cmd(unsigned int cmd)
{
	DSI_T0_INS *t0_tmp;
	DSI_CMDQ_CONFG CONFG_tmp;

	CONFG_tmp.type=SHORT_PACKET_RW;
	CONFG_tmp.BTA=DISABLE_BTA;
	CONFG_tmp.HS=LOW_POWER;
	CONFG_tmp.CL=CL_8BITS;
	CONFG_tmp.TE=DISABLE_TE;
	CONFG_tmp.RPT=DISABLE_RPT;

	t0_tmp->CONFG = *((unsigned char *)(&CONFG_tmp));
	t0_tmp->Data_ID= (cmd&0xFF);
	t0_tmp->Data0 = 0x0;
	t0_tmp->Data1 = 0x0;	

	DSI_Write_T0_INS(t0_tmp);
}


void DSI_write_lcm_regs(unsigned int addr, unsigned int *para, unsigned int nums)
{
	DSI_T2_INS *t2_tmp;
	DSI_CMDQ_CONFG CONFG_tmp;

	CONFG_tmp.type=LONG_PACKET_W;
	CONFG_tmp.BTA=DISABLE_BTA;
	CONFG_tmp.HS=LOW_POWER;
	CONFG_tmp.CL=CL_8BITS;
	CONFG_tmp.TE=DISABLE_TE;
	CONFG_tmp.RPT=DISABLE_RPT;

	t2_tmp->CONFG = *((unsigned char *)(&CONFG_tmp));
	t2_tmp->Data_ID = (addr&0xFF);
	t2_tmp->WC16 = nums;	
	t2_tmp->pdata = para;	

	DSI_Write_T2_INS(t2_tmp);

}

UINT32 DSI_dcs_read_lcm_reg(UINT8 cmd)
{
        UINT32 max_try_count = 5;
        UINT32 recv_data;
        UINT32 recv_data_cnt;
        long ret;
        unsigned int read_timeout_ms;
        unsigned char packet_type;
        DSI_T0_INS t0;  

        do
        {
		   if(max_try_count == 0)
		      return 0;
		   max_try_count--;
           recv_data = 0;
           recv_data_cnt = 0;
           read_timeout_ms = 20;
            
           _WaitForEngineNotBusy();

           t0.CONFG = 0x04;        ///BTA
           t0.Data0 = cmd;
           t0.Data_ID = DSI_DCS_READ_PACKET_ID;
           t0.Data1 = 0;

           OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(&t0));
           OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);

           ///clear read ACK 
           DSI_REG->DSI_RACK.DSI_RACK = 1;
           DSI_REG->DSI_INTSTA.RD_RDY = 1;
           DSI_REG->DSI_INTSTA.CMD_DONE = 1;
           DSI_REG->DSI_INTEN.RD_RDY =  1;
           DSI_REG->DSI_INTEN.CMD_DONE=  1;

           OUTREG32(&DSI_REG->DSI_START, 0);
           OUTREG32(&DSI_REG->DSI_START, 1);

           /// the following code is to
           /// 1: wait read ready
           /// 2: ack read ready
           /// 3: wait for CMDQ_DONE
           /// 3: read data
#if ENABLE_DSI_INTERRUPT
           ret = wait_event_interruptible_timeout(_dsi_wait_queue, 
                                                       !_IsEngineBusy(),
                                                       WAIT_TIMEOUT);
            if (0 == ret) {
                printf(" Wait for DSI engine read ready timeout!!!\n");
                return 0;
            }
#else
            printf(" Start polling DSI read ready!!!\n");
            while(DSI_REG->DSI_INTSTA.RD_RDY == 0)  ///read clear
            {
                ///keep polling
                udelay(1000);
                read_timeout_ms --;
                
                if(read_timeout_ms == 0)
                {
                    printf(" Polling DSI read ready timeout!!!\n");
                    DSI_DumpRegisters();

                    ///do necessary reset here
                    DSI_REG->DSI_RACK.DSI_RACK = 1;
                    DSI_Reset();
                    return 0;
                }
            }
    
            printf(" End polling DSI read ready!!!\n");

           // wait for DSI not busy
           while(DSI_REG->DSI_STA.BUSY)
           {
               ///send read ACK
               printf("DSI is busy: 0x%x !!!\n", DSI_REG->DSI_STA.BUSY);
               DSI_REG->DSI_RACK.DSI_RACK = 1;
           }

           ///clear interrupt status
           DSI_REG->DSI_INTSTA.RD_RDY = 1;
           ///STOP DSI
           OUTREG32(&DSI_REG->DSI_START, 0);

#endif
       
           DSI_REG->DSI_INTEN.RD_RDY =  0;

           printf(" DSI_RX_STA : 0x%x \n", DSI_REG->DSI_RX_STA);
           printf(" DSI_CMDQ_SIZE : 0x%x \n", DSI_REG->DSI_CMDQ_SIZE.CMDQ_SIZE);
           printf(" DSI_CMDQ_DATA0 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte0);
           printf(" DSI_CMDQ_DATA1 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte1);
           printf(" DSI_CMDQ_DATA2 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte2);
           printf("DSI", " DSI_CMDQ_DATA3 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte3);
           printf("DSI", " DSI_RX_DATA.BYTE0 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE0);
           printf("DSI", " DSI_RX_DATA.BYTE1 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE1);
           printf("DSI", " DSI_RX_DATA.BYTE2 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE2);
           printf("DSI", " DSI_RX_DATA.BYTE3 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE3);
           printf("DSI", " DSI_RX_DATA.BYTE4 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE4);
           printf("DSI", " DSI_RX_DATA.BYTE5 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE5);
           printf("DSI", " DSI_RX_DATA.BYTE6 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE6);
           printf("DSI", " DSI_RX_DATA.BYTE7 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE7);

           packet_type = DSI_REG->DSI_RX_DATA.BYTE0;
           printf(" DSI read packet_type is 0x%x \n",packet_type);
           if(DSI_REG->DSI_RX_STA.LONG == 1)
           {
               recv_data_cnt = DSI_REG->DSI_RX_DATA.BYTE1 + DSI_REG->DSI_RX_DATA.BYTE2 * 16;
               if(recv_data_cnt > 4)
               {
                  printf(" DSI read long packet data  exceeds 4 bytes \n");
                  recv_data_cnt = 4;
               }
               memcpy(&recv_data, &DSI_REG->DSI_RX_DATA.BYTE4, recv_data_cnt);
           }
           else
           {
               memcpy(&recv_data, &DSI_REG->DSI_RX_DATA.BYTE1, 2);
           }
           
           printf(" DSI read 0x%x data is 0x%x \n",cmd,  recv_data);
       }while(packet_type != 0x1C && packet_type != 0x21 && packet_type != 0x22 && packet_type != 0x1A);
       /// here: we may receive a ACK packet which packet type is 0x02 (incdicates some error happened)
       /// therefore we try re-read again until no ACK packet
       /// But: if it is a good way to keep re-trying ???
       
       return recv_data;
}

/// return value: the data length we got
UINT32 DSI_dcs_read_lcm_reg_v2(UINT8 cmd, UINT8 *buffer, UINT8 buffer_size)
{
	UINT32 max_try_count = 5;
    UINT32 recv_data_cnt;
    unsigned int read_timeout_ms;
    unsigned char packet_type;
    DSI_T0_INS t0;  

	if (DSI_REG->DSI_MODE_CTRL.MODE)
		return 0;

    if (buffer == NULL || buffer_size == 0)
        return 0;

    do
    {
	   if(max_try_count == 0)
	      return 0;
	   max_try_count--;
       recv_data_cnt = 0;
       read_timeout_ms = 20;
        
       _WaitForEngineNotBusy();

       t0.CONFG = 0x04;        ///BTA
       t0.Data0 = cmd;
	   if (buffer_size < 0x3)
           t0.Data_ID = DSI_DCS_READ_PACKET_ID;
	   else
		   t0.Data_ID = DSI_GERNERIC_READ_LONG_PACKET_ID;
       t0.Data1 = 0;

       OUTREG32(&DSI_CMDQ_REG->data0[0], AS_UINT32(&t0));
       OUTREG32(&DSI_REG->DSI_CMDQ_SIZE, 1);

       ///clear read ACK 
       DSI_REG->DSI_RACK.DSI_RACK = 1;
       DSI_REG->DSI_INTSTA.RD_RDY = 1;
       DSI_REG->DSI_INTSTA.CMD_DONE = 1;
       DSI_REG->DSI_INTEN.RD_RDY =  1;
       DSI_REG->DSI_INTEN.CMD_DONE=  1;

       OUTREG32(&DSI_REG->DSI_START, 0);
       OUTREG32(&DSI_REG->DSI_START, 1);

       /// the following code is to
       /// 1: wait read ready
       /// 2: ack read ready
       /// 3: wait for CMDQ_DONE
       /// 3: read data
#if ENABLE_DSI_INTERRUPT
       static const long WAIT_TIMEOUT = 2 * HZ;    // 2 sec

           long ret = wait_event_interruptible_timeout(_dsi_dcs_read_wait_queue, 
                                                       !_IsEngineBusy(),
                                                       WAIT_TIMEOUT);
        if (0 == ret) {
            printf(" Wait for DSI engine read ready timeout!!!\n");

				DSI_DumpRegisters();
				
				///do necessary reset here
				DSI_REG->DSI_RACK.DSI_RACK = 1;
				DSI_Reset();

                return 0;
            }
#else
        printf(" Start polling DSI read ready!!!\n");
        while(DSI_REG->DSI_INTSTA.RD_RDY == 0)  ///read clear
        {
            ///keep polling
            read_timeout_ms --;
            udelay(1000);
            if(read_timeout_ms == 0)
            {
                printf(" Polling DSI read ready timeout!!!\n");
                DSI_DumpRegisters();

                ///do necessary reset here
                DSI_REG->DSI_RACK.DSI_RACK = 1;
                DSI_Reset();
                return 0;
            }
        }
        printf(" End polling DSI read ready!!!\n");

        DSI_REG->DSI_RACK.DSI_RACK = 1;

       while(DSI_REG->DSI_STA.BUSY)
       {
           ///DSI READ ACK HW bug workaround
           printf("DSI is busy: 0x%x !!!\n", DSI_REG->DSI_STA.BUSY);
           DSI_REG->DSI_RACK.DSI_RACK = 1;
       }


       ///clear interrupt status
       DSI_REG->DSI_INTSTA.RD_RDY = 1;
       ///STOP DSI
       OUTREG32(&DSI_REG->DSI_START, 0);

#endif
   
       DSI_REG->DSI_INTEN.RD_RDY =  0;

       printf(" DSI_RX_STA : 0x%x \n", DSI_REG->DSI_RX_STA);
       printf(" DSI_CMDQ_SIZE : 0x%x \n", DSI_REG->DSI_CMDQ_SIZE.CMDQ_SIZE);
       printf(" DSI_CMDQ_DATA0 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte0);
       printf(" DSI_CMDQ_DATA1 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte1);
       printf(" DSI_CMDQ_DATA2 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte2);
       printf(" DSI_CMDQ_DATA3 : 0x%x \n", DSI_CMDQ_REG->data0[0].byte3);
       printf(" DSI_RX_DATA.BYTE0 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE0);
       printf(" DSI_RX_DATA.BYTE1 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE1);
       printf(" DSI_RX_DATA.BYTE2 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE2);
       printf(" DSI_RX_DATA.BYTE3 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE3);
       printf(" DSI_RX_DATA.BYTE4 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE4);
       printf(" DSI_RX_DATA.BYTE5 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE5);
       printf(" DSI_RX_DATA.BYTE6 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE6);
       printf(" DSI_RX_DATA.BYTE7 : 0x%x \n", DSI_REG->DSI_RX_DATA.BYTE7);

       packet_type = DSI_REG->DSI_RX_DATA.BYTE0;
           
       printf(" DSI read packet_type is 0x%x \n",packet_type);

       if(DSI_REG->DSI_RX_STA.LONG == 1)
       {
           recv_data_cnt = DSI_REG->DSI_RX_DATA.BYTE1 + DSI_REG->DSI_RX_DATA.BYTE2 * 16;
           if(recv_data_cnt > 10)
           {
              printf(" DSI read long packet data  exceeds 4 bytes \n");
              recv_data_cnt = 10;
           }

          if(recv_data_cnt > buffer_size)
          {
              printf(" DSI read long packet data  exceeds buffer size: %d\n", buffer_size);
              recv_data_cnt = buffer_size;
           }
           memcpy((void*)buffer, (void*)&DSI_REG->DSI_RX_DATA.BYTE4, recv_data_cnt);
       }
       else
       {
             if(recv_data_cnt > buffer_size)
             {
                 printf(" DSI read short packet data  exceeds buffer size: %d\n", buffer_size);
                 recv_data_cnt = buffer_size;
             }
           memcpy((void*)buffer, (void*)&DSI_REG->DSI_RX_DATA.BYTE1, 2);
       }
   }while(packet_type != 0x1C && packet_type != 0x21 && packet_type != 0x22 && packet_type != 0x1A);
   /// here: we may receive a ACK packet which packet type is 0x02 (incdicates some error happened)
   /// therefore we try re-read again until no ACK packet
   /// But: if it is a good way to keep re-trying ???
   
   return recv_data_cnt;
}


UINT32 DSI_read_lcm_reg(void)
{
    return 0;
}


DSI_STATUS DSI_write_lcm_fb(unsigned int addr, bool long_length)
{
	DSI_T1_INS *t1_tmp;
	DSI_CMDQ_CONFG CONFG_tmp;

	CONFG_tmp.type=FB_WRITE;
	CONFG_tmp.BTA=DISABLE_BTA;
	CONFG_tmp.HS=HIGH_SPEED;

	if(long_length)
		CONFG_tmp.CL=CL_16BITS;
	else
		CONFG_tmp.CL=CL_8BITS;		

	CONFG_tmp.TE=DISABLE_TE;
	CONFG_tmp.RPT=DISABLE_RPT;


	t1_tmp->CONFG = *((unsigned char *)(&CONFG_tmp));
	t1_tmp->Data_ID= 0x39;
	t1_tmp->mem_start0 = (addr&0xFF);	

	if(long_length)
		t1_tmp->mem_start1 = ((addr>>8)&0xFF);

	DSI_Write_T1_INS(t1_tmp);	
}


DSI_STATUS DSI_read_lcm_fb()
{
	// TBD
}

// -------------------- Retrieve Information --------------------

DSI_STATUS DSI_DumpRegisters(void)
{
    UINT32 i;

    printk("---------- Start dump DSI registers ----------\n");
    
    for (i = 0; i < sizeof(DSI_REGS); i += 4)
    {
        printk("DSI+%04x : 0x%08x\n", i, INREG32(DSI_BASE + i));
    }
/*
    for (i = 0; i < sizeof(DSI_CMDQ_REGS); i += 4)
    {
        printk("DSI_CMDQ+%04x : 0x%08x\n", i, INREG32(DSI_BASE + 0x180 + i));
    }
*/
    for (i = 0; i < sizeof(DSI_PHY_REGS); i += 4)
    {
        printk("DSI_PHY+%04x : 0x%08x\n", i, INREG32(DSI_PHY_BASE + i));
    }

    return DSI_STATUS_OK;
}

