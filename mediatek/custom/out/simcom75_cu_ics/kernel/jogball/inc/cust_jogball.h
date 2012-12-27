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

#ifndef __CUST_JOGBALL_H__
#define __CUST_JOGBALL_H__
/*----------------------------------------------------------------------------*/
typedef enum {
    JBD_DETECT_POLL     = 0,
    JBD_DETECT_EINT     = 1,            
} JBD_DETECT;
/*----------------------------------------------------------------------------*/
typedef enum {
    JBD_CLASS_KEYBOARD,
    JBD_CLASS_TRACKBALL, 
} JBD_CLASS;
/*----------------------------------------------------------------------------*/
#define C_JBD_RESOLUTION    (22)    /*11 pulses/360 => 22 changes/360*/
/*----------------------------------------------------------------------------*/
struct jogball_hw {
    int report_cls; /*refer to JBD_CLASS*/

    /*trackball class*/
    int gain_x;     /*x-axis gain*/
    int gain_y;     /*y-axis gain*/
    
    /*keyboard class*/
    int detect;     /*refer to JBD_DETECT*/
    int delay;      /*the period of scan timer for reporting key event*/
    int gpt_num;    /*the gpt number used for checking jogball status periodically*/
    int gpt_period; /*the sample period for checking jogball status*/
    
    int acc_cnt;    /*the maximum period of triggered event in one direction*/
    int inact_cnt;  /*the maximum period of two consecutive motion event in one direction*/
    int act_cnt;    /*the minimum period of two consecutive motion event in one direction*/
    int step;       /*the minimum count of triggering key event*/
};
/*----------------------------------------------------------------------------*/
extern struct jogball_hw *get_cust_jogball_hw(void);
/*----------------------------------------------------------------------------*/
#endif 
