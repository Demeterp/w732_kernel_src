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

#ifndef _MICROBLAZE_BITOPS_H
#define _MICROBLAZE_BITOPS_H

/*
 * Copyright 1992, Linus Torvalds.
 */

#include <linux/config.h>
#include <asm/byteorder.h>	/* swab32 */
#include <asm/system.h>		/* save_flags */

#ifdef __KERNEL__
/*
 * Function prototypes to keep gcc -Wall happy
 */

/*
 * The __ functions are not atomic
 */

extern void set_bit(int nr, volatile void * addr);
extern void __set_bit(int nr, volatile void * addr);

extern void clear_bit(int nr, volatile void * addr);
#define __clear_bit(nr, addr) clear_bit(nr, addr)
#define PLATFORM__CLEAR_BIT

extern void change_bit(int nr, volatile void * addr);
extern void __change_bit(int nr, volatile void * addr);
extern int test_and_set_bit(int nr, volatile void * addr);
extern int __test_and_set_bit(int nr, volatile void * addr);
extern int test_and_clear_bit(int nr, volatile void * addr);
extern int __test_and_clear_bit(int nr, volatile void * addr);
extern int test_and_change_bit(int nr, volatile void * addr);
extern int __test_and_change_bit(int nr, volatile void * addr);
extern int __constant_test_bit(int nr, const volatile void * addr);
extern int __test_bit(int nr, volatile void * addr);
extern int find_first_zero_bit(void * addr, unsigned size);
extern int find_next_zero_bit (void * addr, int size, int offset);

/*
 * ffz = Find First Zero in word. Undefined if no zero exists,
 * so code should check against ~0UL first..
 */
extern __inline__ unsigned long ffz(unsigned long word)
{
	unsigned long result = 0;

	while(word & 1) {
		result++;
		word >>= 1;
	}
	return result;
}


extern __inline__ void set_bit(int nr, volatile void * addr)
{
	int	* a = (int *) addr;
	int	mask;
	unsigned long flags;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	save_flags_cli(flags);
	*a |= mask;
	restore_flags(flags);
}

extern __inline__ void __set_bit(int nr, volatile void * addr)
{
	int	* a = (int *) addr;
	int	mask;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	*a |= mask;
}
#define PLATFORM__SET_BIT

/*
 * clear_bit() doesn't provide any barrier for the compiler.
 */
#define smp_mb__before_clear_bit()	barrier()
#define smp_mb__after_clear_bit()	barrier()

extern __inline__ void clear_bit(int nr, volatile void * addr)
{
	int	* a = (int *) addr;
	int	mask;
	unsigned long flags;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	save_flags_cli(flags);
	*a &= ~mask;
	restore_flags(flags);
}

extern __inline__ void change_bit(int nr, volatile void * addr)
{
	int mask;
	unsigned long flags;
	unsigned long *ADDR = (unsigned long *) addr;

	ADDR += nr >> 5;
	mask = 1 << (nr & 31);
	save_flags_cli(flags);
	*ADDR ^= mask;
	restore_flags(flags);
}

extern __inline__ void __change_bit(int nr, volatile void * addr)
{
	int mask;
	unsigned long *ADDR = (unsigned long *) addr;

	ADDR += nr >> 5;
	mask = 1 << (nr & 31);
	*ADDR ^= mask;
}

extern __inline__ int test_and_set_bit(int nr, volatile void * addr)
{
	int	mask, retval;
	volatile unsigned int *a = (volatile unsigned int *) addr;
	unsigned long flags;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	save_flags_cli(flags);
	retval = (mask & *a) != 0;
	*a |= mask;
	restore_flags(flags);

	return retval;
}

extern __inline__ int __test_and_set_bit(int nr, volatile void * addr)
{
	int	mask, retval;
	volatile unsigned int *a = (volatile unsigned int *) addr;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	retval = (mask & *a) != 0;
	*a |= mask;
	return retval;
}

extern __inline__ int test_and_clear_bit(int nr, volatile void * addr)
{
	int	mask, retval;
	volatile unsigned int *a = (volatile unsigned int *) addr;
	unsigned long flags;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	save_flags_cli(flags);
	retval = (mask & *a) != 0;
	*a &= ~mask;
	restore_flags(flags);

	return retval;
}

extern __inline__ int __test_and_clear_bit(int nr, volatile void * addr)
{
	int	mask, retval;
	volatile unsigned int *a = (volatile unsigned int *) addr;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	retval = (mask & *a) != 0;
	*a &= ~mask;
	return retval;
}

extern __inline__ int test_and_change_bit(int nr, volatile void * addr)
{
	int	mask, retval;
	volatile unsigned int *a = (volatile unsigned int *) addr;
	unsigned long flags;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	save_flags_cli(flags);
	retval = (mask & *a) != 0;
	*a ^= mask;
	restore_flags(flags);

	return retval;
}

