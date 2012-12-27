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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/suspend.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/console.h>
#include <linux/xlog.h>

#include <asm/io.h>
#include <mach/mt6575_boot.h>
#include <mach/mt6575_sc.h>
#include <mach/mt6575_clock_manager.h>
#include <mach/mt6575_dcm.h>
#include <mach/mtk_rtc.h>
#include <mach/mt6575_gpio.h>
#include <mach/pmic_mt6329_sw.h>
#include <mach/upmu_common_sw.h>

#include <board-custom.h>
#include <mt6575_battery.h>

#define XLOG_MYTAG		"Power/Sleep"

#define slp_read16(addr)	(*(volatile u16 *)(addr))
#define slp_write16(addr, val)	(*(volatile u16 *)(addr) = (u16)(val))

#define slp_read32(addr)	(*(volatile u32 *)(addr))
#define slp_write32(addr, val)	(*(volatile u32 *)(addr) = (u32)(val))

#define slp_write_sync()	\
do {				\
	dsb();			\
	outer_sync();		\
} while (0)

#define slp_xverb(fmt, args...)		\
	xlog_printk(ANDROID_LOG_VERBOSE, XLOG_MYTAG, fmt, ##args)

#define slp_xinfo(fmt, args...)		\
	xlog_printk(ANDROID_LOG_INFO, XLOG_MYTAG, fmt, ##args)

#define slp_xerror(fmt, args...)	\
	xlog_printk(ANDROID_LOG_ERROR, XLOG_MYTAG, fmt, ##args)

extern void pmic_status_for_sleep(void);

static wake_reason_t slp_wake_reason;

/*
 * 0: CPU standby + AP sleep => legacy sleep mode
 * 1: CPU dormant (L1/L2 sleep) + AP sleep
 *    => may not work when VPROC/VM12_INT = 0.9V on E1 due to SRAM issue
 * 2: CPU shutdown (L1/L2 power down) + AP sleep
 */
static unsigned int slp_pwr_level = 2;

static int slp_dump_pmic = 0;
static int slp_dump_gpio = 0;
static int slp_dump_regs = 1;

static unsigned long RM_CLK_SETTLE = 0;
static unsigned long RM_TMR_SSTA = 0;

static DEFINE_SPINLOCK(slp_cc_lock);
static ccci_callback_t slp_ccci_callback = NULL;

void slp_set_ccci_callback(ccci_callback_t cc)
{
	unsigned long flags;

	spin_lock_irqsave(&slp_cc_lock, flags);
	slp_ccci_callback = cc;
	spin_unlock_irqrestore(&slp_cc_lock, flags);
}
EXPORT_SYMBOL(slp_set_ccci_callback);

static int slp_md_sta_show(struct seq_file *m, void *v)
{
	u32 ssta, mdssta = 0;
	int i;

	if (!RM_TMR_SSTA)
		return -ENOMEM;

	sc_request_mdbus_clk(SC_MDBUS_USER_SLEEP);
	ssta = slp_read32(RM_TMR_SSTA);
	sc_unrequest_mdbus_clk(SC_MDBUS_USER_SLEEP);

	for (i = 0; i < 4; i++) {
		/* check if timer is in Pause or Pre-Pause state */
		if ((ssta & 0xf) == 0x2 || (ssta & 0xf) == 0x4)
			mdssta |= (1U << (i * 4));
		ssta >>= 4;
	}

	seq_printf(m, "0x%x\n", mdssta);

	return 0;
}

static int slp_md_sta_open(struct inode *inode, struct file *file)
{
	return single_open(file, slp_md_sta_show, NULL);
}

static struct file_operations slp_md_sta_fops = {
	.open		= slp_md_sta_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static void slp_dump_pm_regs(void)
{
	/* TOPSM registers */
	sc_request_mdbus_clk(SC_MDBUS_USER_SLEEP);
	if (RM_CLK_SETTLE)
		slp_xverb("RM_CLK_SETTLE   0x%x = 0x%x\n", 0xd1090000, slp_read32(RM_CLK_SETTLE));
	sc_unrequest_mdbus_clk(SC_MDBUS_USER_SLEEP);

	/* ACP related registers */
	slp_xverb("PERIAXIBUS_CTL1 0x%x = 0x%x\n", 0xf1000200, slp_read32(0xf1000200));
	slp_xverb("PG_DMA_CACHE    0x%x = 0x%x\n", 0xf1000100, slp_read32(0xf1000100));
	slp_xverb("VDEC_DMACFG     0x%x = 0x%x\n", 0xf2084024, slp_read32(0xf2084024));
	slp_xverb("VENC_MC_CTRL    0x%x = 0x%x\n", 0xf208506c, slp_read32(0xf208506c));

	/* PLL registers */
	slp_xverb("PLL_CON1        0x%x = 0x%x\n", PLL_CON1   , slp_read16(PLL_CON1));
	slp_xverb("PLL_CON2        0x%x = 0x%x\n", PLL_CON2   , slp_read16(PLL_CON2));
	slp_xverb("IPLL_CON0       0x%x = 0x%x\n", IPLL_CON0  , slp_read16(IPLL_CON0));
	slp_xverb("UPLL_CON0       0x%x = 0x%x\n", UPLL_CON0  , slp_read16(UPLL_CON0));
	slp_xverb("MDPLL_CON0      0x%x = 0x%x\n", MDPLL_CON0 , slp_read16(MDPLL_CON0));
	slp_xverb("WPLL_CON0       0x%x = 0x%x\n", WPLL_CON0  , slp_read16(WPLL_CON0));
	slp_xverb("AUDPLL_CON0     0x%x = 0x%x\n", AUDPLL_CON0, slp_read16(AUDPLL_CON0));

	/* TOPCKGEN/PERICFG/MMSYS registers */
	slp_xverb("TOP_CKMUXSEL    0x%x = 0x%x\n", TOP_CKMUXSEL       , slp_read32(TOP_CKMUXSEL));
	slp_xverb("TOP_DCMCTL      0x%x = 0x%x\n", TOP_DCMCTL         , slp_read32(TOP_DCMCTL));
	slp_xverb("TOP_MISC        0x%x = 0x%x\n", TOP_MISC           , slp_read32(TOP_MISC));
	slp_xverb("GLOBALCON_PDN0  0x%x = 0x%x\n", PERI_GLOBALCON_PDN0, slp_read32(PERI_GLOBALCON_PDN0));
	slp_xverb("GLOBALCON_PDN1  0x%x = 0x%x\n", PERI_GLOBALCON_PDN1, slp_read32(PERI_GLOBALCON_PDN1));
	slp_xverb("GLOBALCON_PDN2  0x%x = 0x%x\n", PERI_GLOBALCON_PDN2, slp_read32(PERI_GLOBALCON_PDN2));
	slp_xverb("GLOBALCON_PDN3  0x%x = 0x%x\n", PERI_GLOBALCON_PDN3, slp_read32(PERI_GLOBALCON_PDN3));
	slp_xverb("MMSYS1_CG_CON0  0x%x = 0x%x\n", MMSYS1_CG_CON0     , slp_read32(MMSYS1_CG_CON0));
	slp_xverb("MMSYS1_CG_CON1  0x%x = 0x%x\n", MMSYS1_CG_CON1     , slp_read32(MMSYS1_CG_CON1));
	slp_xverb("MMSYS1_CG_CON2  0x%x = 0x%x\n", MMSYS1_CG_CON2     , slp_read32(MMSYS1_CG_CON2));
	slp_xverb("MMSYS2_CG_CON   0x%x = 0x%x\n", MMSYS2_CG_CON      , slp_read32(MMSYS2_CG_CON));

	/* SLPCTRL registers */
	slp_xverb("SC_CLK_SETTLE   0x%x = 0x%x\n", SC_CLK_SETTLE  , slp_read32(SC_CLK_SETTLE));
	slp_xverb("SC_PWR_SETTLE   0x%x = 0x%x\n", SC_PWR_SETTLE  , slp_read32(SC_PWR_SETTLE));
	slp_xverb("SC_PWR_CON0     0x%x = 0x%x\n", SC_PWR_CON0    , slp_read32(SC_PWR_CON0));
	slp_xverb("SC_PWR_CON1     0x%x = 0x%x\n", SC_PWR_CON1    , slp_read32(SC_PWR_CON1));
	slp_xverb("SC_PWR_CON2     0x%x = 0x%x\n", SC_PWR_CON2    , slp_read32(SC_PWR_CON2));
	slp_xverb("SC_PWR_CON3     0x%x = 0x%x\n", SC_PWR_CON3    , slp_read32(SC_PWR_CON3));
	slp_xverb("SC_PWR_CON4     0x%x = 0x%x\n", SC_PWR_CON4    , slp_read32(SC_PWR_CON4));
	slp_xverb("SC_PWR_CON5     0x%x = 0x%x\n", SC_PWR_CON5    , slp_read32(SC_PWR_CON5));
	slp_xverb("SC_PWR_CON6     0x%x = 0x%x\n", SC_PWR_CON6    , slp_read32(SC_PWR_CON6));
	slp_xverb("SC_PWR_CON8     0x%x = 0x%x\n", SC_PWR_CON8    , slp_read32(SC_PWR_CON8));
	slp_xverb("SC_PWR_CON9     0x%x = 0x%x\n", SC_PWR_CON9    , slp_read32(SC_PWR_CON9));
	slp_xverb("SC_CLK_CON      0x%x = 0x%x\n", SC_CLK_CON     , slp_read32(SC_CLK_CON));
	slp_xverb("SC_MD_CLK_CON   0x%x = 0x%x\n", SC_MD_CLK_CON  , slp_read32(SC_MD_CLK_CON));
	slp_xverb("SC_MD_INTF_CON  0x%x = 0x%x\n", SC_MD_INTF_CON , slp_read32(SC_MD_INTF_CON));
	slp_xverb("SC_MD_INTF_STS  0x%x = 0x%x\n", SC_MD_INTF_STS , slp_read32(SC_MD_INTF_STS));
	slp_xverb("SC_TMR_PWR      0x%x = 0x%x\n", SC_TMR_PWR     , slp_read32(SC_TMR_PWR));
	slp_xverb("SC_PERI_CON     0x%x = 0x%x\n", SC_PERI_CON    , slp_read32(SC_PERI_CON));
	slp_xverb("SC_PWR_STA      0x%x = 0x%x\n", SC_PWR_STA     , slp_read32(SC_PWR_STA));
	slp_xverb("SC_APMCU_PWRCTL 0x%x = 0x%x\n", SC_APMCU_PWRCTL, slp_read32(SC_APMCU_PWRCTL));
	slp_xverb("SC_AP_DVFS_CON  0x%x = 0x%x\n", SC_AP_DVFS_CON , slp_read32(SC_AP_DVFS_CON));
	slp_xverb("SC_FINAL_PAUSE  0x%x = 0x%x\n", SC_FINAL_PAUSE , slp_read32(SC_FINAL_PAUSE));
}

static int slp_suspend_ops_valid(suspend_state_t pm_state)
{
	return pm_state == PM_SUSPEND_MEM;
}

static int slp_suspend_ops_begin(suspend_state_t state)
{
	/* legacy log */
	slp_xinfo("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo("_Chip_pm_begin (%d) @@@@@@@@@@@@@@@@@@\n", slp_pwr_level);
	slp_xinfo(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	slp_wake_reason = WR_NONE;

	return 0;
}

static int slp_suspend_ops_prepare(void)
{
	unsigned long flags;

	/* legacy log */
	slp_xinfo("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo("_Chip_pm_prepare @@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	if (slp_dump_pmic)
		pmic_status_for_sleep();

#ifdef MTK_FD_SUPPORT
	/* let CCCI notify MD to disconnect data connection */
	spin_lock_irqsave(&slp_cc_lock, flags);
	if (slp_ccci_callback)
		slp_ccci_callback();
	spin_unlock_irqrestore(&slp_cc_lock, flags);
#endif

	return 0;
}

static int slp_suspend_ops_enter(suspend_state_t state)
{
	u32 topmisc, pdn0;

	/* legacy log */
	slp_xinfo("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo("_Chip_pm_enter @@@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	if (slp_dump_gpio)
		gpio_dump_regs();

	pdn0 = slp_read32(PERI_GLOBALCON_PDN0);
	if (get_chip_eco_ver() == CHIP_E1) {
		/* power on UART0/1/2/3 to workaround handshake mode issue */
		slp_write32(PERI_GLOBALCON_PDN0, pdn0 & ~(0xf << 24));
	}

	/* keep CA9 clock frequency when WFI to sleep */
	topmisc = slp_read32(TOP_MISC);
	slp_write32(TOP_MISC, topmisc & ~(1U << 0));
	slp_write_sync();

	if (slp_dump_regs)
		slp_dump_pm_regs();

	rtc_disable_writeif();

	slp_wake_reason = sc_go_to_sleep(slp_pwr_level);

	rtc_enable_writeif();

	/* restore TOP_MISC (and PERI_GLOBALCON_PDN0) */
	slp_write32(TOP_MISC, topmisc);
	if (get_chip_eco_ver() == CHIP_E1)
		slp_write32(PERI_GLOBALCON_PDN0, pdn0);
	slp_write_sync();

	return 0;
}

static void slp_suspend_ops_finish(void)
{
	/* legacy log */
	slp_xinfo("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo("_Chip_pm_finish @@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	/* debug help */
	slp_xinfo("Battery_Voltage = %lu\n", BAT_Get_Battery_Voltage(0));
}

static void slp_suspend_ops_end(void)
{
	/* legacy log */
	slp_xinfo("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo("_Chip_pm_end @@@@@@@@@@@@@@@@@@@@@@@@\n");
	slp_xinfo(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
}

static struct platform_suspend_ops slp_suspend_ops = {
	.valid		= slp_suspend_ops_valid,
	.begin		= slp_suspend_ops_begin,
	.prepare	= slp_suspend_ops_prepare,
	.enter		= slp_suspend_ops_enter,
	.finish		= slp_suspend_ops_finish,
	.end		= slp_suspend_ops_end,
};

#if 0
void slp_pmic_init(void)
{
	/* VPROC = 0.9V in sleep mode */
	upmu_buck_vosel_srclken_0(BUCK_VPROC, 0x08);
	upmu_buck_ctrl(BUCK_VPROC, 0x3);

	/* VCORE = 0.9V in sleep mode */
	upmu_buck_vosel(BUCK_VCORE, UPMU_VOLT_0_9_0_0_V);

	/* VM12_INT = 0.9V in sleep mode */
	pmic_config_interface(0x8b, 0x08, 0x1f, 0x0);
	pmic_config_interface(0x8f, 0x1, 0x1, 0x4);
}
#endif

void slp_mod_init(void)
{
	sc_mod_init();

	suspend_set_ops(&slp_suspend_ops);

	RM_CLK_SETTLE = (unsigned long)ioremap_nocache(0xd1090000, 4);
	if (!RM_CLK_SETTLE)
		slp_xerror("ioremap RM_CLK_SETTLE failed\n");

	RM_TMR_SSTA = (unsigned long)ioremap_nocache(0xd1090040, 4);
	if (!RM_TMR_SSTA)
		slp_xerror("ioremap RM_TMR_SSTA failed\n");
	proc_create_data("slp_md_sta", 0444, NULL, &slp_md_sta_fops, NULL);

	//console_suspend_enabled = 0;
}

module_param(slp_pwr_level, uint, 0644);

module_param(slp_dump_pmic, bool, 0644);
module_param(slp_dump_gpio, bool, 0644);
module_param(slp_dump_regs, bool, 0644);

MODULE_AUTHOR("Terry Chang <terry.chang@mediatek.com>");
MODULE_DESCRIPTION("MT6575 Sleep Driver v2.9");
