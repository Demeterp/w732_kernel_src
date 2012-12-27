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
 
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <mach/mt6575_gpt.h>
#include <mach/mt6575_clock_manager.h>
#include <asm/io.h>
#include "dbg.h"


static char cmd_buf[256];

/* for debug zone */
unsigned int sd_debug_zone[HOST_MAX_NUM]={
	0,
	0,
	0,
	0
};

/* mode select */
u32 dma_size[HOST_MAX_NUM]={
	512,
	512,
	512,
	512
};
msdc_mode drv_mode[HOST_MAX_NUM]={
	MODE_SIZE_DEP, /* using DMA or not depend on the size */
	MODE_SIZE_DEP,
	MODE_SIZE_DEP,
	MODE_SIZE_DEP
};
unsigned char msdc_clock_src[HOST_MAX_NUM]={
	0,
	0,
	0,
	0
};
drv_mod msdc_drv_mode[HOST_MAX_NUM];
/* for driver profile */
#define TICKS_ONE_MS  (13000)
u32 gpt_enable = 0;
u32 sdio_pro_enable = 0;   /* make sure gpt is enabled */
u32 sdio_pro_time = 0;     /* no more than 30s */
struct sdio_profile sdio_perfomance = {0};  
void msdc_dump_reg(unsigned int base)
{
	printk("[SD_Debug]Reg[00] MSDC_CFG       = 0x%.8x\n", sdr_read32(base + 0x00));
    printk("[SD_Debug]Reg[04] MSDC_IOCON     = 0x%.8x\n", sdr_read32(base + 0x04));
    printk("[SD_Debug]Reg[08] MSDC_PS        = 0x%.8x\n", sdr_read32(base + 0x08));
    printk("[SD_Debug]Reg[0C] MSDC_INT       = 0x%.8x\n", sdr_read32(base + 0x0C));
    printk("[SD_Debug]Reg[10] MSDC_INTEN     = 0x%.8x\n", sdr_read32(base + 0x10));    
    printk("[SD_Debug]Reg[14] MSDC_FIFOCS    = 0x%.8x\n", sdr_read32(base + 0x14));
    printk("[SD_Debug]Reg[18] MSDC_TXDATA    = not read\n");                    	
    printk("[SD_Debug]Reg[1C] MSDC_RXDATA    = not read\n");
    printk("[SD_Debug]Reg[30] SDC_CFG        = 0x%.8x\n", sdr_read32(base + 0x30));
    printk("[SD_Debug]Reg[34] SDC_CMD        = 0x%.8x\n", sdr_read32(base + 0x34));
    printk("[SD_Debug]Reg[38] SDC_ARG        = 0x%.8x\n", sdr_read32(base + 0x38));
    printk("[SD_Debug]Reg[3C] SDC_STS        = 0x%.8x\n", sdr_read32(base + 0x3C));
    printk("[SD_Debug]Reg[40] SDC_RESP0      = 0x%.8x\n", sdr_read32(base + 0x40));
    printk("[SD_Debug]Reg[44] SDC_RESP1      = 0x%.8x\n", sdr_read32(base + 0x44));
    printk("[SD_Debug]Reg[48] SDC_RESP2      = 0x%.8x\n", sdr_read32(base + 0x48));                                
    printk("[SD_Debug]Reg[4C] SDC_RESP3      = 0x%.8x\n", sdr_read32(base + 0x4C));
    printk("[SD_Debug]Reg[50] SDC_BLK_NUM    = 0x%.8x\n", sdr_read32(base + 0x50));
    printk("[SD_Debug]Reg[58] SDC_CSTS       = 0x%.8x\n", sdr_read32(base + 0x58));
    printk("[SD_Debug]Reg[5C] SDC_CSTS_EN    = 0x%.8x\n", sdr_read32(base + 0x5C));
    printk("[SD_Debug]Reg[60] SDC_DATCRC_STS = 0x%.8x\n", sdr_read32(base + 0x60));
    printk("[SD_Debug]Reg[70] EMMC_CFG0      = 0x%.8x\n", sdr_read32(base + 0x70));                        
    printk("[SD_Debug]Reg[74] EMMC_CFG1      = 0x%.8x\n", sdr_read32(base + 0x74));
    printk("[SD_Debug]Reg[78] EMMC_STS       = 0x%.8x\n", sdr_read32(base + 0x78));
    printk("[SD_Debug]Reg[7C] EMMC_IOCON     = 0x%.8x\n", sdr_read32(base + 0x7C));            
    printk("[SD_Debug]Reg[80] SD_ACMD_RESP   = 0x%.8x\n", sdr_read32(base + 0x80));
    printk("[SD_Debug]Reg[84] SD_ACMD19_TRG  = 0x%.8x\n", sdr_read32(base + 0x84));      
    printk("[SD_Debug]Reg[88] SD_ACMD19_STS  = 0x%.8x\n", sdr_read32(base + 0x88));
    printk("[SD_Debug]Reg[90] DMA_SA         = 0x%.8x\n", sdr_read32(base + 0x90));
    printk("[SD_Debug]Reg[94] DMA_CA         = 0x%.8x\n", sdr_read32(base + 0x94));
    printk("[SD_Debug]Reg[98] DMA_CTRL       = 0x%.8x\n", sdr_read32(base + 0x98));
    printk("[SD_Debug]Reg[9C] DMA_CFG        = 0x%.8x\n", sdr_read32(base + 0x9C));                        
    printk("[SD_Debug]Reg[A0] SW_DBG_SEL     = 0x%.8x\n", sdr_read32(base + 0xA0));
    printk("[SD_Debug]Reg[A4] SW_DBG_OUT     = 0x%.8x\n", sdr_read32(base + 0xA4));
    printk("[SD_Debug]Reg[B0] PATCH_BIT0     = 0x%.8x\n", sdr_read32(base + 0xB0));            
    printk("[SD_Debug]Reg[B4] PATCH_BIT1     = 0x%.8x\n", sdr_read32(base + 0xB4));
    printk("[SD_Debug]Reg[E0] SD_PAD_CTL0  = 0x%.8x\n", sdr_read32(base + 0xE0));        
    printk("[SD_Debug]Reg[E4] SD_PAD_CTL1  = 0x%.8x\n", sdr_read32(base + 0xE4));
    printk("[SD_Debug]Reg[E8] SD_PAD_CTL2  = 0x%.8x\n", sdr_read32(base + 0xE8));
    printk("[SD_Debug]Reg[EC] PAD_TUNE       = 0x%.8x\n", sdr_read32(base + 0xEC));
    printk("[SD_Debug]Reg[F0] DAT_RD_DLY0    = 0x%.8x\n", sdr_read32(base + 0xF0));                        
    printk("[SD_Debug]Reg[F4] DAT_RD_DLY1    = 0x%.8x\n", sdr_read32(base + 0xF4));
    printk("[SD_Debug]Reg[F8] HW_DBG_SEL     = 0x%.8x\n", sdr_read32(base + 0xF8));
    printk("[SD_Debug]Rg[100] MAIN_VER       = 0x%.8x\n", sdr_read32(base + 0x100));     
    printk("[SD_Debug]Rg[104] ECO_VER        = 0x%.8x\n", sdr_read32(base + 0x104));
}
void msdc_set_field(unsigned int address,unsigned int start_bit,unsigned int len,unsigned int value)
{
	unsigned long field;
	if(start_bit > 31 || start_bit < 0|| len > 32 || len <= 0)
		printk("[****SD_Debug****]reg filed beyoned (0~31) or length beyoned (1~32)\n");
	else{
		field = ((1 << len) -1) << start_bit;
		value &= (1 << len) -1; 
		printk("[****SD_Debug****]Original:0x%x (0x%x)\n",address,sdr_read32(address));
		sdr_set_field(address,field, value);
		printk("[****SD_Debug****]Modified:0x%x (0x%x)\n",address,sdr_read32(address));
	}
}
void msdc_get_field(unsigned int address,unsigned int start_bit,unsigned int len,unsigned int value)
{
	unsigned long field;
	if(start_bit > 31 || start_bit < 0|| len > 32 || len <= 0)
		printk("[****SD_Debug****]reg filed beyoned (0~31) or length beyoned (1~32)\n");
	else{
		field = ((1 << len) -1) << start_bit;
		sdr_get_field(address,field,value);
		printk("[****SD_Debug****]Reg:0x%x start_bit(%d)len(%d)(0x%x)\n",address,start_bit,len,value);
		}
}
void msdc_init_gpt(void)
{
    GPT_CONFIG config;	
    
    config.num  = GPT6;
    config.mode = GPT_FREE_RUN;
    config.clkSrc = GPT_CLK_SRC_SYS;
    config.clkDiv = GPT_CLK_DIV_1;   /* 13MHz GPT6 */
            
    if (GPT_Config(config) == FALSE )
        return;                       
        
    GPT_Start(GPT6);	
}

