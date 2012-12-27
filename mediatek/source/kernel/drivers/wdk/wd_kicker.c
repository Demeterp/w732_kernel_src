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

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>
#include <linux/rtpm_prio.h>
#include <linux/rtc.h>
#include "wd_kicker.h"

/*************************************************************************
 * Feature configure region 
 *************************************************************************/
#define __ENABLE_WDT_SYSFS__
#define __ENABLE_WDT_AT_INIT__

//------------------------------------------------------------------------
#define PFX "wdk: "
#define DEBUG_WDK	0
#if DEBUG_WDK
#define dbgmsg(msg...) printk(PFX msg)
#else
#define dbgmsg(...)
#endif

#define msg(msg...) printk(KERN_INFO PFX msg);
#define warnmsg(msg...) printk(KERN_WARNING PFX msg);
#define errmsg(msg...) printk(KERN_WARNING PFX msg);

#define MIN_KICK_INTERVAL	 1
#define MAX_KICK_INTERVAL	30
#define PROC_WK "wdk"


static int kwdt_thread(void *arg);
static int start_kicker(void);

static int debug_sleep = 0;
static int data;
//static spinlock_t lock = SPIN_LOCK_UNLOCKED;
static DEFINE_SPINLOCK(lock);

//Monkey.QHQ
//static struct task_struct *wk_tsk;
struct task_struct *wk_tsk;
//Monkey.QHQ

enum wk_wdt_mode g_wk_wdt_mode = WK_WDT_NORMAL_MODE;
static struct wk_wdt *g_wk_wdt = NULL;
static int g_kinterval = -1;
static int g_timeout = -1;
static int g_need_config = 0;
static int wdt_start = 0;

static char cmd_buf[256];

static int wk_proc_cmd_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int len;
	len = snprintf(buf, count, "mode interval timeout \n%-4d  %-9d %-7d\n", g_wk_wdt_mode, g_kinterval, g_timeout);
	return len;
}

static int wk_proc_cmd_write(struct file *file, const char *buf, unsigned long count, void *data)
{
	int ret;
	//static int wdt_start = 0; // Adjust this variable to file view scope 
	int timeout;
	int mode;
	int kinterval;

	if (count == 0)
		return -1;

	if(count > 255) 
		count = 255;

	ret = copy_from_user(cmd_buf, buf, count);
	if (ret < 0)
		return -1;
	
	cmd_buf[count] = '\0';

	dbgmsg("Write %s\n", cmd_buf);

	sscanf(cmd_buf, "%d %d %d %d", &mode, &kinterval, &timeout, &debug_sleep);

	printk("[WDK] mode=%d interval=%d timeout=%d\n", mode, kinterval, timeout);

	if (timeout < kinterval) {
		errmsg("The interval(%d) value should be smaller than timeout value(%d)\n", kinterval, timeout);
		return -1;
	}

	if ((timeout <  MIN_KICK_INTERVAL) || (timeout > MAX_KICK_INTERVAL)) {
		errmsg("The timeout(%d) is invalid (%d - %d)\n", kinterval, MIN_KICK_INTERVAL, MAX_KICK_INTERVAL);
		return -1;
	}

	if ((kinterval <  MIN_KICK_INTERVAL) || (kinterval > MAX_KICK_INTERVAL)) {
		errmsg("The interval(%d) is invalid (%d - %d)\n",kinterval,  MIN_KICK_INTERVAL, MAX_KICK_INTERVAL);
		return -1;
	}

	if (!((mode == WK_WDT_NORMAL_MODE) || 
	      (mode == WK_WDT_EXP_MODE))) {
		errmsg("Tha watchdog kicker wdt mode is not correct\n");
		errmsg("WK_WDT_NORMAL_MODE = %d\n", WK_WDT_NORMAL_MODE);
		errmsg("WK_WDT_EXP_MODE = %d\n", WK_WDT_EXP_MODE);
		return -1;
	}
		
		
    	spin_lock(&lock);

	g_kinterval = kinterval;
#if 0 //CONFIG_MTK_AEE_FEATURE auto enable or disable WDT isr or hw mode
	g_wk_wdt_mode = WK_WDT_EXP_MODE;
	msg("Enable wdt with interrupt mode only %d\n", g_wk_wdt_mode);
#else
	g_wk_wdt_mode = mode;
#endif
	g_timeout = timeout;
        g_need_config = 1;

	/* Start once only */
	if (wdt_start == 0) {
		start_kicker();
	}

	wdt_start = 1;

    	spin_unlock(&lock);
	return count;
}

