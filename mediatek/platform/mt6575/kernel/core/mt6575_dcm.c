/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
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

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/xlog.h>

#include "linux/leds-mt65xx.h"

#include "mach/mt6575_boot.h"
#include "mach/mt6575_typedefs.h"
#include "mach/mt6575_reg_base.h"
#include "mach/irqs.h"
#include "mach/mt6575_irq.h"
#include "mach/mt6575_gpt.h"
#include "mach/mt6575_dcm.h"
#include "mach/mt6575_clock_manager.h"
#include "mach/mt6575_clkmgr_internal.h"
#include "mach/mt6575_sc.h"


#define TAG     "Power/dcm"

#define IDLE_LOW_POWER_TEST

#define dcm_attr(_name)                         \
static struct kobj_attribute _name##_attr = {   \
    .attr = {                                   \
        .name = __stringify(_name),             \
        .mode = 0644,                           \
    },                                          \
    .show = _name##_show,                       \
    .store = _name##_store,                     \
}

#define TEST_ARCH_IDLE 0

static struct mtk_irq_mask mask;

extern int mt6575_irq_mask_all(struct mtk_irq_mask *mask);
extern int mt6575_irq_mask_restore(struct mtk_irq_mask *mask);

extern void mtk_wdt_disable(void);
extern void mtk_cpufreq_set_max_cpu_vol(void);

#ifdef CONFIG_MTK_MMC
extern void msdc_clk_status(int * status);
#else
void msdc_clk_status(int * status) { *status = 0; }
#endif

extern struct kobject *power_kobj;

#if defined(CONFIG_MTK_FB)
extern int DISP_PanelEnable(BOOL bEnable);
extern int DISP_PowerEnable(BOOL bEnable);
#endif

void enable_low_power_settings(void);
void disable_low_power_settings(void);

/*******************************************************************************
 * FUNCTION
 *	dcm_enable
 *
 * DESCRIPTION
 *	This function will enable DCM
 *
 * PARAMETERS
 *  type: CA9_DCM, TOPAXI_DCM, EMI_DCM, PERI_DCM, MM1_DCM, MM2_DCM, ALL_DCM
 *
 * RETURNS
 *	None
 ******************************************************************************/
void dcm_enable(unsigned short type)
{
    unsigned int temp;

    switch (type)
    {
        case CA9_DCM:
            DRV_WriteReg32(TOP_CA9DCMFSEL, 0x0);
            DRV_WriteReg32(TOP_DCMCTL, 0x6);
            break;
        case TOPAXI_DCM:
            DRV_WriteReg32(INFRA_DCMDBC, 0x17F);
            DRV_WriteReg32(INFRA_DCMFSEL, 0x0);
            DRV_WriteReg32(INFRA_DCMCTL, ((0x1 << 8) | (0x1)));
            break;
        case EMI_DCM:
            temp = DRV_Reg32(0xF00041DC);
            temp |= (0x3 << 24);
            DRV_WriteReg32(0xF00041DC, temp);
            
            DRV_WriteReg32(INFRA_MISC, 0x1);
            DRV_WriteReg32(INFRA_DCMCTL, ((0x1 << 8) | (0x1)));
            break;
        case PERI_DCM:
            DRV_WriteReg32(PERI_GLOBALCON_DCMFSEL, 0x0);
            DRV_WriteReg32(PERI_GLOBALCON_DCMCTL, (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 0));
            break;
        case MM1_DCM:
            DRV_WriteReg32(SMI_LARB0_CON_SET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB1_CON_SET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB2_CON_SET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB3_CON_SET, (0x1 << 11));
            break;
         case MM2_DCM:
            DRV_WriteReg32(MMSYS2_DCMFSEL, 0x0);
            DRV_WriteReg32(MMSYS2_DCMCTL, ((0x1 << 8) | (0x1 << 0)));
            break;
         case ALL_DCM:
            // CA9_DCM
            DRV_WriteReg32(TOP_CA9DCMFSEL, 0x0);
            DRV_WriteReg32(TOP_DCMCTL, 0x6);
            
            // TOPAXI_DCM
            DRV_WriteReg32(INFRA_DCMDBC, 0x17F);
            DRV_WriteReg32(INFRA_DCMFSEL, 0x0);
            DRV_WriteReg32(INFRA_DCMCTL, ((0x1 << 8) | (0x1)));
            
            // EMI_DCM
            temp = DRV_Reg32(0xF00041DC);
            temp |= (0x3 << 24);
            DRV_WriteReg32(0xF00041DC, temp);
            
            DRV_WriteReg32(INFRA_MISC, 0x1);
            DRV_WriteReg32(INFRA_DCMCTL, ((0x1 << 8) | (0x1)));
        
            // PERI_DCM
            DRV_WriteReg32(PERI_GLOBALCON_DCMFSEL, 0x0);
            DRV_WriteReg32(PERI_GLOBALCON_DCMCTL, (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 0));
           
            // MM1_DCM 
            DRV_WriteReg32(SMI_LARB0_CON_SET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB1_CON_SET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB2_CON_SET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB3_CON_SET, (0x1 << 11));
         
            // MM2_DCM
            DRV_WriteReg32(MMSYS2_DCMFSEL, 0x0);
            DRV_WriteReg32(MMSYS2_DCMCTL, ((0x1 << 8) | (0x1 << 0)));
            break;     
        default:
            xlog_printk(ANDROID_LOG_DEBUG, TAG, "[%s]: unknown dcm type 0x%x\n", __func__, type);
            break;
    }
}