u32 msdc_time_calc(u32 old_L32, u32 old_H32, u32 new_L32, u32 new_H32)
{
    u32 ret = 0; 
    
    if (new_H32 == old_H32) {
        ret = new_L32 - old_L32;
    } else if(new_H32 == (old_H32 + 1)) {
        if (new_L32 > old_L32) {	
            printk("msdc old_L<0x%x> new_L<0x%x>\n", old_L32, new_L32);	
        }
        ret = (0xffffffff - old_L32);  	      
        ret += new_L32; 
    } else {
        printk("msdc old_H<0x%x> new_H<0x%x>\n", old_H32, new_H32);		
    }

    return ret;        	
}

void msdc_sdio_profile(struct sdio_profile* result)
{
    struct cmd_profile*  cmd;
    u32 i; 	
    
    printk("sdio === performance dump ===\n");
    printk("sdio === total execute tick<%d> time<%dms> Tx<%dB> Rx<%dB>\n", 
                    result->total_tc, result->total_tc / TICKS_ONE_MS, 
                    result->total_tx_bytes, result->total_rx_bytes);    

    /* CMD52 Dump */
    cmd = &result->cmd52_rx; 
    printk("sdio === CMD52 Rx <%d>times tick<%d> Max<%d> Min<%d> Aver<%d>\n", cmd->count, cmd->tot_tc, 
                    cmd->max_tc, cmd->min_tc, cmd->tot_tc/cmd->count);     
    cmd = &result->cmd52_tx; 
    printk("sdio === CMD52 Tx <%d>times tick<%d> Max<%d> Min<%d> Aver<%d>\n", cmd->count, cmd->tot_tc, 
                    cmd->max_tc, cmd->min_tc, cmd->tot_tc/cmd->count);   
                    
    /* CMD53 Rx bytes + block mode */
    for (i=0; i<512; i++) {
        cmd = &result->cmd53_rx_byte[i];
        if (cmd->count) {
            printk("sdio<%6d><%3dB>_Rx_<%9d><%9d><%6d><%6d>_<%9dB><%2dM>\n", cmd->count, i, cmd->tot_tc, 
                             cmd->max_tc, cmd->min_tc, cmd->tot_tc/cmd->count,
                             cmd->tot_bytes, (cmd->tot_bytes/10)*13 / (cmd->tot_tc/10));                        	
        }	
    }  
    for (i=0; i<100; i++) {
        cmd = &result->cmd53_rx_blk[i];
        if (cmd->count) {
            printk("sdio<%6d><%3d>B_Rx_<%9d><%9d><%6d><%6d>_<%9dB><%2dM>\n", cmd->count, i, cmd->tot_tc, 
                             cmd->max_tc, cmd->min_tc, cmd->tot_tc/cmd->count,
                             cmd->tot_bytes, (cmd->tot_bytes/10)*13 / (cmd->tot_tc/10));                        	
        }	
    }

    /* CMD53 Tx bytes + block mode */
    for (i=0; i<512; i++) {
        cmd = &result->cmd53_tx_byte[i];
        if (cmd->count) {
            printk("sdio<%6d><%3dB>_Tx_<%9d><%9d><%6d><%6d>_<%9dB><%2dM>\n", cmd->count, i, cmd->tot_tc, 
                             cmd->max_tc, cmd->min_tc, cmd->tot_tc/cmd->count,
                             cmd->tot_bytes, (cmd->tot_bytes/10)*13 / (cmd->tot_tc/10));                          	
        }	
    }          
    for (i=0; i<100; i++) {
        cmd = &result->cmd53_tx_blk[i];
        if (cmd->count) {
            printk("sdio<%6d><%3d>B_Tx_<%9d><%9d><%6d><%6d>_<%9dB><%2dM>\n", cmd->count, i, cmd->tot_tc, 
                             cmd->max_tc, cmd->min_tc, cmd->tot_tc/cmd->count,
                             cmd->tot_bytes, (cmd->tot_bytes/10)*13 / (cmd->tot_tc/10));                            	
        }	
    }     
    
    printk("sdio === performance dump done ===\n");      
}

