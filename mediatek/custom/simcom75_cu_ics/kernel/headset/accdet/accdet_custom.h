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

#if defined(SIMCOM_I3000)
//#define ACCDET_EINT
#else
#define ACCDET_EINT
#endif
#ifdef ACCDET_EINT
#define ACCDET_DELAY_ENABLE_TIME 500 //delay 500ms to enable accdet after EINT 
//#define ACCDET_EINT_HIGH_ACTIVE //defaule low active if not define ACCDET_EINT_HIGH_ACTIVE
#endif

struct tv_mode_settings{
    int start_line0;	//range: 1~19
    int end_line0;	//range: 1~19
    int start_line1;	//range: 263~285(NTSC), 310~336(PAL)
    int end_line1;	//range: 263~285(NTSC), 310~336(PAL)
    int pre_line;
    int start_pixel;	//range: 112~850
    int end_pixel;	//range: 112~850
    int fall_delay;
    int rise_delay;
    int div_rate;	//pwm div in tv-out mode 
    int debounce;	//tv-out debounce
};


struct headset_mode_settings{
    int pwm_width;	//pwm frequence
    int pwm_thresh;	//pwm duty 
    int fall_delay;	//falling stable time
    int rise_delay;	//rising stable time
    int debounce0;	//hook switch or double check debounce
    int debounce1;	//mic bias debounce
    int debounce3;	//plug out debounce
};


//remote button customization: long press time
int long_press_time = 2000;


//headset mode register settings(for MT6575)
struct headset_mode_settings cust_headset_settings = {
	0x1900, 0x140, 1, 0x12c, 0x3000, 0x3000, 0x400
};



#ifdef MTK_TVOUT_SUPPORT
#define TV_OUT_SUPPORT
#endif


#ifdef TV_OUT_SUPPORT

//tv mode register settings (for MT6575)
struct tv_mode_settings cust_tv_settings[2]={
        {9, 19, 273, 284, 5, 112, 850, 2, 0x160, 0x20, 0x6}, //NTSC mode
        {9, 19, 320, 334, 5, 112, 850, 2, 0x160, 0x20, 0x6}, //PAL mode
};


#endif
