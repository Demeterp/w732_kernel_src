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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    mt6573_auxadc.c
 *
 * Project:
 * --------
 *   Android_Software
 *
 * Description:
 * ------------
 *   This Module defines functions of mt6573 AUXADC
 *
 * Author:
 * -------
 * James Lo
 *
 ****************************************************************************/
#include <common.h>
#include <asm/io.h>

#include <asm/arch/mt6575_auxadc_sw.h>
#include <asm/arch/mt6575_auxadc_hw.h>

///////////////////////////////////////////////////////////////////////////////////////////
//// Define
typedef unsigned short  kal_uint16;

#define DRV_Reg(addr)               (*(volatile kal_uint16 *)(addr))
#define DRV_WriteReg(addr,data)     ((*(volatile kal_uint16 *)(addr)) = (kal_uint16)(data))

#define DRV_ClearBits(addr,data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp &=~(data);\
   DRV_WriteReg(addr,temp);\
}

#define DRV_SetBits(addr,data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp |= (data);\
   DRV_WriteReg(addr,temp);\
}

#define DRV_SetData(addr, bitmask, value)     {\
   kal_uint16 temp;\
   temp = (~(bitmask)) & DRV_Reg(addr);\
   temp |= (value);\
   DRV_WriteReg(addr,temp);\
}

#define AUXADC_DRV_ClearBits16(addr, data)           DRV_ClearBits(addr,data)
#define AUXADC_DRV_SetBits16(addr, data)             DRV_SetBits(addr,data)
#define AUXADC_DRV_WriteReg16(addr, data)            DRV_WriteReg(addr, data)
#define AUXADC_DRV_ReadReg16(addr)                   DRV_Reg(addr)
#define AUXADC_DRV_SetData16(addr, bitmask, value)   DRV_SetData(addr, bitmask, value)

#define AUXADC_DVT_DELAYMACRO(u4Num)                                     \
{                                                                        \
    unsigned int u4Count = 0 ;                                                 \
    for (u4Count = 0; u4Count < u4Num; u4Count++ );                      \
}

#define AUXADC_SET_BITS(BS,REG)       ((*(volatile u32*)(REG)) |= (u32)(BS))
#define AUXADC_CLR_BITS(BS,REG)       ((*(volatile u32*)(REG)) &= ~((u32)(BS)))

#define VOLTAGE_FULL_RANGE 	2500 // VA voltage
#define ADC_PRECISE 		4096 // 12 bits

///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//// Internal API
inline static void mt65xx_ADC_2G_power_up(void)
{
    //2010/07/27: mt6573, the ADC 2G power on is controlled by APMCU_CG_CLR0
    //#define PDN_CLR0 (0xF7026308)  
    #define PDN_CLR0 (0x70026308)  
    unsigned int poweron = 1 << 4;
    AUXADC_SET_BITS(poweron, PDN_CLR0);
}

inline static void mt65xx_ADC_2G_power_down(void)
{
    //2010/07/27: mt6573, the ADC 2G power on is controlled by APMCU_CG_SET0
    //#define PDN_SET0 (0xF7026304)  
    #define PDN_SET0 (0x70026304)  
    unsigned int poweroff = 1 << 4;
    AUXADC_SET_BITS(poweroff, PDN_SET0);
}

inline static void mt65xx_ADC_3G_power_up(void)
{
    //2010/07/27: mt6573, the ADC 3G power on is controlled by APMCU_CG_CLR0
    //#define PDN_CLR0 (0xF7026308)  
    #define PDN_CLR0 (0x70026308)  
    unsigned int poweron = 1 << 13;
    AUXADC_SET_BITS(poweron, PDN_CLR0);
}