/*******************************************************************************
 * FUNCTION
 *	dcm_disable
 *
 * DESCRIPTION
 *	This function will disable DCM
 *
 * PARAMETERS
 *  type: CA9_DCM, TOPAXI_DCM, EMI_DCM, PERI_DCM, MM1_DCM, MM2_DCM, ALL_DCM
 *
 * RETURNS
 *	None
 ******************************************************************************/
void dcm_disable(unsigned short type)
{
    unsigned int temp;
    
    switch (type)
    {
        case CA9_DCM:
            DRV_WriteReg32(TOP_DCMCTL, 0x0);
            break;
        case TOPAXI_DCM:
            DRV_WriteReg32(INFRA_DCMCTL, 0x0);
            break;
        case EMI_DCM:
            temp = DRV_Reg32(0xF00041DC);
            temp &= 0xFCFFFFFF;
            DRV_WriteReg32(0xF00041DC, temp);
            
            DRV_WriteReg32(INFRA_MISC, 0x0);
            DRV_WriteReg32(INFRA_DCMCTL, 0x0);
            break;
        case PERI_DCM:
            DRV_WriteReg32(PERI_GLOBALCON_DCMCTL, 0x0);
            break;
        case MM1_DCM:
            DRV_WriteReg32(SMI_LARB0_CON_RESET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB1_CON_RESET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB2_CON_RESET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB3_CON_RESET, (0x1 << 11));
            break;
        case MM2_DCM:
            DRV_WriteReg32(MMSYS2_DCMCTL, 0x0);
            break;
        case ALL_DCM:
            // CA9_DCM
            DRV_WriteReg32(TOP_DCMCTL, 0x0);
            
            // TOPAXI_DCM
            DRV_WriteReg32(INFRA_DCMCTL, 0x0);
        
            // EMI_DCM
            temp = DRV_Reg32(0xF00041DC);
            temp &= 0xFCFFFFFF;
            DRV_WriteReg32(0xF00041DC, temp);
            
            DRV_WriteReg32(INFRA_MISC, 0x0);
            DRV_WriteReg32(INFRA_DCMCTL, 0x0);

            // PERI_DCM
            DRV_WriteReg32(PERI_GLOBALCON_DCMCTL, 0x0);
            
            // MM1_DCM
            DRV_WriteReg32(SMI_LARB0_CON_RESET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB1_CON_RESET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB2_CON_RESET, (0x1 << 11));
            DRV_WriteReg32(SMI_LARB3_CON_RESET, (0x1 << 11));
            
            // MM2_DCM
            DRV_WriteReg32(MMSYS2_DCMCTL, 0x0);
            break;
        default:
            xlog_printk(ANDROID_LOG_DEBUG, TAG, "[%s]: unknown dcm type 0x%x\n", __func__, type);
            break;
    }
}

void dcm_enable_all(void)
{
    dcm_enable(ALL_DCM);
}

void dcm_disable_all(void)
{
    dcm_disable(ALL_DCM);
}

EXPORT_SYMBOL(dcm_enable);
EXPORT_SYMBOL(dcm_disable);

EXPORT_SYMBOL(dcm_enable_all);
EXPORT_SYMBOL(dcm_disable_all);


/*
 ********************************** 
 * Idle / Deep idle Work Flow
 **********************************
 */

static int dpidle_chip_ver = 0;

static unsigned int dpidle_counter = 0; 
static unsigned int dpidle_compare = 0;
static unsigned int dpidle_compare_update = 0;

static unsigned int dpidle_time_critera = 26000;
static int dpidle_state = 0;

static int idle_switch = 1;     /* 1: enable idle,   0: disable idle */
static int dpidle_switch = 1;   /* 1: enable dpidle, 0: disable dpidle */

static unsigned int idle_count = 0;
static unsigned int dpidle_count = 0;


enum {
    DEBUG_TRACING   = 1U << 0,
    DEBUG_DETAIL    = 1U << 1,
};

static int dpidle_debug_mask = 0;
static unsigned int dpidle_block_mask[MT65XX_CLOCK_CATEGORY_COUNT];
static unsigned int dpidle_block_interval = 0;
static unsigned int dpidle_cm_block_cnt = 0;
static unsigned int dpidle_sc_block_cnt = 0;
static unsigned int dpidle_single_count = 0;
static unsigned int idle_single_count = 0;

static unsigned int timer_delay = 0;
static unsigned int timer_pre = 0; 
static unsigned int timer_pos = 0; 

#ifdef IDLE_PARAM_RECORD
static unsigned int tick_pre = 0;
static unsigned int tick_post = 0;
static unsigned int counter = 0;
static unsigned int compare = 0;
int idle_recs_idx = 0;
struct idle_record idle_recs[MAX_RECORD_NUM];
#endif

#ifdef DEEP_IDLE_PARAM_RECORD
static unsigned int dpidle_tick_pre = 0; 
static unsigned int dpidle_tick_mid = 0;
static unsigned int dpidle_tick_post = 0;
int dpidle_recs_idx = 0;
struct dpidle_record dpidle_recs[MAX_RECORD_NUM];
#endif

#ifdef ISR_PARAM_RECORD
int isr_recs_idx = 0;
struct isr_record isr_recs[MAX_RECORD_NUM];
#endif


static unsigned int clk_mux_fix = 0;
static unsigned int clk_mux_ca9 = 0;
static unsigned int clk_div1 = 0;
static unsigned int clk_mux_mm = 0;

#define fix_clk_sq2rtc()    \
do {    \
    clk_mux_fix = DRV_Reg32(TOP_CKMUXSEL);  \
    DRV_WriteReg32(TOP_CKMUXSEL, (clk_mux_fix & 0x01ff)); \
} while (0)

