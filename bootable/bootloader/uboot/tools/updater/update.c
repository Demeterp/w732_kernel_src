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
#include <exports.h>

extern unsigned long __dummy;
void do_reset (void);
void do_updater(void);

void _main(void)
{
    int i;
    printf("U-Boot Firmware Updater\n\n\n");
    printf("****************************************************\n"
	       "*  ATTENTION!! PLEASE READ THIS NOTICE CAREFULLY!  *\n"
	       "****************************************************\n\n"
	       "This program  will update your computer's  firmware.\n"
	       "Do NOT  remove the disk,  reset the  machine,  or do\n"
	       "anything that  might disrupt functionality.  If this\n");
    printf("Program fails, your computer  might be unusable, and\n"
	       "you will  need to return your  board for reflashing.\n"
	       "If you find this too risky,  remove the diskette and\n"
	       "switch off your  machine now.  Otherwise  press the \n"
	       "SPACE key now to start the process\n\n");
    do
    {
	char x;
	while (!tstc());
	x = getc();
	if (x == ' ') break;
    } while (1);

    do_updater();

    i = 5;

    printf("\nUpdate done. Please remove diskette.\n");
    printf("The machine will automatically reset in %d seconds\n", i);
    printf("You can switch off/reset now when the floppy is removed\n\n");

    while (i)
    {
	printf("Resetting in %d\r", i);
	udelay(1000000);
	i--;
    }
    do_reset();
    while (1);
}

void do_updater(void)
{
    unsigned long *addr = &__dummy + 65;
    unsigned long flash_size = flash_init();
    int rc;

    flash_sect_protect(0, 0xFFF00000, 0xFFF7FFFF);
    printf("Erasing ");
    flash_sect_erase(0xFFF00000, 0xFFF7FFFF);
    printf("Writing ");
    rc = flash_write((uchar *)addr, 0xFFF00000, 0x7FFFF);
    if (rc != 0) printf("\nFlashing failed due to error %d\n", rc);
    else printf("\ndone\n");
    flash_sect_protect(1, 0xFFF00000, 0xFFF7FFFF);
}
