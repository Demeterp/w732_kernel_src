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

/* drivers/rtc/alarm-dev.c
 *
 * Copyright (C) 2007-2009 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <asm/mach/time.h>
#include <linux/android_alarm.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/sysdev.h>
#include <linux/uaccess.h>
#include <linux/wakelock.h>
#include <linux/xlog.h>

#define XLOG_MYTAG	"Power/Alarm"

#define ANDROID_ALARM_PRINT_INFO (1U << 0)
#define ANDROID_ALARM_PRINT_IO (1U << 1)
#define ANDROID_ALARM_PRINT_INT (1U << 2)
#define ANDROID_ALARM_PRINT_HIDE (1U << 3)

static int debug_mask = ANDROID_ALARM_PRINT_INFO | \
                        ANDROID_ALARM_PRINT_IO;
module_param_named(debug_mask, debug_mask, int, S_IRUGO | S_IWUSR | S_IWGRP);

#define pr_alarm(debug_level_mask, fmt, args...) \
	do { \
		if (debug_mask & ANDROID_ALARM_PRINT_##debug_level_mask) { \
			xlog_printk(ANDROID_LOG_INFO, XLOG_MYTAG, fmt, ##args); \
		} \
	} while (0)

#define ANDROID_ALARM_WAKEUP_MASK ( \
	ANDROID_ALARM_RTC_WAKEUP_MASK | \
	ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP_MASK)

/* support old usespace code */
#define ANDROID_ALARM_SET_OLD               _IOW('a', 2, time_t) /* set alarm */
#define ANDROID_ALARM_SET_AND_WAIT_OLD      _IOW('a', 3, time_t)

static int alarm_opened;
static DEFINE_SPINLOCK(alarm_slock);
static struct wake_lock alarm_wake_lock;
static DECLARE_WAIT_QUEUE_HEAD(alarm_wait_queue);
static uint32_t alarm_pending;
static uint32_t alarm_enabled;
static uint32_t wait_pending;

static struct alarm alarms[ANDROID_ALARM_TYPE_COUNT];

