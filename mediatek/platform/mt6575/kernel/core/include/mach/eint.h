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

#ifndef __MT6575_EINT_H__
#define __MT6575_EINT_H__

/*
 * Define hardware registers.
 */

#define EINT_STA        (EINT_BASE + 0x0000)
#define EINT_INTACK     (EINT_BASE + 0x0008)
#define EINT_EEVT       (EINT_BASE + 0x0010)
#define EINT_MASK       (EINT_BASE + 0x0018)
#define EINT_MASK_SET   (EINT_BASE + 0x0020)
#define EINT_MASK_CLR   (EINT_BASE + 0x0028)
#define EINT_SENS       (EINT_BASE + 0x0030)
#define EINT_SENS_SET   (EINT_BASE + 0x0038)
#define EINT_SENS_CLR   (EINT_BASE + 0x0040)
#define EINT_D0EN       (EINT_BASE + 0x0060)
#define EINT_D1EN       (EINT_BASE + 0x0068)
#define EINT_D2EN       (EINT_BASE + 0x0070)
#define EINT_CON(n)     (EINT_BASE + 0x0080 + 4 * (n))
#define EINT_DMASK      (EINT_BASE + 0x0100)
#define EINT_DMASK_SET  (EINT_BASE + 0x0110)
#define EINT_DMASK_CLR  (EINT_BASE + 0x0120)

/*
 * Define constants.
 */

#define EINT_MAX_CHANNEL    (32)
#define MT65XX_EINT_POL_NEG (0)
#define MT65XX_EINT_POL_POS (1)

/*
 * Define function prototypes.
 */

extern void mt65xx_eint_mask(unsigned int eint_num);
extern void mt65xx_eint_unmask(unsigned int eint_num);
extern void mt65xx_deint_mask(unsigned int eint_num);
extern void mt65xx_deint_unmask(unsigned int eint_num);
extern void mt65xx_eint_set_hw_debounce(unsigned int eint_num, unsigned int ms);
extern void mt65xx_eint_set_polarity(unsigned int eint_num, unsigned int pol);
extern unsigned int mt65xx_eint_set_sens(unsigned int eint_num, unsigned int sens);
extern void mt65xx_eint_registration(unsigned int eint_num, unsigned int is_deb_en, unsigned int pol, void (EINT_FUNC_PTR)(void), unsigned int is_auto_umask);
extern void mt65xx_deint_registration(unsigned int eint_num, unsigned int pol, void (EINT_FUNC_PTR)(void), unsigned int is_auto_umask);
extern int mt65xx_eint_init(void);
extern int get_td_eint_num(char *td_name, int len);

#endif  /*!__MT6575_EINT_H__ */
