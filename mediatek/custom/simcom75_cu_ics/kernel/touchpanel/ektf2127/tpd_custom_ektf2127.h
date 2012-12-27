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

#ifndef TOUCHPANEL_H__
#define TOUCHPANEL_H__

/* Pre-defined definition */
#define TPD_TYPE_CAPACITIVE
//#define TPD_TYPE_RESISTIVE
#define TPD_POWER_SOURCE         MT65XX_POWER_LDO_VGP   
#define TPD_I2C_NUMBER           0
#define TPD_WAKEUP_TRIAL         60
#define TPD_WAKEUP_DELAY         100

///////////////////tpd lcd view setting/////////////////////////
#define ELAN_X_MAX 	576//Elan modify
#define ELAN_Y_MAX	960//Elan modify

#define TP_EKTF_WIDTH 	480
#define TP_EKTF_HEIGHT	800

//Orig. point at upper-right, reverse it.
//#define REVERSE_X_AXIS
//#define REVERSE_Y_AXIS

/////////////////////tpd_button  setting/////////////////////////
//#define TPD_HAVE_BUTTON

//softkey is reported as AXIS
//#define SOFTKEY_AXIS_VER

//softkey is reported via BTN bit
//#define SOFTKEY_BTN_VER

#if defined(SOFTKEY_AXIS_VER)
#define TPD_KEY_COUNT           4

struct osd_offset{
	int left_x;
	int right_x;
	unsigned int key_event;
};

//Elan add for OSD bar coordinate
static struct osd_offset OSD_mapping[] = {
  {35, 99, KEY_MENU},	//menu_left_x, menu_right_x, KEY_MENU
  {203, 267, KEY_HOME},	//home_left_x, home_right_x, KEY_HOME
  {373, 437, KEY_BACK},	//back_left_x, back_right_x, KEY_BACK
  {541, 605, KEY_SEARCH},	//search_left_x, search_right_x, KEY_SEARCH
};
#endif

#if defined(SOFTKEY_BTN_VER)
#define TPD_KEY_COUNT           3
static int TPD_KEYS[4]= {KEY_BACK, KEY_HOMEPAGE, KEY_MENU,0}
#endif


#endif /* TOUCHPANEL_H__ */