static long alarm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int rv = 0;
	unsigned long flags;
	struct timespec new_alarm_time;
	struct timespec new_rtc_time;
	struct timespec tmp_time;
	struct rtc_time prev_time;
	struct rtc_wkalrm pwron_alm;
	enum android_alarm_type alarm_type = ANDROID_ALARM_IOCTL_TO_TYPE(cmd);
	uint32_t alarm_type_mask = 1U << alarm_type;

	if (alarm_type >= ANDROID_ALARM_TYPE_COUNT &&
	    alarm_type != ANDROID_ALARM_POWER_ON &&
	    alarm_type != ANDROID_ALARM_POWER_ON_LOGO) {
		return -EINVAL;
	}

	if (ANDROID_ALARM_BASE_CMD(cmd) != ANDROID_ALARM_GET_TIME(0)) {
		if ((file->f_flags & O_ACCMODE) == O_RDONLY)
			return -EPERM;
		if (file->private_data == NULL &&
		    cmd != ANDROID_ALARM_SET_RTC) {
			spin_lock_irqsave(&alarm_slock, flags);
			if (alarm_opened) {
				spin_unlock_irqrestore(&alarm_slock, flags);
				return -EBUSY;
			}
			alarm_opened = 1;
			file->private_data = (void *)1;
			spin_unlock_irqrestore(&alarm_slock, flags);
		}
	}

	switch (ANDROID_ALARM_BASE_CMD(cmd)) {
	case ANDROID_ALARM_CLEAR(0):
		pr_alarm(IO, "alarm %d clear\n", alarm_type);
		if (alarm_type == ANDROID_ALARM_POWER_ON ||
		    alarm_type == ANDROID_ALARM_POWER_ON_LOGO) {
			new_alarm_time.tv_sec = 0;
			alarm_set_power_on(new_alarm_time, false);
			break;
		}
		spin_lock_irqsave(&alarm_slock, flags);
		alarm_try_to_cancel(&alarms[alarm_type]);
		if (alarm_pending) {
			alarm_pending &= ~alarm_type_mask;
			if (!alarm_pending && !wait_pending)
				wake_unlock(&alarm_wake_lock);
		}
		alarm_enabled &= ~alarm_type_mask;
		spin_unlock_irqrestore(&alarm_slock, flags);
		break;

	case ANDROID_ALARM_SET_OLD:
	case ANDROID_ALARM_SET_AND_WAIT_OLD:
		if (get_user(new_alarm_time.tv_sec, (int __user *)arg)) {
			rv = -EFAULT;
			goto err1;
		}
		new_alarm_time.tv_nsec = 0;
		goto from_old_alarm_set;

	case ANDROID_ALARM_SET_AND_WAIT(0):
	case ANDROID_ALARM_SET(0):
		if (copy_from_user(&new_alarm_time, (void __user *)arg,
		    sizeof(new_alarm_time))) {
			rv = -EFAULT;
			goto err1;
		}
from_old_alarm_set:
		pr_alarm(IO, "alarm %d set %ld.%09ld\n", alarm_type,
			new_alarm_time.tv_sec, new_alarm_time.tv_nsec);
		if (alarm_type == ANDROID_ALARM_POWER_ON) {
			alarm_set_power_on(new_alarm_time, false);
			break;
		}
		if (alarm_type == ANDROID_ALARM_POWER_ON_LOGO) {
			alarm_set_power_on(new_alarm_time, true);
			break;
		}
		spin_lock_irqsave(&alarm_slock, flags);
		alarm_enabled |= alarm_type_mask;
		alarm_start_range(&alarms[alarm_type],
			timespec_to_ktime(new_alarm_time),
			timespec_to_ktime(new_alarm_time));
		spin_unlock_irqrestore(&alarm_slock, flags);
		if (ANDROID_ALARM_BASE_CMD(cmd) != ANDROID_ALARM_SET_AND_WAIT(0)
		    && cmd != ANDROID_ALARM_SET_AND_WAIT_OLD)
			break;
		/* fall through */
	case ANDROID_ALARM_WAIT:
		spin_lock_irqsave(&alarm_slock, flags);
		pr_alarm(IO, "alarm wait (0x%x)\n", alarm_pending);
		if (!alarm_pending && wait_pending) {
			wake_unlock(&alarm_wake_lock);
			wait_pending = 0;
		}
		spin_unlock_irqrestore(&alarm_slock, flags);
		rv = wait_event_interruptible(alarm_wait_queue, alarm_pending);
		if (rv)
			goto err1;
		spin_lock_irqsave(&alarm_slock, flags);
		rv = alarm_pending;
		wait_pending = 1;
		alarm_pending = 0;
		spin_unlock_irqrestore(&alarm_slock, flags);
		break;
	case ANDROID_ALARM_SET_RTC:
		if (copy_from_user(&new_rtc_time, (void __user *)arg,
		    sizeof(new_rtc_time))) {
			rv = -EFAULT;
			goto err1;
		}
		rv = alarm_set_rtc(new_rtc_time, &prev_time);
		spin_lock_irqsave(&alarm_slock, flags);
		alarm_pending |= ANDROID_ALARM_TIME_CHANGE_MASK;
		wake_up(&alarm_wait_queue);
		spin_unlock_irqrestore(&alarm_slock, flags);
		if (rv < 0)
			goto err1;
		rtc_tm_to_time(&prev_time, &tmp_time.tv_sec);
		tmp_time.tv_nsec = 0;
		if (copy_to_user((void __user *)arg, &tmp_time,
		    sizeof(struct timespec))) {
			rv = 0;		/* ignore the error */
		}
		break;
	case ANDROID_ALARM_GET_TIME(0):
		switch (alarm_type) {
		case ANDROID_ALARM_RTC_WAKEUP:
		case ANDROID_ALARM_RTC:
			getnstimeofday(&tmp_time);
			break;
		case ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP:
		case ANDROID_ALARM_ELAPSED_REALTIME:
			tmp_time =
				ktime_to_timespec(alarm_get_elapsed_realtime());
			break;
		case ANDROID_ALARM_SYSTEMTIME:
		default:
			ktime_get_ts(&tmp_time);
		}
		if (copy_to_user((void __user *)arg, &tmp_time,
		    sizeof(tmp_time))) {
			rv = -EFAULT;
			goto err1;
		}
		break;
	case ANDROID_ALARM_GET_POWER_ON:
		alarm_get_power_on(&pwron_alm);
		if (copy_to_user((void __user *)arg, &pwron_alm,
		    sizeof(struct rtc_wkalrm))) {
			rv = -EFAULT;
			goto err1;
		}
		break;

	default:
		rv = -EINVAL;
		goto err1;
	}
