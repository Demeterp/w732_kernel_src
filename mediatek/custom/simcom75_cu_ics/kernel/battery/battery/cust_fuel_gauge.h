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

#include <mach/mt6575_typedefs.h>

#define FGAUGE_VOLTAGE_FACTOR           2.44 // mV
#define FGAUGE_CURRENT_FACTOR           6.25 // uV/Rsns
#define FGAUGE_CURRENT_OFFSET_FACTOR    1.56 // uV/Rsns
#define FGAUGE_CAR_FACTOR               6.25 // uV/Rsns
#define FGAUGE_RSNS_FACTOR              0.02 // Ohm

//#define COMPASATE_OCV                   80 // mV for evb
#define COMPASATE_OCV                   40 // mV for phone

#define BATTERY_VOLTAGE_MINIMUM         3400
#define BATTERY_VOLTAGE_MAXIMUM         4200

#define BATTERY_CAPACITY_MAXIMUM        1213

#define TEMPERATURE_T0                  110
#define TEMPERATURE_T1                  0
#define TEMPERATURE_T2                  25
#define TEMPERATURE_T3                  50
#define TEMPERATURE_T                   255 // This should be fixed, never change the value

//#define BATT_CAPACITY                   1280
#define BATT_CAPACITY                   1254

#define ENABLE_SW_COULOMB_COUNTER       0 // 1 is enable, 0 is disable
//#define ENABLE_SW_COULOMB_COUNTER       1 // 1 is enable, 0 is disable

//#define FG_CURRENT_OFFSET_DISCHARGING 	31
#define FG_CURRENT_OFFSET_DISCHARGING 	0

#define FG_RESISTANCE 	20

#define FG_METER_RESISTANCE 	0
//#define FG_METER_RESISTANCE 	540 // current meter

//#define MAX_BOOTING_TIME_FGCURRENT	5*6 // 5 seconds, 6 points = 1s
#define MAX_BOOTING_TIME_FGCURRENT	1*10 // 10s

#if defined(CONFIG_POWER_EXT)
//#define OCV_BOARD_COMPESATE	32 //mV 
#define OCV_BOARD_COMPESATE	72 //mV 
#define R_FG_BOARD_BASE		1000
#define R_FG_BOARD_SLOPE	1000 //slope
#else
//#define OCV_BOARD_COMPESATE	0 //mV 
//#define OCV_BOARD_COMPESATE	48 //mV 
//#define OCV_BOARD_COMPESATE	25 //mV 
#define OCV_BOARD_COMPESATE	0 //mV 
#define R_FG_BOARD_BASE		1000
#define R_FG_BOARD_SLOPE	1000 //slope
//#define R_FG_BOARD_SLOPE	1057 //slope
//#define R_FG_BOARD_SLOPE	1075 //slope
#endif

#define Q_MAX_POS_50	1213
#define Q_MAX_POS_25	1254
#define Q_MAX_POS_0		1373
#define Q_MAX_NEG_10	971

#define Q_MAX_POS_50_H_CURRENT	1201
#define Q_MAX_POS_25_H_CURRENT	1234
#define Q_MAX_POS_0_H_CURRENT	1320
#define Q_MAX_NEG_10_H_CURRENT	746

#define R_FG_VALUE 				20 // mOhm, base is 20
#define CURRENT_DETECT_R_FG	100  //10mA

#define OSR_SELECT_7			0

#define CAR_TUNE_VALUE			102 //1.00

/////////////////////////////////////////////////////////////////////
// <DOD, Battery_Voltage> Table
/////////////////////////////////////////////////////////////////////
typedef struct _BATTERY_PROFILE_STRUC
{
    kal_int32 percentage;
    kal_int32 voltage;
} BATTERY_PROFILE_STRUC, *BATTERY_PROFILE_STRUC_P;

typedef enum
{
    T1_0C,
    T2_25C,
    T3_50C
} PROFILE_TEMPERATURE;

// T0 -10C
BATTERY_PROFILE_STRUC battery_profile_t0[] =
{
{0	,4171},     
{3	,4134},     
{6	,4075},     
{9	,4028},     
{12	,3971},     
{15	,3951},     
{18	,3937},     
{22	,3924},     
{25	,3912},     
{28	,3900},     
{31	,3887},     
{34	,3875},     
{37	,3864},     
{40	,3853},     
{43	,3845},     
{46	,3836},     
{49	,3829},     
{52	,3820},     
{55	,3812},     
{58	,3804},     
{61	,3795},     
{65	,3786},     
{68	,3775},     
{71	,3766},     
{74	,3754},     
{77	,3743},     
{80	,3732},     
{83	,3724},     
{86	,3715},     
{89	,3708},     
{92	,3694},     
{95	,3650},     
{98	,3589},     
{98	,3555},     
{99	,3537},     
{99	,3524},     
{99	,3517},     
{99	,3513},     
{100,3509},	   
{100,3507},	   
{100,3505},	   
{100,3504},	   
{100,3504},	   
{100,3502},	   
{100,3501},	   
{100,3500},	   
{100,3499},	   
{100,3499},	   
{100,3499},	   
{100,3498},	   
{100,3498},	   
{100,3498},	   
{100,3497},	   
{100,3496},	   
{100,3400}   
		
		

};      
        
