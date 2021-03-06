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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   sensor.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Header file of Sensor driver
 *
 *
 * Author:
 * -------
 *   PC Huang (MTK02204)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 * 09 10 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .alps dual sensor
 *
 * 09 02 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .roll back dual sensor
 *
 * 05 05 2010 sean.cheng
 * [ALPS00005476][Performance][Camera] Camera startup time is slow 
 * .
 * Decrease the sensor init time from 200ms to 30ms
 *
 * Dec 21 2009 mtk70508
 * [DUMA00147177] Winmo sensor  and lens driver  modification
 *
 *
 * Aug 5 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 *
 *
 * Apr 7 2009 mtk02204
 * [DUMA00004012] [Camera] Restructure and rename camera related custom folders and folder name of came
 *
 *
 * Mar 26 2009 mtk02204
 * [DUMA00003515] [PC_Lint] Remove PC_Lint check warnings of camera related drivers.
 *
 *
 * Mar 2 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Feb 24 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Dec 27 2008 MTK01813
 * DUMA_MBJ CheckIn Files
 * created by clearfsimport
 *
 * Dec 10 2008 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Oct 27 2008 mtk01051
 * [DUMA00000851] Camera related drivers check in
 * Modify Copyright Header
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
/* SENSOR FULL SIZE */
#ifndef __SENSOR_H
#define __SENSOR_H


#define FACTORY_START_ADDR 	0
#define ENGINEER_START_ADDR	10

typedef enum group_enum {
	   PRE_GAIN=0,
	   CMMCLK_CURRENT,
	   FRAME_RATE_LIMITATION,
	   REGISTER_EDITOR,
	   GROUP_TOTAL_NUMS
} FACTORY_GROUP_ENUM;

typedef enum register_index
{
	SENSOR_BASEGAIN=FACTORY_START_ADDR,
	PRE_GAIN_R_INDEX,
	PRE_GAIN_Gr_INDEX,
	PRE_GAIN_Gb_INDEX,
	PRE_GAIN_B_INDEX,
	FACTORY_END_ADDR
} FACTORY_REGISTER_INDEX;

typedef enum engineer_index
{
	CMMCLK_CURRENT_INDEX=ENGINEER_START_ADDR,
	ENGINEER_END
} FACTORY_ENGINEER_INDEX;

typedef struct
{
	SENSOR_REG_STRUCT	Reg[ENGINEER_END];
   	SENSOR_REG_STRUCT	CCT[FACTORY_END_ADDR];
} SENSOR_DATA_STRUCT, *PSENSOR_DATA_STRUCT;

#define CURRENT_MAIN_SENSOR				MT9P012_MICRON

/* START GRAB PIXEL OFFSET */
#define IMAGE_SENSOR_START_GRAB_X		        2      //Sean Change to 2 //0	// 0 or 1 recommended
#define IMAGE_SENSOR_START_GRAB_Y		        2 	  //Sean Change to 2 //0 // 0 or 1 recommended

/* MAX/MIN FRAME RATE (FRAMES PER SEC.) */
#define MAX_FRAME_RATE							15		// Limitation for MPEG4 Encode Only
#define MIN_FRAME_RATE							12

//s_porting add
#if 0
/* SENSOR FULL SIZE */
	#define IMAGE_SENSOR_FULL_WIDTH						(2592-16)
	#define IMAGE_SENSOR_FULL_HEIGHT					(1944-14)

/* SENSOR PV SIZE */
	#define IMAGE_SENSOR_PV_WIDTH						(1296-8)//(1280-4)
	#define IMAGE_SENSOR_PV_HEIGHT						(972-8)//(960-4)
#else
//to fit current isp setting mech.
/* SENSOR FULL SIZE */
	#define IMAGE_SENSOR_FULL_WIDTH						(2592-32)
	#define IMAGE_SENSOR_FULL_HEIGHT					(1944-24)

/* SENSOR PV SIZE */
	#define IMAGE_SENSOR_PV_WIDTH						(1288-8)//(1280-4)
	#define IMAGE_SENSOR_PV_HEIGHT						(970-10)//(960-4)
#endif
//e_porting add

/* SENSOR SCALER FACTOR */
#define PV_SCALER_FACTOR					    3
#define FULL_SCALER_FACTOR					    1

/* SENSOR START/EDE POSITION */
	#define FULL_X_START						    			8
	#define FULL_Y_START						    			8
	#define FULL_X_END						        		2601
	#define FULL_Y_END						        		1953
	#define PV_X_START						    				8
	#define PV_Y_START						    				8
	#define PV_X_END						    					2597//2597
	#define PV_Y_END						    					1953//1949

/* SENSOR HORIZONTAL/VERTICAL BLANKING */

	#define IMAGE_SENSOR_FULL_HBLANKING				(30)
	#define IMAGE_SENSOR_FULL_VBLANKING				(126)
	#define IMAGE_SENSOR_PV_HBLANKING					(160)
	#define IMAGE_SENSOR_PV_VBLANKING					(130)

