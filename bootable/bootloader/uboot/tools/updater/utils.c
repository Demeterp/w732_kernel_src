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

#include <common.h>
#include <asm/processor.h>
#include <memio.h>
#include <linux/ctype.h>

static __inline__ unsigned long
get_msr(void)
{
	unsigned long msr;

	asm volatile("mfmsr %0" : "=r" (msr) :);
	return msr;
}

static __inline__ void
set_msr(unsigned long msr)
{
	asm volatile("mtmsr %0" : : "r" (msr));
}

static __inline__ unsigned long
get_dec(void)
{
	unsigned long val;

	asm volatile("mfdec %0" : "=r" (val) :);
	return val;
}


static __inline__ void
set_dec(unsigned long val)
{
	asm volatile("mtdec %0" : : "r" (val));
}


void
enable_interrupts(void)
{
    set_msr (get_msr() | MSR_EE);
}

/* returns flag if MSR_EE was set before */
int
disable_interrupts(void)
{
    ulong msr;

    msr = get_msr();
    set_msr (msr & ~MSR_EE);
    return ((msr & MSR_EE) != 0);
}

u8 in8(u32 port)
{
    return in_byte(port);
}

void out8(u32 port, u8 val)
{
    out_byte(port, val);
}

unsigned long in32(u32 port)
{
    return in_long(port);
}

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

long simple_strtol(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}

static inline void
soft_restart(unsigned long addr)
{
	/* SRR0 has system reset vector, SRR1 has default MSR value */
	/* rfi restores MSR from SRR1 and sets the PC to the SRR0 value */

	__asm__ __volatile__ ("mtspr    26, %0"         :: "r" (addr));
	__asm__ __volatile__ ("li       4, (1 << 6)"    ::: "r4");
	__asm__ __volatile__ ("mtspr    27, 4");
	__asm__ __volatile__ ("rfi");

	while(1);       /* not reached */
}

void
do_reset (void)
{
	ulong addr;
	/* flush and disable I/D cache */
	__asm__ __volatile__ ("mfspr    3, 1008"        ::: "r3");
	__asm__ __volatile__ ("ori      5, 5, 0xcc00"   ::: "r5");
	__asm__ __volatile__ ("ori      4, 3, 0xc00"    ::: "r4");
	__asm__ __volatile__ ("andc     5, 3, 5"        ::: "r5");
	__asm__ __volatile__ ("sync");
	__asm__ __volatile__ ("mtspr    1008, 4");
	__asm__ __volatile__ ("isync");
	__asm__ __volatile__ ("sync");
	__asm__ __volatile__ ("mtspr    1008, 5");
	__asm__ __volatile__ ("isync");
	__asm__ __volatile__ ("sync");

#ifdef CONFIG_SYS_RESET_ADDRESS
	addr = CONFIG_SYS_RESET_ADDRESS;
#else
	/*
	 * note: when CONFIG_SYS_MONITOR_BASE points to a RAM address,
	 * CONFIG_SYS_MONITOR_BASE - sizeof (ulong) is usually a valid
	 * address. Better pick an address known to be invalid on your
	 * system and assign it to CONFIG_SYS_RESET_ADDRESS.
	 */
	addr = CONFIG_SYS_MONITOR_BASE - sizeof (ulong);
#endif
	soft_restart(addr);
	while(1);       /* not reached */
}