// T1 0C
BATTERY_PROFILE_STRUC battery_profile_t1[] =
{
{0	, 4166},     
{2	, 4116},     
{5	, 4092},     
{7	, 4061},     
{9	, 4014},     
{11	, 3989},   
{13	, 3974},   
{15	, 3963},   
{17	, 3955},   
{20	, 3949},   
{22	, 3944},   
{24	, 3934},   
{26	, 3922},   
{28	, 3908},   
{30	, 3892},   
{33	, 3876},   
{35	, 3862},   
{37	, 3851},   
{39	, 3840},   
{41	, 3831},   
{43	, 3824},   
{46	, 3816},   
{48	, 3810},   
{50	, 3805},   
{52	, 3800},   
{54	, 3795},   
{57	, 3791},   
{59	, 3788},   
{61	, 3785},   
{63	, 3784},   
{65	, 3782},   
{67	, 3780},   
{70	, 3777},   
{72	, 3773},   
{74	, 3768},   
{76	, 3763},   
{78	, 3755},   
{80	, 3747},   
{83	, 3738},   
{85	, 3726},   
{87	, 3714},   
{89	, 3708},   
{91	, 3704},   
{93	, 3699},   
{96	, 3662},   
{98	, 3564},   
{99	, 3463},   
{100, 3423},	   
{100, 3404},	   
{100, 3395},	   
{100, 3389},	   
{100, 3383},	   
{100, 3380},	   
{100, 3377},	   
{100, 3376} 
					             

};

// T2 25C
BATTERY_PROFILE_STRUC battery_profile_t2[] =
{
{0	,4179},       
{2	,4158},       
{5	,4139},       
{7	,4120},       
{9	,4103},       
{12	,4088},     
{14	,4082},     
{17	,4064},     
{19	,4028},     
{21	,3999},     
{24	,3980},     
{26	,3966},     
{29	,3951},     
{31	,3935},     
{33	,3922},     
{36	,3908},     
{38	,3894},     
{40	,3883},     
{43	,3872},     
{45	,3863},     
{48	,3853},     
{50	,3844},     
{52	,3836},     
{55	,3828},     
{57	,3821},     
{59	,3813},     
{62	,3806},     
{64	,3798},     
{67	,3790},     
{69	,3781},     
{71	,3771},     
{74	,3761},     
{76	,3752},     
{79	,3742},     
{81	,3730},     
{83	,3719},     
{86	,3704},     
{88	,3696},     
{90	,3694},     
{93	,3691},     
{95	,3662},     
{98	,3569},     
{100,3401},	     
{100,3318},	     
{100,3296},	     
{100,3286},	     
{100,3282},	     
{100,3278},	     
{100,3275},	     
{100,3273},	     
{100,3273},	     
{100,3269},	     
{100,3268},	     
{100,3266},	     
{100,3263}
}; 

// T3 50C
BATTERY_PROFILE_STRUC battery_profile_t3[] =
{
{0	,4186},   
{2	,4165},   
{5	,4145},   
{7	,4126},   
{10	,4107},   
{12	,4090},   
{15	,4073},   
{17	,4062},   
{20	,4039},   
{22	,4013},   
{25	,3993},   
{27	,3976},   
{30	,3960},   
{32	,3945},   
{34	,3928},   
{37	,3913},   
{39	,3900},   
{42	,3889},   
{44	,3877},   
{47	,3867},   
{49	,3858},   
{52	,3849},   
{54	,3839},   
{57	,3830},   
{59	,3822},   
{62	,3811},   
{64	,3802},   
{66	,3790},   
{69	,3777},   
{71	,3766},   
{74	,3755},   
{76	,3744},   
{79	,3733},   
{81	,3720},   
{84	,3706},   
{86	,3694},   
{89	,3687},   
{91	,3683},   
{93	,3677},   
{96	,3629},   
{98	,3526},   
{100,3331},	 
{100,3278},	 
{100,3263},	 
{100,3259},	 
{100,3257},	 
{100,3257},	 
{100,3256},	 
{100,3253},	 
{100,3252},	 
{100,3250},	 
{100,3250},	 
{100,3248},	 
{100,3248},	 
{100,3245} 

};              

// battery profile for actual temperature. The size should be the same as T1, T2 and T3
BATTERY_PROFILE_STRUC battery_profile_temperature[] =
{
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
	  {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0}
};    

/////////////////////////////////////////////////////////////////////
// <Rbat, Battery_Voltage> Table
/////////////////////////////////////////////////////////////////////
typedef struct _R_PROFILE_STRUC
{
    kal_int32 resistance; // Ohm
    kal_int32 voltage;
} R_PROFILE_STRUC, *R_PROFILE_STRUC_P;

