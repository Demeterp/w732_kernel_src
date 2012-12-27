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

/*
 * asm/status_led.h
 *
 * MPC8xx/MPC8260/MPC5xx based status led support functions
 */

#ifndef __ASM_STATUS_LED_H__
#define __ASM_STATUS_LED_H__

/* if not overriden */
#ifndef CONFIG_BOARD_SPECIFIC_LED
# if defined(CONFIG_8xx)
#  include <mpc8xx.h>
# elif defined(CONFIG_8260)
#  include <mpc8260.h>
# elif defined(CONFIG_5xx)
#  include <mpc5xx.h>
# else
#  error CPU specific Status LED header file missing.
#endif

/* led_id_t is unsigned long mask */
typedef unsigned long led_id_t;

static inline void __led_init (led_id_t mask, int state)
{
	volatile immap_t *immr = (immap_t *) CONFIG_SYS_IMMR;

#ifdef STATUS_LED_PAR
	immr->STATUS_LED_PAR &= ~mask;
#endif
#ifdef STATUS_LED_ODR
	immr->STATUS_LED_ODR &= ~mask;
#endif

#if (STATUS_LED_ACTIVE == 0)
	if (state == STATUS_LED_ON)
		immr->STATUS_LED_DAT &= ~mask;
	else
		immr->STATUS_LED_DAT |= mask;
#else
	if (state == STATUS_LED_ON)
		immr->STATUS_LED_DAT |= mask;
	else
		immr->STATUS_LED_DAT &= ~mask;
#endif
#ifdef STATUS_LED_DIR
	immr->STATUS_LED_DIR |= mask;
#endif
}

static inline void __led_toggle (led_id_t mask)
{
	((immap_t *) CONFIG_SYS_IMMR)->STATUS_LED_DAT ^= mask;
}

static inline void __led_set (led_id_t mask, int state)
{
	volatile immap_t *immr = (immap_t *) CONFIG_SYS_IMMR;

#if (STATUS_LED_ACTIVE == 0)
	if (state == STATUS_LED_ON)
		immr->STATUS_LED_DAT &= ~mask;
	else
		immr->STATUS_LED_DAT |= mask;
#else
	if (state == STATUS_LED_ON)
		immr->STATUS_LED_DAT |= mask;
	else
		immr->STATUS_LED_DAT &= ~mask;
#endif

}

#endif

#endif	/* __ASM_STATUS_LED_H__ */
