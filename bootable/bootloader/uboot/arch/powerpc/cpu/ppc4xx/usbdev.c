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

/*USB 1.1,2.0 device*/

#include <common.h>
#include <asm/processor.h>

#if (defined(CONFIG_440EP) || defined(CONFIG_440EPX)) && defined(CONFIG_CMD_USB)

#include <usb.h>
#include <asm/ppc4xx-uic.h>
#include "usbdev.h"

#define USB_DT_DEVICE        0x01
#define USB_DT_CONFIG        0x02
#define USB_DT_STRING        0x03
#define USB_DT_INTERFACE     0x04
#define USB_DT_ENDPOINT      0x05

int set_value = -1;

void process_endpoints(unsigned short usb2d0_intrin)
{
	/*will hold the packet received */
	struct usb_device_descriptor usb_device_packet;
	struct usb_configuration_descriptor usb_config_packet;
	struct usb_string_descriptor usb_string_packet;
	struct devrequest setup_packet;
	unsigned int *setup_packet_pt;
	unsigned char *packet_pt = NULL;
	int temp, temp1;

	int i;

	/*printf("{USB device} - endpoint 0x%X \n", usb2d0_intrin); */

	/*set usb address, seems to not work unless it is done in the next
	   interrupt, so that is why it is done this way */
	if (set_value != -1)
		*(unsigned char *)USB2D0_FADDR_8 = (unsigned char)set_value;

	/*endpoint 1 */
	if (usb2d0_intrin & 0x01) {
		setup_packet_pt = (unsigned int *)&setup_packet;

		/*copy packet */
		setup_packet_pt[0] = *(unsigned int *)USB2D0_FIFO_0;
		setup_packet_pt[1] = *(unsigned int *)USB2D0_FIFO_0;
		temp = *(unsigned int *)USB2D0_FIFO_0;
		temp1 = *(unsigned int *)USB2D0_FIFO_0;

		/*do some swapping */
		setup_packet.value = swap_16(setup_packet.value);
		setup_packet.index = swap_16(setup_packet.index);
		setup_packet.length = swap_16(setup_packet.length);

		/*clear rx packet */
		*(unsigned short *)USB2D0_INCSR0_8 = 0x48;

		/*printf("0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n", setup_packet.requesttype,
		   setup_packet.request, setup_packet.value,
		   setup_packet.index, setup_packet.length, temp, temp1 ); */

		switch (setup_packet.request) {
		case USB_REQ_GET_DESCRIPTOR:

			switch (setup_packet.value >> 8) {
			case USB_DT_DEVICE:
				/*create packet */
				usb_device_packet.bLength = 18;
				usb_device_packet.bDescriptorType =
				    USB_DT_DEVICE;
#ifdef USB_2_0_DEVICE
				usb_device_packet.bcdUSB = swap_16(0x200);
#else
				usb_device_packet.bcdUSB = swap_16(0x110);
#endif
				usb_device_packet.bDeviceClass = 0xff;
				usb_device_packet.bDeviceSubClass = 0;
				usb_device_packet.bDeviceProtocol = 0;
				usb_device_packet.bMaxPacketSize0 = 32;
				usb_device_packet.idVendor = swap_16(1);
				usb_device_packet.idProduct = swap_16(2);
				usb_device_packet.bcdDevice = swap_16(0x300);
				usb_device_packet.iManufacturer = 1;
				usb_device_packet.iProduct = 1;
				usb_device_packet.iSerialNumber = 1;
				usb_device_packet.bNumConfigurations = 1;

				/*put packet in fifo */
				packet_pt = (unsigned char *)&usb_device_packet;
				break;

			case USB_DT_CONFIG:
				/*create packet */
				usb_config_packet.bLength = 9;
				usb_config_packet.bDescriptorType =
				    USB_DT_CONFIG;
				usb_config_packet.wTotalLength = swap_16(25);
				usb_config_packet.bNumInterfaces = 1;
				usb_config_packet.bConfigurationValue = 1;
				usb_config_packet.iConfiguration = 0;
				usb_config_packet.bmAttributes = 0x40;
				usb_config_packet.bMaxPower = 0;

				/*put packet in fifo */
				packet_pt = (unsigned char *)&usb_config_packet;
				break;

			case USB_DT_STRING:
				/*create packet */
				usb_string_packet.bLength = 2;
				usb_string_packet.bDescriptorType =
				    USB_DT_STRING;
				usb_string_packet.wData[0] = 0x0094;

				/*put packet in fifo */
				packet_pt = (unsigned char *)&usb_string_packet;
				break;
			}

			/*put packet in fifo */
			for (i = 0; i < (setup_packet.length); i++) {
				*(unsigned char *)USB2D0_FIFO_0 = packet_pt[i];
			}

			/*give tx command */
			*(unsigned short *)USB2D0_INCSR0_8 = 0x0a;

			break;

		case USB_REQ_SET_ADDRESS:

			/*copy usb address */
			set_value = setup_packet.value;

			break;
		}

	}
}

