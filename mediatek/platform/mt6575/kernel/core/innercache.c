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

#include <linux/module.h>

/*This can be used ONLY by the M4U driver!*/
/*
 *      inner_dcache_flush_all()
 *
 *      Flush (clean + invalidate) the entire L1 data cache.
 *      This can be used ONLY by the M4U driver.
 *      Other drivers should NOT use this function at all. They should use DMA-mapping APIs.
 *
 *      After calling the function, the buffer should not be touched anymore.
 *      And the M4U driver should then call outer_flush_all() immediately.
 *      Here is the example:
 *          // Cannot touch the buffer from here.
 *          inner_dcache_flush_all();
 *          outer_flush_all();
 *          // Can touch the buffer from here.
 *      If preemption occurs and the driver cannot guarantee that no other process will touch the buffer,
 *      the driver should use LOCK to protect this code segment.
 */
void inner_dcache_flush_all(void)
{
    unsigned int way = 0x3; /* 4 ways */
    unsigned int set = 0xFF;    /* 32KB cache size */
    unsigned int Rt = 0;

    asm volatile(
        "1:\n"
        "mov %1, #3\n"
        "2:\n"
        "mov %0, #0\n"
        "orr %0, %0, %1, lsl #30\n"
        "orr %0, %0, %2, lsl #5\n"
        "mcr p15, 0, %0, c7, c14, #2\n"
        "subs %1, %1, #1\n"
        "bge 2b\n"
        "subs %2, %2, #1\n"
        "bge 1b\n"
        "dsb\n"
        "isb\n"
        : "+r" (Rt), "+r" (way), "+r" (set)
        : 
        : "cc"
    );
}

EXPORT_SYMBOL(inner_dcache_flush_all);
