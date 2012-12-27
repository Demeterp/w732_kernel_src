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

/* include/linux/android_alarm.h
 *
 * Copyright (C) 2006-2007 Google, Inc.
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

#ifndef _LINUX_ANDROID_ALARM_H
#define _LINUX_ANDROID_ALARM_H

#include <linux/ioctl.h>
#include <linux/time.h>
#include <linux/rtc.h>

enum android_alarm_type {
	/* return code bit numbers or set alarm arg */
	ANDROID_ALARM_RTC_WAKEUP,
	ANDROID_ALARM_RTC,
	ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP,
	ANDROID_ALARM_ELAPSED_REALTIME,
	ANDROID_ALARM_SYSTEMTIME,

	ANDROID_ALARM_TYPE_COUNT,

	ANDROID_ALARM_POWER_ON = 6,
	ANDROID_ALARM_POWER_ON_LOGO = 7,

	/* return code bit numbers */
	/* ANDROID_ALARM_TIME_CHANGE = 16 */
};

#ifdef __KERNEL__

#include <linux/ktime.h>
#include <linux/rbtree.h>
#include <linux/rtc.h>

/*
 * The alarm interface is similar to the hrtimer interface but adds support
 * for wakeup from suspend. It also adds an elapsed realtime clock that can
 * be used for periodic timers that need to keep runing while the system is
 * suspended and not be disrupted when the wall time is set.
 */

/**
 * struct alarm - the basic alarm structure
 * @node:	red black tree node for time ordered insertion
 * @type:	alarm type. rtc/elapsed-realtime/systemtime, wakeup/non-wakeup.
 * @softexpires: the absolute earliest expiry time of the alarm.
 * @expires:	the absolute expiry time.
 * @function:	alarm expiry callback function
 *
 * The alarm structure must be initialized by alarm_init()
 *
 */

struct alarm {
	struct rb_node 		node;
	enum android_alarm_type type;
	ktime_t			softexpires;
	ktime_t			expires;
	void			(*function)(struct alarm *);
};

void alarm_init(struct alarm *alarm,
	enum android_alarm_type type, void (*function)(struct alarm *));
void alarm_start_range(struct alarm *alarm, ktime_t start, ktime_t end);
int alarm_try_to_cancel(struct alarm *alarm);
int alarm_cancel(struct alarm *alarm);
ktime_t alarm_get_elapsed_realtime(void);

/* set rtc while preserving elapsed realtime */
int alarm_set_rtc(struct timespec new_time, struct rtc_time *prev_time);

void alarm_set_power_on(struct timespec new_pwron_time, bool logo);
void alarm_get_power_on(struct rtc_wkalrm *alm);

#endif

enum android_alarm_return_flags {
	ANDROID_ALARM_RTC_WAKEUP_MASK = 1U << ANDROID_ALARM_RTC_WAKEUP,
	ANDROID_ALARM_RTC_MASK = 1U << ANDROID_ALARM_RTC,
	ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP_MASK =
				1U << ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP,
	ANDROID_ALARM_ELAPSED_REALTIME_MASK =
				1U << ANDROID_ALARM_ELAPSED_REALTIME,
	ANDROID_ALARM_SYSTEMTIME_MASK = 1U << ANDROID_ALARM_SYSTEMTIME,
	ANDROID_ALARM_TIME_CHANGE_MASK = 1U << 16
};

/* Disable alarm */
#define ANDROID_ALARM_CLEAR(type)           _IO('a', 0 | ((type) << 4))

/* Ack last alarm and wait for next */
#define ANDROID_ALARM_WAIT                  _IO('a', 1)

#define ALARM_IOW(c, type, size)            _IOW('a', (c) | ((type) << 4), size)
/* Set alarm */
#define ANDROID_ALARM_SET(type)             ALARM_IOW(2, type, struct timespec)
#define ANDROID_ALARM_SET_AND_WAIT(type)    ALARM_IOW(3, type, struct timespec)
#define ANDROID_ALARM_GET_TIME(type)        ALARM_IOW(4, type, struct timespec)
#define ANDROID_ALARM_SET_RTC               _IOW('a', 5, struct timespec)
//#define ANDROID_ALARM_SET_TIMEZONE        _IOW('a', 6, struct timezone)
#define ANDROID_ALARM_GET_POWER_ON          _IOR('a', 7, struct rtc_wkalrm)
#define ANDROID_ALARM_BASE_CMD(cmd)         (cmd & ~(_IOC(0, 0, 0xf0, 0)))
#define ANDROID_ALARM_IOCTL_TO_TYPE(cmd)    (_IOC_NR(cmd) >> 4)

#endif