void process_other(unsigned char usb2d0_intrusb)
{

	/*check for sof */
	if (usb2d0_intrusb & 0x08) {
		/*printf("{USB device} - sof detected\n"); */
	}

	/*check for reset */
	if (usb2d0_intrusb & 0x04) {
		/*printf("{USB device} - reset detected\n"); */

		/*copy usb address of zero, need to do this when usb reset */
		set_value = 0;
	}

	if (usb2d0_intrusb & 0x02) {
		/*printf("{USB device} - resume detected\n"); */
	}

	if (usb2d0_intrusb & 0x01) {
		/*printf("{USB device} - suspend detected\n"); */
	}
}

int usbInt(void)
{
	/*Must read these 2 registers and use values to clear interrupts.  If you
	   do not read them then the interrupt will not be cleared.  If you do not
	   use the variable the optimizer will not do a read. */
	volatile unsigned short usb2d0_intrin =
	    *(unsigned short *)USB2D0_INTRIN_16;
	volatile unsigned char usb2d0_intrusb =
	    *(unsigned char *)USB2D0_INTRUSB_8;

	/*check if there was an endpoint interrupt */
	if (usb2d0_intrin != 0) {
		process_endpoints(usb2d0_intrin);
	}

	/*check for other interrupts */
	if (usb2d0_intrusb != 0) {
		process_other(usb2d0_intrusb);
	}

	return 0;
}

#if defined(CONFIG_440EPX)
void usb_dev_init()
{
	printf("USB 2.0 Device init\n");

	/*usb dev init */
	*(unsigned char *)USB2D0_POWER_8 = 0xa1;	/* 2.0 */

	/*enable interrupts */
	*(unsigned char *)USB2D0_INTRUSBE_8 = 0x0f;

	irq_install_handler(VECNUM_USBDEV, (interrupt_handler_t *) usbInt,
			    NULL);
}
#else
void usb_dev_init()
{
#ifdef USB_2_0_DEVICE
	printf("USB 2.0 Device init\n");
	/*select 2.0 device */
	mtsdr(SDR0_USB0, 0x0);	/* 2.0 */

	/*usb dev init */
	*(unsigned char *)USB2D0_POWER_8 = 0xa1;	/* 2.0 */
#else
	printf("USB 1.1 Device init\n");
	/*select 1.1 device */
	mtsdr(SDR0_USB0, 0x2);	/* 1.1 */

	/*usb dev init */
	*(unsigned char *)USB2D0_POWER_8 = 0xc0;	/* 1.1 */
#endif

	/*enable interrupts */
	*(unsigned char *)USB2D0_INTRUSBE_8 = 0x0f;

	irq_install_handler(VECNUM_USBDEV, (interrupt_handler_t *) usbInt,
			    NULL);
}
#endif

#endif /* CONFIG_440EP || CONFIG_440EPX */
