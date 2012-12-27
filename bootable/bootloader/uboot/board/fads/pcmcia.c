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

#include <common.h>
#include <mpc8xx.h>
#include <pcmcia.h>

#undef	CONFIG_PCMCIA

#if defined(CONFIG_CMD_PCMCIA)
#define	CONFIG_PCMCIA
#endif

#if defined(CONFIG_CMD_IDE) && defined(CONFIG_IDE_8xx_PCCARD)
#define	CONFIG_PCMCIA
#endif

#ifdef	CONFIG_PCMCIA

#ifdef	CONFIG_ADS
#define	PCMCIA_BOARD_MSG "ADS"
#else
#define	PCMCIA_BOARD_MSG "FADS"
#endif

int pcmcia_voltage_set(int slot, int vcc, int vpp)
{
	u_long reg = 0;

	switch(vpp) {
		case 0: reg = 0; break;
		case 50: reg = 1; break;
		case 120: reg = 2; break;
		default: return 1;
	}

	switch(vcc) {
		case 0: reg = 0; break;
#ifdef CONFIG_ADS
	case 50: reg = BCSR1_PCCVCCON; break;
#endif
#ifdef CONFIG_FADS
	case 33: reg = BCSR1_PCCVCC0 | BCSR1_PCCVCC1; break;
	case 50: reg = BCSR1_PCCVCC1; break;
#endif
	default: return 1;
	}

	/* first, turn off all power */

#ifdef CONFIG_ADS
	*((uint *)BCSR1) |= BCSR1_PCCVCCON;
#endif
#ifdef CONFIG_FADS
	*((uint *)BCSR1) &= ~(BCSR1_PCCVCC0 | BCSR1_PCCVCC1);
#endif
	*((uint *)BCSR1) &= ~BCSR1_PCCVPP_MASK;

	/* enable new powersettings */

#ifdef CONFIG_ADS
	*((uint *)BCSR1) &= ~reg;
#endif
#ifdef CONFIG_FADS
	*((uint *)BCSR1) |= reg;
#endif

	*((uint *)BCSR1) |= reg << 20;

	return 0;
}

int pcmcia_hardware_enable(int slot)
{
	*((uint *)BCSR1) &= ~BCSR1_PCCEN;
	return 0;
}

#if defined(CONFIG_CMD_PCMCIA)
int pcmcia_hardware_disable(int slot)
{
	*((uint *)BCSR1) &= ~BCSR1_PCCEN;
	return 0;
}
#endif

#endif	/* CONFIG_PCMCIA */
