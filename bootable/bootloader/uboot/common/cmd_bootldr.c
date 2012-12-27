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
 * U-boot - bootldr.c
 *
 * Copyright (c) 2005-2008 Analog Devices Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * Licensed under the GPL-2 or later.
 */

#include <config.h>
#include <common.h>
#include <command.h>

#include <asm/blackfin.h>
#include <asm/mach-common/bits/bootrom.h>

/* Simple sanity check on the specified address to make sure it contains
 * an LDR image of some sort.
 */
static bool ldr_valid_signature(uint8_t *data)
{
#if defined(__ADSPBF561__)

	/* BF56x has a 4 byte global header */
	if (data[3] == 0xA0)
		return true;

#elif defined(__ADSPBF531__) || defined(__ADSPBF532__) || defined(__ADSPBF533__) || \
      defined(__ADSPBF534__) || defined(__ADSPBF536__) || defined(__ADSPBF537__) || \
      defined(__ADSPBF538__) || defined(__ADSPBF539__)

	/* all the BF53x should start at this address mask */
	uint32_t addr;
	memmove(&addr, data, sizeof(addr));
	if ((addr & 0xFF0FFF0F) == 0xFF000000)
		return true;
#else

	/* everything newer has a magic byte */
	uint32_t count;
	memmove(&count, data + 8, sizeof(count));
	if (data[3] == 0xAD && count == 0)
		return true;

#endif

	return false;
}

/* If the Blackfin is new enough, the Blackfin on-chip ROM supports loading
 * LDRs from random memory addresses.  So whenever possible, use that.  In
 * the older cases (BF53x/BF561), parse the LDR format ourselves.
 */
#define ZEROFILL  0x0001
#define RESVECT   0x0002
#define INIT      0x0008
#define IGNORE    0x0010
#define FINAL     0x8000
static void ldr_load(uint8_t *base_addr)
{
#if defined(__ADSPBF531__) || defined(__ADSPBF532__) || defined(__ADSPBF533__) || \
  /*defined(__ADSPBF534__) || defined(__ADSPBF536__) || defined(__ADSPBF537__) ||*/\
    defined(__ADSPBF538__) || defined(__ADSPBF539__) || defined(__ADSPBF561__)

	uint32_t addr;
	uint32_t count;
	uint16_t flags;

	/* the bf56x has a 4 byte global header ... but it is useless to
	 * us when booting an LDR from a memory address, so skip it
	 */
# ifdef __ADSPBF561__
	base_addr += 4;
# endif

	memmove(&flags, base_addr + 8, sizeof(flags));
	bfin_write_EVT1(flags & RESVECT ? 0xFFA00000 : 0xFFA08000);

	do {
		/* block header may not be aligned */
		memmove(&addr, base_addr, sizeof(addr));
		memmove(&count, base_addr+4, sizeof(count));
		memmove(&flags, base_addr+8, sizeof(flags));
		base_addr += sizeof(addr) + sizeof(count) + sizeof(flags);

		printf("loading to 0x%08x (0x%x bytes) flags: 0x%04x\n",
			addr, count, flags);

		if (!(flags & IGNORE)) {
			if (flags & ZEROFILL)
				memset((void *)addr, 0x00, count);
			else
				memcpy((void *)addr, base_addr, count);

			if (flags & INIT) {
				void (*init)(void) = (void *)addr;
				init();
			}
		}

		if (!(flags & ZEROFILL))
			base_addr += count;
	} while (!(flags & FINAL));

#endif
}

/* For BF537, we use the _BOOTROM_BOOT_DXE_FLASH funky ROM function.
 * For all other BF53x/BF56x, we just call the entry point.
 * For everything else (newer), we use _BOOTROM_MEMBOOT ROM function.
 */
static void ldr_exec(void *addr)
{
#if defined(__ADSPBF534__) || defined(__ADSPBF536__) || defined(__ADSPBF537__)

	/* restore EVT1 to reset value as this is what the bootrom uses as
	 * the default entry point when booting the final block of LDRs
	 */
	bfin_write_EVT1(L1_INST_SRAM);
	__asm__("call (%0);" : : "a"(_BOOTROM_MEMBOOT), "q7"(addr) : "RETS", "memory");

#elif defined(__ADSPBF531__) || defined(__ADSPBF532__) || defined(__ADSPBF533__) || \
      defined(__ADSPBF538__) || defined(__ADSPBF539__) || defined(__ADSPBF561__)

	void (*ldr_entry)(void) = (void *)bfin_read_EVT1();
	ldr_entry();

#else

	int32_t (*BOOTROM_MEM)(void *, int32_t, int32_t, void *) = (void *)_BOOTROM_MEMBOOT;
	BOOTROM_MEM(addr, 0, 0, NULL);

#endif
}

/*
 * the bootldr command loads an address, checks to see if there
 *   is a Boot stream that the on-chip BOOTROM can understand,
 *   and loads it via the BOOTROM Callback. It is possible
 *   to also add booting from SPI, or TWI, but this function does
 *   not currently support that.
 */
int do_bootldr(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	void *addr;

	/* Get the address */
	if (argc < 2)
		addr = (void *)load_addr;
	else
		addr = (void *)simple_strtoul(argv[1], NULL, 16);

	/* Check if it is a LDR file */
	if (ldr_valid_signature(addr)) {
		printf("## Booting ldr image at 0x%p ...\n", addr);
		ldr_load(addr);

		icache_disable();
		dcache_disable();

		ldr_exec(addr);
	} else
		printf("## No ldr image at address 0x%p\n", addr);

	return 0;
}

U_BOOT_CMD(bootldr, 2, 0, do_bootldr,
	"boot ldr image from memory",
	"[addr]\n"
	""
);