inline static void mt65xx_ADC_3G_power_down(void)
{
    //2010/07/27: mt6573, the ADC 3G power on is controlled by APMCU_CG_SET0
    //#define PDN_SET0 (0xF7026304)  
    #define PDN_SET0 (0x70026304)  
    unsigned int poweroff = 1 << 13;
    AUXADC_SET_BITS(poweroff, PDN_SET0);
}
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//// Common API
#if 0
int IMM_GetOneChannelValue(int dwChannel, int deCount)
{
    unsigned int u4Sample_times = 0;
    unsigned int dat = 0;
	unsigned int u4channel[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	unsigned int adc_esult=0;

    /* Enable ADC power bit */    
    mt65xx_ADC_2G_power_up();
	mt65xx_ADC_3G_power_up();

    /* Initialize ADC control register */
    AUXADC_DRV_WriteReg16(AUXADC_CON0, 0);
    AUXADC_DRV_WriteReg16(AUXADC_CON1, 0);    
    AUXADC_DRV_WriteReg16(AUXADC_CON2, 0);    
    AUXADC_DRV_WriteReg16(AUXADC_CON3, 0);   

    do
    {
        //pmic_adc_vbat_enable(KAL_TRUE);		// move to the whom driver
        //pmic_adc_isense_enable(KAL_TRUE); 	// move to the whom driver

        AUXADC_DRV_WriteReg16(AUXADC_CON1, 0);        
        AUXADC_DRV_WriteReg16(AUXADC_CON1, 0x1FFF);
         
        AUXADC_DVT_DELAYMACRO(1000);

        /* Polling until bit STA = 0 */
        while (0 != (0x01 & AUXADC_DRV_ReadReg16(AUXADC_CON3)));          

        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT0);        
        u4channel[0]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT1);        
        u4channel[1]  += (dat & 0x0FFF);   
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT2);        
        u4channel[2]  += (dat & 0x0FFF);   
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT3);        
        u4channel[3]  += (dat & 0x0FFF);   
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT4);
        u4channel[4]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT5);
        u4channel[5]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT6);
        u4channel[6]  += (dat & 0x0FFF);  
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT7);
        u4channel[7]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT8);
        u4channel[8]  += (dat & 0x0FFF);    
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT9);
        u4channel[9]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT10);
        u4channel[10] += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT11);
        u4channel[11] += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT12);
        u4channel[12] += (dat & 0x0FFF);		
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT13);
        u4channel[13] += (dat & 0x0FFF);
        
        u4Sample_times++;
    }
    while (u4Sample_times < deCount);

    /* Disable ADC power bit */    
    mt65xx_ADC_2G_power_down();
	mt65xx_ADC_3G_power_down();

    #if 0
    printf("BAT_GetVoltage : channel_0  = %d / %d \n", u4channel[0], u4Sample_times );
    printf("BAT_GetVoltage : channel_1  = %d / %d \n", u4channel[1], u4Sample_times );
    printf("BAT_GetVoltage : channel_2  = %d / %d \n", u4channel[2], u4Sample_times );
    printf("BAT_GetVoltage : channel_3  = %d / %d \n", u4channel[3], u4Sample_times );
    printf("BAT_GetVoltage : channel_4  = %d / %d \n", u4channel[4], u4Sample_times );
    printf("BAT_GetVoltage : channel_5  = %d / %d \n", u4channel[5], u4Sample_times );
    printf("BAT_GetVoltage : channel_6  = %d / %d \n", u4channel[6], u4Sample_times );
    printf("BAT_GetVoltage : channel_7  = %d / %d \n", u4channel[7], u4Sample_times );
    printf("BAT_GetVoltage : channel_8  = %d / %d \n", u4channel[8], u4Sample_times );
    printf("BAT_GetVoltage : channel_9  = %d / %d \n", u4channel[9], u4Sample_times );
    printf("BAT_GetVoltage : channel_10 = %d / %d \n", u4channel[10], u4Sample_times );
    printf("BAT_GetVoltage : channel_11 = %d / %d \n", u4channel[11], u4Sample_times );
    printf("BAT_GetVoltage : channel_12 = %d / %d \n", u4channel[12], u4Sample_times );
    printf("BAT_GetVoltage : channel_13 = %d / %d \n", u4channel[13], u4Sample_times );	
    #endif

	/* Value averaging  */ 
    u4channel[0]  = u4channel[0]  / deCount;
    u4channel[1]  = u4channel[1]  / deCount;
    u4channel[2]  = u4channel[2]  / deCount;
    u4channel[3]  = u4channel[3]  / deCount;
    u4channel[4]  = u4channel[4]  / deCount;
    u4channel[5]  = u4channel[5]  / deCount;
    u4channel[6]  = u4channel[6]  / deCount;
    u4channel[7]  = u4channel[7]  / deCount;
    u4channel[8]  = u4channel[8]  / deCount;
    u4channel[9]  = u4channel[9]  / deCount;
    u4channel[10] = u4channel[10] / deCount;
    u4channel[11] = u4channel[11] / deCount;
    u4channel[12] = u4channel[12] / deCount;
    u4channel[13] = u4channel[13] / deCount;

	adc_esult = ((u4channel[dwChannel]*VOLTAGE_FULL_RANGE)/ADC_PRECISE);

	return adc_esult;
	
}
#else//LK@ALPS00256820
static int adc_auto_set =0;//LK@add, not used now
int IMM_GetOneChannelValue(int dwChannel)
{
   unsigned int channel[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int idle_count =0;
   int data_ready_count=0;
   unsigned int adc_result=0;//LK@add
 //  unsigned int i = 0, data = 0;
 //  unsigned int poweron, poweroff;
   // Polling until bit STA = 0

   while ((*(volatile u16 *)AUXADC_CON3) & 0x01) 
   {
        printf("[adc_api]: wait for module idle\n");
        //msleep(100);
        udelay(100000);
        idle_count++;
        if(idle_count>30)
        {
            //wait for idle time out
            printf("[adc_api]: wait for aux/adc idle time out\n");
            return -1;
        }
   } 
   while ((*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & (1<<12)) 
   {
       printf("[adc_api]: wait for channel[%d] data ready\n",dwChannel);

       //  msleep(100);
       udelay(100000);
       data_ready_count++;
       if(data_ready_count>30)
       {
           //wait for idle time out
           printf("[adc_api]: wait for channel[%d] data ready time out\n",dwChannel);
           return -2;
       }
   }  
   //read data
   if(0==adc_auto_set)
   {
      //clear bit
      *(volatile u16 *)AUXADC_CON1 = *(volatile u16 *)AUXADC_CON1 & (~(1 << dwChannel));
      // msleep(20);
      udelay(20000);
      //set bit
      *(volatile u16 *)AUXADC_CON1 = *(volatile u16 *)AUXADC_CON1 | (1 << dwChannel);
   }
   // msleep(20);
   udelay(20000);
   //read data
   channel[dwChannel] = (*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & 0x0FFF;
#if 0//LK@mask
   if(NULL != rawdata)
   {
      *rawdata = channel[dwChannel];
   }
   //printk("[adc_api: imm mode raw data => channel[%d] = %d\n",dwChannel, channel[dwChannel]);

   //printk("[adc_api]: imm mode => channel[%d] = %d.%d\n", dwChannel, (channel[dwChannel] * 250 / 4096 / 100), ((channel[dwChannel] * 250 / 4096) % 100));

   data[0] = (channel[dwChannel] * 250 / 4096 / 100);
   data[1] = ((channel[dwChannel] * 250 / 4096) % 100);
#endif
    adc_result = channel[dwChannel]* 250 / 4096;
  // msleep(20);
   udelay(20000);
   if(0 == adc_auto_set)
   {
       //clear bit
       *(volatile u16 *)AUXADC_CON1 = *(volatile u16 *)AUXADC_CON1 & (~(1 << dwChannel));
   }

   return adc_result;
}

#endif
///////////////////////////////////////////////////////////////////////////////////////////

