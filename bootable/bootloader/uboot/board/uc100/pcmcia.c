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

#if (defined(CONFIG_CMD_IDE)) && defined(CONFIG_IDE_8xx_PCCARD)
#define	CONFIG_PCMCIA
#endif

#ifdef	CONFIG_PCMCIA

#define PCMCIA_BOARD_MSG "UC100"

/*
 * Remark: don't turn off OE "__MY_PCMCIA_GCRX_CXOE" on UC100 board.
 *         This leads to board-hangup! (sr, 8 Dez. 2004)
 */
static void cfg_ports (void)
{
	volatile immap_t	*immap;

	immap = (immap_t *)CONFIG_SYS_IMMR;

	/*
	* Configure Port A for MAX1602 PC-Card Power-Interface Switch
	*/
	immap->im_ioport.iop_padat &= ~0x8000;	/* set port x output to low */
	immap->im_ioport.iop_padir |= 0x8000;	/* enable port x as output */

	debug ("Set Port A: PAR: %08x DIR: %08x DAT: %08x\n",
	       immap->im_ioport.iop_papar, immap->im_ioport.iop_padir,
	       immap->im_ioport.iop_padat);
}

int pcmcia_hardware_enable(int slot)
{
	volatile immap_t	*immap;
	volatile cpm8xx_t	*cp;
	volatile pcmconf8xx_t	*pcmp;
	volatile sysconf8xx_t	*sysp;
	uint reg, mask;

	debug ("hardware_enable: " PCMCIA_BOARD_MSG " Slot %c\n", 'A'+slot);

	udelay(10000);

	immap = (immap_t *)CONFIG_SYS_IMMR;
	sysp  = (sysconf8xx_t *)(&(((immap_t *)CONFIG_SYS_IMMR)->im_siu_conf));
	pcmp  = (pcmconf8xx_t *)(&(((immap_t *)CONFIG_SYS_IMMR)->im_pcmcia));
	cp    = (cpm8xx_t *)(&(((immap_t *)CONFIG_SYS_IMMR)->im_cpm));

	/* Configure Ports for TPS2211A PC-Card Power-Interface Switch */
	cfg_ports ();

	/*
	* Configure SIUMCR to enable PCMCIA port B
	* (VFLS[0:1] are not used for debugging, we connect FRZ# instead)
	*/
	sysp->sc_siumcr &= ~SIUMCR_DBGC11;	/* set DBGC to 00 */

	/* clear interrupt state, and disable interrupts */
	pcmp->pcmc_pscr =  PCMCIA_MASK(_slot_);
	pcmp->pcmc_per &= ~PCMCIA_MASK(_slot_);

	/*
	* Disable interrupts, DMA, and PCMCIA buffers
	* (isolate the interface) and assert RESET signal
	*/
	debug ("Disable PCMCIA buffers and assert RESET\n");
	reg  = 0;
	reg |= __MY_PCMCIA_GCRX_CXRESET;	/* active high */
	PCMCIA_PGCRX(_slot_) = reg;
	udelay(500);

	/*
	* Make sure there is a card in the slot, then configure the interface.
	*/
	udelay(10000);
	debug ("[%d] %s: PIPR(%p)=0x%x\n",
	       __LINE__,__FUNCTION__,
	       &(pcmp->pcmc_pipr),pcmp->pcmc_pipr);
	if (pcmp->pcmc_pipr & (0x18000000 >> (slot << 4))) {
		printf ("   No Card found\n");
		return (1);
	}

	/*
	* Power On.
	*/
	mask = PCMCIA_VS1(slot) | PCMCIA_VS2(slot);
	reg  = pcmp->pcmc_pipr;
	debug ("PIPR: 0x%x ==> VS1=o%s, VS2=o%s\n",
	       reg,
	       (reg&PCMCIA_VS1(slot))?"n":"ff",
	       (reg&PCMCIA_VS2(slot))?"n":"ff");
	if ((reg & mask) == mask) {
		puts (" 5.0V card found: ");
	} else {
		puts (" 3.3V card found: ");
	}

	/*  switch VCC on */
	immap->im_ioport.iop_padat |= 0x8000; /* power enable 3.3V */

	udelay(10000);

	debug ("Enable PCMCIA buffers and stop RESET\n");
	reg  =  PCMCIA_PGCRX(_slot_);
	reg &= ~__MY_PCMCIA_GCRX_CXRESET;	/* active high */
	reg &= ~__MY_PCMCIA_GCRX_CXOE;		/* active low  */
	PCMCIA_PGCRX(_slot_) = reg;

	udelay(250000);	/* some cards need >150 ms to come up :-( */

	debug ("# hardware_enable done\n");

	return (0);
}


#if defined(CONFIG_CMD_PCMCIA)
int pcmcia_hardware_disable(int slot)
{
	volatile immap_t	*immap;
	volatile cpm8xx_t	*cp;
	volatile pcmconf8xx_t	*pcmp;
	u_long reg;

	debug ("hardware_disable: " PCMCIA_BOARD_MSG " Slot %c\n", 'A'+slot);

	immap = (immap_t *)CONFIG_SYS_IMMR;
	pcmp  = (pcmconf8xx_t *)(&(((immap_t *)CONFIG_SYS_IMMR)->im_pcmcia));

	/* switch VCC off */
	immap->im_ioport.iop_padat &= ~0x8000; /* power disable 3.3V */

	/* Configure PCMCIA General Control Register */
	debug ("Disable PCMCIA buffers and assert RESET\n");
	reg  = 0;
	reg |= __MY_PCMCIA_GCRX_CXRESET;	/* active high */
	PCMCIA_PGCRX(_slot_) = reg;

	udelay(10000);

	return (0);
}
#endif


int pcmcia_voltage_set(int slot, int vcc, int vpp)
{
	volatile immap_t	*immap;
	volatile pcmconf8xx_t	*pcmp;
	u_long reg;

	debug ("voltage_set: "
			PCMCIA_BOARD_MSG
			" Slot %c, Vcc=%d.%d, Vpp=%d.%d\n",
	'A'+slot, vcc/10, vcc%10, vpp/10, vcc%10);

	immap = (immap_t *)CONFIG_SYS_IMMR;
	pcmp  = (pcmconf8xx_t *)(&(((immap_t *)CONFIG_SYS_IMMR)->im_pcmcia));
	/*
	* Disable PCMCIA buffers (isolate the interface)
	* and assert RESET signal
	*/
	debug ("Disable PCMCIA buffers and assert RESET\n");
	reg  = PCMCIA_PGCRX(_slot_);
	reg |= __MY_PCMCIA_GCRX_CXRESET;	/* active high */
	PCMCIA_PGCRX(_slot_) = reg;
	udelay(500);

	/*
	* Configure Port C pins for
	* 5 Volts Enable and 3 Volts enable,
	* Turn all power pins to Hi-Z
	*/
	debug ("PCMCIA power OFF\n");
	cfg_ports ();	/* Enables switch, but all in Hi-Z */

	debug ("Enable PCMCIA buffers and stop RESET\n");
	reg  =  PCMCIA_PGCRX(_slot_);
	reg &= ~__MY_PCMCIA_GCRX_CXRESET;	/* active high */
	reg &= ~__MY_PCMCIA_GCRX_CXOE;		/* active low  */
	PCMCIA_PGCRX(_slot_) = reg;
	udelay(500);

	debug ("voltage_set: " PCMCIA_BOARD_MSG " Slot %c, DONE\n",
	       slot+'A');
	return (0);
}

#endif	/* CONFIG_PCMCIA */
