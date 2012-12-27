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

#ifndef I2C_EXPORT_H
#define I2C_EXPORT_H

/****************************************************
 *
 * Copyright Motrola 1999
 *
 ****************************************************/

/* These are the defined return values for the I2C_do_transaction function.
 * Any non-zero value indicates failure.  Failure modes can be added for
 * more detailed error reporting.
 */
typedef enum _i2c_status
{
 I2C_SUCCESS     = 0,
 I2C_ERROR,
} I2C_Status;

/* These are the defined tasks for I2C_do_transaction.
 * Modes for SLAVE_RCV and SLAVE_XMIT will be added.
 */
typedef enum _i2c_transaction_mode
{
	I2C_MASTER_RCV =  0,
	I2C_MASTER_XMIT = 1,
} I2C_TRANSACTION_MODE;

typedef enum _i2c_interrupt_mode
{
	I2C_INT_DISABLE =  0,
	I2C_INT_ENABLE = 1,
} I2C_INTERRUPT_MODE;

typedef enum _i2c_stop
{
	I2C_NO_STOP =  0,
	I2C_STOP = 1,
} I2C_STOP_MODE;

typedef enum _i2c_restart
{
	I2C_NO_RESTART =  0,
	I2C_RESTART = 1,
} I2C_RESTART_MODE;

/******************** App. API ********************
 * The application API is for user level application
 * to use the functionality provided by I2C driver.
 * This is a "generic" I2C interface, it should contain
 * nothing specific to the Kahlua implementation.
 * Only the generic functions are exported by the library.
 *
 * Note: Its App.s responsibility to swap the data
 *       byte. In our API, we just transfer whatever
 *       we are given
 **************************************************/


/*  Initialize I2C unit with the following:
 *  driver's slave address
 *  interrupt enabled
 *  optional pointer to application layer print function
 *
 *  These parameters may be added:
 *  desired clock rate
 *  digital filter frequency sampling rate
 *
 *  This function must be called before I2C unit can be used.
 */
extern I2C_Status I2C_Initialize(
	unsigned char addr,            /* driver's I2C slave address */
	I2C_INTERRUPT_MODE en_int,     /* 1 - enable I2C interrupt
					* 0 - disable I2C interrupt
					*/
	int (*app_print_function)(char *,...)); /* pointer to optional "printf"
						 * provided by application
						 */

/* Perform the given I2C transaction, only MASTER_XMIT and MASTER_RCV
 * are implemented.  Both are only in polling mode.
 *
 * en_int controls interrupt/polling mode
 * act is the type of transaction
 * addr is the I2C address of the slave device
 * len is the length of data to send or receive
 * buffer is the address of the data buffer
 * stop = I2C_NO_STOP, don't signal STOP at end of transaction
 *        I2C_STOP, signal STOP at end of transaction
 * retry is the timeout retry value, currently ignored
 * rsta = I2C_NO_RESTART, this is not continuation of existing transaction
 *        I2C_RESTART, this is a continuation of existing transaction
 */
extern I2C_Status I2C_do_transaction( I2C_INTERRUPT_MODE en_int,
				      I2C_TRANSACTION_MODE act,
				      unsigned char i2c_addr,
				      unsigned char data_addr,
				      int len,
				      char *buffer,
				      I2C_STOP_MODE stop,
				      int retry,
				      I2C_RESTART_MODE rsta);
#endif
