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
 */

/*
 * This is used to for host and peripheral modes of the driver for
 * Inventra (Multidrop) Highspeed Dual-Role Controllers:  (M)HDRC.
 *
 * Board initialization should put one of these into dev->platform_data,
 * probably on some platform_device named "musb_hdrc".  It encapsulates
 * key configuration differences between boards.
 */

#ifndef __LINUX_USB_MUSB_H
#define __LINUX_USB_MUSB_H

/* The USB role is defined by the connector used on the board, so long as
 * standards are being followed.  (Developer boards sometimes won't.)
 */
enum musb_mode {
	MUSB_UNDEFINED = 0,
	MUSB_HOST,		/* A or Mini-A connector */
	MUSB_PERIPHERAL,	/* B or Mini-B connector */
	MUSB_OTG		/* Mini-AB connector */
};

struct clk;

enum musb_fifo_style {
	FIFO_RXTX,
	FIFO_TX,
	FIFO_RX
} __attribute__ ((packed));

enum musb_buf_mode {
	BUF_SINGLE,
	BUF_DOUBLE
} __attribute__ ((packed));

enum musb_ep_mode {
	EP_CONT,
	EP_INT,
	EP_BULK,
	EP_ISO
} __attribute__ ((packed));


struct musb_fifo_cfg {
	u8			hw_ep_num;
	enum musb_ep_mode ep_mode;
	enum musb_fifo_style	style;
	enum musb_buf_mode	mode;
	u16			maxpacket;
};

#define MUSB_EP_FIFO(ep, st, m, pkt)		\
{						\
	.hw_ep_num	= ep,			\
	.style		= st,			\
	.mode		= m,			\
	.maxpacket	= pkt,			\
}

#define MUSB_EP_FIFO_SINGLE(ep, st, pkt)	\
	MUSB_EP_FIFO(ep, st, BUF_SINGLE, pkt)

#define MUSB_EP_FIFO_DOUBLE(ep, st, pkt)	\
	MUSB_EP_FIFO(ep, st, BUF_DOUBLE, pkt)

struct musb_hdrc_eps_bits {
	const char	name[16];
	u8		bits;
};

struct musb_hdrc_config {
	struct musb_fifo_cfg	*fifo_cfg;	/* board fifo configuration */
	unsigned		fifo_cfg_size;	/* size of the fifo configuration */

	/* MUSB configuration-specific details */
	unsigned	multipoint:1;	/* multipoint device */
	unsigned	dyn_fifo:1 __deprecated; /* supports dynamic fifo sizing */
	unsigned	soft_con:1 __deprecated; /* soft connect required */
	unsigned	utm_16:1 __deprecated; /* utm data witdh is 16 bits */
	unsigned	big_endian:1;	/* true if CPU uses big-endian */
	unsigned	mult_bulk_tx:1;	/* Tx ep required for multbulk pkts */
	unsigned	mult_bulk_rx:1;	/* Rx ep required for multbulk pkts */
	unsigned	high_iso_tx:1;	/* Tx ep required for HB iso */
	unsigned	high_iso_rx:1;	/* Rx ep required for HD iso */
	unsigned	dma:1 __deprecated; /* supports DMA */
	unsigned	vendor_req:1 __deprecated; /* vendor registers required */

	u8		num_eps;	/* number of endpoints _with_ ep0 */
	u8		dma_channels __deprecated; /* number of dma channels */
	u8		dyn_fifo_size;	/* dynamic size in bytes */
	u8		vendor_ctrl __deprecated; /* vendor control reg width */
	u8		vendor_stat __deprecated; /* vendor status reg witdh */
	u8		dma_req_chan __deprecated; /* bitmask for required dma channels */
	u8		ram_bits;	/* ram address size */

	struct musb_hdrc_eps_bits *eps_bits __deprecated;
#ifdef CONFIG_BLACKFIN
	/* A GPIO controlling VRSEL in Blackfin */
	unsigned int	gpio_vrsel;
	unsigned int	gpio_vrsel_active;
#endif

};

struct musb_hdrc_platform_data {
	/* MUSB_HOST, MUSB_PERIPHERAL, or MUSB_OTG */
	u8		mode;

	/* for clk_get() */
	const char	*clock;

	/* (HOST or OTG) switch VBUS on/off */
	int		(*set_vbus)(struct device *dev, int is_on);

	/* (HOST or OTG) mA/2 power supplied on (default = 8mA) */
	u8		power;

	/* (PERIPHERAL) mA/2 max power consumed (default = 100mA) */
	u8		min_power;

	/* (HOST or OTG) msec/2 after VBUS on till power good */
	u8		potpgt;

	/* (HOST or OTG) program PHY for external Vbus */
	unsigned	extvbus:1;

	/* Power the device on or off */
	int		(*set_power)(int state);

	/* Turn device clock on or off */
	int		(*set_clock)(struct clk *clock, int is_on);

	/* MUSB configuration-specific details */
	struct musb_hdrc_config	*config;

	/* Architecture specific board data	*/
	void		*board_data;
};


/* TUSB 6010 support */

#define	TUSB6010_OSCCLK_60	16667	/* psec/clk @ 60.0 MHz */
#define	TUSB6010_REFCLK_24	41667	/* psec/clk @ 24.0 MHz XI */
#define	TUSB6010_REFCLK_19	52083	/* psec/clk @ 19.2 MHz CLKIN */

#ifdef	CONFIG_ARCH_OMAP2

extern int __init tusb6010_setup_interface(
		struct musb_hdrc_platform_data *data,
		unsigned ps_refclk, unsigned waitpin,
		unsigned async_cs, unsigned sync_cs,
		unsigned irq, unsigned dmachan);

extern int tusb6010_platform_retime(unsigned is_refclk);

#endif	/* OMAP2 */

#endif /* __LINUX_USB_MUSB_H */
