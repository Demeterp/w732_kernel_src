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
 * ppmc7xx.c
 * ---------
 *
 * Main board-specific routines for Wind River PPMC 7xx/74xx board.
 *
 * By Richard Danter (richard.danter@windriver.com)
 * Copyright (C) 2005 Wind River Systems
 */

#include <common.h>
#include <command.h>
#include <netdev.h>


/* Define some MPC107 (memory controller) registers */
#define MPC107_EUMB_GCR         0xfce41020
#define MPC107_EUMB_IACKR       0xfce600a0


/* Function prototypes */
extern void unlock_ram_in_cache( void );
extern void _start_warm(void);


/*
 * initdram()
 *
 * This function normally initialises the (S)DRAM of the system. For this board
 * the SDRAM was already initialised by board_asm_init (see init.S) so we just
 * return the size of RAM.
 */
phys_size_t initdram( int board_type )
{
    return CONFIG_SYS_SDRAM_SIZE;
}


/*
 * after_reloc()
 *
 * This is called after U-Boot has been copied from Flash/ROM to RAM. It gives
 * us an opportunity to do some additional setup before the rest of the system
 * is initialised. We don't need to do anything, so we just call board_init_r()
 * which should never return.
 */
void after_reloc( ulong dest_addr, gd_t* gd )
{
	/* Jump to the main U-Boot board init code */
	board_init_r( gd, dest_addr );
}


/*
 * checkboard()
 *
 * We could do some board level checks here, such as working out what version
 * it is, but for this board we simply display it's name (on the console).
 */
int checkboard( void )
{
    puts( "Board: Wind River PPMC 7xx/74xx\n" );
    return 0;
}


/*
 * misc_init_r
 *
 * Used for other setup which needs to be done late in the bring-up phase.
 */
int misc_init_r( void )
{
	/* Reset the EPIC and clear pending interrupts */
	out32r(MPC107_EUMB_GCR, 0xa0000000);
	while( in32r( MPC107_EUMB_GCR ) & 0x80000000 );
	out32r( MPC107_EUMB_GCR, 0x20000000 );
	while( in32r( MPC107_EUMB_IACKR ) != 0xff );

	/* Enable the I-Cache */
	icache_enable();

	return 0;
}


/*
 * do_reset()
 *
 * Shell command to reset the board.
 */
void do_reset( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[] )
{
	printf( "Resetting...\n" );

	/* Disabe and invalidate cache */
	icache_disable();
	dcache_disable();

	/* Jump to warm start (in RAM) */
	_start_warm();

	/* Should never get here */
	while(1);
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}
