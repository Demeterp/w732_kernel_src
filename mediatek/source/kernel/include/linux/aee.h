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

#if !defined(__AEE_H__)
#define __AEE_H__

#define AEE_MODULE_NAME_LENGTH 64
#define AEE_PROCESS_NAME_LENGTH 256
#define AEE_BACKTRACE_LENGTH 4096

typedef enum {
    AE_DEFECT_FATAL,
    AE_DEFECT_EXCEPTION,
    AE_DEFECT_WARNING,
    AE_DEFECT_REMINDING,
    AE_DEFECT_ATTR_END
} AE_DEFECT_ATTR;

typedef enum {
    AE_KE = 0, /* Fatal Exception */
    AE_NE, 
    AE_JE, 
    AE_SWT,
    AE_EE, 
    AE_EXP_ERR_END,
    AE_ANR, /* Error or Warning or Defect */
    AE_RESMON,  
    AE_WRN_ERR_END,
    AE_MANUAL, /* Manual Raise */
    AE_EXP_CLASS_END,

    AE_KERNEL_PROBLEM_REPORT = 1000,
    AE_PROCESS_PROBLEM_REPORT,
} AE_EXP_CLASS; /* General Program Exception Class */

struct aee_oops
{
	AE_DEFECT_ATTR attr;
	AE_EXP_CLASS clazz;

	char module[AEE_MODULE_NAME_LENGTH];
	char process_path[AEE_PROCESS_NAME_LENGTH];
	char backtrace[AEE_BACKTRACE_LENGTH];

	char *detail;
	int detail_len;

	char *console;
	int console_len;

	char *android_main;
	int android_main_len;
	char *android_radio;
	int android_radio_len;
	char *android_system;
	int android_system_len;

	char *userspace_info;
	int  userspace_info_len;
};

struct aee_kernel_api {
    void (*kernel_exception)(const char *module, const char *msg);
    void (*kernel_warning)(const char *module, const char *msg);
    void (*kernel_reminding)(const char *module, const char *msg);
    void (*md_exception1)(const int *log, int log_size, const int *phy, int phy_size, 
        const char *assert_type, const char *exp_filename, unsigned int exp_linenum, 
        unsigned int fatal1, unsigned int fatal2);
    void (*md_exception2)(const int *log, int log_size, const int *phy, int phy_size, 
        const char *detail);
};

void aee_trigger_kdb(void);
struct aee_oops *aee_oops_create(AE_DEFECT_ATTR attr, AE_EXP_CLASS clazz, const char *module);
void aee_oops_set_backtrace(struct aee_oops *oops, const char *backtrace);
void aee_oops_set_process_path(struct aee_oops *oops, const char *process_path);
void aee_oops_free(struct aee_oops *oops);

void aee_kernel_exception(const char *module, const char *msg, ...);
void aee_kernel_warning(const char *module, const char *msg, ...);
void aee_kernel_reminding(const char *module, const char *msg, ...);
void aed_md_exception1(const int *log, int log_size, const int *phy, int phy_size, const char *assert_type, const char *exp_filename, unsigned int exp_linenum, unsigned int fatal1, unsigned int fatal2);

#if defined(CONFIG_MTK_AEE_IPANIC)
/* Begin starting panic record */
void ipanic_oops_start(const char *str, int err, struct task_struct *task);
void ipanic_oops_end(void);
#else
#define ipanic_oops_start(str, err, task)
#define ipanic_oops_end()
#endif

#endif // __AEE_H__