// T0 -10C
R_PROFILE_STRUC r_profile_t0[] =
{
{275,4171 },
{378,4134 },
{393,4075 },
{408,4028 },
{685,3971 },
{808,3951 },
{818,3937 },
{823,3924 },
{820,3912 },
{818,3900 },
{810,3887 },
{813,3875 },
{818,3864 },
{815,3853 },
{823,3845 },
{828,3836 },
{835,3829 },
{838,3820 },
{845,3812 },
{850,3804 },
{853,3795 },
{855,3786 },
{853,3775 },
{860,3766 },
{853,3754 },
{853,3743 },
{855,3732 },
{870,3724 },
{895,3715 },
{935,3708 },
{978,3694 },
{100,36503},
{975,3589 },
{890,3555 },
{843,3537 },
{813,3524 },
{798,3517 },
{783,3513 },
{780,3509 },
{775,3507 },
{763,3505 },
{763,3504 },
{770,3504 },
{763,3502 },
{758,3501 },
{758,3500 },
{753,3499 },
{750,3499 },
{750,3499 },
{753,3498 },
{755,3498 },
{745,3498 },
{758,3497 },
{760,3496 },
{513,3400 }
	

		
};

// T1 0C
R_PROFILE_STRUC r_profile_t1[] =
{
{245,4166},
{280,4116},
{438,4092},
{450,4061},
{448,4014},
{455,3989},
{460,3974},
{468,3963},
{473,3955},
{478,3949},
{485,3944},
{485,3934},
{483,3922},
{475,3908},
{465,3892},
{448,3876},
{440,3862},
{438,3851},
{435,3840},
{433,3831},
{435,3824},
{435,3816},
{438,3810},
{443,3805},
{445,3800},
{445,3795},
{448,3791},
{448,3788},
{448,3785},
{453,3784},
{458,3782},
{458,3780},
{458,3777},
{460,3773},
{460,3768},
{463,3763},
{460,3755},
{463,3747},
{463,3738},
{460,3726},
{460,3714},
{475,3708},
{503,3704},
{555,3699},
{585,3662},
{625,3564},
{660,3463},
{563,3423},
{513,3404},
{490,3395},
{475,3389},
{465,3383},
{458,3380},
{453,3377},
{445,3376}
}; 

// T2 25C
R_PROFILE_STRUC r_profile_t2[] =
{
{138,4179},
{228,4158},
{235,4139},
{238,4120},
{248,4103},
{248,4088},
{268,4082},
{278,4064},
{275,4028},
{278,3999},
{280,3980},
{275,3966},
{265,3951},
{253,3935},
{245,3922},
{243,3908},
{238,3894},
{238,3883},
{240,3872},
{243,3863},
{243,3853},
{245,3844},
{248,3836},
{250,3828},
{253,3821},
{253,3813},
{255,3806},
{253,3798},
{255,3790},
{250,3781},
{245,3771},
{243,3761},
{243,3752},
{245,3742},
{240,3730},
{243,3719},
{233,3704},
{230,3696},
{248,3694},
{273,3691},
{273,3662},
{268,3569},
{300,3401},
{295,3318},
{245,3296},
{223,3286},
{205,3282},
{198,3278},
{195,3275},
{190,3273},
{188,3273},
{185,3269},
{185,3268},
{183,3266},
{188,3263}
		 

}; 

// T3 50C
R_PROFILE_STRUC r_profile_t3[] =
{
{120,4186},
{160,4165},
{160,4145},
{165,4126},
{168,4107},
{173,4090},
{178,4073},
{188,4062},
{185,4039},
{188,4013},
{193,3993},
{188,3976},
{180,3960},
{178,3945},
{170,3928},
{170,3913},
{168,3900},
{170,3889},
{170,3877},
{175,3867},
{178,3858},
{180,3849},
{178,3839},
{183,3830},
{188,3822},
{183,3811},
{185,3802},
{180,3790},
{173,3777},
{170,3766},
{173,3755},
{173,3744},
{175,3733},
{170,3720},
{168,3706},
{168,3694},
{168,3687},
{175,3683},
{195,3677},
{180,3629},
{190,3526},
{233,3331},
{203,3278},
{165,3263},
{155,3259},
{143,3257},
{145,3257},
{143,3256},
{143,3253},
{140,3252},
{140,3250},
{138,3250},
{135,3248},
{138,3248},
{138,3245}

}; 

// r-table profile for actual temperature. The size should be the same as T1, T2 and T3
R_PROFILE_STRUC r_profile_temperature[] =
{
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
	  {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},   
    {0, 0}
};     


int fgauge_get_saddles(void);
BATTERY_PROFILE_STRUC_P fgauge_get_profile(kal_uint32 temperature);

int fgauge_get_saddles_r_table(void);
R_PROFILE_STRUC_P fgauge_get_profile_r_table(kal_uint32 temperature);
