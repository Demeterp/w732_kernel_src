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
 * (C) Copyright 2010 DENX Software Engineering,
 * Anatolij Gustschin, agust@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Co-Processor communication POST
 */
#include <common.h>
#include <post.h>
#include <serial.h>

#if defined(CONFIG_SERIAL_MULTI)

/*
 * Actually the termination sequence of the coprocessor
 * commands is "\r\n" (CR LF), but here we use a side effect of
 * the putc() routine of the serial driver which checks for LF
 * and sends CR before sending LF. Therefore the termination
 * sequence in the command below is only "\n".
 * "alive" string is the coprocessor response for ping command
 * and not a command, therefore it is terminated with "\r\n".
 */
char alive[] = "$AL;38\r\n";
char ping[] = "$PI;2C\n";

int coprocessor_post_test(int flags)
{
	struct stdio_dev *cop_port;
	int ret;
	char buf[10];

	/* Test IO Coprocessor communication */
	cop_port = open_port(4, CONFIG_SYS_PDM360NG_COPROC_BAUDRATE);
	if (!cop_port)
		return -1;

	write_port(cop_port, ping);
	udelay(CONFIG_SYS_PDM360NG_COPROC_READ_DELAY);

	memset(buf, 0, sizeof(buf));
	ret = read_port(cop_port, buf, sizeof(buf));
	close_port(4);
	if (ret <= 0) {
		post_log("Error: Can't read IO Coprocessor port.\n");
		return -1;
	}

	if (strcmp(buf, alive)) {
		post_log("Error: IO-Cop. resp.: %s\n", buf);
		return -1;
	}

	/* Test WD Coprocessor communication */
	cop_port = open_port(1, CONFIG_SYS_PDM360NG_COPROC_BAUDRATE);
	if (!cop_port) {
		post_log("Error: Can't open WD Coprocessor port.\n");
		return -1;
	}

	write_port(cop_port, ping);
	udelay(CONFIG_SYS_PDM360NG_COPROC_READ_DELAY);

	memset(buf, 0, sizeof(buf));
	ret = read_port(cop_port, buf, sizeof(buf));
	close_port(1);
	if (ret <= 0) {
		post_log("Error: Can't read WD Coprocessor port.\n");
		return -1;
	}

	if (strcmp(buf, alive)) {
		post_log("Error: WD-Cop. resp.: %s\n", buf);
		return -1;
	}

	return 0;
}
#endif	/* CONFIG_SERIAL_MULTI */
