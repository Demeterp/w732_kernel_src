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
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "kd_camera_hw.h"

#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_camera_feature.h"

/******************************************************************************
 * Debug configuration
******************************************************************************/
#define PFX "[kd_camera_hw]"
#define PK_DBG_NONE(fmt, arg...)    do {} while (0)
#define PK_DBG_FUNC(fmt, arg...)    printk(KERN_INFO PFX "%s: " fmt, __FUNCTION__ ,##arg)

#define DEBUG_CAMERA_HW_K
#ifdef DEBUG_CAMERA_HW_K
#define PK_DBG PK_DBG_FUNC
#define PK_ERR(fmt, arg...)         printk(KERN_ERR PFX "%s: " fmt, __FUNCTION__ ,##arg)
#else
#define PK_DBG(a,...)
#define PK_ERR(a,...)
#endif

extern void mt_isp_mclk_ctrl(MINT32 en);
 
#if defined(MT6516)
int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, BOOL On, char* mode_name)
{
u32 pinSetIdx = 0;//default main sensor
u32 pinSet[2][4] = {
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

    if (DUAL_CAMERA_MAIN_SENSOR == SensorIdx){
        pinSetIdx = 0;
    }
    else if (DUAL_CAMERA_SUB_SENSOR == SensorIdx) {
        pinSetIdx = 1;
    }

    //power ON
    if (On) {
        //in case
        if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
        {
            PK_DBG("[ON_OV3640YUV case 1.5V]sensorIdx:%d \n",SensorIdx);
            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))
            {
                PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                //return -EIO;
                goto _kdCISModulePowerOn_exit_;
            }
        }
        else //general case on
        {
            PK_DBG("[ON_general 1.8V]sensorIdx:%d \n",SensorIdx);
            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))
            {
                PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                //return -EIO;
                goto _kdCISModulePowerOn_exit_;
            }
        }

        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }

        //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][0]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][0],pinSet[1-pinSetIdx][1])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][2],pinSet[1-pinSetIdx][3])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][0],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][2],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][0],GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][2],GPIO_OUT_ONE)){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }

        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][0]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][0],pinSet[pinSetIdx][1])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][0],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][0],GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(10);
            if(mt_set_gpio_out(pinSet[pinSetIdx][0],GPIO_OUT_ONE)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][2],pinSet[pinSetIdx][3])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][2],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][2],GPIO_OUT_ZERO)){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
    }
    else {//power OFF

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][0]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][0],pinSet[pinSetIdx][1])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][2],pinSet[pinSetIdx][3])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][0],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][2],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][0],GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
    	    if(mt_set_gpio_out(pinSet[pinSetIdx][2],GPIO_OUT_ONE)){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
    }//

	return 0;

_kdCISModulePowerOn_exit_:
    return -EIO;
}

EXPORT_SYMBOL(kdCISModulePowerOn);

#elif defined(MT6573)

int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, BOOL On, char* mode_name)
{
u32 pinSetIdx = 0;//default main sensor

#define IDX_PS_CMRST 0
#define IDX_PS_CMPDN 4

#define IDX_PS_MODE 1
#define IDX_PS_ON   2
#define IDX_PS_OFF  3
u32 pinSet[2][8] = {
                    //for main sensor 
                    {GPIO_CAMERA_CMRST_PIN,
                        GPIO_CAMERA_CMRST_PIN_M_GPIO,   /* mode */
                        GPIO_OUT_ONE,                   /* ON state */
                        GPIO_OUT_ZERO,                  /* OFF state */
                     GPIO_CAMERA_CMPDN_PIN,
                        GPIO_CAMERA_CMPDN_PIN_M_GPIO,
                        GPIO_OUT_ZERO,
                        GPIO_OUT_ONE,
                    },
                    //for sub sensor 
                    {GPIO_CAMERA_CMRST1_PIN,
                     GPIO_CAMERA_CMRST1_PIN_M_GPIO,
                        GPIO_OUT_ONE,
                        GPIO_OUT_ZERO,
                     GPIO_CAMERA_CMPDN1_PIN,
                        GPIO_CAMERA_CMPDN1_PIN_M_GPIO,
                        GPIO_OUT_ZERO,
                        GPIO_OUT_ONE,
                    }
                   };

    if (DUAL_CAMERA_MAIN_SENSOR == SensorIdx){
        pinSetIdx = 0;
    }
    else if (DUAL_CAMERA_SUB_SENSOR == SensorIdx) {
        pinSetIdx = 1;
    }

    //power ON
    if (On) {
        //in case
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
        
        if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
        {
            PK_DBG("[ON_OV3640YUV case 1.5V]sensorIdx:%d \n",SensorIdx);
            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))
            {
                PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                //return -EIO;
                goto _kdCISModulePowerOn_exit_;
            }
        }
        else //general case on
        {
            PK_DBG("[ON_general 1.8V]sensorIdx:%d \n",SensorIdx);
            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_2800,mode_name))
            {
                PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                //return -EIO;
                goto _kdCISModulePowerOn_exit_;
            }                    
        }

        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }


        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }
        
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }        

        //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }        

        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(10);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
    }
    else {//power OFF
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
        

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
    	    if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }

    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }                    
    }//

	return 0;

