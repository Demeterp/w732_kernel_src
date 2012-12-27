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
 * SPI flash interface
 *
 * Copyright (C) 2008 Atmel Corporation
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>

#include "spi_flash_internal.h"

int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, size_t len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, 8, &cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	if (len) {
		ret = spi_xfer(spi, len * 8, NULL, response, SPI_XFER_END);
		if (ret)
			debug("SF: Failed to read response (%zu bytes): %d\n",
					len, ret);
	}

	return ret;
}

int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send read command (%zu bytes): %d\n",
				cmd_len, ret);
	} else if (data_len != 0) {
		ret = spi_xfer(spi, data_len * 8, NULL, data, SPI_XFER_END);
		if (ret)
			debug("SF: Failed to read %zu bytes of data: %d\n",
					data_len, ret);
	}

	return ret;
}

int spi_flash_cmd_write(struct spi_slave *spi, const u8 *cmd, size_t cmd_len,
		const void *data, size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send read command (%zu bytes): %d\n",
				cmd_len, ret);
	} else if (data_len != 0) {
		ret = spi_xfer(spi, data_len * 8, data, NULL, SPI_XFER_END);
		if (ret)
			debug("SF: Failed to read %zu bytes of data: %d\n",
					data_len, ret);
	}

	return ret;
}


int spi_flash_read_common(struct spi_flash *flash, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	struct spi_slave *spi = flash->spi;
	int ret;

	spi_claim_bus(spi);
	ret = spi_flash_cmd_read(spi, cmd, cmd_len, data, data_len);
	spi_release_bus(spi);

	return ret;
}

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_slave *spi;
	struct spi_flash *flash;
	int ret;
	u8 idcode[5];

	spi = spi_setup_slave(bus, cs, max_hz, spi_mode);
	if (!spi) {
		debug("SF: Failed to set up slave\n");
		return NULL;
	}

	ret = spi_claim_bus(spi);
	if (ret) {
		debug("SF: Failed to claim SPI bus: %d\n", ret);
		goto err_claim_bus;
	}

	/* Read the ID codes */
	ret = spi_flash_cmd(spi, CMD_READ_ID, &idcode, sizeof(idcode));
	if (ret)
		goto err_read_id;

	debug("SF: Got idcode %02x %02x %02x %02x %02x\n", idcode[0],
			idcode[1], idcode[2], idcode[3], idcode[4]);

	switch (idcode[0]) {
#ifdef CONFIG_SPI_FLASH_SPANSION
	case 0x01:
		flash = spi_flash_probe_spansion(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_ATMEL
	case 0x1F:
		flash = spi_flash_probe_atmel(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_MACRONIX
	case 0xc2:
		flash = spi_flash_probe_macronix(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND
	case 0xef:
		flash = spi_flash_probe_winbond(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_STMICRO
	case 0x20:
		flash = spi_flash_probe_stmicro(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_SST
	case 0xBF:
		flash = spi_flash_probe_sst(spi, idcode);
		break;
#endif
	default:
		debug("SF: Unsupported manufacturer %02X\n", idcode[0]);
		flash = NULL;
		break;
	}

	if (!flash)
		goto err_manufacturer_probe;

	spi_release_bus(spi);

	return flash;

err_manufacturer_probe:
err_read_id:
	spi_release_bus(spi);
err_claim_bus:
	spi_free_slave(spi);
	return NULL;
}

void spi_flash_free(struct spi_flash *flash)
{
	spi_free_slave(flash->spi);
	free(flash);
}
