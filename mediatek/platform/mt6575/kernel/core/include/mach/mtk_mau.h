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


#ifndef _MTK_MAU_H_
#define _MTK_MAU_H_



#define MTK_MAU_MAJOR_NUMBER 190

#define MTK_IOW(num, dtype)     _IOW('O', num, dtype)
#define MTK_IOR(num, dtype)     _IOR('O', num, dtype)
#define MTK_IOWR(num, dtype)    _IOWR('O', num, dtype)
#define MTK_IO(num)             _IO('O', num)

// --------------------------------------------------------------------------
#define MTK_CONFIG_MM_MAU       MTK_IOW(10, unsigned long)
#define MTK_CONFIG_MM_MPU       MTK_IOW(11, unsigned long)
/*
typedef enum {
    MAU0_MASK_RSV_0 = 0,
    MAU0_MASK_DEFECT,
    MAU0_MASK_JPE_ENC,
    MAU0_MASK_ROT_DMA0_OUT0,
    MAU0_MASK_ROT_DMA1_OUT0,
    MAU0_MASK_TV_ROT_OUT0,
    MAU0_MASK_CAM,
    MAU0_MASK_FD0,
    MAU0_MASK_FD2,
    MAU0_MASK_JPG_DEC0,
    MAU0_MASK_R_DMA0_OUT0,
    MAU0_MASK_R_DMA0_OUT1,
    MAU0_MASK_R_DMA0_OUT2,
    MAU0_MASK_FD1,
    MAU0_MASK_PCA,
    MAU0_MASK_JPGDMA_R,  // 15
    MAU0_MASK_JPGDMA_W,  // 15
    MAU0_MASK_ROT_DMA0_OUT1,// 15
    MAU0_MASK_ROT_DMA0_OUT2,// 15
    MAU0_MASK_ROT_DMA1_OUT1,// 15
    MAU0_MASK_ROT_DMA1_OUT2,// 15
    MAU0_MASK_TV_ROT_OUT1,  // no use
    MAU0_MASK_TV_ROT_OUT2,  // no use
    MAU0_MASK_R_DMA0_OUT3, // no use
    MAU0_MASK_JPG_DEC1,    // 15

    MAU0_MASK_ALL,

    MAU1_MASK_RSV_0 = MAU0_MASK_ALL,
    MAU1_MASK_OVL_MSK,    // 1
    MAU1_MASK_OVL_DCP,
    MAU1_MASK_DPI,
    MAU1_MASK_ROT_DMA2_OUT0,
    MAU1_MASK_ROT_DMA3_OUT0,
    MAU1_MASK_ROT_DMA4_OUT0,
    MAU1_MASK_TVC,
    MAU1_MASK_LCD_R,
    MAU1_MASK_LCD_W,
    MAU1_MASK_R_DMA1_OUT0,
    MAU1_MASK_R_DMA1_OUT1,
    MAU1_MASK_R_DMA1_OUT2,
    MAU1_MASK_SPI,
    MAU1_MASK_RSV_14,
    MAU1_MASK_DPI_HWC,
    MAU1_MASK_VRZ,            //16
    MAU1_MASK_ROT_DMA2_OUT1,  //20
    MAU1_MASK_ROT_DMA2_OUT2,  //20
    MAU1_MASK_ROT_DMA3_OUT1,  //20
    MAU1_MASK_ROT_DMA3_OUT2,  //20
    MAU1_MASK_ROT_DMA4_OUT1,  //20
    MAU1_MASK_ROT_DMA4_OUT2,  //20
    MAU1_MASK_GREQ_BLKW,      //17
    MAU1_MASK_GREQ_BLKR,      //18
    MAU1_MASK_TVC_PFH,      //no use
    MAU1_MASK_TVC_RESZ,      //no use
    MAU1_MASK_R_DMA1_OUT3,      //no use
    MAU1_MASK_EIS,          //19

    MAU1_MASK_ALL,

    MAU2_MASK_RSV_0 = MAU1_MASK_ALL,
    MAU2_MASK_VENC_MC,
    MAU2_MASK_VENC_BSDMA,
    MAU2_MASK_VENC_MVQP,
    MAU2_MASK_VENC_DMA,
    MAU2_MASK_VENC_REC,
    MAU2_MASK_VENC_POST0,
    MAU2_MASK_VENC_POST1,

    MAU2_MASK_ALL,

    MAU3_MASK_RSV_0 = MAU2_MASK_ALL,
    MAU3_MASK_G2D_R,
    MAU3_MASK_G2D_W,
    MAU3_MASK_AUDIO,

    MAU3_MASK_ALL,
    MAU_MASK_ALL = MAU3_MASK_ALL
}MTK_MAU0_MASK;
*/