_kdCISModulePowerOn_exit_:
    return -EIO;
}

EXPORT_SYMBOL(kdCISModulePowerOn);


#elif defined(MT6575)
#define CAMERA_POWER_VCAM_AF MT65XX_POWER_LDO_VCAM_AF

#if defined(PHILIPS_TITAN)
#define SENSOR_OV5647_RAW_PWR_ON() do {	\
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_2800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A2\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
            }while (0)
#elif defined(ACER_C8)
#define SENSOR_OV5647_RAW_PWR_ON() do {	\
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(10);                                                                                                 \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
               /*if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
               }                                                                                                           \
                mdelay(10);     //IOVDD use 1.8V,  DVDD use internel LDO*/                                                                                         \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A2\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
            }while (0)
#else
#define SENSOR_OV5647_RAW_PWR_ON() do {	\
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_2800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(10);                                                                                                 \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
               if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
               }                                                                                                           \
                mdelay(10);                                                                      \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A2\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
            }while (0)
#endif

#if defined(ACER_C8)
#define SENSOR_MT9P017_RAW_PWR_ON() do {	\
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                 \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_D\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(3);                                                                                                  \
                /*if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(10);   */                                                                                               \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A2\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(2);                                                                                                  \
            }while (0)
#else
#define SENSOR_MT9P017_RAW_PWR_ON() do {	\
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(10);                                                                                                 \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(10);                                                                                                  \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A2\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
            }while (0)
#endif

#define SENSOR_OV5640_RAW_PWR_ON() do {	\
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(10);                                                                                                 \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(10);                                                                                                  \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A2\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
            }while (0)
			

#define SENSOR_GC0329_YUV_PWR_ON() do {									\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)){										\
	       	PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");						\
	       	goto _kdCISModulePowerOn_exit_;												\
       	}																							\
       	mdelay(10); 																						\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)){ 										\
       		PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");							\
       		goto _kdCISModulePowerOn_exit_; 																		\
       	}																										\
       	mdelay(5);																							\
       	}while (0)


  #if defined(PHILIPS_TITAN)
#define SENSOR_GC0309_YUV_PWR_ON() do {									\
	       if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_2800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A2\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(5);                                                                                                  \
       	}while (0)
#else    	
#define SENSOR_GC0309_YUV_PWR_ON() do {									\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_2800,mode_name)){										\
	       	PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");						\
	       	goto _kdCISModulePowerOn_exit_;												\
       	}																							\
       	mdelay(10); 																						\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)){ 										\
       		PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");							\
       		goto _kdCISModulePowerOn_exit_; 																		\
       	}																										\
       	mdelay(5);																							\
		}while (0)
#endif

#define SENSOR_OV7675_YUV_PWR_ON() do {									\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)){										\
	       	PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");						\
	       	goto _kdCISModulePowerOn_exit_;												\
       	}																							\
       	mdelay(10); 																						\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)){ 										\
       		PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");							\
       		goto _kdCISModulePowerOn_exit_; 																		\
       	}																										\
       	mdelay(5);																							\
       	}while (0)

