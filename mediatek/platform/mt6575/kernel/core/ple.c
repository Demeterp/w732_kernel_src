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

#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>

#if 0   /* obsolete defines for legacy interface */
#define SIZE_PER_PLE_BLOCK (4 * 16 * 1024)  /* 16K word */
#define PLE_BLOCK_MAX 256
#define MAX_SIZE_PER_PRELOAD (SIZE_PER_PLE_BLOCK * PLE_BLOCK_MAX)
#endif
#define LENGTH_MAX 0x3FFF
#define STRIDE_MAX 0xFF
#define NR_BLK_MAX 0xFF

static unsigned int ple_addr, ple_len, ple_stride, ple_nr_blk;

/*
 * ple_write: implement the write operation
 */
static ssize_t ple_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    unsigned int addr, Rt;
    unsigned int *addr1 = (unsigned int *)buf, *addr2 = (unsigned int *)(buf + 4);

    if (get_user(addr, addr1)) {
        return -EFAULT;
    }
    if (get_user(Rt, addr2)) {
        return -EFAULT;
    }
    asm volatile (
        "mcrr p15, 0, %0, %1, c11\n"
        :
        : "r" (addr), "r" (Rt)
        : "cc");

    return count;
}

/*
 * ple_addr_show: implement the sysfs show method
 */
static ssize_t ple_addr_show(struct device_driver *driver, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "addr = 0x%x\n", ple_addr);
}

/*
 * ple_addr_store: implement the sysfs store method
 */
static ssize_t ple_addr_store(struct device_driver *driver, const char *buf, size_t count)
{
    char *p = (char *)buf;

    ple_addr = simple_strtoul(p, &p, 16);
    ple_addr = ALIGN(ple_addr, 4);

    return count;
}

DRIVER_ATTR(addr, 0664, ple_addr_show, ple_addr_store);

/*
 * ple_len_show: implement the sysfs show method
 */
static ssize_t ple_len_show(struct device_driver *driver, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "len = 0x%x\n", ple_len);
}

/*
 * ple_len_store: implement the sysfs store method
 */
static ssize_t ple_len_store(struct device_driver *driver, const char *buf, size_t count)
{
    char *p = (char *)buf;
    unsigned val;

#if 0   /* obsolete code for legacy interface */
    ple_len = simple_strtoul(p, &p, 16);
    ple_len &= ~(4 - 1); 
    if (ple_len > MAX_SIZE_PER_PRELOAD) {
        ple_len = MAX_SIZE_PER_PRELOAD;
    }
#endif
    val = simple_strtoul(p, &p, 16);
    if (val <= LENGTH_MAX) {
        ple_len = val;
    }

    return count;
}

DRIVER_ATTR(len, 0664, ple_len_show, ple_len_store);

/*
 * ple_stride_show: implement the sysfs show method
 */
static ssize_t ple_stride_show(struct device_driver *driver, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "stride = 0x%x\n", ple_stride);
}

/*
 * ple_stride_store: implement the sysfs store method
 */
static ssize_t ple_stride_store(struct device_driver *driver, const char *buf, size_t count)
{
    char *p = (char *)buf;
    unsigned int val;

    val = simple_strtoul(p, &p, 16);
    if (val <= STRIDE_MAX) {
        ple_stride = val;
    }

    return count;
}

DRIVER_ATTR(stride, 0664, ple_stride_show, ple_stride_store);

/*
 * ple_nr_blk_show: implement the sysfs show method
 */
static ssize_t ple_nr_blk_show(struct device_driver *driver, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "number of blocks = 0x%x\n", ple_nr_blk);
}

/*
 * ple_nr_blk_store: implement the sysfs store method
 */
static ssize_t ple_nr_blk_store(struct device_driver *driver, const char *buf, size_t count)
{
    char *p = (char *)buf;
    unsigned int val;

    val = simple_strtoul(p, &p, 16);
    if (val <= NR_BLK_MAX) {
        ple_nr_blk = val;
    }

    return count;
}

DRIVER_ATTR(nr_blk, 0664, ple_nr_blk_show, ple_nr_blk_store);

/*
 * ple_show: implement the sysfs show method
 */
static ssize_t preload_show(struct device_driver *driver, char *buf)
{
    return 0;
}

/*
 * ple_store: implement the sysfs store method
 */
static ssize_t preload_store(struct device_driver *driver, const char *buf, size_t count)
{
#if 0   /* obsolete code for legacy interface */
    unsigned int l, nr_blk, Rt;

    if (ple_len < SIZE_PER_PLE_BLOCK) {
        l = (ple_len >> 2) - 1;
        Rt = l << 18;
        asm volatile (
            "mcrr p15, 0, %0, %1, c11\n"
            :
            : "r" (ple_addr), "r" (Rt)
            : "cc");
    } else {
        l = (SIZE_PER_PLE_BLOCK >> 2) - 1;
        nr_blk = (ple_len / SIZE_PER_PLE_BLOCK) - 1;
        Rt = (l << 18) | (nr_blk << 2);
        asm volatile (
            "mcrr p15, 0, %0, %1, c11\n"
            :
            : "r" (ple_addr), "r" (Rt)
            : "cc");

        if ((ple_len % SIZE_PER_PLE_BLOCK) != 0) {
            l = ((ple_len % SIZE_PER_PLE_BLOCK) >> 2) - 1;
            Rt = l << 18;
            asm volatile (
                "mcrr p15, 0, %0, %1, c11\n"
                :
                : "r" (ple_addr + SIZE_PER_PLE_BLOCK * (nr_blk + 1)), "r" (Rt)
                : "cc");
        }
    }
#endif

    unsigned int Rt;

    Rt = (ple_len << 18) | (ple_stride << 10) | (ple_nr_blk << 2);
    asm volatile (
        "mcrr p15, 0, %0, %1, c11\n"
        :
        : "r" (ple_addr), "r" (Rt)
        : "cc");

    return count;
}

DRIVER_ATTR(preload, 0664, preload_show, preload_store);

static struct device_driver ple_drv =
{
    .name = "ple",
    .bus = &platform_bus_type,
    .owner = THIS_MODULE,
};

static struct file_operations ple_fops =
{
    .write = ple_write,
};

static struct miscdevice ple_dev =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ple",
    .fops = &ple_fops,
};

/*
 * ple_init: module init function
 */
static int __init ple_init(void)
{
    int err;

    err = driver_register(&ple_drv);
    if (err) {
        printk(KERN_ALERT "Unable to create ple driver\n");
        goto ple_init_exit;
    }
    err = driver_create_file(&ple_drv, &driver_attr_addr);
    err |= driver_create_file(&ple_drv, &driver_attr_len);
    err |= driver_create_file(&ple_drv, &driver_attr_stride);
    err |= driver_create_file(&ple_drv, &driver_attr_nr_blk);
    err |= driver_create_file(&ple_drv, &driver_attr_preload);
    if (err) {
        printk(KERN_ALERT "Unable to create ple sysfs files\n");
        goto ple_init_exit;
    }

    err = misc_register(&ple_dev);
    if (err) {
        printk(KERN_ALERT "Unable to register ple device! err = %d\n", err);
        goto ple_init_exit;
    }

ple_init_exit:
    return 0;
}

/*
 * ple_exit: module exit function
 */
static void __exit ple_exit(void)
{
}

module_init(ple_init);
module_exit(ple_exit);
