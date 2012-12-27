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

#if !defined(__AEE_IPANIC_H__)
#define __AEE_IPANIC_H__

#include <linux/autoconf.h>
#include <linux/kallsyms.h>
#include <linux/xlog.h>

#define AEE_IPANIC_PLABEL "expdb"

#define IPANIC_MODULE_TAG "KERNEL-PANIC"

#define IPANIC_LOG_TAG "aee/ipanic"

#define IPANIC_OOPS_BLOCK_COUNT 320

#define AEE_IPANIC_MAGIC 0xaee0dead
#define AEE_IPANIC_PHDR_VERSION   0x04
#define AEE_IPANIC_DATALENGTH_MAX (512 * 1024)

struct ipanic_header {
	/* The magic/version field cannot be moved or resize */
	u32 magic;
	u32 version;

	u32 oops_header_offset;
	u32 oops_header_length;

	u32 oops_detail_offset;
	u32 oops_detail_length;

	u32 console_offset;
	u32 console_length;

	u32 android_main_offset;
	u32 android_main_length;
	
	u32 android_event_offset;
	u32 android_event_length;
	
	u32 android_radio_offset;
	u32 android_radio_length;
	
	u32 android_system_offset;
	u32 android_system_length;

	u32 userspace_info_offset;
	u32 userspace_info_length;
};

#define IPANIC_OOPS_HEADER_PROCESS_NAME_LENGTH 256
#define IPANIC_OOPS_HEADER_BACKTRACE_LENGTH 3840

struct ipanic_oops_header 
{
	char process_path[IPANIC_OOPS_HEADER_PROCESS_NAME_LENGTH];
	char backtrace[IPANIC_OOPS_HEADER_BACKTRACE_LENGTH];
};

struct ipanic_ops {

	struct aee_oops *(*oops_copy)(void);
  
	void (*oops_free)(struct aee_oops *oops, int erase);
};

void register_ipanic_ops(struct ipanic_ops *op);

extern int log_buf_copy2(char *dest, int dest_len, int log_copy_start, int log_copy_end);

struct aee_oops *ipanic_oops_copy(void);

void ipanic_oops_free(struct aee_oops *oops, int erase);

extern unsigned log_start;

extern unsigned log_end;

extern unsigned ipanic_detail_start;

extern unsigned ipanic_detail_end;

extern struct ipanic_oops_header oops_header;

/*
 * Check if valid header is legitimate
 * return
 *  0: contain good panic data 
 *  1: no panic data
 *  2: contain bad panic data
 */
int ipanic_header_check(const struct ipanic_header *hdr);

void ipanic_header_dump(const struct ipanic_header *header);

void ipanic_block_scramble(u8 *buf, int buflen);

void ipanic_oops_start(const char *str, int err, struct task_struct *tsk);

void ipanic_oops_end(void);

/* User space process support functions */

#define MAX_NATIVEINFO  32*1024
#define MAX_NATIVEHEAP  2048

extern char NativeInfo[MAX_NATIVEINFO]; //check that 32k is enought??

extern unsigned long User_Stack[MAX_NATIVEHEAP];//8K Heap

int DumpNativeInfo(void);

/* External ipanic support functions */

int card_dump_func_read(unsigned char* buf, unsigned int len, unsigned int offset, unsigned int dev);

int card_dump_func_write(unsigned char* buf, unsigned int len, unsigned int offset, unsigned int dev);

int panic_dump_android_log(char *buf, size_t size, int type);

#endif