#define SENSOR_OV7690_YUV_PWR_ON() do {									\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)){										\
	       	PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");						\
	       	goto _kdCISModulePowerOn_exit_;												\
       	}																							\
       	mdelay(10); 																						\
       	if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)){ 										\
       		PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");							\
       		goto _kdCISModulePowerOn_exit_; 																		\
       	}																										\
       	mdelay(5);																							\
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)) {\
			PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");\
			goto _kdCISModulePowerOn_exit_;\
		}\
		mdelay(10); \
       	}while (0)
       	
#define SENSOR_OV3660_YUV_PWR_ON() do {                                                                                     \
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)){                                                \
		PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                              \
		goto _kdCISModulePowerOn_exit_;                                                                             \
		}                                                                                                               \
		mdelay(5);                                                                                                     \
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)){                                                 \
		PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                                \
		goto _kdCISModulePowerOn_exit_;                                                                             \
		}                                                                                                               \
		mdelay(5);                                                                                                      \
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)){                                                 \
		PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                               \
		goto _kdCISModulePowerOn_exit_;                                                                             \
		}                                                                                                               \
		mdelay(10);                                                                                                      \
		}while (0)

#define SENSOR_S5K5CAGX_YUV_PWR_ON() do {  \
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D\n");                           \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                 \
                mdelay(1);                                                                                                 \
                if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
                    PK_DBG("[CAMERA SENSOR] Fail to enable analog power:CAMERA_POWER_VCAM_A\n");                            \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                }                                                                                                           \
                mdelay(2);                                                                                                  \
                 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name)) {                                           \
                    PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_D2\n");                          \
                    goto _kdCISModulePowerOn_exit_;                                                                         \
                } mdelay(5);                                                                                                  \
              }while (0)

#if defined(PHILIPS_TITAN)
#define SENSOR_PWR_OFF() do {\
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_A power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2, mode_name)) {\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_A2 power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D,mode_name)){\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_D power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name)){\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_A2 power\n");\
        }\
	}while (0)
#else			  
#define SENSOR_PWR_OFF() do {\
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_A power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_D power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name)){\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_D2 power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_AF,mode_name)){\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_AF af\n");\
        }\
	}while (0)
#endif


#if defined(PHILIPS_ATLAS)//chenhaojun
#define SENSOR_PWR_OFF1() do {\
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_A power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_D power\n");\
        }\
       mdelay(5);\
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name)){\
            PK_DBG("[CAMERA SENSOR] Fail to disable CAMERA_POWER_VCAM_D2 power\n");\
        }\
	}while (0)
extern kal_uint16 OV5647_write_cmos_sensor(kal_uint32 addr, kal_uint32 para);
#endif
#if defined(S5K5CAGX_YUV)
extern int s5k5cagetid;
#endif
int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, BOOL On, char* mode_name)
{
u32 pinSetIdx = 0;//default main sensor

#define IDX_PS_CMRST 0
#define IDX_PS_CMPDN 4

#define IDX_PS_MODE 1
#define IDX_PS_ON   2
#define IDX_PS_OFF  3
u32 pinSet[3][8] = {
                    //for main sensor 
#if defined(SIMCOM_I6000) // chenhaojun
                    {GPIO_CAMERA_CMRST_PIN,
                        GPIO_CAMERA_CMRST_PIN_M_GPIO,   /* mode */
                        GPIO_OUT_ONE,                   /* ON state */
                        GPIO_OUT_ZERO,                  /* OFF state */
                     GPIO_CAMERA_CMPDN_PIN,
                        GPIO_CAMERA_CMPDN_PIN_M_GPIO,
                        GPIO_OUT_ONE,
                        GPIO_OUT_ZERO,
                    },
#else
                    {GPIO_CAMERA_CMRST_PIN,
                        GPIO_CAMERA_CMRST_PIN_M_GPIO,   /* mode */
                        GPIO_OUT_ONE,                   /* ON state */
                        GPIO_OUT_ZERO,                  /* OFF state */
                     GPIO_CAMERA_CMPDN_PIN,
                        GPIO_CAMERA_CMPDN_PIN_M_GPIO,
                        GPIO_OUT_ZERO,
                        GPIO_OUT_ONE,
                    },
#endif
                    //for sub sensor 
                    {GPIO_CAMERA_CMRST1_PIN,
                     GPIO_CAMERA_CMRST1_PIN_M_GPIO,
                        GPIO_OUT_ONE,
                        GPIO_OUT_ZERO,
                     GPIO_CAMERA_CMPDN1_PIN,
                        GPIO_CAMERA_CMPDN1_PIN_M_GPIO,
                        GPIO_OUT_ZERO,
                        GPIO_OUT_ONE,
                    },
                    //for main_2 sensor 
                    {GPIO_CAMERA_2_CMRST_PIN,
                        GPIO_CAMERA_2_CMRST_PIN_M_GPIO,   /* mode */
                        GPIO_OUT_ONE,                   /* ON state */
                        GPIO_OUT_ZERO,                  /* OFF state */
                     GPIO_CAMERA_2_CMPDN_PIN,
                        GPIO_CAMERA_2_CMPDN_PIN_M_GPIO,
                        GPIO_OUT_ZERO,
                        GPIO_OUT_ONE,
                    }
                   };

    if (DUAL_CAMERA_MAIN_SENSOR == SensorIdx){
        pinSetIdx = 0;
    }
    else if (DUAL_CAMERA_SUB_SENSOR == SensorIdx) {
        pinSetIdx = 1;
    }
    else if (DUAL_CAMERA_MAIN_SECOND_SENSOR == SensorIdx) {
        pinSetIdx = 2;
    }

//some sensor is working on pdn high
#if defined(I5000_P150)//chenhaojun
	if ((currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K5CAGX_YUV,currSensorName))) &&(pinSetIdx==1))
	{
		return 0;
	}
	if( (currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K5CAGX_YUV,currSensorName)))&&(pinSetIdx ==0))
	{

		pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON] = GPIO_OUT_ONE;
		pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF] = GPIO_OUT_ZERO;
	}