//========= sdio command table ===========
void msdc_performance(u32 opcode, u32 sizes, u32 bRx, u32 ticks)
{
    struct sdio_profile* result = &sdio_perfomance; 
    struct cmd_profile*  cmd; 
    u32 block;     	

    if (sdio_pro_enable == 0) {
        return;
    }

    if (opcode == 52) {
        cmd = bRx ?  &result->cmd52_rx : &result->cmd52_tx;   	
    } else if (opcode == 53) {
        if (sizes < 512) {
            cmd = bRx ?  &result->cmd53_rx_byte[sizes] : &result->cmd53_tx_byte[sizes];    	
        } else {
            block = sizes / 512; 
            if (block >= 99) {
               printk("cmd53 error blocks\n"); 
               while(1);	
            }
            cmd = bRx ?  &result->cmd53_rx_blk[block] : &result->cmd53_tx_blk[block];       	
        }   	
    } else {
        return; 	
    }
        
    /* update the members */
    if (ticks > cmd->max_tc){
        cmd->max_tc = ticks;	
    }
    if (cmd->min_tc == 0 || ticks < cmd->min_tc) {
        cmd->min_tc = ticks; 	  
    }
    cmd->tot_tc += ticks;
    cmd->tot_bytes += sizes; 
    cmd->count ++; 
    
    if (bRx) {
        result->total_rx_bytes += sizes;    	
    } else {
        result->total_tx_bytes += sizes; 	
    }
    result->total_tc += ticks; 
    
    /* dump when total_tc > 30s */
    if (result->total_tc >= sdio_pro_time * TICKS_ONE_MS * 1000) {
        msdc_sdio_profile(result);       
        memset(result, 0 , sizeof(struct sdio_profile));                                             
    }
}