#define fix_clk_rtc2sq()    \
do {    \
    unsigned int temp = DRV_Reg32(TOP_CKMUXSEL);	\
    temp = (temp & 0x01ff) | (clk_mux_fix & 0x0200);	\
    DRV_WriteReg32(TOP_CKMUXSEL, temp); \
} while (0)

#if 0
#define dpidle_clkmux_backup()  \
do {    \
    clk_mux = DRV_Reg32(TOP_CKMUXSEL);  \
    DRV_WriteReg32(TOP_CKMUXSEL, (clk_mux & 0x01cf)); \
} while (0)
#endif

#define ca9_clk_pll2sq()    \
do {    \
    clk_mux_ca9 = DRV_Reg32(TOP_CKMUXSEL);  \
    DRV_WriteReg32(TOP_CKMUXSEL, (clk_mux_ca9 & 0x03f3)); \
} while (0)

#define ca9_clk_sq2pll()    \
do {    \
    unsigned int temp = DRV_Reg32(TOP_CKMUXSEL);	\
    temp = (temp & 0x03f3) | (clk_mux_ca9 & 0x000c);	\
    DRV_WriteReg32(TOP_CKMUXSEL, temp); \
} while (0)

#define ca9_no_div()	\
do {	\
    clk_div1 = DRV_Reg32(TOP_CKDIV1);	\
    DRV_WriteReg32(TOP_CKDIV1, 0x0);	\
} while (0)

#define ca9_div_restore()	\
do {	\
    DRV_WriteReg32(TOP_CKDIV1, clk_div1);	\
} while (0)

#define mm_clk_pll2sq() \
do {    \
    clk_mux_mm = DRV_Reg32(TOP_CKMUXSEL);  \
    DRV_WriteReg32(TOP_CKMUXSEL, (clk_mux_mm & 0x03cf)); \
} while (0)

#define mm_clk_sq2pll() \
do {    \
    unsigned int temp = DRV_Reg32(TOP_CKMUXSEL);	\
    temp = (temp & 0x03cf) | (clk_mux_mm & 0x0030);	\
    DRV_WriteReg32(TOP_CKMUXSEL, temp); \
} while (0)

#define wfi_with_l2off()    \
do {    \
    dsb();  \
    DRV_WriteReg32(PL310_BASE + 0x0100, 0);   \
    __asm__ __volatile__("wfi");        \
    DRV_WriteReg32(PL310_BASE + 0x0100, 1); \
} while (0)

#define wfi_with_l2sync()   \
do {    \
    dsb();  \
    DRV_WriteReg32(PL310_BASE + 0x0730, 0); \
    __asm__ __volatile__("wfi");    \
} while (0)


/* 
 * bit = 0, when the clock is on, CPU should keep at high speed
 */

static unsigned int hispeed_condition_mask[][MT65XX_CLOCK_CATEGORY_COUNT] = {
    {  
        /* for E2 */ 
        0x77980180, /* PERI_PDN0, SIM,HIF,IRDA,UART,I2C */
        0x00000000, /* PERI_PDN1, no skip bit */
        0x00000000, /* MMSYS1_PDN0, no skip bit */
        0x610E0120, /* MMSYS1_PDN1, LCD,R_DMA1,OVL_DMA_MIMO,OVL_DMA_BPS,OVL_DMA,JPEG_DMA,R_DMA0_MOUT,R_DMA0 */
        0x00000132, /* MMSYS1_PDN2, SPI,DPI,DSI */
        0x00084010, /* MMSYS2_PDN, G3D, M4U */
        0x00000060, /* AUDIO_PDN, ADC, I2S */
    }, {   
        /* for E1 */ 
        0x70001000, /* PERI_PDN0, APDMA,I2C */
        0x00000000, /* PERI_PDN1, no skip bit */
        0x00000000, /* MMSYS1_PDN0, no skip bit */
        0x610E0120, /* MMSYS1_PDN1, LCD,R_DMA1,OVL_DMA_MIMO,OVL_DMA_BPS,OVL_DMA,JPEG_DMA,R_DMA0_MOUT,R_DMA0 */
        0x00000132, /* MMSYS1_PDN2, SPI,DPI,DSI */
        0x00084010, /* MMSYS2_PDN, G3D, M4U */
        0x00000060, /* AUDIO_PDN, ADC, I2S */
    },
};


/* 
 * bit = 1, the clock bit can't be skipped, deep idle is skipped
 *          that is to say, if clock is on, we can't enter deep idle
 * bit = 0, the clock bit could be skipped or omit to enter deep idle
 *          not-clock bit will also be set zero
 */
static unsigned int dpidle_condition_mask[][MT65XX_CLOCK_CATEGORY_COUNT] = {
    {  
        /* for E2 */ 
        0x77980781, /* PERI_PDN0, NFI,SIM,USB,HIF,IRDA,UART,I2C */
        0x00000000, /* PERI_PDN1, no skip bit */
        0x00000000, /* MMSYS1_PDN0, no skip bit */
        0x610E0120, /* MMSYS1_PDN1, LCD,R_DMA1,OVL_DMA_MIMO,OVL_DMA_BPS,OVL_DMA,JPEG_DMA,R_DMA0_MOUT,R_DMA0 */
        0x00000132, /* MMSYS1_PDN2, SPI,DPI,DSI */
        0x00084010, /* MMSYS2_PDN, G3D, M4U */
        0x00000060, /* AUDIO_PDN, ADC, I2S */
    }, {   
        /* for E1 */ 
        0x70001601, /* PERI_PDN0, NFI,USB1,USB2,APDMA,I2C */
        0x00000000, /* PERI_PDN1, no skip bit */
        0x00000000, /* MMSYS1_PDN0, no skip bit */
        0x610E0120, /* MMSYS1_PDN1, LCD,R_DMA1,OVL_DMA_MIMO,OVL_DMA_BPS,OVL_DMA,JPEG_DMA,R_DMA0_MOUT,R_DMA0 */
        0x00000132, /* MMSYS1_PDN2, SPI,DPI,DSI */
        0x00084010, /* MMSYS2_PDN, G3D, M4U */
        0x00000060, /* AUDIO_PDN, ADC, I2S */
    },
};


