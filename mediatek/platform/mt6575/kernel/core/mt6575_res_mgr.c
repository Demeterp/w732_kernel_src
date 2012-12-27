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
//----------------------------------------------------------------------------
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <mach/mt6575_res_mgr.h>
//----------------------------------------------------------------------------
typedef unsigned long   MUINT32;
typedef long            MINT32;
typedef unsigned char   MUINT8;
typedef char            MINT8;
typedef bool            MBOOL;
#define MTRUE           true
#define MFALSE          false
//----------------------------------------------------------------------------
static volatile MUINT32 ResMgrLog = RES_MGR_LOG_ERR;
//----------------------------------------------------------------------------
#define LOG_MSG(fmt, arg...)\
    if(ResMgrLog & RES_MGR_LOG_MSG) \
    { \
        printk(KERN_DEBUG "[RES_MGR][%s]" fmt,__FUNCTION__, ##arg); \
    }
#define LOG_WRN(fmt, arg...) \
    if(ResMgrLog & RES_MGR_LOG_WRN) \
    { \
        printk(KERN_DEBUG "[RES_MGR][%s]Warning(%5d):" fmt, __FUNCTION__,__LINE__, ##arg); \
    }
#define LOG_ERR(fmt, arg...) \
    if(ResMgrLog & RES_MGR_LOG_ERR) \
    { \
        printk(KERN_ERR   "[RES_MGR][%s]Err(%5d):" fmt, __FUNCTION__,__LINE__, ##arg); \
    }
//----------------------------------------------------------------------------
#define RES_MGR_DEV_NUM         1
#define RES_MGR_DEV_MINOR_NUM   1
#define RES_MGR_DEV_NO_MINOR    0
//----------------------------------------------------------------------------
typedef struct
{
    pid_t   Pid;
    pid_t   Tgid;
    MUINT32 Resource;
}RES_MGR_OPEN_INFO_STRUCT;

typedef struct
{
    MUINT32             ResLockMask;
    MUINT32             PermanentMask;
    spinlock_t          SpinLock;
    dev_t               DevNo;
    struct cdev*        pCharDrv;
    struct class*       pClass;
    wait_queue_head_t   WaitQueueHead;
}RES_MGR_STRUCT;
//----------------------------------------------------------------------------
static RES_MGR_STRUCT ResMgr;
//----------------------------------------------------------------------------
static inline void ResMgr_SpinLock(void)
{
    //LOG_MSG("[SpinLock]\r\n");
    spin_lock(&(ResMgr.SpinLock));
}
//----------------------------------------------------------------------------
static inline void ResMgr_SpinUnlock(void)
{
    //LOG_MSG("[SpinUnlock]\r\n");
    spin_unlock(&(ResMgr.SpinLock));
}
//----------------------------------------------------------------------------
static unsigned long ResMgr_MsToJiffies(MUINT32 Ms)
{
    return ((Ms*HZ + 512) >> 10);
}
//----------------------------------------------------------------------------
static MBOOL ResMgr_ResLock(RES_MGR_RES_LOCK_STRUCT* pResLock)
{
    MBOOL   Ret = MTRUE;
    MUINT32 Timeout;
    RES_MGR_RES_LOCK_STRUCT ResLock;
    //
    memcpy(
        (void*)&ResLock,
        (void*)pResLock,
        sizeof(RES_MGR_RES_LOCK_STRUCT));
    //
    if((ResMgr.ResLockMask & ResLock.ResMask) == 0)
    {
        ResMgr.ResLockMask |= ResLock.ResMask;
        ResMgr.PermanentMask |= ResLock.PermanentMask;
        LOG_MSG("Lock OK, Resource(0x%08lX), Permanent(0x%08lX), ResMask(0x%08lX)\r\n",ResMgr.ResLockMask,ResMgr.PermanentMask,ResLock.ResMask);
    }
    else
    if((ResMgr.PermanentMask & ResLock.ResMask) != 0)
    {
        LOG_ERR("Lock fail, Resource(0x%08lX), Permanent(0x%08lX), ResMask(0x%08lX)\r\n",ResMgr.ResLockMask,ResMgr.PermanentMask,ResLock.ResMask);
        Ret = MFALSE;
    }
    else
    {
        ResMgr_SpinUnlock();
        Timeout = wait_event_interruptible_timeout(
                    ResMgr.WaitQueueHead, 
                    (ResMgr.ResLockMask & ResLock.ResMask) == 0,
                    ResMgr_MsToJiffies(ResLock.Timeout));
        ResMgr_SpinLock();
        if((ResMgr.ResLockMask & ResLock.ResMask) == 0)
        {
            ResMgr.ResLockMask |= ResLock.ResMask;
            ResMgr.PermanentMask |= (ResLock.PermanentMask & ResLock.ResMask); //Make sure ResLock.PermanentMask is not out of range for ResLock.ResMask.
            LOG_MSG("Lock OK, Resource(0x%08lX), Permanent(0x%08lX), ResMask(0x%08lX)\r\n",ResMgr.ResLockMask,ResMgr.PermanentMask,ResLock.ResMask);
        }
        else
        if( Timeout == 0 &&
            (ResMgr.ResLockMask & ResLock.ResMask) != 0)
        {
            LOG_ERR("Lock timeout, Resource(0x%08lX), Permanent(0x%08lX), ResMask(0x%08lX)\r\n",ResMgr.ResLockMask,ResMgr.PermanentMask,ResLock.ResMask);
            Ret = MFALSE;
        }
        else
        {
            LOG_ERR("Lock unknow, Timeout(%lu), Resource(0x%08lX), Permanent(0x%08lX), ResMask(0x%08lX)\r\n",Timeout,ResMgr.ResLockMask,ResMgr.PermanentMask,ResLock.ResMask);
            Ret = MFALSE;
        }
    }
    //
    return Ret;
}
//----------------------------------------------------------------------------
static void ResMgr_ResUnlock(MUINT32 ResMask)
{
    ResMgr.ResLockMask &= (~ResMask);
    ResMgr.PermanentMask &= (~ResMask);
    wake_up_interruptible(&(ResMgr.WaitQueueHead));
    //
    LOG_MSG("Unlock OK, Resource(0x%08lX), Permanent(0x%08lX), ResMask(0x%08lX)\r\n",ResMgr.ResLockMask,ResMgr.PermanentMask,ResMask);
}
//----------------------------------------------------------------------------
static int ResMgr_open(
    struct inode*   pInode,
    struct file*    pFile)
{
    int Ret = 0;
    RES_MGR_OPEN_INFO_STRUCT*   pOpenInfo;
    //
    LOG_MSG("Current:process(%s), pid(%d), tgid(%d)\r\n", current->comm, current->pid, current->tgid);
    //
    ResMgr_SpinLock();
    //
    pFile->private_data = NULL;
    pFile->private_data = kmalloc(sizeof(RES_MGR_OPEN_INFO_STRUCT) , GFP_ATOMIC);
    if(pFile->private_data == NULL)
    {
        LOG_ERR("No enough memory,Current:process(%s), pid(%d), tgid(%d)\r\n", current->comm, current->pid, current->tgid);
        Ret = -ENOMEM;
    }
    else
    {
        pOpenInfo = (RES_MGR_OPEN_INFO_STRUCT*)pFile->private_data;
        pOpenInfo->Pid = 0;
        pOpenInfo->Tgid = 0;
        pOpenInfo->Resource = 0;
    }
    //
    ResMgr_SpinUnlock();
    //
    //LOG_MSG("OK\r\n");
    return Ret;
}
//----------------------------------------------------------------------------
static int ResMgr_release(
    struct inode*   pInode,
    struct file*    pFile)
{
    RES_MGR_OPEN_INFO_STRUCT*   pOpenInfo;
    //
    LOG_MSG("Current:process(%s), pid(%d), tgid(%d)\r\n", current->comm, current->pid, current->tgid);
    //
    ResMgr_SpinLock();
    //
    if(pFile->private_data != NULL)
    {
        pOpenInfo = (RES_MGR_OPEN_INFO_STRUCT*)pFile->private_data;
        LOG_MSG("OpenInfo:pid(%d), tgid(%d), Resource(0x%08lX)\r\n",pOpenInfo->Pid,pOpenInfo->Tgid,pOpenInfo->Resource);
        if(pOpenInfo->Resource)
        {
            LOG_WRN("free resource and memory,OpenInfo:pid(%d), tgid(%d), Resource(0x%08lX)\r\n",pOpenInfo->Pid,pOpenInfo->Tgid,pOpenInfo->Resource);
            ResMgr_ResUnlock(pOpenInfo->Resource);
            pOpenInfo->Resource = 0;
        }
        kfree(pFile->private_data);
        pFile->private_data = NULL;
    }
    //
    ResMgr_SpinUnlock();
    //
    //LOG_MSG("OK\r\n");
    return 0;
}
//----------------------------------------------------------------------------
static int ResMgr_flush(
    struct file*    pFile,
    fl_owner_t      Id)
{
    RES_MGR_OPEN_INFO_STRUCT*   pOpenInfo;
    //
    LOG_MSG("Current:process(%s), pid(%d), tgid(%d)\r\n", current->comm, current->pid, current->tgid);
    //
    ResMgr_SpinLock();
    //
    if(pFile->private_data != NULL)
    {
        pOpenInfo = (RES_MGR_OPEN_INFO_STRUCT*)pFile->private_data;
        LOG_MSG("OpenInfo:pid(%d), tgid(%d), Resource(0x%08lX)\r\n",pOpenInfo->Pid,pOpenInfo->Tgid,pOpenInfo->Resource);
        if(pOpenInfo->Tgid == 0)
        {
            if(pOpenInfo->Resource != 0)
            {
                LOG_ERR("No Tgid info about resource\r\n");
                LOG_ERR("OpenInfo:pid(%d), tgid(%d), Resource(0x%08lX)\r\n",pOpenInfo->Pid,pOpenInfo->Tgid,pOpenInfo->Resource);
                LOG_ERR("Current:process(%s), pid(%d), tgid(%d)\r\n",current->comm , current->pid, current->tgid);
            }
        }
        else
        if( (pOpenInfo->Tgid == current->tgid) ||
            ((pOpenInfo->Tgid != current->tgid) && (strcmp(current->comm, "binder") == 0)))
        {
            if(pOpenInfo->Resource)
            {
                LOG_WRN("free resource and memory,OpenInfo:pid(%d), tgid(%d), Resource(0x%08lX)\r\n",pOpenInfo->Pid,pOpenInfo->Tgid,pOpenInfo->Resource);
                ResMgr_ResUnlock(pOpenInfo->Resource);
                pOpenInfo->Resource = 0;
            }
        }
    }
    //
    ResMgr_SpinUnlock();
    //
    //LOG_MSG("OK\r\n");
    return 0;
}
//----------------------------------------------------------------------------
static long ResMgr_ioctl(
    struct file*    pFile,
    unsigned int    Cmd,
    unsigned long   Param)
{
    MINT32  Ret = 0;
    MUINT32 LogMask;
    RES_MGR_RES_LOCK_STRUCT     ResLock;
    RES_MGR_RES_CHECK_STRUCT    ResCheck;
    RES_MGR_OPEN_INFO_STRUCT*   pOpenInfo = (RES_MGR_OPEN_INFO_STRUCT*)pFile->private_data;
    //
    //LOG_MSG("[ioctl]\r\n");
    //
    if(pFile->private_data == NULL)
    {
        LOG_ERR("private_data is NULL,Current:process(%s), pid(%d), tgid(%d)\r\n", current->comm , current->pid, current->tgid);
        Ret = -EFAULT;
        goto EXIT;
    }
    //
    switch(Cmd)
    {
        case RES_MGR_RES_LOCK:
        {
            if(copy_from_user(&ResLock, (void*)Param, sizeof(RES_MGR_RES_LOCK_STRUCT)) == 0)
            {
                ResMgr_SpinLock();
                if(ResMgr_ResLock(&ResLock))
                {
                    pOpenInfo->Resource |= ResLock.ResMask;
                    if(pOpenInfo->Tgid == 0)
                    {
                        LOG_MSG("Lock Resource(0x%08lX),Current:process(%s), pid(%d), tgid(%d)\r\n",ResLock.ResMask,current->comm , current->pid, current->tgid);
                        pOpenInfo->Pid = current->pid;
                        pOpenInfo->Tgid = current->tgid;
                    }
                    else
                    if(pOpenInfo->Tgid != current->tgid)
                    {
                        LOG_ERR("Tgid is inconsistent,OpenInfo.Tgid(%d) != Current.Tgid(%d)\r\n",pOpenInfo->Tgid,current->tgid);
                    }
                }
                else
                {
                    LOG_ERR("Lock more than once\r\n");
                    Ret = -EFAULT;
                }
                ResMgr_SpinUnlock();
            }
            else
            {
                LOG_ERR("copy_from_user fail\r\n");
                Ret = -EFAULT;
            }
            break;
        }

        case RES_MGR_RES_UNLOCK:
        {
            if(copy_from_user(&(ResLock.ResMask), (void*)Param, sizeof(unsigned long)) == 0)
            {
                ResMgr_SpinLock();
                if(pOpenInfo->Resource & ResLock.ResMask)
                {
                    ResMgr_ResUnlock(ResLock.ResMask);
                    pOpenInfo->Resource &= (~ResLock.ResMask);
                    if(pOpenInfo->Resource == 0)
                    {
                        LOG_MSG("Unlock Resource(0x%08lX),Current:process(%s), pid(%d), tgid(%d)\r\n",ResLock.ResMask,current->comm , current->pid, current->tgid);
                        pOpenInfo->Pid = 0;
                        pOpenInfo->Tgid = 0;
                    }
                }
                else
                {
                    LOG_ERR("Unlock resource which was not locked before\r\n");
                    Ret = -EFAULT;
                }
                ResMgr_SpinUnlock();
            }
            else
            {
                LOG_ERR("copy_from_user fail\r\n");
                Ret = -EFAULT;
            }
            break;
        }

        case RES_MGR_RES_CHECK:
        {
            ResMgr_SpinLock();
            ResCheck.ResLockMask = ResMgr.ResLockMask;
            ResCheck.PermanentMask = ResMgr.PermanentMask;
            ResMgr_SpinUnlock();

            if(copy_to_user((void*)Param, (void*)&ResCheck, sizeof(RES_MGR_RES_CHECK_STRUCT)) != 0)
            {
                LOG_ERR("copy_to_user fail\n");
                Ret = -EFAULT;
            }
            //LOG_MSG("[ioctl]ResLockMask(0x%08lX), ResWaitMask(0x%08lX)\r\n",pResCheck->ResLockMask,pResCheck->ResWaitMask);
            break;
        }

        case RES_MGR_LOG_ENABLE:
        {
            if(copy_from_user(&LogMask, (void*)Param, sizeof(unsigned long)) == 0)
            {
                ResMgr_SpinLock();
                ResMgrLog |= LogMask;
                ResMgr_SpinUnlock();
            }
            else
            {
                LOG_ERR("copy_from_user fail\r\n");
                Ret = -EFAULT;
            }
            break;
        }

        case RES_MGR_LOG_DISABLE:
        {
            if(copy_from_user(&LogMask, (void*)Param, sizeof(unsigned long)) == 0)
            {
                ResMgr_SpinLock();
                ResMgrLog &= (~LogMask);
                ResMgr_SpinUnlock();
            }
            else
            {
                LOG_ERR("copy_from_user fail\r\n");
                Ret = -EFAULT;
            }
            break;
        }

        default:
        {
            LOG_ERR("Unknow cmd(%d)\r\n",Cmd);
            Ret = -EFAULT;
            break;
        }
    }

    if(Ret != 0)
    {
        LOG_ERR("Fail\r\n");
        LOG_ERR("OpenInfo:pid(%d), tgid(%d), Resource(0x%08lX)\r\n",pOpenInfo->Pid,pOpenInfo->Tgid,pOpenInfo->Resource);
        LOG_ERR("Current:process(%s), pid(%d), tgid(%d)\r\n",current->comm , current->pid, current->tgid);
    }

    EXIT:
    return Ret;
}
//----------------------------------------------------------------------------
static const struct file_operations ResMgr_FileOper = 
{
    .owner          = THIS_MODULE,
    .open           = ResMgr_open,
    .release        = ResMgr_release,
    .flush          = ResMgr_flush,
    .unlocked_ioctl = ResMgr_ioctl
};
//----------------------------------------------------------------------------
static int ResMgr_RegCharDev(void)
{
    MINT32 Ret = 0;
    //
    LOG_MSG("\r\n");
    //
    ResMgr.DevNo = 0;
    Ret = alloc_chrdev_region(&(ResMgr.DevNo), RES_MGR_DEV_NO_MINOR, RES_MGR_DEV_NUM, RES_MGR_DEV_NAME);
    if(Ret < 0)
    {
        LOG_ERR("alloc_chrdev_region fail:Ret(%ld)\r\n", Ret);
        return Ret;
    }
    //Allocate memory for driver
    ResMgr.pCharDrv = cdev_alloc();
    if(ResMgr.pCharDrv == NULL)
    {
        unregister_chrdev_region(ResMgr.DevNo, RES_MGR_DEV_NUM);
        LOG_ERR("Allocate mem for kobject failed\r\n");
        return -ENOMEM;
    }
    //Attatch file operation.
    cdev_init(ResMgr.pCharDrv, &ResMgr_FileOper);
    ResMgr.pCharDrv->owner = THIS_MODULE;
    //Add to system
    if(cdev_add(ResMgr.pCharDrv, ResMgr.DevNo, RES_MGR_DEV_MINOR_NUM))
    {
        LOG_ERR("Attatch file operation failed\r\n");
        unregister_chrdev_region(ResMgr.DevNo, RES_MGR_DEV_NUM);
        return -EAGAIN;
    }
    //
    LOG_MSG("OK\r\n");
    return Ret;
}
//----------------------------------------------------------------------------
static inline void ResMgr_UnregCharDev(void)
{
    LOG_MSG("\r\n");
    //Release char driver
    cdev_del(ResMgr.pCharDrv);
    unregister_chrdev_region(ResMgr.DevNo, RES_MGR_DEV_NUM);
    //
    LOG_MSG("OK\r\n");
}
//----------------------------------------------------------------------------
static int ResMgr_probe(struct platform_device *pDev)
{
    MINT32 Ret = 0;    
    struct device* pDevice = NULL;
    //
    LOG_MSG("\r\n");
    //
    Ret = ResMgr_RegCharDev();
    if(Ret < 0)
    {
        LOG_ERR("RegCharDev fail:Ret(%ld)\r\n", Ret);
        return Ret;
    }

    ResMgr.pClass = class_create(THIS_MODULE, "ResourceMgrDrv");
    if (IS_ERR(ResMgr.pClass)) {
        Ret = PTR_ERR(ResMgr.pClass);
        LOG_ERR("class_create fail:Ret(%ld)\r\n", Ret);
        return Ret;            
    }
    pDevice = device_create(
                ResMgr.pClass,
                NULL,
                ResMgr.DevNo,
                NULL,
                RES_MGR_DEV_NAME);
    if(IS_ERR(pDevice))
    {
        LOG_ERR("device_create fail\r\n");
        return (int)pDevice;
    }
    //Initial variable
    ResMgrLog = RES_MGR_LOG_ERR;
    spin_lock_init(&(ResMgr.SpinLock));
    init_waitqueue_head(&(ResMgr.WaitQueueHead));
    //
    LOG_MSG("OK\r\n");
    return Ret;
}
//----------------------------------------------------------------------------
static int ResMgr_remove(struct platform_device *pdev)
{
    //
    LOG_MSG("\r\n");
    //unregister char driver.
    ResMgr_UnregCharDev();
    //
    device_destroy(ResMgr.pClass, ResMgr.DevNo);
    class_destroy(ResMgr.pClass);
    //
    LOG_MSG("OK\r\n");
    return 0;
}
//----------------------------------------------------------------------------
static int ResMgr_suspend(
    struct platform_device* pDev,
    pm_message_t            Mesg)
{
    LOG_MSG("\r\n");
    return 0;
}
//----------------------------------------------------------------------------
static int ResMgr_resume(struct platform_device *pDev)
{
    LOG_MSG("\r\n");
    return 0;
}
//----------------------------------------------------------------------------
static struct platform_driver ResMgr_PlatformDriver =
{
    .probe      = ResMgr_probe,
    .remove     = ResMgr_remove,
    .suspend    = ResMgr_suspend,
    .resume     = ResMgr_resume,
    .driver     =
    {
        .name   = RES_MGR_DEV_NAME,
        .owner  = THIS_MODULE,
    }
};
//----------------------------------------------------------------------------
static int ResMgr_ReadLog(
    char*   pPage,
    char**  ppStart,
    off_t   Off,
    int     Count,
    int*    pEof,
    void*   pData)
{
    char *p = pPage;
    MUINT32 Len = 0;
    //
    p += sprintf(p, "[RSE_MGR]ResMgrLog(0x%08lX)\r\n",ResMgrLog);
    //
    *ppStart = pPage + Off;
    Len = p - pPage;
    if(Len > Off)
    {
        Len -= Off;
    }
    else
    {
        Len = 0;
    }
    //
    return Len < Count ? Len  : Count;
}
//----------------------------------------------------------------------------
static int ResMgr_WriteLog(
    struct file*    pFile,
    const char*     pBuffer,
    unsigned long   Count,
    void*           pData)
{
    char Buf[32]; 
    MUINT32 CopySize = 0, Log = 0;
    //
    CopySize = (Count < (sizeof(Buf) - 1)) ? Count : (sizeof(Buf) - 1);
    if(copy_from_user(Buf, pBuffer, CopySize))
    {
        return 0;
    }
    Buf[CopySize] = '\0';
    //
    sscanf(Buf, "%lX", &Log);
    ResMgrLog = Log;
    //
    return Count;
}
//----------------------------------------------------------------------------
static int __init ResMgr_Init(void)
{
    MINT32 Ret = 0;
    struct proc_dir_entry *pEntry;
    //
    LOG_MSG("\r\n");
    //
    Ret = platform_driver_register(&ResMgr_PlatformDriver);
    if(Ret < 0)
    {
        LOG_ERR("Failed to register driver:Ret(%ld)\r\n", Ret);
        return Ret;
    }
    //
    pEntry = create_proc_entry("driver/ResMgrLog", 0, NULL); 
    if(pEntry)
    {
        pEntry->read_proc = ResMgr_ReadLog;
        pEntry->write_proc = ResMgr_WriteLog;
    }
    else
    {
        LOG_ERR("add /proc/driver/ResMgrLog entry fail \n");
    }
    //
    LOG_MSG("OK\r\n");
    return Ret;
}
//----------------------------------------------------------------------------
static void __exit ResMgr_Exit(void)
{
    LOG_MSG("\r\n");
    platform_driver_unregister(&ResMgr_PlatformDriver);
    LOG_MSG("OK\r\n");
}
//----------------------------------------------------------------------------
module_init(ResMgr_Init);
module_exit(ResMgr_Exit);
MODULE_DESCRIPTION("MT6575 Resource Manager Driver");
MODULE_AUTHOR("Marx <Marx.Chiu@Mediatek.com>");
MODULE_LICENSE("GPL");
//----------------------------------------------------------------------------

