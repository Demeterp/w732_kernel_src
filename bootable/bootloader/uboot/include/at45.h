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


#ifndef	_AT45_H_
#define	_AT45_H_
#ifdef	CONFIG_DATAFLASH_MMC_SELECT
extern	void AT91F_SelectMMC(void);
extern	void AT91F_SelectSPI(void);
extern	int AT91F_GetMuxStatus(void);
#endif
extern	void AT91F_SpiInit(void);
extern	void AT91F_SpiEnable(int cs);
extern	unsigned int AT91F_SpiWrite ( AT91PS_DataflashDesc pDesc );
extern	AT91S_DataFlashStatus AT91F_DataFlashSendCommand(
		AT91PS_DataFlash pDataFlash,
		unsigned char OpCode,
		unsigned int CmdSize,
		unsigned int DataflashAddress);
extern	AT91S_DataFlashStatus AT91F_DataFlashGetStatus (
	AT91PS_DataflashDesc pDesc);
extern	AT91S_DataFlashStatus AT91F_DataFlashWaitReady (
	AT91PS_DataflashDesc pDataFlashDesc,
	unsigned int timeout);
extern	AT91S_DataFlashStatus AT91F_DataFlashContinuousRead (
	AT91PS_DataFlash pDataFlash,
	int src,
	unsigned char *dataBuffer,
	int sizeToRead );
extern	AT91S_DataFlashStatus AT91F_DataFlashPagePgmBuf(
	AT91PS_DataFlash pDataFlash,
	unsigned char *src,
	unsigned int dest,
	unsigned int SizeToWrite);
extern	AT91S_DataFlashStatus AT91F_MainMemoryToBufferTransfert(
	AT91PS_DataFlash pDataFlash,
	unsigned char BufferCommand,
	unsigned int page);
extern	AT91S_DataFlashStatus AT91F_DataFlashWriteBuffer (
	AT91PS_DataFlash pDataFlash,
	unsigned char BufferCommand,
	unsigned char *dataBuffer,
	unsigned int bufferAddress,
	int SizeToWrite );
extern	AT91S_DataFlashStatus AT91F_PageErase(
	AT91PS_DataFlash pDataFlash,
	unsigned int page);
extern	AT91S_DataFlashStatus AT91F_BlockErase(
	AT91PS_DataFlash pDataFlash,
	unsigned int block);
extern	AT91S_DataFlashStatus AT91F_WriteBufferToMain (
	AT91PS_DataFlash pDataFlash,
	unsigned char BufferCommand,
	unsigned int dest );
extern	AT91S_DataFlashStatus AT91F_PartialPageWrite (
	AT91PS_DataFlash pDataFlash,
	unsigned char *src,
	unsigned int dest,
	unsigned int size);
extern	AT91S_DataFlashStatus AT91F_DataFlashWrite(
	AT91PS_DataFlash pDataFlash,
	unsigned char *src,
	int dest,
	int size );
extern	int AT91F_DataFlashRead(
	AT91PS_DataFlash pDataFlash,
	unsigned long addr,
	unsigned long size,
	char *buffer);
extern	int AT91F_DataflashProbe(int cs, AT91PS_DataflashDesc pDesc);

#endif