static DEFINE_MUTEX(dpidle_condition);

/*
 * If the clock bit is on, we should keep CPU at high speed
 */
static void enable_hispeed_by_mask(int category, unsigned int mask)
{
    mutex_lock(&dpidle_condition);
    hispeed_condition_mask[dpidle_chip_ver][category] &= ~mask;
    mutex_unlock(&dpidle_condition);
}

void enable_hispeed_by_bit(enum mt65xx_clock_id id)
{
    int category = id / 32;
    int offset = id % 32;
    BUG_ON(INVALID_CATEGORY_ID(category));
    enable_hispeed_by_mask(category, 0x1 << offset);
}
EXPORT_SYMBOL(enable_hispeed_by_bit);

/*
 * Even when the clock is on, we no need to keep CPU at high speed 
 */
static void disable_hispeed_by_mask(int category, unsigned int mask)
{
    mutex_lock(&dpidle_condition);
    hispeed_condition_mask[dpidle_chip_ver][category] |= mask;
    mutex_unlock(&dpidle_condition);
}

void disable_hispeed_by_bit(enum mt65xx_clock_id id)
{
    int category = id / 32;
    int offset = id % 32;
    BUG_ON(INVALID_CATEGORY_ID(category));
    disable_hispeed_by_mask(category, 0x1 << offset);
}
EXPORT_SYMBOL(disable_hispeed_by_bit);


/*
 * Even when the clock bit is on, we can enter into dpidle
 */
static void enable_dpidle_by_mask(int category, unsigned int mask)
{
    mutex_lock(&dpidle_condition);
    dpidle_condition_mask[dpidle_chip_ver][category] &= ~mask;
    hispeed_condition_mask[dpidle_chip_ver][category] &= ~mask;
    mutex_unlock(&dpidle_condition);
}

void enable_dpidle_by_bit(enum mt65xx_clock_id id)
{
    int category = id / 32;
    int offset = id % 32;
    BUG_ON(INVALID_CATEGORY_ID(category));
    enable_dpidle_by_mask(category, 0x1 << offset);
}
EXPORT_SYMBOL(enable_dpidle_by_bit);

/*
 * If the clock bit is on, we can't enter into dpidle
 */
static void disable_dpidle_by_mask(int category, unsigned int mask)
{
    mutex_lock(&dpidle_condition);
    dpidle_condition_mask[dpidle_chip_ver][category] |= mask;
    hispeed_condition_mask[dpidle_chip_ver][category] |= mask;
    mutex_unlock(&dpidle_condition);
}

void disable_dpidle_by_bit(enum mt65xx_clock_id id)
{
    int category = id / 32;
    int offset = id % 32;
    BUG_ON(INVALID_CATEGORY_ID(category));
    disable_dpidle_by_mask(category, 0x1 << offset);
}
EXPORT_SYMBOL(disable_dpidle_by_bit);

/*
 * true: CPU should keep at high speed 
 *  scenario when CPU should keep at high speed: 
 *      1. meet dpidle condition
 *      2. USB works
 *      3. Nand works
 */
bool clkmgr_high_speed_check(void)
{
    int i = 0;
    unsigned int block_mask = 0;
    unsigned int sd_mask = 0;

    if (!get_clkmgr_init_state()) {
        return false;
    }

    msdc_clk_status(&sd_mask);
    if (sd_mask) {
        return false;
    }

    for (i = 0; i < MT65XX_CLOCK_CATEGORY_COUNT; i++) {        
        block_mask = clk_mgr.clock_state[i] & hispeed_condition_mask[dpidle_chip_ver][i];
        if (block_mask) {
            return false;  
        }
    }

    return true;
}
EXPORT_SYMBOL(clkmgr_high_speed_check);

static bool clkmgr_dpidle_can_enter(void)
{
    int i = 0;
    unsigned int block_mask = 0;
    unsigned int sd_mask = 0;

    if (!get_clkmgr_init_state()) {
        return false;
    }

    msdc_clk_status(&sd_mask);
    if (sd_mask) {
        if (dpidle_debug_mask & DEBUG_TRACING) {
            dpidle_block_mask[MT65XX_CLOCK_PERI_PDN0] |= sd_mask;
            dpidle_cm_block_cnt++;
        }
        return false;
    }

    for (i = 0; i < MT65XX_CLOCK_CATEGORY_COUNT; i++) {        
        block_mask = clk_mgr.clock_state[i] & dpidle_condition_mask[dpidle_chip_ver][i];
        if (block_mask) {
            if (dpidle_debug_mask & DEBUG_TRACING) {
                dpidle_block_mask[i] |= block_mask;
                dpidle_cm_block_cnt++;
            }
            return false;  
        }
    }

    return true;
}

bool sc_dpidle_can_enter(void)
{
#ifdef DEEP_IDLE_PARAM_RECORD
    dpidle_tick_pre = GPT_GetCounter(GPT2);
#endif
    dpidle_counter = GPT_GetCounter(GPT1);
    dpidle_compare = GPT_GetCompare(GPT1);

    if (dpidle_compare >= dpidle_counter + dpidle_time_critera) { // 13000 => 1ms
        dpidle_state = 1;
        return true; 
    }

    if ((dpidle_debug_mask & DEBUG_TRACING)) {
        unsigned int delta = dpidle_compare - dpidle_counter;
        if (delta > dpidle_block_interval) {
            dpidle_block_interval = delta;
        }
        dpidle_sc_block_cnt++;
    }

    dpidle_state = 0;
    return false;
}

