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

#include <common.h>
#include <asm/arch/mt65xx.h>
#include <asm/arch/mt65xx_typedefs.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/arch/mt6575_nand.h>
#include "cust_nand.h"
#include "mt65xx_partition.h"
#include "pmt.h"

#ifdef MTK_EMMC_SUPPORT
#define PMT 1
#else
#define PMT 1
#endif
//common
//BLK_SIZE is 512, block_size is from flash is 128K
static u32 block_size;
static u32 page_size;
extern flashdev_info devinfo;
extern pt_resident lastest_part[PART_MAX_COUNT];
extern part_t mt6575_parts[];

extern int total_size;
extern struct mt6575_CMD g_kCMD;
static pt_info pi;

//if used malloc func ,the pdata = (uchar*)malloc(sizeof(uchar)*size);
// in recovery_check_command_trigger will return 0
//static char *page_buf;  

unsigned char page_buf[4096+128];
unsigned char backup_buf[2048];
#ifdef MTK_EMMC_SUPPORT
#define CFG_EMMC_PMT_SIZE 0xc00000
extern int g_user_virt_addr;
extern u64 g_emmc_size;
pt_resident32 lastest_part32[PART_MAX_COUNT];
#endif

#ifdef PMT
void get_part_tab_from_complier(void)
{
#ifdef MTK_EMMC_SUPPORT
	int index=0;
	printf("get_pt_from_complier \n");
	while(mt6575_parts[index].flags!= PART_FLAG_END)
	{
    		
		memcpy(lastest_part[index].name,mt6575_parts[index].name,MAX_PARTITION_NAME_LEN);
		lastest_part[index].size = (u64)mt6575_parts[index].blknum*BLK_SIZE ;
		lastest_part[index].offset = (u64)mt6575_parts[index].startblk * BLK_SIZE;
		lastest_part[index].mask_flags =  mt6575_parts[index].flags;  //this flag in kernel should be fufilled even though in flash is 0.
		printf ("get_ptr  %s %016llx \n",lastest_part[index].name,lastest_part[index].offset);
		index++;
	}
#else
	int index=0;
	printf("get_pt_from_complier \n");
	while(mt6575_parts[index].flags!= PART_FLAG_END)
	{
    		
		memcpy(lastest_part[index].name,mt6575_parts[index].name,MAX_PARTITION_NAME_LEN);
		lastest_part[index].size = mt6575_parts[index].blknum*BLK_SIZE ;
		lastest_part[index].offset = mt6575_parts[index].startblk * BLK_SIZE;
		lastest_part[index].mask_flags =  mt6575_parts[index].flags;  //this flag in kernel should be fufilled even though in flash is 0.
		printf ("get_ptr  %s %lx \n",lastest_part[index].name,lastest_part[index].offset);
		index++;
	}
#endif
}

