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

/*
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/mt65xx.h>
#include <asm/arch/mt65xx_typedefs.h>
#include <cust_key.h>
#include <asm/arch/mt6575_key.h>
#include <asm/arch/boot_mode.h>
#include <asm/arch/mt6575_pmic6329.h>

extern mt6575_detect_powerkey(void);
extern mt6575_detect_homekey(void);

bool mt6575_detect_key(unsigned short key)	/* key: HW keycode */
{
	unsigned short idx, bit, din;

	if (key >= KPD_NUM_KEYS)
		return false;

	if (key % 9 == 8)
		key = 8;

#if 0 //KPD_PWRKEY_USE_EINT 
    if (key == 8)
    {                         /* Power key */
        idx = KPD_PWRKEY_EINT_GPIO / 16;
        bit = KPD_PWRKEY_EINT_GPIO % 16;

        din = DRV_Reg16 (GPIO_DIN_BASE + (idx << 4)) & (1U << bit);
        din >>= bit;
        if (din == KPD_PWRKEY_GPIO_DIN)
        {
            dbg_print ("power key is pressed\n");
            return true;
        }
        return false;
    }
#else // check by PMIC
    if (key == 8)
    {                         /* Power key */
        if (1 == mt6575_detect_powerkey())
        {
            //dbg_print ("power key is pressed\n");
            return true;
        }
        return false;
    }    
#endif

	if(get_chip_eco_ver() == CHIP_E2) /* for E1 board's VR301 issue */
	{
		if (key == MT65XX_PMIC_RST_KEY) 
		{
			if (1 == mt6575_detect_homekey())
			{
				return true;
			}
			// return false; /* to support common EVB */
		}
	}

	idx = key / 16;
	bit = key % 16;

	din = DRV_Reg16(KP_MEM1 + (idx << 2)) & (1U << bit);
	if (!din) {
		printf("key %d is pressed\n", key);
		return true;
	}
	return false;
}

bool mt6575_detect_pmic_just_rst()
{
	kal_uint8 just_rst;
	
	printf("detecting pmic just reset\n");
	if(get_chip_eco_ver() == CHIP_E2)
	{
		pmic_read_interface(0x15, &just_rst, 0x01, 0x07);
		if(just_rst)
		{
			printf("Just recover form a reset\n"); 
			pmic_bank1_config_interface(0x22, 0x01, 0x01, 0x07);
			return TRUE;
		}
	}
	return FALSE;
}
