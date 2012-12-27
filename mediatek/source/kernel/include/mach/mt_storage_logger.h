/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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

/*
 * Copyright (C) 2011 MediaTek, Inc.
 *
 * Author: Holmes Chiou <holmes.chiou@mediatek.com>
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

#ifndef __MT_STORAGE_LOGGER_H__
#define __MT_STORAGE_LOGGER_H__


#define SLTAG "[StorageLogger]"
#define SLog_MSG(fmt, args...) \
do {    \
		printk(KERN_INFO SLTAG""fmt" <- %s(): L<%d>  PID<%s><%d>\n", \
            	##args , __FUNCTION__, __LINE__, current->comm, current->pid); \
} while(0);

#define STrace_MSG(fmt, args...) \
do {    \
		printk(KERN_INFO SLTAG""fmt" in PID<%s><%d>\n", \
            	##args, current->comm, current->pid); \
} while(0);

struct _loggerFuncName
{
	int     FuncID;
	char* 	FuncName;
};

struct _loggerMsgFormat
{
	int		MsgID;
	char* 	MsgFmt;
	char* 	DispFmt;
};

enum Storage_Logger_MsgID
{
	STORAGE_LOGGER_MSG_FIRST_ONE = 0,
	STORAGE_LOGGER_MSG_ISSUE_RQ = STORAGE_LOGGER_MSG_FIRST_ONE,
	STORAGE_LOGGER_MSG_ISSUE_RQ_1,
	STORAGE_LOGGER_MSG_SUBMITBIO,
	STORAGE_LOGGER_MSG_VFS_SDCARD,
	STORAGE_LOGGER_MSG_VFS_SDCARD_END,
	STORAGE_LOGGER_MSG_VFS_OPEN_SDCARD,
	STORAGE_LOGGER_MSG_VFS_OPEN_SDCARD_END,
	STORAGE_LOGGER_MSG_VFS_INTFS,
	STORAGE_LOGGER_MSG_VFS_INTFS_END,
	STORAGE_LOGGER_MSG_VFS_OPEN_INTFS,
	STORAGE_LOGGER_MSG_VFS_OPEN_INTFS_END,	
	STORAGE_LOGGER_MSG_IOSCHED1,
	STORAGE_LOGGER_MSG_IOSCHED2,
	STORAGE_LOGGER_MSG_MSDC_DO,
	STORAGE_LOGGER_MSG_MSDC_DO_END,
	STORAGE_LOGGER_MSG_GET_REQUEST,
	STORAGE_LOGGER_MSG_GET_REQUEST_END,
	STORAGE_LOGGER_MSG_LAST_ONE, // MUST BE THE LAST STORAGE MSG ID

	USB_LOGGER_MSG_FIRST_ONE = STORAGE_LOGGER_MSG_LAST_ONE + 1,
	USB_LOGGER_MSG_MUSB_INTERRUPT = USB_LOGGER_MSG_FIRST_ONE,
	USB_LOGGER_MSG_MUSB_STAGE0_IRQ,
	USB_LOGGER_MSG_MUSB_G_EP0_IRQ,
	USB_LOGGER_MSG_TXSTATE,
	USB_LOGGER_MSG_TXSTATE_END,
	USB_LOGGER_MSG_MUSB_G_TX,
	USB_LOGGER_MSG_MUSB_G_RX,
	USB_LOGGER_MSG_MUSB_READ_SETUP,
	USB_LOGGER_MSG_FORWARD_TO_DRIVER,
	USB_LOGGER_MSG_COMPOSITE_SETUP,
	USB_LOGGER_MSG_USB_ADD_FUNCTION,
	USB_LOGGER_MSG_SET_CONFIG,
	USB_LOGGER_MSG_DEVICE_DESCRIPTOR,
	USB_LOGGER_MSG_INTERFACE_DESCRIPTOR,
	USB_LOGGER_MSG_ENDPOINT_DESCRIPTOR,
	USB_LOGGER_MSG_ANDROID_WORK,
	USB_LOGGER_MSG_GS_RX_PUSH,
	USB_LOGGER_MSG_GS_START_TX,
	USB_LOGGER_MSG_ACM_SETUP,
	USB_LOGGER_MSG_ACM_SET_ALT,
	USB_LOGGER_MSG_ACM_BIND,
	USB_LOGGER_MSG_GS_OPEN,
	USB_LOGGER_MSG_GS_CLOSE,
	USB_LOGGER_MSG_ACM_CDC_LINE_CODING,
	USB_LOGGER_MSG_STRING,
	USB_LOGGER_MSG_HEX_NUM,
	USB_LOGGER_MSG_DEC_NUM,
	USB_LOGGER_MSG_LAST_ONE, // MUST BE THE LAST USB MSG ID
	LOGGER_MSG_ID_MAX = USB_LOGGER_MSG_LAST_ONE// THE WHOLE ENUM LAST ONE
};

/*========STORAGE PART========*/
enum Storage_LogAPI
{
	STORAGE_LOG_API___submit_bio__func = 0,
	STORAGE_LOG_API___mmc_blk_issue_rq__func,
	STORAGE_LOG_API___do_sys_open__func,
	STORAGE_LOG_API___vfs_read__func,
	STORAGE_LOG_API___vfs_write__func,
	STORAGE_LOG_API___msdc_do_request__func,
	STORAGE_LOG_API___msdc_ops_request__func,
	STORAGE_LOG_API___io_schedule__func,
	STORAGE_LOG_API___get_request_wait__func,
	STORAGE_LOG_API___get_request__func,
	STORAGE_LOG_API___make_request__func,
	STORAGE_LOG_API___MAX__func
};