#endif

	if((pinSetIdx ==0) && (currSensorName && (0 == strcmp(SENSOR_DRVNAME_MT9P017_RAW,currSensorName))))
	{

		pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON] = GPIO_OUT_ONE;
		pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF] = GPIO_OUT_ZERO;
	}
	
    //power ON
    if (On) {
        //in case
				
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

#if defined(S5K5CAGX_YUV)
if((DUAL_CAMERA_SUB_SENSOR == SensorIdx)&&(s5k5cagetid==1))
{
	SENSOR_S5K5CAGX_YUV_PWR_ON();
	mt_set_gpio_mode(GPIO_CAMERA_CMPDN_PIN,GPIO_CAMERA_CMPDN1_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CAMERA_CMPDN_PIN,GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CAMERA_CMPDN_PIN,GPIO_OUT_ONE);
	mdelay(1);
	mt_set_gpio_mode(GPIO_CAMERA_CMRST_PIN,GPIO_CAMERA_CMPDN1_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CAMERA_CMRST_PIN,GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CAMERA_CMRST_PIN,GPIO_OUT_ONE);
	mdelay(10);
	mt_set_gpio_mode(GPIO_CAMERA_CMPDN_PIN,GPIO_CAMERA_CMPDN1_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CAMERA_CMPDN_PIN,GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CAMERA_CMPDN_PIN,GPIO_OUT_ZERO);
	mdelay(200);
}
#endif

	if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV5647_RAW,currSensorName))) 
	{
		printk("SENSOR_OV5647_RAW_PWR_ON'");
		SENSOR_OV5647_RAW_PWR_ON();
	}	
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV7675_YUV,currSensorName))) 
	{
		printk("SENSOR_OV7675_YUV_PWR_ON2'");
		SENSOR_OV7675_YUV_PWR_ON();
	}
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV7690_YUV,currSensorName))) 
	{
		printk("SENSOR_OV7690_YUV_PWR_ON2'");
#if defined(PHILIPS_ATLAS)//chenhaojun
		SENSOR_OV5647_RAW_PWR_ON();
		pinSetIdx = 0;
		if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
			if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			mdelay(30);
			if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
			if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
			mdelay(30);
		}   
		//enable active sensor
		//RST pin
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
			mdelay(10);
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
			mdelay(30);

			//PDN pin
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		}
		mdelay(30);
		OV5647_write_cmos_sensor(0x3000,0x00);
		OV5647_write_cmos_sensor(0x3001,0x00);
		OV5647_write_cmos_sensor(0x3002,0x00);
		
		if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
		if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
		if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		pinSetIdx = 1;