err1:
	return rv;
}

static int alarm_open(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	pr_alarm(INFO, "alarm_open (%d:%d)\n", current->tgid, current->pid);
	return 0;
}

static int alarm_release(struct inode *inode, struct file *file)
{
	int i;
	unsigned long flags;

	spin_lock_irqsave(&alarm_slock, flags);
	if (file->private_data != 0) {
		for (i = 0; i < ANDROID_ALARM_TYPE_COUNT; i++) {
			uint32_t alarm_type_mask = 1U << i;
			if (alarm_enabled & alarm_type_mask) {
				pr_alarm(INFO, "alarm_release: clear alarm %d, "
					"pending %d\n", i,
					!!(alarm_pending & alarm_type_mask));
				alarm_enabled &= ~alarm_type_mask;
			}
			spin_unlock_irqrestore(&alarm_slock, flags);
			alarm_cancel(&alarms[i]);
			spin_lock_irqsave(&alarm_slock, flags);
		}
		if (alarm_pending | wait_pending) {
			if (alarm_pending)
				pr_alarm(INFO, "alarm_release: clear "
					"pending alarms 0x%x\n", alarm_pending);
			wake_unlock(&alarm_wake_lock);
			wait_pending = 0;
			alarm_pending = 0;
		}
		alarm_opened = 0;
	}
	spin_unlock_irqrestore(&alarm_slock, flags);
	pr_alarm(INFO, "alarm_release (%d:%d)(%d)\n",
	         current->tgid, current->pid, (int)file->private_data);
	return 0;
}

static void alarm_triggered(struct alarm *alarm)
{
	unsigned long flags;
	uint32_t alarm_type_mask = 1U << alarm->type;

	pr_alarm(INT, "alarm_triggered type %d\n", alarm->type);
	spin_lock_irqsave(&alarm_slock, flags);
	if (alarm_enabled & alarm_type_mask) {
		wake_lock_timeout(&alarm_wake_lock, 5 * HZ);
		alarm_enabled &= ~alarm_type_mask;
		alarm_pending |= alarm_type_mask;
		wake_up(&alarm_wait_queue);
	}
	spin_unlock_irqrestore(&alarm_slock, flags);
}

static const struct file_operations alarm_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = alarm_ioctl,
	.open = alarm_open,
	.release = alarm_release,
};

static struct miscdevice alarm_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "alarm",
	.fops = &alarm_fops,
};

static int __init alarm_dev_init(void)
{
	int err;
	int i;

	err = misc_register(&alarm_device);
	if (err)
		return err;

	for (i = 0; i < ANDROID_ALARM_TYPE_COUNT; i++)
		alarm_init(&alarms[i], i, alarm_triggered);
	wake_lock_init(&alarm_wake_lock, WAKE_LOCK_SUSPEND, "alarm");

	return 0;
}

static void  __exit alarm_dev_exit(void)
{
	misc_deregister(&alarm_device);
	wake_lock_destroy(&alarm_wake_lock);
}

module_init(alarm_dev_init);
module_exit(alarm_dev_exit);

MODULE_DESCRIPTION("Alarm Device Driver v1.5");
