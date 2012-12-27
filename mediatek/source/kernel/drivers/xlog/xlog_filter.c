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
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/security.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/uaccess.h>
#include <linux/personality.h>
#include <linux/bitops.h>
#include <linux/mutex.h>
#include <linux/shmem_fs.h>
#include <linux/miscdevice.h>

#define XLOG_MODULE_MAX 1024
#define XLOG_MODULE_NAME_MAX_LEN 64
#define XLOG_FIND_MODULE 1
#define XLOG_SET_MASK    2
#define XLOG_GET_MASK    3
#define XLOG_MODIFY_ALL  4


/* xLog Driver */

u8 *xLogMem;
char xLog_mask = 0xff;

static DEFINE_SPINLOCK(xLog_avl_mutex);

struct _xLog_parcel {
    char name[XLOG_MODULE_NAME_MAX_LEN];
    int offset;
};

struct avl {
    struct avl *left;
    struct avl *right;
    struct avl *parent;
    char name[XLOG_MODULE_NAME_MAX_LEN];
    int offset;
    int depth;
};

struct avl modulemap[XLOG_MODULE_MAX];
int empty = 0;
struct avl *root = NULL;

static int xLog_insert(const char *name);

int xLog_isOn(const char *name, int level) 
{
	if (xLogMem != NULL) {
		char *map = &(((char *)xLogMem)[xLog_insert(name)]);
		return ((*map)&(0x1<<level))?1:0;
	}
	else {
		return 1;
	}
}

void xLog_set(const char *name, int level, int status) 
{
	if (xLogMem != NULL) {
		char *map = &(((char *)xLogMem)[xLog_insert(name)]);
		char mask = ~(0x1<<level);
		*map = ((*map)&mask)+(status?(0x1<<level):0);
	}
}

static void xLog_redepth(struct avl *node) {
   int left, right;
   if(!node) return;
   left = node->left?node->left->depth:0;
   right = node->right?node->right->depth:0;
   node->depth = (left>right?left:right)+1;
}

static void xLog_balance(struct avl *node) {
    struct avl *left, *right, **store;
    int depth;
    while(node) {
        xLog_redepth(node);
        left = node->left;
        right = node->right;
        if(!node->parent) store = &root;
        else store = ((node->parent->left==node)?&(node->parent->left):&(node->parent->right));
        depth = (left?left->depth:0)-(right?right->depth:0);
        if(depth>1) {
            if((left->left?left->left->depth:0)>(left->right?left->right->depth:0)) {
                node->left = left->right;
                left->right = node;
                left->parent = node->parent;
                node->parent = left;
                if(node->left) node->left->parent = node;
                *store = left;
                xLog_redepth(node); xLog_redepth(left);
            } else {
                right = left->right;
                node->left = right->right;
                right->right = node;
                left->right = right->left;
                right->left = left;
                right->parent = node->parent;
                node->parent = right;
                left->parent = right;
                if(node->left) node->left->parent = node;
                if(left->right) left->right->parent = left;
                *store = right;
                xLog_redepth(node); xLog_redepth(left); xLog_redepth(right);
            }
        } else if(depth<-1) {
            if((right->right?right->right->depth:0)>(right->left?right->left->depth:0)) {
                node->right = right->left;
                right->left = node;
                right->parent = node->parent;
                node->parent = right;
                if(node->right) node->right->parent = node;
                *store = right;
                xLog_redepth(node); xLog_redepth(right);
            } else {
                left = right->left;
                node->right = left->left;
                left->left = node;
                right->left = left->right;
                left->right = right;
                left->parent = node->parent;
                node->parent = left;
                right->parent = left;
                if(node->right) node->right->parent = node;
                if(right->left) right->left->parent = right;
                *store = left;
                xLog_redepth(node); xLog_redepth(right); xLog_redepth(left);
            }
        }
        node = node->parent;
    }
}