#endif
		SENSOR_OV7690_YUV_PWR_ON();
	}
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_MT9P017_RAW,currSensorName))) 
	{
		printk("SENSOR_MT9P017_RAW_PWR_ON'");
		#if defined(ACER_C8)		
			mt_isp_mclk_ctrl(0);
			//RST pin
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
				if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
				if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
				mdelay(5);
				if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
				if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
				if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
				mdelay(5);
			}
			SENSOR_MT9P017_RAW_PWR_ON();
			mdelay(2); 
			mt_isp_mclk_ctrl(1);

			if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name)) {                                            \
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power:CAMERA_POWER_VCAM_A\n");                           \
				goto _kdCISModulePowerOn_exit_;                                                                         \
			}                                                                                                           \
			mdelay(30);     

			 //enable active sensor
		        //RST pin
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				//PDN pin
				if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
				if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
				if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
				mdelay(30);
				if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
				if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
				if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
			}	
		#else
			SENSOR_MT9P017_RAW_PWR_ON();
		#endif
	}
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC0329_YUV,currSensorName)))   //amy0509 
	{
		#if defined(I5000_N618)//tengdeqiang
		SENSOR_MT9P017_RAW_PWR_ON();
		pinSetIdx = 0;
		if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
			if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
			if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
		}   
		//enable active sensor
		//RST pin
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
			mdelay(10);
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
			mdelay(1);

			//PDN pin
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		}
		mdelay(10);
		
		if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
		if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
		if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		pinSetIdx = 1;
		#else		
		printk("SENSOR_GC0329_YUV_PWR_ON'");
		SENSOR_GC0329_YUV_PWR_ON();
		#endif
	} 
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC0309_YUV,currSensorName)))  //amy0509 
	{
		printk("SENSOR_GC0309_YUV_PWR_ON'");
		SENSOR_GC0309_YUV_PWR_ON();
	} 
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3660_YUV,currSensorName))) 
	{
		printk("SENSOR_OV3660_YUV_PWR_ON'");
		SENSOR_OV3660_YUV_PWR_ON();
	}
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K5CAGX_YUV,currSensorName))) 
	{
		printk("SENSOR_S5K5CAGX_YUV_PWR_ON'");
		SENSOR_S5K5CAGX_YUV_PWR_ON();
	}	
	else
	{
	        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
	        {
	            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
	            //return -EIO;
	            goto _kdCISModulePowerOn_exit_;
	        }
		mdelay(3);
	        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
	        {
	            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
	            //return -EIO;
	            goto _kdCISModulePowerOn_exit_;
	        }  
	        mdelay(50);
	        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))
	        {
	             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
	             //return -EIO;
	             goto _kdCISModulePowerOn_exit_;
	        }        
	        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
	        {
	            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
	            //return -EIO;
	            goto _kdCISModulePowerOn_exit_;
	        } 
	        // wait power to be stable 
	        mdelay(5); 
	}
	
        //disable inactive sensor
#if defined(S5K5CAGX_YUV)
if((DUAL_CAMERA_SUB_SENSOR == SensorIdx)&&(s5k5cagetid==1))
{
}
else
{
#endif
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            mdelay(30);
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(30);
        }        
#if defined(S5K5CAGX_YUV)
}
#endif

	#if defined(ACER_C8)
		if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_MT9P017_RAW,currSensorName)))  // for ACER_C8
		{
			//do nothing , reset and pwdn has been set before
		}
		else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV5647_RAW,currSensorName)))  // for ACER_C8
		{
			//enable active sensor
		        //RST pin
		        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
		            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
		            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
		            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
		            msleep(10);
		            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
		            mdelay(30);

		            //PDN pin
		            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
		            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
		            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		            mdelay(30);
		        }
	    	}
	#else
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            msleep(10);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(30);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
            mdelay(30);
        }
	#endif
    }
    else {//power OFF

#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
    	    if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
	#if defined(ACER_C8)
		msleep(5);
	#endif
	SENSOR_PWR_OFF();
#if defined(PHILIPS_ATLAS)
	SENSOR_PWR_OFF1();
#endif               
    }

	return 0;

_kdCISModulePowerOn_exit_:
	SENSOR_PWR_OFF();
#if defined(PHILIPS_ATLAS)
	SENSOR_PWR_OFF1();
#endif   
    return -EIO;
}

EXPORT_SYMBOL(kdCISModulePowerOn);
#else 
#error Error !! forget to implement power control for image sensor

#endif 



