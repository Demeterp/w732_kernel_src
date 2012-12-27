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

#ifndef __CUST_OFN_H__
#define __CUST_OFN_H__

#include <mach/mt6575_reg_base.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_gpio.h>
#include <mach/mt6575_pm_ldo.h>

/*----------------------------------------------------------------------------*/
typedef enum {
    OFN1026 = 1026,
    OFN1086 = 1086,
    OFN1090 = 1090,
}OFN_ID;
/*----------------------------------------------------------------------------*/
typedef enum {
    OFN_DETECT_POLL     = 0,
    OFN_DETECT_EINT     = 1,            
} OFN_DETECT;
/*----------------------------------------------------------------------------*/
typedef enum {
    OFN_CLASS_KEYBOARD,
    OFN_CLASS_TRACKBALL, 
} OFN_CLASS;
/*----------------------------------------------------------------------------*/
struct ofn_hw {
    int             power_id;   /*!< LDO usage */
    int             power_vol;  /*!< LDO usage */

    int             report_cls; /*!< refer to OFN_CLASS*/
    OFN_ID          chip_id;    /*!< different chip id will different power on sequence*/
    int             slave_addr; /*!< the slave address will be changed according to layout*/   
    int             i2c_num;    /*!< the number of used i2c bus*/
    unsigned int    layout;     /*!< Three bits: (XY_SWAP, Y_INV, X_INV)*/

    /*trackball class*/
    int             quan_x;     /*!< quantized step in x-axis*/
    int             quan_y;     /*!< quantized step in y-axis*/
    int             accu_max;   /*!< the maximum step accumulated*/

    /*keyboard class*/    
    int             detect;     /*!< 0:poll; 1: detect*/        
    int             gpt_num;    /*!< The GPT number used in polling mode*/
    int             gpt_period; /*!< the GPT period used in polling mode*/
    
    int             acc_cnt;    /*!< the maximum period of triggered event in one direction*/
    int             inact_cnt;  /*!< the maximum period of two consecutive motion event in one direction*/
    int             act_cnt;    /*!< the minimum period of two consecutive motion event in one direction*/
    int             sup_cnt;    /*!< supress level: any value report in single motion event larger than this value will be set as this value*/
    int             step;       /*!< the minimum count of triggering key event*/
};
/*----------------------------------------------------------------------------*/
extern struct ofn_hw *get_cust_ofn_hw(void);
/*----------------------------------------------------------------------------*/
#endif