static int start_kicker_thread_with_default_setting(void)
{
	int ret = 0;

	spin_lock(&lock);

	g_kinterval = 20;  // default interval: 20s

	g_need_config = 0; // Note, we DO NOT want to call configure function

	wdt_start = 1;     // Start once only
	start_kicker();

    	spin_unlock(&lock);

	return ret;
}

int wk_proc_init(void) {

	struct proc_dir_entry *de = create_proc_entry(PROC_WK, 0667, 0);

	printk("[WDK] Initialize proc\n");

	de->read_proc = wk_proc_cmd_read;
	de->write_proc = wk_proc_cmd_write;

	return 0 ;
}


void wk_proc_exit(void) 
{

	remove_proc_entry(PROC_WK, NULL);

}

int wk_register_wdt(struct wk_wdt *wk_wdt)
{
	if (!wk_wdt) {
		BUG();
	}

	g_wk_wdt = wk_wdt;

	return 0;
}
EXPORT_SYMBOL(wk_register_wdt);

static int kwdt_thread(void *arg)
{
	
	struct sched_param param = { .sched_priority = RTPM_PRIO_WDT};
	struct rtc_time tm;
	struct timeval tv = {0};
	/* android time */
	struct rtc_time tm_android;
	struct timeval tv_android = {0};

        sched_setscheduler(current, SCHED_FIFO, &param);

        set_current_state(TASK_INTERRUPTIBLE);

	printk("[WDK] WDT kicker thread start\n");

	for(;;)
	{
		if (kthread_should_stop()) break;

		//msleep(g_kinterval * 1000);

		if (g_wk_wdt && g_wk_wdt->kick_wdt && g_wk_wdt->config) {
			
			if (g_need_config) {
				spin_lock(&lock);
				g_wk_wdt->config(g_wk_wdt_mode, g_timeout);
				g_need_config = 0;
				spin_unlock(&lock);
			}
			g_wk_wdt->kick_wdt();
		}
		else {
			errmsg("No watch dog driver is hooked\n");
			BUG();
		}

		msleep(g_kinterval * 1000);

		#if (DEBUG_WDK==1)
		msleep(debug_sleep * 1000);
		dbgmsg("WD kicker woke up %d\n", debug_sleep);
		#endif
		do_gettimeofday(&tv);
		tv_android = tv;
		rtc_time_to_tm(tv.tv_sec, &tm);
		tv_android.tv_sec -= sys_tz.tz_minuteswest*60;
		rtc_time_to_tm(tv_android.tv_sec, &tm_android);
		printk("%d-%02d-%02d %02d:%02d:%02d.%u UTC; android time %d-%02d-%02d %02d:%02d:%02d.%03d\n",
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec,
		(unsigned int) tv.tv_usec,
		tm_android.tm_year + 1900, tm_android.tm_mon + 1, tm_android.tm_mday,
		tm_android.tm_hour, tm_android.tm_min, tm_android.tm_sec,
		(unsigned int) tv_android.tv_usec	);


	}
	printk("[WDK] WDT kicker thread stop\n");
	return 0;
}	

static int start_kicker(void)
{
	spin_unlock(&lock);
	wk_tsk = kthread_create(kwdt_thread, &data, "wdtk");
	spin_lock(&lock);
	if (IS_ERR(wk_tsk)) {
		int ret = PTR_ERR(wk_tsk);
		wk_tsk = NULL;
		return ret;
	}
	wake_up_process(wk_tsk);

	return 0;
}