/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
	#define FULL_ACTIVE_PIXEL_NUMS						(FULL_X_END-FULL_X_START+1-2)//2594
	#define FULL_ACTIVE_LINE_NUMS							(FULL_Y_END-FULL_Y_START+1-2)//1946
	#define PV_ACTIVE_PIXEL_NUMS							((PV_X_END-PV_X_START+1)/((PV_SCALER_FACTOR+1)/2))//1295
	#define PV_ACTIVE_LINE_NUMS								((PV_Y_END-PV_Y_START+1)/((PV_SCALER_FACTOR+1)/2))//976

	#define FULL_PERIOD_PIXEL_NUMS						(FULL_ACTIVE_PIXEL_NUMS+IMAGE_SENSOR_FULL_HBLANKING)//(2624)
	#define FULL_PERIOD_LINE_NUMS							(FULL_ACTIVE_LINE_NUMS+IMAGE_SENSOR_FULL_VBLANKING)//(2072)
	#define PV_PERIOD_PIXEL_NUMS							(PV_ACTIVE_PIXEL_NUMS+IMAGE_SENSOR_PV_HBLANKING)//(1455)
	#define PV_PERIOD_LINE_NUMS					    	(PV_ACTIVE_LINE_NUMS+IMAGE_SENSOR_PV_VBLANKING)//(1006)

/* DUMMY NEEDS TO BE INSERTED */
/* SETUP TIME NEED TO BE INSERTED */
#define IMAGE_SENSOR_PV_INSERTED_PIXELS			2	// Sync, Nosync=2
#define IMAGE_SENSOR_PV_INSERTED_LINES			2

#define IMAGE_SENSOR_FULL_INSERTED_PIXELS		2
#define IMAGE_SENSOR_FULL_INSERTED_LINES		2

/* SENSOR READ/WRITE ID */
	#define MT9P012_WRITE_ID							0x6C
	#define MT9P012_READ_ID								0x6D
	#define MT9P012_WRITE_ID_1							0x6C
	#define MT9P012_READ_ID_1							0x6D
	#define MT9P012_READ_REG_ID							0xF1

//s_porting start
	/* SENSOR CHIP VERSION */
//	#define MT9P012_SENSOR_ID							0x2800
//	#define MT9P012_SENSOR_ID_REV7						0x2801
//s_porting end

//s_porting add
//s_porting add
//s_porting add
#define PV_VGA_ACTUAL_WIDTH     648
#define PV_VGA_ACTUAL_HEIGHT    490
#define PV_VGA_TOTAL_PIXEL_PER_LINE (2068 + 648)
#define PV_VGA_TOTAL_LINE_PER_FRAME (662 + 490)

#define PV_1280X960_ACTUAL_WIDTH     1288 //(PV_ACTIVE_PIXEL_NUMS+1)
#define PV_1280X960_ACTUAL_HEIGHT    970 //(PV_ACTIVE_LINE_NUMS+1)
#define PV_1280X960_TOTAL_PIXEL_PER_LINE (1614 + 1288)
#define PV_1280X960_TOTAL_LINE_PER_FRAME (106 + 970)

#define FULL_ACTUAL_WIDTH     2592
#define FULL_ACTUAL_HEIGHT    1944
#define FULL_TOTAL_PIXEL_PER_LINE (2838 + 2592)
#define FULL_TOTAL_LINE_PER_FRAME (112 + 1944)


//format index
#define MT9P012_PV_FORMAT_INDEX   1 //1280x960 mode
#define MT9P012_FULL_FORMAT_INDEX 2 //Full resolution mode

//customize
#define CAM_SIZE_QVGA_WIDTH 	320
#define CAM_SIZE_QVGA_HEIGHT 	240
#define CAM_SIZE_VGA_WIDTH 		640
#define CAM_SIZE_VGA_HEIGHT 	480
#define CAM_SIZE_05M_WIDTH 		800
#define CAM_SIZE_05M_HEIGHT 	600
#define CAM_SIZE_1M_WIDTH 		1280
#define CAM_SIZE_1M_HEIGHT 		960
#define CAM_SIZE_2M_WIDTH 		1600
#define CAM_SIZE_2M_HEIGHT 		1200
#define CAM_SIZE_3M_WIDTH 		2048
#define CAM_SIZE_3M_HEIGHT 		1536
#define CAM_SIZE_5M_WIDTH 		2592
#define CAM_SIZE_5M_HEIGHT 		1944

//
//initial config function
//
int pfInitCfg_VGA(void);
int pfInitCfg_1280X960(void);
int pfInitCfg_FULL(void);

//export functions
UINT32 MT9P012Open(void);
UINT32 MT9P012Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 MT9P012FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
UINT32 MT9P012GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 MT9P012GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
UINT32 MT9P012Close(void);

//#define Sleep(ms) mdelay(ms)
#define Sleep(us) udelay(us)

#define RETAILMSG(x,...)
#define TEXT
//e_porting add
//e_porting add
//e_porting add


#endif /* __SENSOR_H */

