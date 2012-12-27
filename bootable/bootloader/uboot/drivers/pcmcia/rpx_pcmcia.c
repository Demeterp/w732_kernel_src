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

/* -------------------------------------------------------------------- */
/* RPX Boards from Embedded Planet					*/
/* -------------------------------------------------------------------- */
#include <common.h>
#ifdef CONFIG_8xx
#include <mpc8xx.h>
#endif
#include <pcmcia.h>

#undef	CONFIG_PCMCIA

#if defined(CONFIG_CMD_PCMCIA)
#define	CONFIG_PCMCIA
#endif

#if defined(CONFIG_CMD_IDE) && defined(CONFIG_IDE_8xx_PCCARD)
#define	CONFIG_PCMCIA
#endif

#if	defined(CONFIG_PCMCIA)	\
	&& (defined(CONFIG_RPXCLASSIC) || defined(CONFIG_RPXLITE))

#define	PCMCIA_BOARD_MSG	"RPX CLASSIC or RPX LITE"

int pcmcia_voltage_set(int slot, int vcc, int vpp)
{
	u_long reg = 0;

	switch(vcc) {
		case 0: break;
		case 33: reg |= BCSR1_PCVCTL4; break;
		case 50: reg |= BCSR1_PCVCTL5; break;
		default: return 1;
	}

	switch(vpp) {
		case 0: break;
		case 33:
		case 50:
			if(vcc == vpp)
				reg |= BCSR1_PCVCTL6;
			else
				return 1;
			break;
		case 120:
			reg |= BCSR1_PCVCTL7;
			default: return 1;
	}

	/* first, turn off all power */
	*((uint *)RPX_CSR_ADDR) &= ~(BCSR1_PCVCTL4 | BCSR1_PCVCTL5
			| BCSR1_PCVCTL6 | BCSR1_PCVCTL7);

	/* enable new powersettings */
	*((uint *)RPX_CSR_ADDR) |= reg;

	return 0;
}

int pcmcia_hardware_enable (int slot)
{
	return 0;	/* No hardware to enable */
}

#if defined(CONFIG_CMD_PCMCIA)
static int pcmcia_hardware_disable(int slot)
{
	return 0;	/* No hardware to disable */
}
#endif


#endif	/* CONFIG_PCMCIA && (CONFIG_RPXCLASSIC || CONFIG_RPXLITE) */
