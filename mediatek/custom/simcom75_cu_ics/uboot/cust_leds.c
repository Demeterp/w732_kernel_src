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

#include <asm/arch/custom/inc/cust_leds.h>
#include <asm/arch/mt6575_pwm.h>

extern int DISP_SetBacklight(int level);

static struct cust_mt65xx_led cust_led_list[MT65XX_LED_TYPE_TOTAL] = {
#if defined(PHILIPS_ATLAS)
	{"red",               MT65XX_LED_MODE_NONE, -1,{0}},
	{"green",             MT65XX_LED_MODE_NONE, -1,{0}},
	{"blue",              MT65XX_LED_MODE_NONE, -1,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_BUTTON,{0}},
	{"lcd-backlight",     MT65XX_LED_MODE_CUST, (int)DISP_SetBacklight,{0}},
#elif defined(SIMCOM_I6000)
	{"red",               MT65XX_LED_MODE_PWM, PWM2,{0}},
	{"green",            MT65XX_LED_MODE_NONE, -1,{0}},
	{"blue",              MT65XX_LED_MODE_NONE, -1,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_BUTTON,{0}},
	/*punk,pwm clk 100Hz -> 30k*/
	{"lcd-backlight",     MT65XX_LED_MODE_PWM, PWM1,{PWM_CLK_NEW_MODE_BLOCK,CLK_DIV1,24,24}}, 
#elif defined(ACER_C8)
	{"red",               MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK4},
	{"green",             MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK5},
	{"blue",              MT65XX_LED_MODE_NONE, -1,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	//{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_BUTTON,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC,  -1,{0}},
	/*punk,pwm clk 100Hz -> 30k*/
	{"lcd-backlight",     MT65XX_LED_MODE_PWM, PWM1,{PWM_CLK_NEW_MODE_BLOCK,CLK_DIV1,24,24}}, 
#elif defined(SIMCOM_I5000)||defined(PHILIPS_TITAN)//amy0504 	
	{"red",               MT65XX_LED_MODE_NONE, -1,{0}},
	{"green",             MT65XX_LED_MODE_NONE, -1,{0}},
	{"blue",              MT65XX_LED_MODE_NONE, -1,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_BUTTON,{0}},
	{"lcd-backlight",     MT65XX_LED_MODE_PWM, PWM1,{PWM_CLK_NEW_MODE_BLOCK,CLK_DIV1,24,24}},
#elif defined(SIMCOM_I3000)//LK@I3000
	{"red",               MT65XX_LED_MODE_PWM, PWM2,{0}},
	{"green",             MT65XX_LED_MODE_PWM, PWM1,{0}},
	{"blue",              MT65XX_LED_MODE_PWM, PWM3,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_BUTTON,{0}},
	{"lcd-backlight",     MT65XX_LED_MODE_PWM, PWM0,{PWM_CLK_NEW_MODE_BLOCK,CLK_DIV1,24,24}}, 

#else
	{"red",               MT65XX_LED_MODE_NONE, -1,{0}},
	{"green",             MT65XX_LED_MODE_NONE, -1,{0}},
	{"blue",              MT65XX_LED_MODE_NONE, -1,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_BUTTON,{0}},
	{"lcd-backlight",     MT65XX_LED_MODE_PWM, PWM1,{0}},

#endif
};

struct cust_mt65xx_led *get_cust_led_list(void)
{
	return cust_led_list;
}
