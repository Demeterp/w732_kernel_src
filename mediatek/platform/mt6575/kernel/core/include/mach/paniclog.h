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

#ifndef __MT6575_PANICLOG_H__
#define __MT6575_PANICLOG_H__

#define PANICLOG_BUF_LEN 16384
#define PANICLOG_HEADER_SIZE 512
#define PANICLOG_SECTION_SIZE (PANICLOG_BUF_LEN) + (PANICLOG_HEADER_SIZE)

#ifndef __ASSEMBLY__

#define PANICLOG_VALID_PATTERN 0x90EFABCD
#define PANICLOG_BACKTRACE_NUM 4
#define PANICLOG_PROCESS_NAME_LENGTH 256

struct paniclog {
	int valid;		/* log is valid if valid == PANICLOG_VALID_PATTERN */
	int crc;

	struct {
		unsigned long bt_where[PANICLOG_BACKTRACE_NUM];
		unsigned long bt_from[PANICLOG_BACKTRACE_NUM];
		char process_path[PANICLOG_PROCESS_NAME_LENGTH];

		char buf[PANICLOG_BUF_LEN];
		int buf_len;
	} c;
};

extern struct paniclog *paniclog;

#ifdef CONFIG_PANICLOG

/* Begin starting panic record */
void paniclog_start(void);

void paniclog_end(void);

/* Record the task acenstor into current paniclog */
void paniclog_ptree_store(struct task_struct *tsk);

/* Record stack trace info into current paniclog */
void paniclog_stack_store(unsigned long where, unsigned long from);

/* Check if panic log available */
int paniclog_is_available(void);

/* Dump current paniclog to kernel log buffer */
void paniclog_dump(void);

/* Copy the current panic log and clear the panic log before return */
void paniclog_copy_and_clear(struct paniclog *log);

#else

#define paniclog_start(a)

#define  paniclog_end()

#define paniclog_ptree_store(tsk)

#define paniclog_stack_store(where, from)

#define paniclog_is_available() 0

#define paniclog_copy_and_clear(log)

#endif /* CONFIG_PANALOG_LOG */ 

#endif /* __ASSEMBLY */

#endif  /* !__MT6575_PANICLOG_H__ */