/******************************************************************************
 * SYSFS support
******************************************************************************/
#ifdef __ENABLE_WDT_SYSFS__
/*---------------------------------------------------------------------------*/
/* Externanl functions */
extern void mtk_wdt_disable(void);
extern int mtk_wdt_get_en_setting(void);
/*---------------------------------------------------------------------------*/
/*define sysfs entry for configuring debug level and sysrq*/
ssize_t mtk_rgu_attr_show(struct kobject *kobj, struct attribute *attr, char *buffer);
ssize_t mtk_rgu_attr_store(struct kobject *kobj, struct attribute *attr, const char *buffer, size_t size);
ssize_t mtk_rgu_pause_wdt_show(struct kobject *kobj, char *page);
ssize_t mtk_rgu_pause_wdt_store(struct kobject *kobj, const char *page, size_t size);
/*---------------------------------------------------------------------------*/
struct sysfs_ops mtk_rgu_sysfs_ops = {
	.show   = mtk_rgu_attr_show,
	.store  = mtk_rgu_attr_store,
};
/*---------------------------------------------------------------------------*/
struct mtk_rgu_sys_entry {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj, char *page);
	ssize_t (*store)(struct kobject *kobj, const char *page, size_t size);
};
/*---------------------------------------------------------------------------*/
static struct mtk_rgu_sys_entry pause_wdt_entry = {
	{ .name = "pause", .mode = S_IRUGO | S_IWUSR }, // remove  .owner = NULL,  
	mtk_rgu_pause_wdt_show,
	mtk_rgu_pause_wdt_store,
};
/*---------------------------------------------------------------------------*/
struct attribute *mtk_rgu_attributes[] = {
	&pause_wdt_entry.attr,
	NULL,
};
/*---------------------------------------------------------------------------*/
struct kobj_type mtk_rgu_ktype = {
	.sysfs_ops = &mtk_rgu_sysfs_ops,
	.default_attrs = mtk_rgu_attributes,
};
/*---------------------------------------------------------------------------*/
static struct mtk_rgu_sysobj {
	struct kobject kobj;
} rgu_sysobj;
/*---------------------------------------------------------------------------*/
int mtk_rgu_sysfs(void) 
{
	struct mtk_rgu_sysobj *obj = &rgu_sysobj;

	memset(&obj->kobj, 0x00, sizeof(obj->kobj));
    
	obj->kobj.parent = kernel_kobj;
	if (kobject_init_and_add(&obj->kobj, &mtk_rgu_ktype, NULL, "mtk_rgu")) {
		kobject_put(&obj->kobj);
		return -ENOMEM;
	}
	kobject_uevent(&obj->kobj, KOBJ_ADD);

	return 0;
}
/*---------------------------------------------------------------------------*/
ssize_t mtk_rgu_attr_show(struct kobject *kobj, struct attribute *attr, char *buffer) 
{
	struct mtk_rgu_sys_entry *entry = container_of(attr, struct mtk_rgu_sys_entry, attr);
	return entry->show(kobj, buffer);
}
/*---------------------------------------------------------------------------*/
ssize_t mtk_rgu_attr_store(struct kobject *kobj, struct attribute *attr, const char *buffer, size_t size) 
{
	struct mtk_rgu_sys_entry *entry = container_of(attr, struct mtk_rgu_sys_entry, attr);
	return entry->store(kobj, buffer, size);
}
/*---------------------------------------------------------------------------*/
ssize_t mtk_rgu_pause_wdt_show(struct kobject *kobj, char *buffer) 
{
	int remain = PAGE_SIZE;
	int len;
	char *ptr = buffer;

        len = scnprintf(ptr, remain, "0x%1x\n", !mtk_wdt_get_en_setting());
        ptr += len;
        remain -= len;

	return (PAGE_SIZE-remain);
}
/*---------------------------------------------------------------------------*/
ssize_t mtk_rgu_pause_wdt_store(struct kobject *kobj, const char *buffer, size_t size) 
{
	int pause_wdt;
	int res = sscanf(buffer, "%x", &pause_wdt);

	if (res != 1) {
		printk("%s: expect 1 numbers\n", __FUNCTION__);
	}else{
		// For real case, pause wdt if get value is not zero. Suspend and resume may enable wdt again
		if(pause_wdt)
			mtk_wdt_disable();
	}
	return size;
}
/*---------------------------------------------------------------------------*/
#endif /*__ENABLE_WDT_SYSFS__*/
/*---------------------------------------------------------------------------*/

static int __init init_wk(void)
{
	#ifdef __ENABLE_WDT_SYSFS__
	mtk_rgu_sysfs();
	#endif 

	#ifdef __ENABLE_WDT_AT_INIT__
	start_kicker_thread_with_default_setting();
	#endif

	wk_proc_init();

	return 0;
}

static void __exit exit_wk(void)
{
	wk_proc_exit();
	kthread_stop(wk_tsk);
}

late_initcall(init_wk);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mediatek inc.");
MODULE_DESCRIPTION("The watchdog kicker");