#ifndef MTK_EMMC_SUPPORT
bool find_mirror_pt_from_bottom(int *start_addr,part_dev_t *dev)
{
	int mpt_locate;
	int mpt_start_addr;
	int current_start_addr=0;
	char pmt_spare[4];
	mpt_start_addr = total_size+block_size;
	//mpt_start_addr=MPT_LOCATION*block_size-page_size;
	for(mpt_locate=(block_size/page_size);mpt_locate>0;mpt_locate--)
	{
		memset(pmt_spare,0xFF,PT_SIG_SIZE);
		
		current_start_addr = mpt_start_addr+mpt_locate*page_size;
		if(!dev->read(dev,current_start_addr, page_buf,page_size))
		{
			printf ("find_mirror read  failed %x %x \n",current_start_addr,mpt_locate);
		}
		memcpy(&page_buf[page_size],g_kCMD.au1OOB,16);
		memcpy(pmt_spare,&page_buf[page_size] ,PT_SIG_SIZE);
		//need enhance must be the larget sequnce number
		
		if(is_valid_mpt(page_buf)&&is_valid_mpt(&pmt_spare))
		{
		      //if no pt, pt.has space is 0;
			pi.sequencenumber = page_buf[PT_SIG_SIZE+page_size];
			printf ("find_mirror find valid pt at %x sq %x \n",current_start_addr,pi.sequencenumber);
			break;
		}
		else
		{
			continue;
		}
	}
	if(mpt_locate==0)
	{
		printf ("no valid mirror page\n");
		pi.sequencenumber =  0;
		return FALSE;
	}
	else
	{
		*start_addr = current_start_addr;
		return TRUE;
	}
}
#endif
int load_exist_part_tab(u8 *buf,part_dev_t *dev)
{
		#ifdef MTK_EMMC_SUPPORT
			int reval = ERR_NO_EXIST;
			int index = 0;
			int i,j;
			int len=0;
			char *buf_p;
			int pt_start = g_user_virt_addr + 1024;
			int mpt_start = pt_start + 2048;
		
		
			int PAGE_SIZE = 512;
			pt_resident* lp_pmt;
		
		
			printf("============func=%s===scan pmt from %x=====\n", __func__,pt_start);
			/* try to find the pmt at fixed address, signature:0x50547631 */
			for(i=0;i<CFG_EMMC_PMT_SIZE/4096;i++)
			{
				buf_p = page_buf;
			  	dev->read(dev,pt_start + i*4096,(u8*)page_buf,4096);
			  	for(j=0;j<4096/PAGE_SIZE;j++){
			  	
			//	printf("search %x %x\n",buf_p,pt_start + i*4096+j*PAGE_SIZE);
					if(is_valid_pt(buf_p)){
				
						printf("find h-pt at %x \n",pt_start + i*4096+j*PAGE_SIZE);
						if(j*PAGE_SIZE > 2048){
							len = 4096- j*PAGE_SIZE;
							printf("left %d j=%d\n",len,j);
							memcpy(backup_buf,&buf_p[PT_SIG_SIZE],len-PT_SIG_SIZE);
							dev->read(dev,pt_start + (i+1)*4096,(u8*)page_buf,2048);
							if(is_valid_pt(&page_buf[2048-4-len])){
								printf("find pt at %x \n",pt_start + i*4096+j*PAGE_SIZE);
								memcpy(&backup_buf[len-PT_SIG_SIZE],page_buf,sizeof(lastest_part)-len+PT_SIG_SIZE);
								memcpy(buf,backup_buf,sizeof(lastest_part));
								reval=DM_ERR_OK;
								goto find;//return reval;
							}
							
						}else{
							if(is_valid_pt(&buf_p[2048-4])){
								printf("find pt at %x \n",pt_start + i*4096+j*PAGE_SIZE);
								memcpy(buf,&buf_p[PT_SIG_SIZE],sizeof(lastest_part));
								reval=DM_ERR_OK;
								goto find;//return reval;
							}
						}
						break;
					}
				buf_p += PAGE_SIZE;
			  }
			}
			if(i == CFG_EMMC_PMT_SIZE/4096)
			{
				for(i=0;i<CFG_EMMC_PMT_SIZE/4096;i++){
				/* try to find the backup pmt at fixed address, signature:0x4d505431 */
				buf_p = page_buf;
				dev->read(dev,mpt_start + i*4096,(u8*)page_buf,4096);
				
				for(j=0;j<4096/PAGE_SIZE;j++){

					if(is_valid_mpt(buf_p)){
				
						printf("find h-pt at %x \n",mpt_start + i*4096+j*PAGE_SIZE);
						if(j*PAGE_SIZE > 2048){
							len = 4096- j*PAGE_SIZE;
							printf("left %d j=%d\n",len,j);
							memcpy(backup_buf,&buf_p[PT_SIG_SIZE],len-PT_SIG_SIZE);
							dev->read(dev,mpt_start + (i+1)*4096,(u8*)page_buf,2048);
							if(is_valid_mpt(&page_buf[2048-4-len])){
								printf("find mpt at %x \n",pt_start + i*4096+j*PAGE_SIZE);
								memcpy(&backup_buf[len-PT_SIG_SIZE],page_buf,sizeof(lastest_part)-len+PT_SIG_SIZE);
								memcpy(buf,backup_buf,sizeof(lastest_part));
								reval=DM_ERR_OK;
								goto find;//return reval;
							}
							
						}else{
							if(is_valid_mpt(&buf_p[2048-4])){
								printf("find mpt at %x \n",mpt_start + i*4096+j*PAGE_SIZE);
								memcpy(buf,&buf_p[PT_SIG_SIZE],sizeof(lastest_part));
								reval=DM_ERR_OK;
								goto find;//return reval;
							}
						}
						break;
					}
					buf_p += PAGE_SIZE;
				}
				}
				
				}
			if(i == CFG_EMMC_PMT_SIZE/4096)
				printf("find no pt or mpt\n");
			return reval;
		find:

		if(g_emmc_size<0x100000000){ //32bit
			printf("32bit parse PMT\n");
			memcpy(&lastest_part32,buf,PART_MAX_COUNT*sizeof(pt_resident32));
			
			memset(&lastest_part,0,PART_MAX_COUNT*sizeof(pt_resident));
			for(i=0;i<PART_MAX_COUNT;i++)
			{
				if(lastest_part32[i].size!=0){
					memcpy(lastest_part[i].name,lastest_part32[i].name,MAX_PARTITION_NAME_LEN);
					lastest_part[i].size= lastest_part32[i].size;
					lastest_part[i].offset= lastest_part32[i].offset;
					lastest_part[i].mask_flags= lastest_part32[i].mask_flags;
				}
			}
		}else{
			printf("64bit parse PMT, size pt = %d\n",sizeof(pt_resident));
		}

	return reval;
		
		
#else
	int pt_start_addr;
	int pt_cur_addr;
	int pt_locate;
	int reval=DM_ERR_OK;
	int mirror_address;
	char pmt_spare[PT_SIG_SIZE];

	block_size= devinfo.blocksize*1024;
	page_size = devinfo.pagesize;
	
	//page_buf = malloc(page_size);	 

	pt_start_addr = total_size;
	printf("load_pt from 0x%x \n",pt_start_addr);
	//pt_start_addr=PT_LOCATION*block_size;
	for(pt_locate=0;pt_locate<(block_size/page_size);pt_locate++)
	{
		pt_cur_addr = pt_start_addr+pt_locate*page_size;
		memset(pmt_spare,0xFF,PT_SIG_SIZE);

		if(!dev->read(dev,pt_cur_addr, page_buf,page_size))
		{
			printf ("load_pt read pt failded: %x\n",pt_cur_addr);
		}
             memcpy(&page_buf[page_size],g_kCMD.au1OOB,16);
		#if 0
		{
			int i;
			for(i=0;i<8;i++)
			{
				printf ("%x %x \n",page_buf[i],page_buf[2048+i]);
			}

		}
		#endif
		memcpy(pmt_spare,&page_buf[page_size] ,PT_SIG_SIZE); //skip bad block flag
		if(is_valid_pt(page_buf)&&is_valid_pt(pmt_spare))
		{
			pi.sequencenumber = page_buf[PT_SIG_SIZE+page_size];
			printf("load_pt find valid pt at %x sq %x \n",pt_start_addr,pi.sequencenumber);
			break;
		}
		else
		{
			continue;
		}
	}
	//for test 
	//pt_locate==(block_size/page_size);
	if(pt_locate==(block_size/page_size))
	{
		//first download or download is not compelte after erase or can not download last time
		printf ("load_pt find pt failed \n");
		pi.pt_has_space = 0; //or before download pt power lost
		
		if(!find_mirror_pt_from_bottom(&mirror_address,dev))
		{
			printf ("First time download \n");
			reval=ERR_NO_EXIST;
			return reval;
		}
		else
		{
			//used the last valid mirror pt, at lease one is valid.
			dev->read(dev,mirror_address, page_buf,page_size);
		}
	}
	memcpy(buf,&page_buf[PT_SIG_SIZE],sizeof(lastest_part));

	return reval;
#endif
}
void part_init_pmt(unsigned long totalblks,part_dev_t *dev)
{
#ifdef MTK_EMMC_SUPPORT
	part_t *part = &mt6575_parts[0];
	unsigned long lastblk;
	int retval=0;
	int i=0;
	printf ("mt6575_part_init_pmt \n");
	if (!totalblks) return;

	/* updater the number of blks of first part. */
	if (totalblks <= part->blknum)
	part->blknum = totalblks;

	totalblks -= part->blknum;
	lastblk = part->startblk + part->blknum;

	while(totalblks) 
	{
		part++;
		if (!part->name)
		break;

		if (part->flags & PART_FLAG_LEFT || totalblks <= part->blknum)
		part->blknum = totalblks;

		part->startblk = lastblk;
		totalblks -= part->blknum;
		lastblk = part->startblk + part->blknum;
	}
	
	memset(&pi,0xFF,sizeof(pi));
	memset(&lastest_part,0,PART_MAX_COUNT*sizeof(pt_resident));
	retval=load_exist_part_tab((u8 *)&lastest_part,dev);
	if (retval==ERR_NO_EXIST) //first run preloader before dowload
	{
		//and valid mirror last download or first download 
		printf ("no pt \n");
		get_part_tab_from_complier(); //get from complier
	}
	else
	{
		printf ("Find pt \n");
		for(i=0;i<PART_MAX_COUNT;i++)
		{	
			if(lastest_part[i].size == 0)
				break;
			printf ("partition %s size %016llx %016llx \n",lastest_part[i].name,lastest_part[i].offset,lastest_part[i].size);
		}
	}
#else
part_t *part = &mt6575_parts[0];
	unsigned long lastblk;
	int retval=0;
	int i=0;
	printf ("mt6575_part_init_pmt \n");
	if (!totalblks) return;

	/* updater the number of blks of first part. */
	if (totalblks <= part->blknum)
	part->blknum = totalblks;

	totalblks -= part->blknum;
	lastblk = part->startblk + part->blknum;

	while(totalblks) 
	{
		part++;
		if (!part->name)
		break;

		if (part->flags & PART_FLAG_LEFT || totalblks <= part->blknum)
		part->blknum = totalblks;

		part->startblk = lastblk;
		totalblks -= part->blknum;
		lastblk = part->startblk + part->blknum;
	}
	
	memset(&pi,0xFF,sizeof(pi));
	memset(&lastest_part,0,PART_MAX_COUNT*sizeof(pt_resident));
	retval=load_exist_part_tab((u8 *)&lastest_part,dev);
	if (retval==ERR_NO_EXIST) //first run preloader before dowload
	{
		//and valid mirror last download or first download 
		printf ("no pt \n");
		get_part_tab_from_complier(); //get from complier
	}
	else
	{
		printf ("Find pt \n");
		for(i=0;i<PART_MAX_COUNT;i++)
		{	
			if(lastest_part[i].size == 0)
				break;
			printf ("partition %s size %lx %lx \n",lastest_part[i].name,lastest_part[i].offset,lastest_part[i].size);
		}
	}
#endif
}

#endif