static unsigned int topmisc;
static int mmsys_switched_off = 0;

void sc_dpidle_before_wfi(void)
{
    //fix_clk_sq2rtc();

    /* keep CA9 clock frequency when WFI to sleep */
    topmisc = DRV_Reg32(TOP_MISC);
    DRV_WriteReg32(TOP_MISC, (topmisc & ~(1U << 0)));
    
    if (unlikely(get_chip_eco_ver() == CHIP_E1)) {        
		DRV_ClrReg32(WPLL_CON0, 0x1);
    } else {
        if (!(clk_mgr.subsys_state & 0x30) && clk_mgr.mmsys_state) {
            mm_clk_pll2sq();
            DRV_SetReg16(MDPLL_CON0, 0x1);
            mmsys_switched_off = 1;
        }
    }
    
    if (mmsys_switched_off) {
        dpidle_compare_update = dpidle_compare - dpidle_counter - 260;
    } else {
        dpidle_compare_update = dpidle_compare - dpidle_counter;
    }

    gpt_set_next_compare(dpidle_compare_update);

#ifdef DEEP_IDLE_PARAM_RECORD
    dpidle_tick_mid = GPT_GetCounter(GPT2);
#endif
}

void sc_dpidle_after_wfi(void)
{
#ifdef DEEP_IDLE_PARAM_RECORD
    dpidle_tick_post = GPT_GetCounter(GPT2);
    if (dpidle_recs_idx < MAX_RECORD_NUM) {
        dpidle_recs[dpidle_recs_idx].dpidle_tick_pre = dpidle_tick_pre;
        dpidle_recs[dpidle_recs_idx].dpidle_tick_mid = dpidle_tick_mid;
        dpidle_recs[dpidle_recs_idx].dpidle_tick_post = dpidle_tick_post;
        dpidle_recs[dpidle_recs_idx].dpidle_counter = dpidle_counter;
        dpidle_recs[dpidle_recs_idx].dpidle_compare = dpidle_compare;
        dpidle_recs[dpidle_recs_idx].dpidle_compare_update = dpidle_compare_update;
        dpidle_recs[dpidle_recs_idx].dpidle_wakeup_src = DRV_Reg32(SC_WAKEUP_SRC);
        if (dpidle_recs_idx) {
            dpidle_recs[dpidle_recs_idx-1].dpidle_irq = irq_nr();
        }
        dpidle_recs_idx++;
    }        
#endif

    if (get_chip_eco_ver() == CHIP_E1) {
        DRV_SetReg32(WPLL_CON0, 0x1);        
    } else {
        if (mmsys_switched_off) {
            DRV_ClrReg16(MDPLL_CON0, 0x1);
            udelay(20);
            mm_clk_sq2pll();
            mmsys_switched_off = 0;
        }
    }   

    //fix_clk_rtc2sq();
    
    /* restore TOP_MISC */
    DRV_WriteReg32(TOP_MISC, topmisc);

    dpidle_count++;
    if ((dpidle_debug_mask & DEBUG_TRACING)) {
        dpidle_single_count++;
    }
}

static int ca9_div_switched_off = 0;

static void idle_before_wfi(void)
{
    if (DRV_Reg32(TOP_CKDIV1)) {
        fix_clk_sq2rtc();
        ca9_clk_pll2sq();	
        ca9_no_div();
        DRV_SetReg32(TOP_MISC, 0x2);
        fix_clk_rtc2sq();
        ca9_div_switched_off = 1;
    } else {
        DRV_SetReg32(TOP_MISC, 0x2);
    }

#ifdef IDLE_PARAM_RECORD
    tick_pre = GPT_GetCounter(GPT2);
    counter = GPT_GetCounter(GPT1);
    compare = GPT_GetCompare(GPT1);	
#endif
}

static void idle_after_wfi(void)
{
#ifdef IDLE_PARAM_RECORD
    tick_post = GPT_GetCounter(GPT2);

    if (!subsys_is_on(MT65XX_SUBSYS_MM1) && (idle_recs_idx < MAX_RECORD_NUM)) {
        idle_recs[idle_recs_idx].tick_pre = tick_pre;    
        idle_recs[idle_recs_idx].tick_post = tick_post;
        idle_recs[idle_recs_idx].counter = counter;    
        idle_recs[idle_recs_idx].compare = compare;
        if (idle_recs_idx) {
            idle_recs[idle_recs_idx-1].irq = irq_nr();    
        }
        idle_recs_idx++;
    }
#endif

    if (ca9_div_switched_off) {
        fix_clk_sq2rtc();
        DRV_ClrReg32(TOP_MISC, 0x2);
        ca9_div_restore();
        ca9_clk_sq2pll();
        fix_clk_rtc2sq();
        ca9_div_switched_off = 0;
    } else {
        DRV_ClrReg32(TOP_MISC, 0x2);
    }

    idle_count++;
    if ((dpidle_debug_mask & DEBUG_TRACING)) {
        idle_single_count++;
    }
}

static void go_to_idle(void)
{
    idle_before_wfi();

    if (get_chip_eco_ver() == CHIP_E1) {
        wfi_with_l2off();
    } else {
        wfi_with_l2sync();
    }

    idle_after_wfi();
}

//static unsigned int dpidle_level = 0;
static unsigned int dpidle_level = 2;

void arch_idle(void)
{
    if (dpidle_switch) {
        if (clkmgr_dpidle_can_enter()) {
            sc_go_to_dpidle(dpidle_level);
        }
        
        if (dpidle_state == 1) {
            dpidle_state = 0;
            return;
        }
    } 

    if (idle_switch) {
        go_to_idle();
    }
}


