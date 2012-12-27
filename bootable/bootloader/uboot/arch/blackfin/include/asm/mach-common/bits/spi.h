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
 * SPI Masks
 */

#ifndef __BFIN_PERIPHERAL_SPI__
#define __BFIN_PERIPHERAL_SPI__

/* SPI_CTL Masks */
#define	TIMOD			0x0003	/* Transfer Initiate Mode */
#define RDBR_CORE		0x0000	/* RDBR Read Initiates, IRQ When RDBR Full */
#define	TDBR_CORE		0x0001	/* TDBR Write Initiates, IRQ When TDBR Empty */
#define RDBR_DMA		0x0002	/* DMA Read, DMA Until FIFO Empty */
#define TDBR_DMA		0x0003	/* DMA Write, DMA Until FIFO Full */
#define SZ			0x0004	/* Send Zero (When TDBR Empty, Send Zero/Last*) */
#define GM			0x0008	/* Get More (When RDBR Full, Overwrite/Discard*) */
#define PSSE			0x0010	/* Slave-Select Input Enable */
#define EMISO			0x0020	/* Enable MISO As Output */
#define SIZE			0x0100	/* Size of Words (16/8* Bits) */
#define LSBF			0x0200	/* LSB First */
#define CPHA			0x0400	/* Clock Phase */
#define CPOL			0x0800	/* Clock Polarity */
#define MSTR			0x1000	/* Master/Slave* */
#define WOM			0x2000	/* Write Open Drain Master */
#define SPE			0x4000	/* SPI Enable */

/* SPI_FLG Masks */
#define FLS1			0x0002	/* Enables (=1) SPI_FLOUT1 as flag output for SPI Slave-select */
#define FLS2			0x0004	/* Enables (=1) SPI_FLOUT2 as flag output for SPI Slave-select */
#define FLS3			0x0008	/* Enables (=1) SPI_FLOUT3 as flag output for SPI Slave-select */
#define FLS4			0x0010	/* Enables (=1) SPI_FLOUT4 as flag output for SPI Slave-select */
#define FLS5			0x0020	/* Enables (=1) SPI_FLOUT5 as flag output for SPI Slave-select */
#define FLS6			0x0040	/* Enables (=1) SPI_FLOUT6 as flag output for SPI Slave-select */
#define FLS7			0x0080	/* Enables (=1) SPI_FLOUT7 as flag output for SPI Slave-select */
#define FLG1			0x0200	/* Activates (=0) SPI_FLOUT1 as flag output for SPI Slave-select */
#define FLG2			0x0400	/* Activates (=0) SPI_FLOUT2 as flag output for SPI Slave-select */
#define FLG3			0x0800	/* Activates (=0) SPI_FLOUT3 as flag output for SPI Slave-select */
#define FLG4			0x1000	/* Activates (=0) SPI_FLOUT4 as flag output for SPI Slave-select */
#define FLG5			0x2000	/* Activates (=0) SPI_FLOUT5 as flag output for SPI Slave-select */
#define FLG6			0x4000	/* Activates (=0) SPI_FLOUT6 as flag output for SPI Slave-select */
#define FLG7			0x8000	/* Activates (=0) SPI_FLOUT7 as flag output for SPI Slave-select */

/* SPI_FLG Bit Positions */
#define FLS1_P			0x0001	/* Enables (=1) SPI_FLOUT1 as flag output for SPI Slave-select */
#define FLS2_P			0x0002	/* Enables (=1) SPI_FLOUT2 as flag output for SPI Slave-select */
#define FLS3_P			0x0003	/* Enables (=1) SPI_FLOUT3 as flag output for SPI Slave-select */
#define FLS4_P			0x0004	/* Enables (=1) SPI_FLOUT4 as flag output for SPI Slave-select */
#define FLS5_P			0x0005	/* Enables (=1) SPI_FLOUT5 as flag output for SPI Slave-select */
#define FLS6_P			0x0006	/* Enables (=1) SPI_FLOUT6 as flag output for SPI Slave-select */
#define FLS7_P			0x0007	/* Enables (=1) SPI_FLOUT7 as flag output for SPI Slave-select */
#define FLG1_P			0x0009	/* Activates (=0) SPI_FLOUT1 as flag output for SPI Slave-select */
#define FLG2_P			0x000A	/* Activates (=0) SPI_FLOUT2 as flag output for SPI Slave-select */
#define FLG3_P			0x000B	/* Activates (=0) SPI_FLOUT3 as flag output for SPI Slave-select */
#define FLG4_P			0x000C	/* Activates (=0) SPI_FLOUT4 as flag output for SPI Slave-select */
#define FLG5_P			0x000D	/* Activates (=0) SPI_FLOUT5 as flag output for SPI Slave-select */
#define FLG6_P			0x000E	/* Activates (=0) SPI_FLOUT6 as flag output for SPI Slave-select */
#define FLG7_P			0x000F	/* Activates (=0) SPI_FLOUT7 as flag output for SPI Slave-select */

/* SPI_STAT Masks */
#define SPIF			0x0001	/* SPI Finished (Single-Word Transfer Complete) */
#define MODF			0x0002	/* Mode Fault Error (Another Device Tried To Become Master) */
#define TXE			0x0004	/* Transmission Error (Data Sent With No New Data In TDBR) */
#define TXS			0x0008	/* SPI_TDBR Data Buffer Status (Full/Empty*) */
#define RBSY			0x0010	/* Receive Error (Data Received With RDBR Full) */
#define RXS			0x0020	/* SPI_RDBR Data Buffer Status (Full/Empty*) */
#define TXCOL			0x0040	/* Transmit Collision Error (Corrupt Data May Have Been Sent) */

#endif