static int xLog_insert(const char *name) {
    int cmp,offset;
    struct avl **ptr = &(root);
    struct avl *parent = NULL;
    unsigned long flags;

    spin_lock_irqsave(&xLog_avl_mutex, flags);
    while(*ptr!=NULL) {
        cmp = strcmp((*ptr)->name,name);
        if(cmp==0) { offset = ((*ptr)->offset); goto insert_out; }
        if(cmp<0) parent=*ptr, ptr = &((*ptr)->left);
        if(cmp>0) parent=*ptr, ptr = &((*ptr)->right);
    }
    if(empty>=XLOG_MODULE_MAX) {
        offset = -1;
        goto insert_out;
    }
    strncpy(modulemap[empty].name,name,XLOG_MODULE_NAME_MAX_LEN);
    modulemap[empty].offset = empty; // value;
    modulemap[empty].left = NULL;
    modulemap[empty].right = NULL;
    modulemap[empty].parent = parent;
    *ptr = &(modulemap[empty++]);
    xLog_balance(*ptr);
    offset = modulemap[empty-1].offset;
    ((char *)xLogMem)[offset] = xLog_mask;
insert_out:
    spin_unlock_irqrestore(&xLog_avl_mutex, flags);
    return offset;
}

static int xLog_fault(struct vm_area_struct *vma, struct vm_fault *vmf) {
    struct page *page = NULL;
    unsigned long offset;
    offset = (((unsigned long)vmf->virtual_address - vma->vm_start) + (vma->vm_pgoff << PAGE_SHIFT));
    if(offset>PAGE_SIZE<<4) goto nopage_out;
    page = virt_to_page(xLogMem+offset);
    vmf->page = page;
    get_page(page);
nopage_out:
    return 0;
}

static struct vm_operations_struct xLog_vmops = {
    .fault  = xLog_fault,
};

static int xLog_open(struct inode *inode, struct file *file) {
    int ret;
    ret = nonseekable_open(inode, file);
    if(unlikely(ret)) return ret;
    file->private_data = NULL;
    return 0;
}

static int xLog_release(struct inode *ignored, struct file *file) {
    return 0;
}

static int xLog_mmap(struct file *file, struct vm_area_struct *vma) {
    vma->vm_ops = &xLog_vmops;
    vma->vm_flags |= VM_RESERVED;
    vma->vm_private_data = file->private_data;
    return 0;
}


long xlog_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	struct _xLog_parcel xLog_parcel;

	switch(cmd) {
	case XLOG_FIND_MODULE:
		if(copy_from_user((void *)&xLog_parcel, (const void __user *)arg, sizeof(xLog_parcel)))
			return -EFAULT;
		xLog_parcel.offset = xLog_insert(xLog_parcel.name);
		if(copy_to_user((void __user *)arg, (void *)&xLog_parcel, sizeof(xLog_parcel)))
			return -EFAULT;
		return 0;
		break;

	case XLOG_SET_MASK:
		xLog_mask = (char)arg;
		break;
		
	case XLOG_GET_MASK:
		if(copy_to_user((void __user *)arg, (void *)&xLog_mask, sizeof(char)));
		break;

	case XLOG_MODIFY_ALL:
		xLog_mask = arg;
		memset((void *)xLogMem, xLog_mask, PAGE_SIZE << 1);
		break;

	default: 
		break;
	}
	return ret;
}

static struct file_operations xlog_fops = {
    .owner   = THIS_MODULE,
    .unlocked_ioctl = xlog_ioctl,
    .mmap    = xLog_mmap,
    .open    = xLog_open,
    .release = xLog_release,
};

static struct miscdevice xlog_dev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "xLog",
    .fops    = &xlog_fops,
};

static int __init xLog_init(void) {
	int err;
	// TODO check if it is correct
	xLogMem = (u8 *)__get_free_pages(GFP_KERNEL, 1);
	memset((void *)xLogMem, 0xff, PAGE_SIZE << 1);

	err = misc_register(&xlog_dev);
	if (unlikely(err)) {
		printk(KERN_ERR "xLog: failed to unregister device\n");
		return -1;
	}
	else  {
		printk(KERN_INFO "xLog: inited.\n");
		return 0;
	}
}

static void __exit xLog_exit(void) 
{
	int err;

	free_pages((long unsigned)xLogMem, 4);

	err = misc_deregister(&xlog_dev);
	if (unlikely(err))
		printk(KERN_ERR "xLog: failed to unregister device!\n");
	printk("xLog: exited.\n");
}

module_init(xLog_init);
module_exit(xLog_exit);

//TODO module license & information

MODULE_DESCRIPTION("MEDIATEK Module Log Filtering Driver");
MODULE_AUTHOR("Kirby Wu<kirby.wu@mediatek.com>");
MODULE_LICENSE("GPL");