static ssize_t dpidle_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int len = 0;
    char *p = buf;
    int i = 0;

#ifdef IDLE_PARAM_RECORD
    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "%10s %10s %10s %10s %10s %10s %10s\n",
        "idle_idx", "tick_pre", "tick_post", "tick_isr", "cnt", "cmp", "irq");
    for (i = 0; i < MAX_RECORD_NUM; i++) {
        xlog_printk(ANDROID_LOG_VERBOSE, TAG, "%6d %10u %10u %10u %10u %10u %4d\n",
                i, idle_recs[i].tick_pre, idle_recs[i].tick_post,
                idle_recs[i].tick_isr, idle_recs[i].counter, 
                idle_recs[i].compare, idle_recs[i].irq);
    }  
#endif

#ifdef DEEP_IDLE_PARAM_RECORD
    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "%10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n",
        "dp_idx", "tick_pre", "tick_mid", "tick_post", "tick_isr", "cnt", "cmp", "cmp_update", "src", "irq");
    for (i = 0; i < MAX_RECORD_NUM; i++) {
        xlog_printk(ANDROID_LOG_VERBOSE, TAG, "%6d %10u %10u %10u %10u %10u %10u %10u %10u %4d\n",
            i, dpidle_recs[i].dpidle_tick_pre, dpidle_recs[i].dpidle_tick_mid, 
            dpidle_recs[i].dpidle_tick_post, dpidle_recs[i].dpidle_tick_isr,
            dpidle_recs[i].dpidle_counter, dpidle_recs[i].dpidle_compare,
            dpidle_recs[i].dpidle_compare_update, dpidle_recs[i].dpidle_wakeup_src,
            dpidle_recs[i].dpidle_irq);
    }  
#endif

#ifdef ISR_PARAM_RECORD
    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "%10s %10s\n", "isr_idx", "tick_isr");
    for (i = 0; i < MAX_RECORD_NUM; i++) {
        xlog_printk(ANDROID_LOG_VERBOSE, TAG, "%10u %10u\n", i,  isr_recs[i].tick_isr);
    }
#endif

    p += sprintf(p, "*********** Deep Idle State ************\n");
    p += sprintf(p, "idle_count = %d, dpidle_count = %d\n", idle_count, dpidle_count);
    p += sprintf(p, "idle_switch = %d, dpidle_switch = %d\n", idle_switch, dpidle_switch);
    p += sprintf(p, "dpidle_level = %u\n", dpidle_level);
    p += sprintf(p, "dpidle_time_critera = %d\n", dpidle_time_critera);
    p += sprintf(p, "dpidle_debug_mask = %d\n", dpidle_debug_mask);
    for (i = 0; i < MT65XX_CLOCK_CATEGORY_COUNT; i++) {
        p += sprintf(p, "hispeed_condition_mask[%d][%d] = 0x%08x\n", dpidle_chip_ver, i, hispeed_condition_mask[dpidle_chip_ver][i]);
    }

    for (i = 0; i < MT65XX_CLOCK_CATEGORY_COUNT; i++) {
        p += sprintf(p, "dpidle_condition_mask[%d][%d] = 0x%08x\n", dpidle_chip_ver, i, dpidle_condition_mask[dpidle_chip_ver][i]);
    }

    p += sprintf(p, "dpidle_cm_block_cnt = %d, dpidle_sc_block_cnt = %d\n", dpidle_cm_block_cnt, dpidle_sc_block_cnt);
    p += sprintf(p, "idle_single_count = %d, dpidle_single_count = %d\n", idle_single_count, dpidle_single_count);
    p += sprintf(p, "dpidle_block_interval = %d\n", dpidle_block_interval);
    for (i = 0; i < MT65XX_CLOCK_CATEGORY_COUNT; i++) {
        p += sprintf(p, "dpidle_block_mask[%d] = 0x%08x\n", i, dpidle_block_mask[i]);
    }

    p += sprintf(p, "timer_delay = %d, timer_pre = %u, timer_pos = %u\n", timer_delay, timer_pre, timer_pos);

    p += sprintf(p, "\n*********** Deep Idle Command Help ************\n");
    p += sprintf(p, "enable idle:               echo idle 1 > /sys/power/dpidle_state\n");
    p += sprintf(p, "disable idle:              echo idle 0 > /sys/power/dpidle_state\n");
    p += sprintf(p, "enable deep idle:          echo [dpidle] 1 > /sys/power/dpidle_state\n");
    p += sprintf(p, "disable deep idle:         echo [dpidle] 0 > /sys/power/dpidle_state\n");
    p += sprintf(p, "modify deep idle level:    echo level 0/1/2 > /sys/power/dpidle_state\n");
    p += sprintf(p, "modify time critera:       echo time value(decimal) > /sys/power/dpidle_state\n");
    p += sprintf(p, "enable dpidle condition:   echo enable id > /sys/power/dpidle_state\n");
    p += sprintf(p, "disable dpidle condition:  echo disable id > /sys/power/dpidle_state\n");
    p += sprintf(p, "enable hispeed condition:  echo enable_hi id > /sys/power/dpidle_state\n");
    p += sprintf(p, "disable hispeed condition: echo disable_hi id > /sys/power/dpidle_state\n");
    p += sprintf(p, "start single tracing:      echo trace 1 > /sys/power/dpidle_state\n");
    p += sprintf(p, "stop single tracing:       echo trace 0 > /sys/power/dpidle_state\n");
    p += sprintf(p, "test GPT timer :           echo timer 5 > /sys/power/dpidle_state\n");