typedef enum {
    MAU0_MASK_RSV_0 = 0,
    MAU0_MASK_DEFECT,
    MAU0_MASK_JPE_ENC,
    MAU0_MASK_ROT_DMA0_OUT0,
    MAU0_MASK_ROT_DMA1_OUT0,
    MAU0_MASK_TV_ROT_OUT0,
    MAU0_MASK_CAM,
    MAU0_MASK_FD0,
    MAU0_MASK_FD2,
    MAU0_MASK_JPG_DEC0,
    MAU0_MASK_R_DMA0_OUT0,
    MAU0_MASK_R_DMA0_OUT1,
    MAU0_MASK_R_DMA0_OUT2,
    MAU0_MASK_FD1,
    MAU0_MASK_PCA,
    MAU0_MASK_JPGDMA_R,  // 15
    MAU0_MASK_JPGDMA_W,  // 15
    MAU0_MASK_ROT_DMA0_OUT1,// 15
    MAU0_MASK_ROT_DMA0_OUT2,// 15
    MAU0_MASK_ROT_DMA1_OUT1,// 15
    MAU0_MASK_ROT_DMA1_OUT2,// 15
    MAU0_MASK_TV_ROT_OUT1,  // no use
    MAU0_MASK_TV_ROT_OUT2,  // no use
    MAU0_MASK_R_DMA0_OUT3, // no use
    MAU0_MASK_JPG_DEC1,    // 15

    MAU0_MASK_ALL,
}MTK_MAU0_MASK;

typedef enum {
    MAU1_MASK_RSV_0,
    MAU1_MASK_OVL_MSK,    // 1
    MAU1_MASK_OVL_DCP,
    MAU1_MASK_DPI,
    MAU1_MASK_ROT_DMA2_OUT0,
    MAU1_MASK_ROT_DMA3_OUT0,
    MAU1_MASK_ROT_DMA4_OUT0,
    MAU1_MASK_TVC,
    MAU1_MASK_LCD_R,
    MAU1_MASK_LCD_W,
    MAU1_MASK_R_DMA1_OUT0,
    MAU1_MASK_R_DMA1_OUT1,
    MAU1_MASK_R_DMA1_OUT2,
    MAU1_MASK_SPI,
    MAU1_MASK_RSV_14,
    MAU1_MASK_DPI_HWC,
    MAU1_MASK_VRZ,            //16
    MAU1_MASK_ROT_DMA2_OUT1,  //20
    MAU1_MASK_ROT_DMA2_OUT2,  //20
    MAU1_MASK_ROT_DMA3_OUT1,  //20
    MAU1_MASK_ROT_DMA3_OUT2,  //20
    MAU1_MASK_ROT_DMA4_OUT1,  //20
    MAU1_MASK_ROT_DMA4_OUT2,  //20
    MAU1_MASK_GREQ_BLKW,      //17
    MAU1_MASK_GREQ_BLKR,      //18
    MAU1_MASK_TVC_PFH,      //no use
    MAU1_MASK_TVC_RESZ,      //no use
    MAU1_MASK_R_DMA1_OUT3,      //no use
    MAU1_MASK_EIS,          //19

    MAU1_MASK_ALL,
}MTK_MAU1_MASK;


typedef enum {
    MAU2_MASK_RSV_0,
    MAU2_MASK_VENC_MC,
    MAU2_MASK_VENC_BSDMA,
    MAU2_MASK_VENC_MVQP,
    MAU2_MASK_VENC_DMA,
    MAU2_MASK_VENC_REC,
    MAU2_MASK_VENC_POST0,
    MAU2_MASK_VENC_POST1,

    MAU2_MASK_ALL,

}MTK_MAU2_MASK;

typedef enum {
    MAU3_MASK_RSV_0,
    MAU3_MASK_G2D_R,
    MAU3_MASK_G2D_W,
    MAU3_MASK_AUDIO,

    MAU3_MASK_ALL,
}MTK_MAU3_MASK;





typedef enum
{
    MAU_PA,   //phsical address
    MAU_MVA       //m4u virtual addrss
} MTK_MAU_MODE;



typedef enum
{
    MAU_ENTRY_0 = 0,
    MAU_ENTRY_1,
    MAU_ENTRY_2,
    MAU_ENTRY_ALL
} MTK_MAU_ENTRY;


typedef struct
{
    MTK_MAU_ENTRY   EntryID;						    // Entry ID 0~2
    bool            Enable;
    MTK_MAU_MODE    Mode;
    unsigned int    InvalidMasterLARB0;	// one bit represent one master, 1: allow, 0: not allow, usd by MAU and MPU
	unsigned int    InvalidMasterLARB1;    // one bit represent one master, 1: allow, 0: not allow, only used by MPU
    unsigned int    InvalidMasterLARB2;
    unsigned int    InvalidMasterLARB3;
	unsigned int    ReadEn; 					    // check read transaction, 1:enable, 0:disable
	unsigned int    WriteEn;				      // check write transaction, 1:enable, 0:disable
	unsigned int    StartAddr;					  // start address
	unsigned int    EndAddr;					    // end address
} MTK_MAU_CONFIG;








typedef enum
{
    MAU_ID_0,
    MAU_ID_1,
    MAU_ID_2,
    MAU_ID_3,
    MAU_ID_ALL
} MTK_MAU_ID;




int  MAU_Config(MTK_MAU_CONFIG* pMauConf);
void MAU_PrintStatus(char* buf, unsigned int buf_len, unsigned int* num);
void MAU_DumpReg(MTK_MAU_ID mauID);
void MAU_LogSwitch(bool enable);
void MAU_BackupReg(MTK_MAU_ID mauID);
void MAU_RestoreReg(MTK_MAU_ID mauID);
int MAU_get_port_with_m4u(unsigned  int start_addr, unsigned int end_addr);

int  MAU0_PowerOn(void);
int  MAU0_PowerOff(void);
int  MAU1_PowerOn(void);
int  MAU1_PowerOff(void);
int  MAU2_PowerOn(void);
int  MAU2_PowerOff(void);
int  MAU3_PowerOn(void);
int  MAU3_PowerOff(void);


#endif