extern __inline__ int __test_and_change_bit(int nr, volatile void * addr)
{
	int	mask, retval;
	volatile unsigned int *a = (volatile unsigned int *) addr;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	retval = (mask & *a) != 0;
	*a ^= mask;
	return retval;
}

/*
 * This routine doesn't need to be atomic.
 */
extern __inline__ int __constant_test_bit(int nr, const volatile void * addr)
{
	return ((1UL << (nr & 31)) & (((const volatile unsigned int *) addr)[nr >> 5])) != 0;
}

extern __inline__ int __test_bit(int nr, volatile void * addr)
{
	int	* a = (int *) addr;
	int	mask;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *a) != 0);
}

#define test_bit(nr,addr) \
(__builtin_constant_p(nr) ? \
 __constant_test_bit((nr),(addr)) : \
 __test_bit((nr),(addr)))

#define find_first_zero_bit(addr, size) \
	find_next_zero_bit((addr), (size), 0)

extern __inline__ int find_next_zero_bit (void * addr, int size, int offset)
{
	unsigned long *p = ((unsigned long *) addr) + (offset >> 5);
	unsigned long result = offset & ~31UL;
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset &= 31UL;
	if (offset) {
		tmp = *(p++);
		tmp |= ~0UL >> (32-offset);
		if (size < 32)
			goto found_first;
		if (~tmp)
			goto found_middle;
		size -= 32;
		result += 32;
	}
	while (size & ~31UL) {
		if (~(tmp = *(p++)))
			goto found_middle;
		result += 32;
		size -= 32;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp |= ~0UL >> size;
found_middle:
	return result + ffz(tmp);
}

/*
 * hweightN: returns the hamming weight (i.e. the number
 * of bits set) of a N-bit word
 */

#define hweight32(x) generic_hweight32(x)
#define hweight16(x) generic_hweight16(x)
#define hweight8(x) generic_hweight8(x)


extern __inline__ int ext2_set_bit(int nr, volatile void * addr)
{
	int		mask, retval;
	unsigned long	flags;
	volatile unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	save_flags_cli(flags);
	retval = (mask & *ADDR) != 0;
	*ADDR |= mask;
	restore_flags(flags);
	return retval;
}

extern __inline__ int ext2_clear_bit(int nr, volatile void * addr)
{
	int		mask, retval;
	unsigned long	flags;
	volatile unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	save_flags_cli(flags);
	retval = (mask & *ADDR) != 0;
	*ADDR &= ~mask;
	restore_flags(flags);
	return retval;
}

extern __inline__ int ext2_test_bit(int nr, const volatile void * addr)
{
	int			mask;
	const volatile unsigned char	*ADDR = (const unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	return ((mask & *ADDR) != 0);
}

#define ext2_find_first_zero_bit(addr, size) \
	ext2_find_next_zero_bit((addr), (size), 0)

extern __inline__ unsigned long ext2_find_next_zero_bit(void *addr, unsigned long size, unsigned long offset)
{
	unsigned long *p = ((unsigned long *) addr) + (offset >> 5);
	unsigned long result = offset & ~31UL;
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset &= 31UL;
	if(offset) {
		/* We hold the little endian value in tmp, but then the
		 * shift is illegal. So we could keep a big endian value
		 * in tmp, like this:
		 *
		 * tmp = __swab32(*(p++));
		 * tmp |= ~0UL >> (32-offset);
		 *
		 * but this would decrease preformance, so we change the
		 * shift:
		 */
		tmp = *(p++);
		tmp |= __swab32(~0UL >> (32-offset));
		if(size < 32)
			goto found_first;
		if(~tmp)
			goto found_middle;
		size -= 32;
		result += 32;
	}
	while(size & ~31UL) {
		if(~(tmp = *(p++)))
			goto found_middle;
		result += 32;
		size -= 32;
	}
	if(!size)
		return result;
	tmp = *p;

found_first:
	/* tmp is little endian, so we would have to swab the shift,
	 * see above. But then we have to swab tmp below for ffz, so
	 * we might as well do this here.
	 */
	return result + ffz(__swab32(tmp) | (~0UL << size));
found_middle:
	return result + ffz(__swab32(tmp));
}

/* Bitmap functions for the minix filesystem.  */
#define minix_test_and_set_bit(nr,addr) test_and_set_bit(nr,addr)
#define minix_set_bit(nr,addr) set_bit(nr,addr)
#define minix_test_and_clear_bit(nr,addr) test_and_clear_bit(nr,addr)
#define minix_test_bit(nr,addr) test_bit(nr,addr)
#define minix_find_first_zero_bit(addr,size) find_first_zero_bit(addr,size)

/**
 * hweightN - returns the hamming weight of a N-bit word
 * @x: the word to weigh
 *
 * The Hamming Weight of a number is the total number of bits set in it.
 */

#define hweight32(x) generic_hweight32(x)
#define hweight16(x) generic_hweight16(x)
#define hweight8(x) generic_hweight8(x)

#endif /* __KERNEL__ */

#endif /* _MICROBLAZE_BITOPS_H */
