
#ifndef _MT6575_DCM_H
#define _MT6575_DCM_H

#include <linux/module.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include "mach/sync_write.h"
#include "mach/mt6575_reg_base.h"
#include "mach/mt6575_typedefs.h"
#include "mach/mt6575_clock_manager.h"

/*******************************************************************************
 * DCM Register Definition
 ******************************************************************************/
#define TOP_DCMCTL              (INFRA_SYS_CFG_BASE + 0x010)
#define TOP_MISC                (INFRA_SYS_CFG_BASE + 0x014)
#define TOP_CA9DCMFSEL          (INFRA_SYS_CFG_BASE + 0x018)

#define INFRA_DCMCTL            (INFRA_SYS_CFG_BASE + 0x050)
#define INFRA_DCMDBC            (INFRA_SYS_CFG_BASE + 0x054)
#define INFRA_DCMFSEL           (INFRA_SYS_CFG_BASE + 0x058)

#define INFRA_MISC              (INFRA_SYS_CFG_BASE + 0xF00)

#define PERI_GLOBALCON_DCMCTL   (PERICFG_BASE + 0x020)
#define PERI_GLOBALCON_DCMDBC   (PERICFG_BASE + 0x024)
#define PERI_GLOBALCON_DCMFSEL  (PERICFG_BASE + 0x028)

#define MMSYS2_DCMCTL           (MMSYS2_CONFG_BASE + 0x320)
#define MMSYS2_DCMDBC           (MMSYS2_CONFG_BASE + 0x324)
#define MMSYS2_DCMFSEL          (MMSYS2_CONFG_BASE + 0x328)

#define SMI_LARB0_CON           (SMI_LARB0_BASE + 0x010)
#define SMI_LARB0_CON_SET       (SMI_LARB0_BASE + 0x014)
#define SMI_LARB0_CON_RESET     (SMI_LARB0_BASE + 0x018)
#define SMI_LARB1_CON           (SMI_LARB1_BASE + 0x010)
#define SMI_LARB1_CON_SET       (SMI_LARB1_BASE + 0x014)
#define SMI_LARB1_CON_RESET     (SMI_LARB1_BASE + 0x018)
#define SMI_LARB2_CON           (SMI_LARB2_BASE + 0x010)
#define SMI_LARB2_CON_SET       (SMI_LARB2_BASE + 0x014)
#define SMI_LARB2_CON_RESET     (SMI_LARB2_BASE + 0x018)
#define SMI_LARB3_CON           (SMI_LARB3_BASE + 0x010)
#define SMI_LARB3_CON_SET       (SMI_LARB3_BASE + 0x014)
#define SMI_LARB3_CON_RESET     (SMI_LARB3_BASE + 0x018)

#define CA9_DCM     0x1
#define TOPAXI_DCM  0x2
#define EMI_DCM     0x3
#define PERI_DCM    0x4
#define MM1_DCM     0x5
#define MM2_DCM     0x6
#define ALL_DCM     0x7

#define AHB0_OFF_AHB1_OFF   ((0 << 10) | (0 << 9))
#define AHB0_ON_AHB1_OFF    ((0 << 10) | (1 << 9))
#define AHB0_OFF_AHB1_ON    ((1 << 10) | (0 << 9))
#define AHB0_ON_AHB1_ON     ((1 << 10) | (1 << 9)) 

// DCM API
extern void dcm_enable_all(void);
extern void dcm_disable_all(void);

extern void dcm_enable(unsigned short type);
extern void dcm_disable(unsigned short type);

extern void enable_dpidle_by_bit(enum mt65xx_clock_id id);
extern void disable_dpidle_by_bit(enum mt65xx_clock_id id);

extern void enable_hispeed_by_bit(enum mt65xx_clock_id id);
extern void disable_hispeed_by_bit(enum mt65xx_clock_id id);

extern void mt6575_dcm_init(void);


//#define IDLE_PARAM_RECORD
//#define DEEP_IDLE_PARAM_RECORD
//#define ISR_PARAM_RECORD

#define MAX_RECORD_NUM  100

#ifdef IDLE_PARAM_RECORD
struct idle_record {
    unsigned int tick_pre;   /* tick before enter WFI */
    unsigned int tick_post;  /* tick after exit WFI */
    unsigned int tick_isr;
    unsigned int counter;
    unsigned int compare;
    int irq;
};

extern int idle_recs_idx;
extern struct idle_record idle_recs[MAX_RECORD_NUM];
#endif

#ifdef DEEP_IDLE_PARAM_RECORD
struct dpidle_record {
    unsigned int dpidle_tick_pre;
    unsigned int dpidle_tick_mid;
    unsigned int dpidle_tick_post;
    unsigned int dpidle_tick_isr;
    unsigned int dpidle_counter;
    unsigned int dpidle_compare;
    unsigned int dpidle_compare_update;
    unsigned int dpidle_wakeup_src;
    int dpidle_irq;
};

extern int dpidle_recs_idx;
extern struct dpidle_record dpidle_recs[MAX_RECORD_NUM];
#endif

#ifdef ISR_PARAM_RECORD
struct isr_record {
    unsigned int tick_isr;
};

extern int isr_recs_idx;
extern struct isr_record isr_recs[MAX_RECORD_NUM];
#endif

#endif
