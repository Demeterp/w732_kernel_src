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

#ifndef __ATA_PIIX_H__
#define __ATA_PIIX_H__

#if (DEBUG_SATA)
#define PRINTF(fmt,args...)	printf (fmt ,##args)
#else
#define PRINTF(fmt,args...)
#endif

struct sata_ioports {
	unsigned long cmd_addr;
	unsigned long data_addr;
	unsigned long error_addr;
	unsigned long feature_addr;
	unsigned long nsect_addr;
	unsigned long lbal_addr;
	unsigned long lbam_addr;
	unsigned long lbah_addr;
	unsigned long device_addr;
	unsigned long status_addr;
	unsigned long command_addr;
	unsigned long altstatus_addr;
	unsigned long ctl_addr;
	unsigned long bmdma_addr;
	unsigned long scr_addr;
};

struct sata_port {
	unsigned char port_no;		/* primary=0, secondary=1	*/
	struct sata_ioports ioaddr;	/* ATA cmd/ctl/dma reg blks	*/
	unsigned char ctl_reg;
	unsigned char last_ctl;
	unsigned char port_state;	/* 1-port is available and	*/
					/* 0-port is not available	*/
	unsigned char dev_mask;
};

/***********SATA LIBRARY SPECIFIC DEFINITIONS AND DECLARATIONS**************/
#ifdef SATA_DECL		/*SATA library specific declarations */
inline void
ata_dump_id (u16 * id)
{
	PRINTF ("49 = 0x%04x  "
		"53 = 0x%04x  "
		"63 = 0x%04x  "
		"64 = 0x%04x  "
		"75 = 0x%04x  \n", id[49], id[53], id[63], id[64], id[75]);
	PRINTF ("80 = 0x%04x  "
		"81 = 0x%04x  "
		"82 = 0x%04x  "
		"83 = 0x%04x  "
		"84 = 0x%04x  \n", id[80], id[81], id[82], id[83], id[84]);
	PRINTF ("88 = 0x%04x  " "93 = 0x%04x\n", id[88], id[93]);
}
#endif

#ifdef SATA_DECL		/*SATA library specific declarations */
int sata_bus_softreset (int num);
void sata_identify (int num, int dev);
void sata_port (struct sata_ioports *ioport);
void set_Feature_cmd (int num, int dev);
int sata_devchk (struct sata_ioports *ioaddr, int dev);
void dev_select (struct sata_ioports *ioaddr, int dev);
u8 sata_busy_wait (struct sata_ioports *ioaddr, int bits, unsigned int max);
u8 sata_chk_status (struct sata_ioports *ioaddr);
ulong sata_read (int device, ulong blknr,lbaint_t blkcnt, void * buffer);
ulong sata_write (int device,ulong blknr, lbaint_t blkcnt, void * buffer);
void msleep (int count);
#endif

/************DRIVER SPECIFIC DEFINITIONS AND DECLARATIONS**************/

#ifdef DRV_DECL			/*Driver specific declaration */
int init_sata (int dev);
#endif

#ifdef DRV_DECL			/*Defines Driver Specific variables */
struct sata_port port[CONFIG_SYS_SATA_MAXBUS];
#endif

#endif /* __ATA_PIIX_H__ */
