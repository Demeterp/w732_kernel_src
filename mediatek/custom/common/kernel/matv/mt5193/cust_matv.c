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

//For mt6573_evb
///#include <mach/mt6575_pll.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/workqueue.h>
///#include <linux/delay.h>


#include <linux/hrtimer.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>

#include <linux/jiffies.h>
#include <linux/timer.h>

#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_pm_ldo.h>

#include "cust_matv.h"
#include "cust_matv_comm.h"


int cust_matv_power_on(void)
{  
    //set GPIO94 for power
    ///int pinSetIdx = 0;//default main sensor
    int pinSet[2][4] = {
    			//for main sensor 
    			{GPIO_CAMERA_CMRST_PIN,
    			 GPIO_CAMERA_CMRST_PIN_M_GPIO,
    			 GPIO_CAMERA_CMPDN_PIN,
    			 GPIO_CAMERA_CMPDN_PIN_M_GPIO},
    			//for sub sensor 
    			{GPIO_CAMERA_CMRST1_PIN,
    			 GPIO_CAMERA_CMRST1_PIN_M_GPIO,
    			 GPIO_CAMERA_CMPDN1_PIN,
    			 GPIO_CAMERA_CMPDN1_PIN_M_GPIO}
    		   };

	MATV_LOGE("[MATV] cust_matv_power_on Start\n");

    if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,"MT5192"))
    {
    MATV_LOGE("[CAMERA SENSOR] Fail to enable digital power\n");
    //return -EIO;
    return 0;
    }                    

    if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,"MT5192"))
    {
    MATV_LOGE("[CAMERA SENSOR] Fail to enable analog power\n");
    //return -EIO;
    return 0;
    }


    if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1300,"MT5192"))
    {
    MATV_LOGE("[CAMERA SENSOR] Fail to enable digital power\n");
    //return -EIO;
    return 0;
    }

    if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,"MT5192"))
    {
    MATV_LOGE("[CAMERA SENSOR] Fail to enable analog power\n");
    //return -EIO;
    return 0;
    }    

    mt_set_gpio_mode(pinSet[0][0],pinSet[0][1]);
    mt_set_gpio_dir(pinSet[0][0],GPIO_DIR_OUT);
    mt_set_gpio_out(pinSet[0][0],GPIO_OUT_ZERO);
    
    mt_set_gpio_mode(pinSet[1][0],pinSet[1][1]);
    mt_set_gpio_dir(pinSet[1][0],GPIO_DIR_OUT);
    mt_set_gpio_out(pinSet[1][0],GPIO_OUT_ZERO);
    
    mt_set_gpio_mode(pinSet[0][2],pinSet[0][3]);
    mt_set_gpio_dir(pinSet[0][2],GPIO_DIR_OUT);
    mt_set_gpio_out(pinSet[0][2],GPIO_OUT_ZERO);
    
    mt_set_gpio_mode(pinSet[1][2],pinSet[1][3]);
    mt_set_gpio_dir(pinSet[1][2],GPIO_DIR_OUT);
    mt_set_gpio_out(pinSet[1][2],GPIO_OUT_ZERO);
    
#if 0
    mdelay(10);
    mt_set_gpio_out(pinSet[0][0],GPIO_OUT_ONE);
    
    mt_set_gpio_out(pinSet[1][0],GPIO_OUT_ONE);
    
    mdelay(1);
    //PDN pin
    mt_set_gpio_mode(pinSet[0][2],pinSet[0][3]);
    mt_set_gpio_dir(pinSet[0][2],GPIO_DIR_OUT);
    mt_set_gpio_out(pinSet[0][2],GPIO_OUT_ONE);
    
    mt_set_gpio_mode(pinSet[1][2],pinSet[1][3]);
    mt_set_gpio_dir(pinSet[1][2],GPIO_DIR_OUT);
    mt_set_gpio_out(pinSet[1][2],GPIO_OUT_ONE);

    mdelay(5);
    mt_set_gpio_out(pinSet[0][0],GPIO_OUT_ZERO);
    mt_set_gpio_out(pinSet[0][2],GPIO_OUT_ZERO);
    
    mt_set_gpio_out(pinSet[1][0],GPIO_OUT_ZERO);
    mt_set_gpio_out(pinSet[1][2],GPIO_OUT_ZERO);
        
    }
#endif

    return 0;
}


int cust_matv_power_off(void)
{  
    MATV_LOGE("[MATV] cust_matv_power_off Start\n");

    if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,"MT5192")) {
        MATV_LOGE("[CAMERA SENSOR] Fail to OFF analog power\n");
        //return -EIO;
        return 0;
    }
    if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,"MT5192"))
    {
        MATV_LOGE("[CAMERA SENSOR] Fail to enable analog power\n");
        //return -EIO;
        return 0;
    }       
    if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, "MT5192")) {
        MATV_LOGE("[CAMERA SENSOR] Fail to OFF digital power\n");
        //return -EIO;
        return 0;
    }
    if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,"MT5192"))
    {
        MATV_LOGE("[CAMERA SENSOR] Fail to enable digital power\n");
        //return -EIO;
        return 0;
    }                    
	
    return 0;
}

int cust_matv_gpio_on(void)
{
	MATV_LOGE("[MATV] mt5193 cust_matv_gpio_on Start\n");
#ifdef GPIO_MATV_I2S_DAT_PIN    
    mt_set_gpio_mode(GPIO_MATV_I2S_DAT_PIN, GPIO_MATV_I2S_DAT_PIN_M_I2S0_DAT);
    mt_set_gpio_mode(GPIO_MATV_I2S_WS_PIN, GPIO_MATV_I2S_WS_PIN_M_I2S0_WS);
    mt_set_gpio_mode(GPIO_MATV_I2S_CK_PIN, GPIO_MATV_I2S_CK_PIN_M_I2S0_CK);
#endif

#if 0
    mt_set_gpio_mode(GPIO_I2S1_DAT_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_I2S1_DAT_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S1_DAT_PIN,GPIO_OUT_ZERO);
    mt_set_gpio_mode(GPIO_I2S1_WS_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_I2S1_WS_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S1_WS_PIN,GPIO_OUT_ZERO);
    mt_set_gpio_mode(GPIO_I2S1_CK_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_I2S1_CK_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_I2S1_CK_PIN,GPIO_OUT_ZERO);
#endif

    return 1;

}

int cust_matv_gpio_off(void)
{
	MATV_LOGE("[MATV] mt5193 cust_matv_gpio_off Start\n");

#ifdef GPIO_MATV_I2S_CK_PIN
    mt_set_gpio_mode(GPIO_MATV_I2S_CK_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_MATV_I2S_CK_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_MATV_I2S_CK_PIN,GPIO_OUT_ZERO);
    mt_set_gpio_mode(GPIO_MATV_I2S_WS_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_MATV_I2S_WS_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_MATV_I2S_WS_PIN,GPIO_OUT_ZERO);
    mt_set_gpio_mode(GPIO_MATV_I2S_DAT_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_MATV_I2S_DAT_PIN,GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_MATV_I2S_DAT_PIN,GPIO_OUT_ZERO);
#endif

    return 1;

}