//========== driver proc interface ===========
static int msdc_debug_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char *p = page;
    int len = 0;

    p += sprintf(p, "\n=========================================\n");

    p += sprintf(p, "Index<0> + Id + Zone\n");   
    p += sprintf(p, "-> PWR<9> WRN<8> | FIO<7> OPS<6> FUN<5> CFG<4> | INT<3> RSP<2> CMD<1> DMA<0>\n");        
    p += sprintf(p, "-> echo 0 3 0x3ff >msdc_bebug -> host[3] debug zone set to 0x3ff\n");              
    p += sprintf(p, "-> MSDC[0] Zone: 0x%.8x\n", sd_debug_zone[0]);
    p += sprintf(p, "-> MSDC[1] Zone: 0x%.8x\n", sd_debug_zone[1]);
    p += sprintf(p, "-> MSDC[2] Zone: 0x%.8x\n", sd_debug_zone[2]);
    p += sprintf(p, "-> MSDC[3] Zone: 0x%.8x\n", sd_debug_zone[3]);

    p += sprintf(p, "Index<1> + ID:4|Mode:4 + DMA_SIZE\n");  
    p += sprintf(p, "-> 0)PIO 1)DMA 2)SIZE\n");      
    p += sprintf(p, "-> echo 1 22 0x200 >msdc_bebug -> host[2] size mode, dma when >= 512\n");        
    p += sprintf(p, "-> MSDC[0] mode<%d> size<%d>\n", drv_mode[0], dma_size[0]);
    p += sprintf(p, "-> MSDC[1] mode<%d> size<%d>\n", drv_mode[1], dma_size[1]);    
    p += sprintf(p, "-> MSDC[2] mode<%d> size<%d>\n", drv_mode[2], dma_size[2]);
    p += sprintf(p, "-> MSDC[3] mode<%d> size<%d>\n", drv_mode[3], dma_size[3]);

    p += sprintf(p, "Index<3> + SDIO_PROFILE + TIME\n"); 
    p += sprintf(p, "-> echo 3 1 0x1E >msdc_bebug -> enable sdio_profile, 30s\n"); 
    p += sprintf(p, "-> SDIO_PROFILE<%d> TIME<%ds>\n", sdio_pro_enable, sdio_pro_time); 
    p += sprintf(p, "-> Clokc SRC selection Host[0]<%d>\n", msdc_clock_src[0]); 
	p += sprintf(p, "-> Clokc SRC selection Host[1]<%d>\n", msdc_clock_src[1]); 
	p += sprintf(p, "-> Clokc SRC selection Host[2]<%d>\n", msdc_clock_src[2]); 
	p += sprintf(p, "-> Clokc SRC selection Host[3]<%d>\n", msdc_clock_src[3]); 
    p += sprintf(p, "-> Driving mode Host[0] clk_drv<%d> cmd_drv<%d> dat_drv<%d>\n", msdc_drv_mode[0].clk_drv,msdc_drv_mode[0].cmd_drv,msdc_drv_mode[0].dat_drv); 
	p += sprintf(p, "-> Driving mode Host[1] clk_drv<%d> cmd_drv<%d> dat_drv<%d>\n", msdc_drv_mode[1].clk_drv,msdc_drv_mode[1].cmd_drv,msdc_drv_mode[1].dat_drv); 
	p += sprintf(p, "-> Driving mode Host[2] clk_drv<%d> cmd_drv<%d> dat_drv<%d>\n", msdc_drv_mode[2].clk_drv,msdc_drv_mode[2].cmd_drv,msdc_drv_mode[2].dat_drv); 
	p += sprintf(p, "-> Driving mode Host[3] clk_drv<%d> cmd_drv<%d> dat_drv<%d>\n", msdc_drv_mode[3].clk_drv,msdc_drv_mode[3].cmd_drv,msdc_drv_mode[3].dat_drv); 

    p += sprintf(p, "=========================================\n\n");
    
    *start = page + off;

    len = p - page;
    if (len > off)
        len -= off;
    else
        len = 0;

    return len < count ? len : count;
} 