#ifdef IDLE_PARAM_RECORD
    p += sprintf(p, "reset idle rec:            echo reset 1 > /sys/power/dpidle_state\n");
#endif
#ifdef DEEP_IDLE_PARAM_RECORD
    p += sprintf(p, "reset deep idle rec:       echo reset 2 > /sys/power/dpidle_state\n");
#endif
#ifdef ISR_PARAM_RECORD
    p += sprintf(p, "reset isr rec:             echo reset 3 > /sys/power/dpidle_state\n");
#endif

    len = p - buf;
    return len;
}

static ssize_t dpidle_state_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t n)
{
    char command[32];
    int temp;

    if (sscanf(buf, "%s %d", command, &temp) == 2) {
        if (!strcmp(command, "idle")) {
            idle_switch = temp;
        } else if (!strcmp(command, "dpidle")) {
            dpidle_switch = temp;
        } else if (!strcmp(command, "level")) {
			if (unlikely(temp < 0 || temp > 2)) {
				xlog_printk(ANDROID_LOG_WARN, TAG, "[%s]invalid dpidle_level %d\n", __func__, temp);
			} else {
	            dpidle_level = (unsigned int)temp;
			}
        } else if (!strcmp(command, "time")) {
            dpidle_time_critera = temp;
        } else if (!strcmp(command, "enable")) {
            enable_dpidle_by_bit(temp);
        } else if (!strcmp(command, "disable")) {
            disable_dpidle_by_bit(temp);
        } else if (!strcmp(command, "enable_hi")) {
            enable_hispeed_by_bit(temp);
        } else if (!strcmp(command, "disable_hi")) {
            disable_hispeed_by_bit(temp);
        } else if (!strcmp(command, "trace")) {
            if (temp == 1) {
                idle_single_count = 0;
                dpidle_single_count = 0;
                dpidle_cm_block_cnt = 0;
                dpidle_sc_block_cnt = 0;
                dpidle_block_interval = 0;
                memset(dpidle_block_mask, 0, MT65XX_CLOCK_CATEGORY_COUNT * sizeof(unsigned int));
                dpidle_debug_mask = DEBUG_TRACING;
            } else if (temp == 0) {
                dpidle_debug_mask = 0;
            }
        } else if (!strcmp(command, "timer")) {
            timer_delay = temp;
            timer_pre = GPT_GetCounter(GPT2);
            udelay(timer_delay);
            timer_pos = GPT_GetCounter(GPT2);
        } else if (!strcmp(command, "reset")) {
#ifdef IDLE_PARAM_RECORD
            if (temp == 1 && idle_recs_idx >= MAX_RECORD_NUM) {
                memset(idle_recs, 0, MAX_RECORD_NUM * sizeof(struct idle_record));
                idle_recs_idx = 0; 
            }
#endif
#ifdef DEEP_IDLE_PARAM_RECORD
            if (temp == 2 && dpidle_recs_idx >= MAX_RECORD_NUM) {
                memset(dpidle_recs, 0, MAX_RECORD_NUM * sizeof(struct dpidle_record));
                dpidle_recs_idx = 0;
            }
#endif
#ifdef ISR_PARAM_RECORD
            if (temp == 3 && isr_recs_idx >= MAX_RECORD_NUM) {
                memset(isr_recs, 0, MAX_RECORD_NUM * sizeof(struct isr_record));
                isr_recs_idx = 0;
            }
#endif
        }
        return n;
    } else if (sscanf(buf, "%x", &temp) == 1) {
        dpidle_switch = temp;
        return n;
    }

    return -EINVAL;
}
dcm_attr(dpidle_state);

void factory_idle_test(void)
{
    mtk_wdt_disable(); // disable watch dog
    
    //this should be set by low power requirement.
#ifdef IDLE_LOW_POWER_TEST
    enable_low_power_settings();
#endif
    local_irq_disable();
    go_to_idle();
    local_irq_enable();
#ifdef IDLE_LOW_POWER_TEST
    disable_low_power_settings();
#endif
}

static ssize_t idle_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int len = 0;
    char *p = buf;

    p += sprintf(p, "=============== Idle State ===============\n");
    p += sprintf(p, "unused for now\n");
    p += sprintf(p, "=========================================\n");

    len = p - buf;
    return len;
}

static ssize_t idle_state_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t n)
{
    DWORD test_mode;

    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "\n=============== Idle State ================\n");
    if (sscanf(buf, "%d", &test_mode) == 1) {
        if (test_mode == TEST_ARCH_IDLE) {
            factory_idle_test();
            xlog_printk(ANDROID_LOG_VERBOSE, TAG, "=========================================\n");
            return n;
        } else {
            xlog_printk(ANDROID_LOG_ERROR, TAG, "  bad argument!!\n");
        }
    } else {
        xlog_printk(ANDROID_LOG_ERROR, TAG, "  bad argument!!\n");
    }

    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "=========================================\n");

    return -EINVAL;
}
dcm_attr(idle_state);

extern void mt6575_irq_unmask_for_sleep(unsigned int irq);

#ifdef IDLE_LOW_POWER_TEST
void enable_low_power_settings(void)
{
    int ret;
    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "[%s]: entry\n", __func__);
    
    // turn off backlight
    #if defined(CONFIG_MTK_LEDS)
    mt65xx_leds_brightness_set(MT65XX_LED_TYPE_LCD, 0);
    #endif
    
    // turn off LCD
    //#if defined(CONFIG_MTK_FB)
    #if 0
    DISP_PanelEnable(FALSE);
    DISP_PowerEnable(FALSE);	
    #endif
    
    // save interrupt status and mask all interrupt
    ret = mt6575_irq_mask_all(&mask);
    if (!ret) {
        xlog_printk(ANDROID_LOG_VERBOSE, TAG, "[%s]: success to mask all irq lines\n", __func__);
    } else {
        xlog_printk(ANDROID_LOG_ERROR, TAG, "[%s]: fail to mask all irq lines\n", __func__);
    }
    
    mt6575_irq_unmask_for_sleep(MT6575_KP_IRQ_ID);
}
#endif

