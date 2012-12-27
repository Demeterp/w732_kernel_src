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
#include <config.h>

#include <pcmcia.h>
#include <asm/arch/pxa-regs.h>
#include <asm/io.h>

static inline void msWait(unsigned msVal)
{
	udelay(msVal*1000);
}

int pcmcia_on (void)
{
	unsigned int reg_arr[] = {
		0x48000028, CONFIG_SYS_MCMEM0_VAL,
		0x4800002c, CONFIG_SYS_MCMEM1_VAL,
		0x48000030, CONFIG_SYS_MCATT0_VAL,
		0x48000034, CONFIG_SYS_MCATT1_VAL,
		0x48000038, CONFIG_SYS_MCIO0_VAL,
		0x4800003c, CONFIG_SYS_MCIO1_VAL,

		0, 0
	};
	int i, rc;

#ifdef CONFIG_EXADRON1
	int cardDetect;
	volatile unsigned int *v_pBCRReg =
			(volatile unsigned int *) 0x08000000;
#endif

	debug ("%s\n", __FUNCTION__);

	i = 0;
	while (reg_arr[i]) {
		(*(volatile unsigned int *) reg_arr[i]) |= reg_arr[i + 1];
		i += 2;
	}
	udelay (1000);

	debug ("%s: programmed mem controller \n", __FUNCTION__);

#ifdef CONFIG_EXADRON1

/*define useful BCR masks */
#define BCR_CF_INIT_VAL			    0x00007230
#define BCR_CF_PWRON_BUSOFF_RESETOFF_VAL    0x00007231
#define BCR_CF_PWRON_BUSOFF_RESETON_VAL     0x00007233
#define BCR_CF_PWRON_BUSON_RESETON_VAL      0x00007213
#define BCR_CF_PWRON_BUSON_RESETOFF_VAL     0x00007211

	/* we see from the GPIO bit if the card is present */
	cardDetect = !(GPLR0 & GPIO_bit (14));

	if (cardDetect) {
		printf ("No PCMCIA card found!\n");
	}

	/* reset the card via the BCR line */
	*v_pBCRReg = (unsigned) BCR_CF_INIT_VAL;
	msWait (500);

	*v_pBCRReg = (unsigned) BCR_CF_PWRON_BUSOFF_RESETOFF_VAL;
	msWait (500);

	*v_pBCRReg = (unsigned) BCR_CF_PWRON_BUSOFF_RESETON_VAL;
	msWait (500);

	*v_pBCRReg = (unsigned) BCR_CF_PWRON_BUSON_RESETON_VAL;
	msWait (500);

	*v_pBCRReg = (unsigned) BCR_CF_PWRON_BUSON_RESETOFF_VAL;
	msWait (1500);

	/* enable address bus */
	GPCR1 = 0x01;
	/* and the first CF slot */
	MECR = 0x00000002;

#endif /* EXADRON 1 */

	rc = check_ide_device (0);	/* use just slot 0 */

	return rc;
}

#if defined(CONFIG_CMD_PCMCIA)
int pcmcia_off (void)
{
	return 0;
}
#endif
