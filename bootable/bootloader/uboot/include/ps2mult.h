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

#ifndef __LINUX_PS2MULT_H
#define __LINUX_PS2MULT_H

#define kbd_request_region()		ps2mult_init()
#define kbd_request_irq(handler)	ps2mult_request_irq(handler)

#define kbd_read_input()		ps2mult_read_input()
#define kbd_read_status()		ps2mult_read_status()
#define kbd_write_output(val)		ps2mult_write_output(val)
#define kbd_write_command(val)		ps2mult_write_command(val)

#define aux_request_irq(hand, dev_id)	0
#define aux_free_irq(dev_id)

#define PS2MULT_KB_SELECTOR		0xA0
#define PS2MULT_MS_SELECTOR		0xA1
#define PS2MULT_ESCAPE			0x7D
#define PS2MULT_BSYNC			0x7E
#define PS2MULT_SESSION_START		0x55
#define PS2MULT_SESSION_END		0x56

#define	PS2BUF_SIZE			512	/* power of 2, please */

#ifndef CONFIG_PS2MULT_DELAY
#define CONFIG_PS2MULT_DELAY	(CONFIG_SYS_HZ/2)	/* Initial delay	*/
#endif

  /* PS/2 controller interface (include/asm/keyboard.h)
   */
extern int ps2mult_init (void);
extern int ps2mult_request_irq(void (*handler)(void *));
extern u_char ps2mult_read_input(void);
extern u_char ps2mult_read_status(void);
extern void ps2mult_write_output(u_char val);
extern void ps2mult_write_command(u_char val);

extern void ps2mult_early_init (void);
extern void ps2mult_callback (int in_cnt);

  /* Simple serial interface
   */
extern int ps2ser_init(void);
extern void ps2ser_putc(int chr);
extern int ps2ser_getc(void);
extern int ps2ser_check(void);


  /* Serial related stuff
   */
struct serial_state {
	int	baud_base;
	int	irq;
	u8	*iomem_base;
};

#endif /* __LINUX_PS2MULT_H */
