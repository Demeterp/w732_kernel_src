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

/* linux/arch/arm/mach-mt6516/mt6516_rfkill.c
 *
 * (C) Copyright 2008
 * MediaTek <www.mediatek.com>
 * JinKwan Huang <jk.huang@mediatek.com>
 *
 * MT6516 RFKILL Driver -- Control bluetooth power
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#include <mach/mt_bt.h>


struct hci_dev* btpm_hdev = NULL;
extern wait_queue_head_t eint_wait;
extern int eint_gen;

void mt_bt_eirq_handler(void* par)
{
    struct hci_dev* hdev = NULL;

    printk(KERN_ERR "[BT]mt_bt_eirq_handler\r\n ");

    if (btpm_hdev != NULL){  /*to avoid the early interrupt before we register the btpm_hdev*/
        //hci_send_cmd(btpm_hdev, 0xfcc1, 0, NULL);
    #ifdef CONFIG_BT_HCIUART
        hdev = hci_dev_get(0);
        if(hdev == NULL){
            printk(KERN_ERR "[BT]NULL dev\r\n ");
        }else{
            printk(KERN_ERR "[BT] set mt_bt_eirq_handler send cmd\r\n ");
            hci_send_cmd(hdev, 0xfcc1, 0, NULL);
            /* mt_bt_eirq_handler in invoked in request_irq. Should not invoke task let scheduler?? */
        }
    #endif
    }
    else  //for ESI stack
    {
        eint_gen = 1;
        wake_up_interruptible(&eint_wait);
    }
    
    return;
}

void mt_bt_pm_init(void* hdev)
{
    printk(KERN_ERR "[BT]mt_bt_pm_init\r\n ");
    btpm_hdev = (struct hci_dev*)hdev;
    //mt_bt_enable_irq() is enabled when HCI0 is up
    return;
}

void mt_bt_pm_deinit(void* hdev)
{
    printk(KERN_ERR "[BT]mt_bt_pm_deinit\r\n ");
    //mt_bt_enable_irq() is disabled when HCI0 is down
    btpm_hdev = NULL;
    return;
}
