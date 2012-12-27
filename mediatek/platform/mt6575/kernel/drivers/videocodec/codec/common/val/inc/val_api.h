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

/** 
 * @file 
 *   val_api.h 
 *
 * @par Project:
 *   MFlexVideo 
 *
 * @par Description:
 *   Video Abstraction Layer APIs
 *
 * @par Author:
 *   Jackal Chen (mtk02532)
 *
 * @par $Revision: #18 $
 * @par $Modtime:$
 * @par $Log:$
 *
 */

#ifndef _VAL_API_H_
#define _VAL_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "val_types.h"
//#include "mfv_drv_base.h"

/*=============================================================================
 *                             Function Declaration
 *===========================================================================*/
/**
* @par Function       
*   eVideoMemAlloc
* @par Description    
*   alloc memory for video codec driver
* @param              
*   a_prParam           [IN/OUT]    The VAL_MEMORY_T structure
* @param             
*   a_u4ParamSize       [IN]        The size of VAL_MEMORY_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoMemAlloc(
    VAL_MEMORY_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoMemFree
* @par Description    
*   free memory for video codec driver 
* @param              
*   a_prParam           [IN/OUT]    The VAL_MEMORY_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_MEMORY_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoMemFree(
    VAL_MEMORY_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoMemSet
* @par Description    
*   set a specific value to a range of memory 
* @param              
*   a_prParam           [IN]        The VAL_MEMORY_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of a_prParam
* @param              
*   a_u4Value           [IN]        The specific value
* @param              
*   a_u4Size            [IN]        The range of memory
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoMemSet(
    VAL_MEMORY_T *a_prParam,
    VAL_UINT32_T a_u4ParamSize,
    VAL_INT32_T a_u4Value,
    VAL_UINT32_T a_u4Size
);

/**
* @par Function       
*   eVideoMemCpy
* @par Description    
*   copy a range of memory from src memory to dst memory 
* @param              
*   a_prParamDst        [IN]        The VAL_MEMORY_T structure
* @param              
*   a_u4ParamDstSize    [IN]        The size of a_prParamDst
* @param              
*   a_prParamSrc        [IN]        The VAL_MEMORY_T structure
* @param              
*   a_u4ParamSrcSize    [IN]        The size of a_prParamSrc
* @param              
*   a_u4Size            [IN]        The range of memory
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoMemCpy(
    VAL_MEMORY_T *a_prParamDst,
    VAL_UINT32_T a_u4ParamDstSize,
    VAL_MEMORY_T *a_prParamSrc,
    VAL_UINT32_T a_u4ParamSrcSize,
    VAL_UINT32_T a_u4Size
);

/**
* @par Function       
*   eVideoMemCmp
* @par Description    
*   compare a range of memory with Src1 memory and Src2 memory 
* @param              
*   a_prParamSrc1       [IN]        The VAL_MEMORY_T structure
* @param              
*   a_u4ParamSrc1Size   [IN]        The size of a_prParamSrc1
* @param              
*   a_prParamSrc2       [IN]        The VAL_MEMORY_T structure
* @param              
*   a_u4ParamSrc2Size   [IN]        The size of a_prParamSrc2
* @param              
*   a_u4Size            [IN]        The range of memory
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoMemCmp(
    VAL_MEMORY_T *a_prParamSrc1,
    VAL_UINT32_T a_u4ParamSrc1Size,
    VAL_MEMORY_T *a_prParamSrc2,
    VAL_UINT32_T a_u4ParamSrc2Size,
    VAL_UINT32_T a_u4Size
);

/**
* @par Function       
*   eVideoIntMemUsed
* @par Description    
*   used Internal memory
* @param              
*   a_prParam           [IN/OUT]    The VAL_INTMEM_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_INTMEM_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoIntMemUsed(
    VAL_INTMEM_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoIntMemAlloc
* @par Description    
*   Allocate Internal memory
* @param              
*   a_prParam           [IN/OUT]    The VAL_INTMEM_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_INTMEM_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoIntMemAlloc(
    VAL_INTMEM_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoIntMemFree
* @par Description    
*   Free Internal memory
* @param              
*   a_prParam           [IN/OUT]    The VAL_INTMEM_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_INTMEM_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoIntMemFree(
    VAL_INTMEM_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoCreateEvent
* @par Description    
*   create event  
* @param              
*   a_prParam           [IN/OUT]    The VAL_EVENT_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_EVENT_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoCreateEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoCloseEvent
* @par Description    
*   close event  
* @param              
*   a_prParam           [IN/OUT]    The VAL_EVENT_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_EVENT_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoCloseEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoWaitEvent
* @par Description    
*   wait event (ex. wait hw decode done)  
* @param              
*   a_prParam           [IN/OUT]    The VAL_EVENT_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_EVENT_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoWaitEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoSetEvent
* @par Description    
*   set event (ex. when hw decode done, ant then do something depend on this event...)  
* @param              
*   a_prParam           [IN/OUT]    The VAL_EVENT_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_EVENT_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoSetEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoCreateMutex
* @par Description    
*   create mutex
* @param              
*   a_prParam           [IN/OUT]    The VAL_MUTEX_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_MUTEX_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoCreateMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoCloseMutex
* @par Description    
*   create mutex
* @param              
*   a_prParam           [IN/OUT]    The VAL_MUTEX_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_MUTEX_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoCloseMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoWaitMutex
* @par Description    
*   create mutex
* @param              
*   a_prParam           [IN/OUT]    The VAL_MUTEX_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_MUTEX_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoWaitMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoReleaseMutex
* @par Description    
*   create mutex
* @param              
*   a_prParam           [IN/OUT]    The VAL_MUTEX_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_MUTEX_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoReleaseMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoGetTimeOfDay
* @par Description    
*   Get current system time
* @param              
*   a_prParam         [IN/OUT]   The VAL_TIME_T structure
* @param              
*   a_u4ParamSize     [IN]       The size of VAL_TIME_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoGetTimeOfDay(
    VAL_TIME_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function     
*   eVideoStrStr
* @par Description    
*   Return a pointer to the first occurrence of a search string in a string
* @param              
*   a_prParam           [IN/OUT]    The VAL_STRSTR_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_STRSTR_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoStrStr(
    VAL_STRSTR_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

/**
* @par Function       
*   eVideoAtoi
* @par Description    
*   Convert a string to integer.
* @param              
*   a_prParam           [IN/OUT]    The VAL_ATOI_T structure
* @param              
*   a_u4ParamSize       [IN]        The size of VAL_ATOI_T
* @par Returns        
*   VAL_RESULT_T
*/
VAL_RESULT_T eVideoAtoi(
    VAL_ATOI_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoInitLockHW( VAL_VCODEC_OAL_HW_REGISTER_T *prParam, int size);
VAL_RESULT_T eVideoDeInitLockHW( VAL_VCODEC_OAL_HW_REGISTER_T *prParam, int size);
VAL_RESULT_T eVideoVcodecSetThreadID (VAL_VCODEC_THREAD_ID_T *a_prThreadID);

VAL_RESULT_T eVideoVcodecGetCpuLoadingInfo (VAL_VCODEC_CPU_LOADING_INFO_T *a_prCpuLoadingInfo);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _VAL_API_H_