#ifdef IDLE_LOW_POWER_TEST
void disable_low_power_settings(void)
{
    int ret;
    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "[%s]: entry\n", __func__);
    
    // restore interrupt mask
    ret = mt6575_irq_mask_restore(&mask);
    if (!ret) {
        xlog_printk(ANDROID_LOG_VERBOSE, TAG, "[%s]: success to restore all irq lines\n", __func__);
    } else {
        xlog_printk(ANDROID_LOG_ERROR, TAG, "[%s]: fail to restore all irq lines\n", __func__);
    }

    // turn on LCD
    //#if defined(CONFIG_MTK_FB)
    #if 0
    DISP_PowerEnable(TRUE);
    DISP_PanelEnable(TRUE);	
    #endif
    
    // turn on backlight
    #if defined(CONFIG_MTK_LEDS)
    mt65xx_leds_brightness_set(MT65XX_LED_TYPE_LCD, LED_FULL);
    #endif
}
#endif

static ssize_t dcm_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int len = 0;
    char *p = buf;
    
    if (DRV_Reg32(TOP_DCMCTL) != 0x6)
        p += sprintf(p, "disable - top (0x%x)\n", DRV_Reg32(TOP_DCMCTL));
    else if ((DRV_Reg32(INFRA_DCMCTL) & 0x0001) != 0x0001)
        p += sprintf(p, "disable - infra (0x%x)\n", DRV_Reg32(INFRA_DCMCTL));
    else if ((DRV_Reg32(PERI_GLOBALCON_DCMCTL) & 0x0001) != 0x0001)
        p += sprintf(p, "disable - peri (0x%x)\n", DRV_Reg32(PERI_GLOBALCON_DCMCTL));
    else if ((DRV_Reg32(MMSYS2_DCMCTL) & 0x0001) != 0x0001)
        p += sprintf(p, "disable - mmsys2 (0x%x)\n", DRV_Reg32(MMSYS2_DCMCTL));
    else if ((DRV_Reg32(SMI_LARB0_CON) & 0x0800) != 0x0800)
        p += sprintf(p, "disable - mmsys1 larb0 (0x%x)\n", DRV_Reg32(SMI_LARB0_CON));
    else if ((DRV_Reg32(SMI_LARB1_CON) & 0x0800) != 0x0800)
        p += sprintf(p, "disable - mmsys1 larb1 (0x%x)\n", DRV_Reg32(SMI_LARB1_CON));
    else if ((DRV_Reg32(SMI_LARB2_CON) & 0x0800) != 0x0800)
        p += sprintf(p, "disable - mmsys1 larb2 (0x%x)\n", DRV_Reg32(SMI_LARB2_CON));
    else if ((DRV_Reg32(SMI_LARB3_CON) & 0x0800) != 0x0800)
        p += sprintf(p, "disable - mmsys1 larb3 (0x%x)\n", DRV_Reg32(SMI_LARB3_CON));
    else
        p += sprintf(p, "enable\n");

    len = p - buf;
    return len;
}

static ssize_t dcm_state_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t n)
{
    char dcm_mode[20];

    xlog_printk(ANDROID_LOG_VERBOSE, TAG, "\n=============== DCM State ================\n");
    if (sscanf(buf, "%s", dcm_mode) == 1) {
        if (!strncmp(dcm_mode, "enable", 6)) {
            xlog_printk(ANDROID_LOG_DEBUG, TAG, "  enable dcm\n");
            dcm_enable_all();
            xlog_printk(ANDROID_LOG_VERBOSE, TAG, "==========================================\n");
            return n;
        } else if (!strncmp(dcm_mode, "disable", 7)) {
            xlog_printk(ANDROID_LOG_DEBUG, TAG, "  disable dcm\n");
            dcm_disable_all();
            xlog_printk(ANDROID_LOG_VERBOSE, TAG, "==========================================\n");
            return n;
        } else {
            xlog_printk(ANDROID_LOG_ERROR, TAG, "  bad argument!! argument should be \"enable\" or \"disable\"\n");
            xlog_printk(ANDROID_LOG_VERBOSE, TAG, "==========================================\n");
        }
    } else {
        xlog_printk(ANDROID_LOG_ERROR, TAG, "  bad argument!!\n");
        xlog_printk(ANDROID_LOG_VERBOSE, TAG, "==========================================\n");
    }
    
    return -EINVAL;
}
dcm_attr(dcm_state);

void mt6575_dcm_init(void)
{
    int ret = 0;
    
    xlog_printk(ANDROID_LOG_INFO, TAG, "[%s]: enable HW dcm\n", __func__);
    dcm_enable_all();

    dpidle_chip_ver = (get_chip_eco_ver() == CHIP_E1) ? 1 : 0; 
    memset(dpidle_block_mask, 0, MT65XX_CLOCK_CATEGORY_COUNT * sizeof(unsigned int));
    
    ret = sysfs_create_file(power_kobj, &dcm_state_attr.attr);
    ret = sysfs_create_file(power_kobj, &idle_state_attr.attr);
    ret = sysfs_create_file(power_kobj, &dpidle_state_attr.attr);
}

void mt6575_dcm_exit(void)
{
    xlog_printk(ANDROID_LOG_INFO, TAG, "[%s]: disable HW dcm\n", __func__);
    dcm_disable_all();
}
