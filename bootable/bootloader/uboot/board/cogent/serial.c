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
 * Simple serial driver for Cogent motherboard serial ports
 * for use during boot
 */

#include <common.h>
#include <board/cogent/serial.h>

DECLARE_GLOBAL_DATA_PTR;

#if (CMA_MB_CAPS & CMA_MB_CAP_SERPAR)

#if (defined(CONFIG_8xx) && defined(CONFIG_8xx_CONS_NONE)) || \
     (defined(CONFIG_8260) && defined(CONFIG_CONS_NONE))

#if CONFIG_CONS_INDEX == 1
#define CMA_MB_SERIAL_BASE	CMA_MB_SERIALA_BASE
#elif CONFIG_CONS_INDEX == 2
#define CMA_MB_SERIAL_BASE	CMA_MB_SERIALB_BASE
#elif CONFIG_CONS_INDEX == 3 && (CMA_MB_CAPS & CMA_MB_CAP_SER2)
#define CMA_MB_SERIAL_BASE	CMA_MB_SER2A_BASE
#elif CONFIG_CONS_INDEX == 4 && (CMA_MB_CAPS & CMA_MB_CAP_SER2)
#define CMA_MB_SERIAL_BASE	CMA_MB_SER2B_BASE
#else
#error CONFIG_CONS_INDEX must be configured for Cogent motherboard serial
#endif

int serial_init (void)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_SERIAL_BASE;

	cma_mb_reg_write (&mbsp->ser_ier, 0x00);	/* turn off interrupts */
	serial_setbrg ();
	cma_mb_reg_write (&mbsp->ser_lcr, 0x03);	/* 8 data, 1 stop, no parity */
	cma_mb_reg_write (&mbsp->ser_mcr, 0x03);	/* RTS/DTR */
	cma_mb_reg_write (&mbsp->ser_fcr, 0x07);	/* Clear & enable FIFOs */

	return (0);
}

void serial_setbrg (void)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_SERIAL_BASE;
	unsigned int divisor;
	unsigned char lcr;

	if ((divisor = br_to_div (gd->baudrate)) == 0)
		divisor = DEFDIV;

	lcr = cma_mb_reg_read (&mbsp->ser_lcr);
	cma_mb_reg_write (&mbsp->ser_lcr, lcr | 0x80);	/* Access baud rate(set DLAB) */
	cma_mb_reg_write (&mbsp->ser_brl, divisor & 0xff);
	cma_mb_reg_write (&mbsp->ser_brh, (divisor >> 8) & 0xff);
	cma_mb_reg_write (&mbsp->ser_lcr, lcr);	/* unset DLAB */
}

void serial_putc (const char c)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_SERIAL_BASE;

	if (c == '\n')
		serial_putc ('\r');

	while ((cma_mb_reg_read (&mbsp->ser_lsr) & LSR_THRE) == 0);

	cma_mb_reg_write (&mbsp->ser_thr, c);
}

void serial_puts (const char *s)
{
	while (*s != '\0')
		serial_putc (*s++);
}

int serial_getc (void)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_SERIAL_BASE;

	while ((cma_mb_reg_read (&mbsp->ser_lsr) & LSR_DR) == 0);

	return ((int) cma_mb_reg_read (&mbsp->ser_rhr) & 0x7f);
}

int serial_tstc (void)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_SERIAL_BASE;

	return ((cma_mb_reg_read (&mbsp->ser_lsr) & LSR_DR) != 0);
}

#endif /* CONS_NONE */

#if defined(CONFIG_CMD_KGDB) && \
    defined(CONFIG_KGDB_NONE)

#if CONFIG_KGDB_INDEX == CONFIG_CONS_INDEX
#error Console and kgdb are on the same serial port - this is not supported
#endif

#if CONFIG_KGDB_INDEX == 1
#define CMA_MB_KGDB_SER_BASE	CMA_MB_SERIALA_BASE
#elif CONFIG_KGDB_INDEX == 2
#define CMA_MB_KGDB_SER_BASE	CMA_MB_SERIALB_BASE
#elif CONFIG_KGDB_INDEX == 3 && (CMA_MB_CAPS & CMA_MB_CAP_SER2)
#define CMA_MB_KGDB_SER_BASE	CMA_MB_SER2A_BASE
#elif CONFIG_KGDB_INDEX == 4 && (CMA_MB_CAPS & CMA_MB_CAP_SER2)
#define CMA_MB_KGDB_SER_BASE	CMA_MB_SER2B_BASE
#else
#error CONFIG_KGDB_INDEX must be configured for Cogent motherboard serial
#endif

void kgdb_serial_init (void)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_KGDB_SER_BASE;
	unsigned int divisor;

	if ((divisor = br_to_div (CONFIG_KGDB_BAUDRATE)) == 0)
		divisor = DEFDIV;

	cma_mb_reg_write (&mbsp->ser_ier, 0x00);	/* turn off interrupts */
	cma_mb_reg_write (&mbsp->ser_lcr, 0x80);	/* Access baud rate(set DLAB) */
	cma_mb_reg_write (&mbsp->ser_brl, divisor & 0xff);
	cma_mb_reg_write (&mbsp->ser_brh, (divisor >> 8) & 0xff);
	cma_mb_reg_write (&mbsp->ser_lcr, 0x03);	/* 8 data, 1 stop, no parity */
	cma_mb_reg_write (&mbsp->ser_mcr, 0x03);	/* RTS/DTR */
	cma_mb_reg_write (&mbsp->ser_fcr, 0x07);	/* Clear & enable FIFOs */

	printf ("[on cma10x serial port B] ");
}

void putDebugChar (int c)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_KGDB_SER_BASE;

	while ((cma_mb_reg_read (&mbsp->ser_lsr) & LSR_THRE) == 0);

	cma_mb_reg_write (&mbsp->ser_thr, c & 0xff);
}

void putDebugStr (const char *str)
{
	while (*str != '\0') {
		if (*str == '\n')
			putDebugChar ('\r');
		putDebugChar (*str++);
	}
}

int getDebugChar (void)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_KGDB_SER_BASE;

	while ((cma_mb_reg_read (&mbsp->ser_lsr) & LSR_DR) == 0);

	return ((int) cma_mb_reg_read (&mbsp->ser_rhr) & 0x7f);
}

void kgdb_interruptible (int yes)
{
	cma_mb_serial *mbsp = (cma_mb_serial *) CMA_MB_KGDB_SER_BASE;

	if (yes == 1) {
		printf ("kgdb: turning serial ints on\n");
		cma_mb_reg_write (&mbsp->ser_ier, 0xf);
	} else {
		printf ("kgdb: turning serial ints off\n");
		cma_mb_reg_write (&mbsp->ser_ier, 0x0);
	}
}

#endif /* KGDB && KGDB_NONE */

#endif /* CAPS & SERPAR */