/*========USB PART========*/
enum usb_func_string_index
{
	USB_FUNC_STRING_INDEX_MUSB_INTERRUPT = 0,
	USB_FUNC_STRING_INDEX_MUSB_STAGE0_IRQ,
	USB_FUNC_STRING_INDEX_MUSB_G_EP0_IRQ,
	USB_FUNC_STRING_INDEX_TXSTATE,
	USB_FUNC_STRING_INDEX_MUSB_G_TX,
	USB_FUNC_STRING_INDEX_MUSB_G_RX,
	USB_FUNC_STRING_INDEX_MUSB_READ_SETUP,
	USB_FUNC_STRING_INDEX_FORWARD_TO_DRIVER,
	USB_FUNC_STRING_INDEX_COMPOSITE_SETUP,
	USB_FUNC_STRING_INDEX_USB_ADD_FUNCTION,
	USB_FUNC_STRING_INDEX_SET_CONFIG,
	USB_FUNC_STRING_INDEX_CONFIG_BUF,
	USB_FUNC_STRING_INDEX_USB_DESCRIPTOR_FILLBUF,
	USB_FUNC_STRING_INDEX_ANDROID_WORK,
	USB_FUNC_STRING_INDEX_GS_RX_PUSH,
	USB_FUNC_STRING_INDEX_GS_START_TX,
	USB_FUNC_STRING_INDEX_ACM_SETUP,
	USB_FUNC_STRING_INDEX_ACM_SET_ALT,
	USB_FUNC_STRING_INDEX_ACM_BIND,
	USB_FUNC_STRING_INDEX_GS_OPEN,
	USB_FUNC_STRING_INDEX_GS_CLOSE,
	USB_FUNC_STRING_INDEX_ACM_COMPLETE_SET_LINE,
	USB_FUNC_STRING_INDEX_ADB_READ,
	USB_FUNC_STRING_INDEX_ADB_WRITE,
	USB_FUNC_STRING_INDEX_MAX
};
/*========================*/

enum logger_type
{
	LOGGER_TYPE_START = 0,
	LOGGER_TYPE_STORAGE = LOGGER_TYPE_START,
	LOGGER_TYPE_USB,
	LOGGER_TYPE_END
};

#ifndef USER_BUILD_KERNEL//engineering mode

#define CREATE_PROC_ENTRY(proc,x,y,z) proc = create_proc_entry(x,y,z)

#else

#define CREATE_PROC_ENTRY(proc,x,y,z)

#endif

extern void add_trace(enum logger_type type, unsigned int msg_id,
			unsigned int line_cnt, unsigned int func_id, ...);

/*========STORAGE PART========*/
extern bool dumpMMCqd(void);
extern bool dumpBlkLayer(void);
extern bool dumpVFS(void);
extern bool dumpMSDC(void);
extern bool ioschedule_dump(void);

#define AddStorageTrace(msg_id,name, ...) \
	add_trace(LOGGER_TYPE_STORAGE, msg_id, __LINE__, \
		STORAGE_LOG_API___##name##__func, __VA_ARGS__);
/*========USB PART========*/
extern bool is_dump_musb(void);
extern bool is_dump_usb_gadget(void);

#define ADD_USB_TRACE(func_id, name, ...) \
	add_trace(LOGGER_TYPE_USB, USB_LOGGER_MSG_##func_id, __LINE__, \
		USB_FUNC_STRING_INDEX_##name, __VA_ARGS__);
/*========================*/
#endif/* !__MT_STORAGE_LOGGER_H__ */