static int msdc_debug_proc_write(struct file *file, const char *buf, unsigned long count, void *data)
{
	int ret;
	
	int cmd, p1, p2, p3, p4, p5, p6;   
	int id, zone;
	int mode, size;  
	unsigned int base;
  	unsigned int offset = 0;
	unsigned int reg_value;
	if (count == 0)return -1;
	if(count > 255)count = 255;

	ret = copy_from_user(cmd_buf, buf, count);
	if (ret < 0)return -1;
	
	cmd_buf[count] = '\0';
	printk("[****SD_Debug****]msdc Write %s\n", cmd_buf);

	sscanf(cmd_buf, "%x %x %x %x %x %x %x", &cmd, &p1, &p2, &p3, &p4, &p5, &p6);
	
	if(cmd == SD_TOOL_ZONE) {
		id = p1; zone = p2; //zone &= 0x3ff;		
		printk("[****SD_Debug****]msdc host_id<%d> zone<0x%.8x>\n", id, zone);
		if(id >=0 && id<=HOST_MAX_NUM-1){
			sd_debug_zone[id] = zone;
		}
		else if(id == HOST_MAX_NUM){
			sd_debug_zone[0] = sd_debug_zone[1] = zone;
			sd_debug_zone[2] = sd_debug_zone[3] = zone;
		}
		else{
			printk("[****SD_Debug****]msdc host_id error when set debug zone\n");
		}
	} else if (cmd == SD_TOOL_DMA_SIZE) {
		id = p1;  mode = p2; size = p3; 
		if(id >=0 && id<=HOST_MAX_NUM-1){
			drv_mode[id] = mode;
			dma_size[id] = p3; 
		}
		else if(id == HOST_MAX_NUM){
			drv_mode[0] = drv_mode[1] = mode;
			drv_mode[2] = drv_mode[3] = mode;
			dma_size[0] = dma_size[1] = p3; 
			dma_size[2] = dma_size[3] = p3;
		}
		else{
			printk("[****SD_Debug****]msdc host_id error when select mode\n");
		}	
	} else if (cmd == SD_TOOL_SDIO_PROFILE) {
		if (p1 == 1) { /* enable profile */
			if (gpt_enable == 0) {
				msdc_init_gpt();
				gpt_enable = 1;
			} 
			sdio_pro_enable = 1;
			if (p2 == 0) p2 = 1; if (p2 >= 30) p2 = 30; 				
			sdio_pro_time = p2 ; 
		}	else if (p1 == 0) {
			/* todo */
			sdio_pro_enable = 0;
		}			
	}else if (cmd == SD_TOOL_CLK_SRC_SELECT){
		id = p2;
		if(id>=0 && id < HOST_MAX_NUM && p3 >= 0 && p3< CLK_SRC_MAX_NUM && p1 == 0)
			msdc_clock_src[id] = p3;
		else if(p1 == 1){
				printk("[****SD_Debug****][CLK_SRC Reg]:0x%x (0x%x)\n",MSDC_CLKSRC_REG,sdr_read32(MSDC_CLKSRC_REG));
			}
		else
			printk("[****SD_Debug****]msdc host_id error/clock source index error/illegal args when select clock source\n");
	}else if (cmd == SD_TOOL_REG_ACCESS){
		id = p2;
		offset = (unsigned int)p3;
		
		if(id >= HOST_MAX_NUM || id < 0)
			printk("[****SD_Debug****]msdc host_id error when modify msdc reg\n");
		else{
			if(id == 0)
				base = MSDC_0_BASE;
			if(id == 1)
				base = MSDC_1_BASE;
			if(id == 2)
				base = MSDC_2_BASE;
			if(id == 3)
				base = MSDC_3_BASE;				
			
			if((offset == 0x18 || offset == 0x1C) && p1 != 4){
				printk("[****SD_Debug****]Err: Accessing TXDATA and RXDATA is forbidden\n");
				return count;
				}
			enable_clock(PERI_MSDC0_PDN + id, "SD"); 
			if(p1 == 0){
				reg_value = p4;
				printk("[****SD_Debug****][MSDC Reg]Original:0x%x+0x%x (0x%x)\n",base,offset,sdr_read32(base+offset));
				sdr_write32(base+offset,reg_value );
				printk("[****SD_Debug****][MSDC Reg]Modified:0x%x+0x%x (0x%x)\n",base,offset,sdr_read32(base+offset));
				}
			else if(p1 == 1)
				printk("[****SD_Debug****][MSDC Reg]Reg:0x%x+0x%x (0x%x)\n",base,offset,sdr_read32(base+offset));
			else if(p1 == 2)
				msdc_set_field(base+offset,p4,p5,p6);
			else if(p1 == 3)
				msdc_get_field(base+offset,p4,p5,p6);
			else if(p1 == 4)
				msdc_dump_reg(base);
			disable_clock(PERI_MSDC0_PDN + id, "SD"); 
		}
			
	} 
	else if(cmd == SD_TOOL_SET_DRIVING){
		id = p1;
		if(id >= HOST_MAX_NUM || id < 0)
			printk("[****SD_Debug****]msdc host_id error when modify msdc driving\n");
		else if((unsigned char)p2 > 7 || (unsigned char)p3 > 7 ||(unsigned char)p4 > 7)
			printk("[****SD_Debug****]Some drving value was not right(correct:0~7)\n");
		else{
				msdc_drv_mode[id].clk_drv = (unsigned char)p2;
				msdc_drv_mode[id].cmd_drv = (unsigned char)p3;
				msdc_drv_mode[id].dat_drv = (unsigned char)p4;
			}
		}
	else if(cmd == SD_TOOL_DESENSE){
		if(p1 == 0){
			reg_value = p2;
			printk("[****SD_Debug****][De-Sense Reg]Original:0x%x(0x%x)\n",MSDC_DESENSE_REG,sdr_read32(MSDC_DESENSE_REG));
			sdr_write32(MSDC_DESENSE_REG,reg_value);
			printk("[****SD_Debug****][De-Sense Reg]Modified:0x%x(0x%x)\n",MSDC_DESENSE_REG,sdr_read32(MSDC_DESENSE_REG));
		}
		else if(p1 == 1)
			printk("[****SD_Debug****][De-Sense Reg]Reg:0x%x(0x%x)\n",MSDC_DESENSE_REG,sdr_read32(MSDC_DESENSE_REG));
		else if(p1 == 2)
			msdc_set_field(MSDC_DESENSE_REG,p2,p3,p4);
		else if(p1 == 3)
			msdc_get_field(MSDC_DESENSE_REG,p2,p3,p4);
		}
	
	return count;
}

int msdc_debug_proc_init(void) 
{   	
    struct proc_dir_entry *de = create_proc_entry("msdc_debug", 0667, 0);
    de->read_proc  = msdc_debug_proc_read;
    de->write_proc = msdc_debug_proc_write;
    memset(msdc_drv_mode,0,sizeof(msdc_drv_mode));
    return 0 ;
}
EXPORT_SYMBOL_GPL(msdc_debug_proc_init);
