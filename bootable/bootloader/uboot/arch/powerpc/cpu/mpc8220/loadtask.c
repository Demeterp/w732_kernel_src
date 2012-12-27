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
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * This file is based on code
 * (C) Copyright Motorola, Inc., 2000
 */

#include <common.h>
#include <mpc8220.h>

/* Multichannel DMA microcode */
extern int taskTable;

void loadtask (int basetask, int tasks)
{
	int *sram = (int *) (MMAP_SRAM + 512);
	int *task_org = &taskTable;
	unsigned int start, offset, end;
	int i;

#ifdef DEBUG
	printf ("basetask = %d, tasks = %d\n", basetask, tasks);
	printf ("task_org = 0x%08x\n", (unsigned int) task_org);
#endif

	/* setup TaskBAR register */
	*(vu_long *) MMAP_DMA = (MMAP_SRAM + 512);

	/* relocate task table entries */
	offset = (unsigned int) sram;
	for (i = basetask; i < basetask + tasks; i++) {
		sram[i * 8 + 0] = task_org[i * 8 + 0] + offset;
		sram[i * 8 + 1] = task_org[i * 8 + 1] + offset;
		sram[i * 8 + 2] = task_org[i * 8 + 2] + offset;
		sram[i * 8 + 3] = task_org[i * 8 + 3] + offset;
		sram[i * 8 + 4] = task_org[i * 8 + 4];
		sram[i * 8 + 5] = task_org[i * 8 + 5];
		sram[i * 8 + 6] = task_org[i * 8 + 6] + offset;
		sram[i * 8 + 7] = task_org[i * 8 + 7];
	}

	/* relocate task descriptors */
	start = (sram[basetask * 8] - (unsigned int) sram);
	end = (sram[(basetask + tasks - 1) * 8 + 1] - (unsigned int) sram);

#ifdef DEBUG
	printf ("TDT start = 0x%08x, end = 0x%08x\n", start, end);
#endif

	start /= 4;
	end /= 4;
	for (i = start; i <= end; i++) {
		sram[i] = task_org[i];
	}

	/* relocate variables */
	start = (sram[basetask * 8 + 2] - (unsigned int) sram);
	end = (sram[(basetask + tasks - 1) * 8 + 2] + 256 -
	       (unsigned int) sram);
	start /= 4;
	end /= 4;
	for (i = start; i < end; i++) {
		sram[i] = task_org[i];
	}

	/* relocate function decriptors */
	start = ((sram[basetask * 8 + 3] & 0xfffffffc) - (unsigned int) sram);
	end = ((sram[(basetask + tasks - 1) * 8 + 3] & 0xfffffffc) + 256 -
	       (unsigned int) sram);
	start /= 4;
	end /= 4;
	for (i = start; i < end; i++) {
		sram[i] = task_org[i];
	}

	asm volatile ("sync");
}
