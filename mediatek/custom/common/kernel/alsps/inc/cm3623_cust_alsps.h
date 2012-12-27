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

#ifndef __CUST_ALSPS_H__
#define __CUST_ALSPS_H__

#define C_CUST_ALS_LEVEL    16 //6//16
#define C_CUST_I2C_ADDR_NUM 4

//cm3623 fator=cm3623 lux/actual lux (<1)
//tmd2771 fator=actual lux/tmd2771 lux/ (>1)
#if 0 //amy0510 
#if defined(SIMCOM_I3000)
	#define C_CUST_ALS_FACTOR    142//0.142
#elif defined(SIMCOM_I5000)
	#define C_CUST_ALS_FACTOR    150//0.26
#elif defined(PHILIPS_ATLAS)
	#define C_CUST_ALS_FACTOR    3300  //3.3   1.94
#else
	#define C_CUST_ALS_FACTOR    1000  //1
#endif
#else

#endif

struct alsps_hw {
    int i2c_num;                                    /*!< the i2c bus used by ALS/PS */
    int power_id;                                   /*!< the power id of the chip */
    int power_vol;                                  /*!< the power voltage of the chip */
	int polling_mode;                               /*!< 1: polling mode ; 0:interrupt mode*/
	int polling_mode_ps;                               /*!< 1: polling mode ; 0:interrupt mode*/
	int polling_mode_als;                               /*!< 1: polling mode ; 0:interrupt mode*/
    unsigned char   i2c_addr[C_CUST_I2C_ADDR_NUM];  /*!< i2c address list, some chip will have multiple address */
    unsigned int    als_level[C_CUST_ALS_LEVEL-1];  /*!< (C_CUST_ALS_LEVEL-1) levels divides all range into C_CUST_ALS_LEVEL levels*/
    unsigned int    als_value[C_CUST_ALS_LEVEL];    /*!< the value reported in each level */
    unsigned int    ps_threshold;                   /*!< the threshold of proximity sensor */
	unsigned int    als_window_loss;                /*!< the window loss  */
	unsigned int    ps_threshold_high;
	unsigned int    ps_threshold_low;
};

extern struct alsps_hw* cm3623_get_cust_alsps_hw(void);
#endif 
