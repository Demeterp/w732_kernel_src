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
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MED IATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

 
#include "tpd.h"
#include <linux/interrupt.h>
#include <cust_eint.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/rtpm_prio.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>

#include "tpd_custom_ft5306.h"
#ifdef MT6575
#include <mach/mt6575_pm_ldo.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_boot.h>
#endif

#include <linux/proc_fs.h>/*punk*/
#include "cust_gpio_usage.h"

#include "ft5306_driver.h"


#define CFG_SUPPORT_MULTI_TP
#define TP_INFO_PROC
#define CFG_SUPPORT_DYNAMICS_SENSITIVITY

#ifdef CFG_SUPPORT_MULTI_TP

//#define TP_SUPPORT_HW_ID
#define TP_SUPPORT_FW_ID
#define TP_C8_TRULY_FW_VID          0x5a
#define TP_C8_SUNRISE_FW_VID        0x31 


#define FTS_TAG                  "[FTS_TP]"
#define FTS_FUN(f)               printk(KERN_ERR FTS_TAG"%s\n", __FUNCTION__)
#define FTS_FUN1(f)               printk(KERN_ERR FTS_TAG"%s -%d\n", __FUNCTION__,f)
#define FTS_FUN2(a,b)               printk(KERN_ERR FTS_TAG"%s -%d  para=%d\n", __FUNCTION__,a,b)
#define FTS_ERR(fmt, args...)    printk(KERN_ERR FTS_TAG"%s %d : "fmt, __FUNCTION__, __LINE__, ##args)
#define FTS_LOG(fmt, args...)    printk(KERN_ERR FTS_TAG fmt, ##args)
#define FTS_DBG(fmt, args...)    printk(KERN_ERR fmt, ##args)                 


enum {
    TP_C8_TRULY_ID =1,
    TP_C8_SUNRISE_ID
};

#endif
 
#ifdef TP_INFO_PROC
#define TP_INFO_PROC_FILE    "driver/tp_info"
static struct proc_dir_entry *tp_info_proc = NULL;
#endif

extern struct tpd_device *tpd;
 
struct i2c_client *i2c_client = NULL;
struct task_struct *thread = NULL;
struct hrtimer ft5306_timer;
 
static DECLARE_WAIT_QUEUE_HEAD(waiter);
 
 
static void tpd_eint_interrupt_handler(void);
 
 
/*punk*/
#ifdef MT6575
extern kal_bool upmu_is_chr_det(void);
#endif

 extern void mt65xx_eint_unmask(unsigned int line);
 extern void mt65xx_eint_mask(unsigned int line);
 extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
 extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
 extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
									  kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
									  kal_bool auto_umask);

 
static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tpd_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
static int __devexit tpd_remove(struct i2c_client *client);
static int touch_event_handler(void *unused);
 

static int tpd_flag = 0;
static int tpd_halt=0;
static int point_num = 0;
static int p_point_num = 0;

//#define TPD_CLOSE_POWER_IN_SLEEP
//#define ESD_CHECK
#define TPD_RESET_ISSUE_WORKAROUND
#define TPD_MAX_RESET_COUNT 3

#define TPD_OK 0
//register define


#ifdef TPD_HAVE_BUTTON 
static int tpd_keys_local[TPD_KEY_COUNT] = TPD_KEYS;
static int tpd_keys_dim_local[TPD_KEY_COUNT][4] = TPD_KEYS_DIM;
#endif
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
static int tpd_wb_start_local[TPD_WARP_CNT] = TPD_WARP_START;
static int tpd_wb_end_local[TPD_WARP_CNT]   = TPD_WARP_END;
#endif
#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
static int tpd_calmat_local[8]     = TPD_CALIBRATION_MATRIX;
static int tpd_def_calmat_local[8] = TPD_CALIBRATION_MATRIX;
#endif

#define ACER_C8

#if defined(ACER_C8)
char truly_info[] = "TP vendor : Truly , IC : FT5306 ";
char sr_info[] = "TP vendor : Sunrise , IC : FT5306 ";
char unknown_info[] = "Unknow TP";
#else

#endif

//////////////////////////////////////////////////////////////////////////////////
//#define VELOCITY_CUSTOM_FT5206
#ifdef VELOCITY_CUSTOM_FT5206
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

// for magnify velocity********************************************
#define TOUCH_IOC_MAGIC 'A'

#define TPD_GET_VELOCITY_CUSTOM_X _IO(TOUCH_IOC_MAGIC,0)
#define TPD_GET_VELOCITY_CUSTOM_Y _IO(TOUCH_IOC_MAGIC,1)

int g_v_magnify_x =TPD_VELOCITY_CUSTOM_X;
int g_v_magnify_y =TPD_VELOCITY_CUSTOM_Y;
static int tpd_misc_open(struct inode *inode, struct file *file)
{
/*
	file->private_data = adxl345_i2c_client;

	if(file->private_data == NULL)
	{
		printk("tpd: null pointer!!\n");
		return -EINVAL;
	}
	*/
	return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int tpd_misc_release(struct inode *inode, struct file *file)
{
	//file->private_data = NULL;
	return 0;
}
/*----------------------------------------------------------------------------*/
//static int adxl345_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
//       unsigned long arg)
static long tpd_unlocked_ioctl(struct file *file, unsigned int cmd,
       unsigned long arg)
{
	//struct i2c_client *client = (struct i2c_client*)file->private_data;
	//struct adxl345_i2c_data *obj = (struct adxl345_i2c_data*)i2c_get_clientdata(client);	
	char strbuf[256];
	void __user *data;
	
	long err = 0;
	
	if(_IOC_DIR(cmd) & _IOC_READ)
	{
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	}
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
	{
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	}

	if(err)
	{
		printk("tpd: access error: %08X, (%2d, %2d)\n", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
		return -EFAULT;
	}

	switch(cmd)
	{
		case TPD_GET_VELOCITY_CUSTOM_X:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}			
			
			if(copy_to_user(data, &g_v_magnify_x, sizeof(g_v_magnify_x)))
			{
				err = -EFAULT;
				break;
			}				 
			break;

	   case TPD_GET_VELOCITY_CUSTOM_Y:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}			
			
			if(copy_to_user(data, &g_v_magnify_y, sizeof(g_v_magnify_y)))
			{
				err = -EFAULT;
				break;
			}				 
			break;


		default:
			printk("tpd: unknown IOCTL: 0x%08x\n", cmd);
			err = -ENOIOCTLCMD;
			break;
			
	}

	return err;
}


static struct file_operations tpd_fops = {
//	.owner = THIS_MODULE,
	.open = tpd_misc_open,
	.release = tpd_misc_release,
	.unlocked_ioctl = tpd_unlocked_ioctl,
};
/*----------------------------------------------------------------------------*/
static struct miscdevice tpd_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "touch",
	.fops = &tpd_fops,
};

//**********************************************
#endif
//////////////////////////////////////////////////////////////////////////////////


struct touch_info {
    int y[CFG_MAX_TOUCH_POINTS];
    int x[CFG_MAX_TOUCH_POINTS];
    int p[CFG_MAX_TOUCH_POINTS];
    int id[CFG_MAX_TOUCH_POINTS];	
    int count;
};
 
 static const struct i2c_device_id ft5306_tpd_id[] = {{"ft5306",0},{}};
 //unsigned short force[] = {0,0x70,I2C_CLIENT_END,I2C_CLIENT_END}; 
 //static const unsigned short * const forces[] = { force, NULL };
 //static struct i2c_client_address_data addr_data = { .forces = forces, };
 static struct i2c_board_info __initdata ft5306_i2c_tpd={ I2C_BOARD_INFO("ft5306", (0x70>>1))};
 
 
 static struct i2c_driver tpd_i2c_driver = {
  .driver = {
	 .name = "ft5306",//.name = TPD_DEVICE,
//	 .owner = THIS_MODULE,
  },
  .probe = tpd_probe,
  .remove = __devexit_p(tpd_remove),
  .id_table = ft5306_tpd_id,
  .detect = tpd_detect,
//  .address_data = &addr_data,
 };


  ///////////////////////////////////////////////////////////////////////////////////
static int ft5x0x_write_reg(u8 addr, u8 para)
{
    int ret = -1;

    ret = i2c_smbus_write_i2c_block_data(i2c_client, addr, 1, &para);;
    if (ret < 0) {
        pr_err("write reg failed! %#x ret: %d", addr, ret);
        return -1;
    }
    
    return 0;
}

static int ft5x0x_read_reg(u8 addr, u8 *pdata)
{
    int ret;
    
    ret = i2c_smbus_read_i2c_block_data(i2c_client, addr, 1, pdata);
    if (ret < 0)
        pr_err("msg %s i2c read error: %d\n", __func__, ret);

    return ret;
}


static unsigned char ft5x0x_read_fw_ver(void)
{
    unsigned char ver;
    i2c_smbus_read_i2c_block_data(i2c_client, 0xa6, 1, &ver);
    return(ver);
}


#if 1  //upgrade related
typedef enum
{
    ERR_OK,
    ERR_MODE,
    ERR_READID,
    ERR_ERASE,
    ERR_STATUS,
    ERR_ECC,
    ERR_DL_ERASE_FAIL,
    ERR_DL_PROGRAM_FAIL,
    ERR_DL_VERIFY_FAIL
}E_UPGRADE_ERR_TYPE;

typedef unsigned char         FTS_BYTE;     //8 bit
typedef unsigned short        FTS_WORD;    //16 bit
typedef unsigned int          FTS_DWRD;    //16 bit
typedef unsigned char         FTS_BOOL;    //8 bit

typedef struct _FTS_CTP_PROJECT_SETTING_T
{
    unsigned char uc_i2C_addr;             //I2C slave address (8 bit address)
    unsigned char uc_io_voltage;           //IO Voltage 0---3.3v;    1----1.8v
    unsigned char uc_panel_factory_id;     //TP panel factory ID
}FTS_CTP_PROJECT_SETTING_T;

#define FTS_NULL                0x0
#define FTS_TRUE                0x01
#define FTS_FALSE              0x0
#define I2C_CTPM_ADDRESS       0x70




static int ft5x0x_i2c_rxdata(char *rxdata, int length)
{
    int ret;

    struct i2c_msg msgs[] = {
        {
            .addr    = i2c_client->addr,
            .flags    = 0,
            .len    = 1,
            .buf    = rxdata,
        },
        {
            .addr    = i2c_client->addr,
            .flags    = I2C_M_RD,
            .len    = length,
            .buf    = rxdata,
        },
    };

    //msleep(1);
    ret = i2c_transfer(i2c_client->adapter, msgs, 2);
    if (ret < 0)
        pr_err("msg %s i2c read error: %d\n", __func__, ret);
    
    return ret;
}

static int ft5x0x_i2c_txdata(char *txdata, int length)
{
    int ret;

    struct i2c_msg msg[] = {
        {
            .addr    = i2c_client->addr,
            .flags    = 0,
            .len    = length,
            .buf    = txdata,
        },
    };

       //msleep(1);
    ret = i2c_transfer(i2c_client->adapter, msg, 1);
    if (ret < 0)
        pr_err("%s i2c write error: %d\n", __func__, ret);

    return ret;
}


FTS_BOOL i2c_read_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    int ret;
    
    ret=i2c_master_recv(i2c_client, pbt_buf, dw_lenth);

    if(ret<=0)
    {
        printk("[FTS]i2c_read_interface error\n");
        return FTS_FALSE;
    }
  
    return FTS_TRUE;
}


FTS_BOOL i2c_write_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    int ret;
    ret=i2c_master_send(i2c_client, pbt_buf, dw_lenth);
    if(ret<=0)
    {
        printk("[FTS]i2c_write_interface error line = %d, ret = %d\n", __LINE__, ret);
        return FTS_FALSE;
    }

    return FTS_TRUE;
}

/*
[function]: 
    send a command to ctpm.
[parameters]:
    btcmd[in]        :command code;
    btPara1[in]    :parameter 1;    
    btPara2[in]    :parameter 2;    
    btPara3[in]    :parameter 3;    
    num[in]        :the valid input parameter numbers, if only command code needed and no parameters followed,then the num is 1;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL cmd_write(FTS_BYTE btcmd,FTS_BYTE btPara1,FTS_BYTE btPara2,FTS_BYTE btPara3,FTS_BYTE num)
{
    FTS_BYTE write_cmd[4] = {0};

    write_cmd[0] = btcmd;
    write_cmd[1] = btPara1;
    write_cmd[2] = btPara2;
    write_cmd[3] = btPara3;
    return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, num);
}

/*
[function]: 
    write data to ctpm , the destination address is 0.
[parameters]:
    pbt_buf[in]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL byte_write(FTS_BYTE* pbt_buf, FTS_DWRD dw_len)
{
    
    return i2c_write_interface(I2C_CTPM_ADDRESS, pbt_buf, dw_len);
}

/*
[function]: 
    read out data from ctpm,the destination address is 0.
[parameters]:
    pbt_buf[out]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL byte_read(FTS_BYTE* pbt_buf, FTS_BYTE bt_len)
{
    return i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len);
}


/*
[function]: 
    burn the FW to ctpm.
[parameters]:(ref. SPEC)
    pbt_buf[in]    :point to Head+FW ;
    dw_lenth[in]:the length of the FW + 6(the Head length);    
    bt_ecc[in]    :the ECC of the FW
[return]:
    ERR_OK        :no error;
    ERR_MODE    :fail to switch to UPDATE mode;
    ERR_READID    :read id fail;
    ERR_ERASE    :erase chip fail;
    ERR_STATUS    :status error;
    ERR_ECC        :ecc error.
*/


#define    FTS_PACKET_LENGTH        128

static unsigned char CTPM_FW[]=
{
#include "ft_app.i"
};



#ifdef CFG_SUPPORT_DYNAMICS_SENSITIVITY

void fts_chk_chr(void)
{
	kal_bool ischarging;	
    unsigned char val=0x5a;
    static unsigned char reg80h,reg80h_chr;
    unsigned char temp;
    FTS_FUN();

	if(i2c_client==NULL){
		FTS_LOG("i2c_client==NULL");
		return;
	}

    i2c_smbus_read_i2c_block_data(i2c_client, 0x86, 1, &val);
	ischarging = upmu_is_chr_det();
	FTS_LOG("reg86=%x, ischarging=%d,reg80h=%x, reg80h_charge=%x",val,ischarging,reg80h,reg80h_chr);

	if(val == 2){
		i2c_smbus_read_i2c_block_data(i2c_client, 0x80, 1, &reg80h_chr);
		if(!ischarging ){
			temp = 1;
			i2c_smbus_write_i2c_block_data(i2c_client, 0x86, 1, &temp);  
			i2c_smbus_read_i2c_block_data(i2c_client, 0x86, 1, &temp);
			FTS_LOG("updated reg86=%x, ischarging=%d -1",temp,ischarging);
		}
	}
	else if(val == 1){
		i2c_smbus_read_i2c_block_data(i2c_client, 0x80, 1, &reg80h);
		if(ischarging ){
			temp = 2;
			i2c_smbus_write_i2c_block_data(i2c_client, 0x86, 1, &temp);  
			i2c_smbus_read_i2c_block_data(i2c_client, 0x86, 1, &temp);
			FTS_LOG("updated reg86=%x, ischarging=%d -2",temp,ischarging);
		}
	}
	else{ 
			FTS_LOG("invalid value, reg86=%x, ",val);
	}

}


EXPORT_SYMBOL(fts_chk_chr);
#endif



#ifdef CFG_SUPPORT_MULTI_TP


#if defined(ACER_C8)

static unsigned char CTPM_FW_C8_TRULY[]=
{
#include "ft_app_c8_truly.i"
};
static unsigned char CTPM_FW_C8_SUNRISE[]=
{
#include "ft_app_c8_sunrise.i"
};

#endif




#endif


E_UPGRADE_ERR_TYPE  fts_ctpm_fw_upgrade(FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    FTS_BYTE reg_val[2] = {0};
    FTS_DWRD i = 0;

    FTS_DWRD  packet_number;
    FTS_DWRD  j;
    FTS_DWRD  temp;
    FTS_DWRD  lenght;
    FTS_BYTE  packet_buf[FTS_PACKET_LENGTH + 6];
    FTS_BYTE  auc_i2c_write_buf[10];
    FTS_BYTE bt_ecc;
    int      i_ret;

    /*********Step 1:Reset  CTPM *****/
    /*write 0xaa to register 0xfc*/
    ft5x0x_write_reg(0xfc,0xaa);
    msleep(50);
     /*write 0x55 to register 0xfc*/
    ft5x0x_write_reg(0xfc,0x55);
    printk("[FTS] Step 1: Reset CTPM test\n");
   
    msleep(30);   


    /*********Step 2:Enter upgrade mode *****/
    auc_i2c_write_buf[0] = 0x55;
    auc_i2c_write_buf[1] = 0xaa;
    do
    {
        i ++;
        i_ret = ft5x0x_i2c_txdata(auc_i2c_write_buf, 2);
        msleep(5);
    }while(i_ret <= 0 && i < 5 );

    /*********Step 3:check READ-ID***********************/        
    cmd_write(0x90,0x00,0x00,0x00,4);
    byte_read(reg_val,2);
    if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
    {
        printk("[FTS] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
    }
    else
    {
        return ERR_READID;
        //i_is_new_protocol = 1;
    }

    cmd_write(0xcd,0x0,0x00,0x00,1);
    byte_read(reg_val,1);
    printk("[FTS] bootloader version = 0x%x\n", reg_val[0]);

     /*********Step 4:erase app and panel paramenter area ********************/
    cmd_write(0x61,0x00,0x00,0x00,1);  //erase app area
    msleep(1500); 
    cmd_write(0x63,0x00,0x00,0x00,1);  //erase panel parameter area
    msleep(100);
    printk("[FTS] Step 4: erase. \n");

    /*********Step 5:write firmware(FW) to ctpm flash*********/
    bt_ecc = 0;
    printk("[FTS] Step 5: start upgrade. \n");
    dw_lenth = dw_lenth - 8;
    packet_number = (dw_lenth) / FTS_PACKET_LENGTH;
    packet_buf[0] = 0xbf;
    packet_buf[1] = 0x00;
    for (j=0;j<packet_number;j++)
    {
        temp = j * FTS_PACKET_LENGTH;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;
        lenght = FTS_PACKET_LENGTH;
        packet_buf[4] = (FTS_BYTE)(lenght>>8);
        packet_buf[5] = (FTS_BYTE)lenght;

        for (i=0;i<FTS_PACKET_LENGTH;i++)
        {
            packet_buf[6+i] = pbt_buf[j*FTS_PACKET_LENGTH + i]; 
            bt_ecc ^= packet_buf[6+i];
        }
        
        byte_write(&packet_buf[0],FTS_PACKET_LENGTH + 6);
        msleep(FTS_PACKET_LENGTH/6 + 1);
        if ((j * FTS_PACKET_LENGTH % 1024) == 0)
        {
              printk("[FTS] upgrade the 0x%x th byte.\n", ((unsigned int)j) * FTS_PACKET_LENGTH);
        }
    }

    if ((dw_lenth) % FTS_PACKET_LENGTH > 0)
    {
        temp = packet_number * FTS_PACKET_LENGTH;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;

        temp = (dw_lenth) % FTS_PACKET_LENGTH;
        packet_buf[4] = (FTS_BYTE)(temp>>8);
        packet_buf[5] = (FTS_BYTE)temp;

        for (i=0;i<temp;i++)
        {
            packet_buf[6+i] = pbt_buf[ packet_number*FTS_PACKET_LENGTH + i]; 
            bt_ecc ^= packet_buf[6+i];
        }

        byte_write(&packet_buf[0],temp+6);    
        msleep(20);
    }

    //send the last six byte
    for (i = 0; i<6; i++)
    {
        temp = 0x6ffa + i;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;
        temp =1;
        packet_buf[4] = (FTS_BYTE)(temp>>8);
        packet_buf[5] = (FTS_BYTE)temp;
        packet_buf[6] = pbt_buf[ dw_lenth + i]; 
        bt_ecc ^= packet_buf[6];

        byte_write(&packet_buf[0],7);  
        msleep(20);
    }


    /*********Step 6: read out checksum***********************/
    /*send the opration head*/
    cmd_write(0xcc,0x00,0x00,0x00,1);
    byte_read(reg_val,1);
    printk("[FTS] Step 6:  ecc read 0x%x, new firmware 0x%x. \n", reg_val[0], bt_ecc);
    if(reg_val[0] != bt_ecc)
    {
        return ERR_ECC;
    }

    /*********Step 7: reset the new FW***********************/
    cmd_write(0x07,0x00,0x00,0x00,1);

    msleep(300);  //make sure CTP startup normally
    
    return ERR_OK;
}

int fts_ctpm_auto_clb(void)
{
    unsigned char uc_temp;
    unsigned char i ;

    printk("[FTS] start auto CLB.\n");
    msleep(200);
    ft5x0x_write_reg(0, 0x40);  
    msleep(100);   //make sure already enter factory mode
    ft5x0x_write_reg(2, 0x4);  //write command to start calibration
    msleep(300);
    for(i=0;i<100;i++)
    {
        ft5x0x_read_reg(0,&uc_temp);
        if ( ((uc_temp&0x70)>>4) == 0x0)  //return to normal mode, calibration finish
        {
            break;
        }
        msleep(200);
        printk("[FTS] waiting calibration %d\n",i);
        
    }
    printk("[FTS] calibration OK.\n");
    
    msleep(300);
    ft5x0x_write_reg(0, 0x40);  //goto factory mode
    msleep(100);   //make sure already enter factory mode
    ft5x0x_write_reg(2, 0x5);  //store CLB result
    msleep(300);
    ft5x0x_write_reg(0, 0x0); //return to normal mode 
    msleep(300);
    printk("[FTS] store CLB result OK.\n");
    return 0;
}

int fts_ctpm_fw_upgrade_with_i_file(void)
{
   FTS_BYTE*     pbt_buf = FTS_NULL;
   int i_ret;

   //punk temparaly
   return 0;
    //=========FW upgrade========================*/
   pbt_buf = CTPM_FW;
   /*call the upgrade function*/
   i_ret =  fts_ctpm_fw_upgrade(pbt_buf,sizeof(CTPM_FW));
   if (i_ret != 0)
   {
       printk("[FTS] upgrade failed i_ret = %d.\n", i_ret);
       //error handling ...
       //TBD
   }
   else
   {
       printk("[FTS] upgrade successfully.\n");
       fts_ctpm_auto_clb();  //start auto CLB
   }

   return i_ret;
}

unsigned char fts_ctpm_get_i_file_ver(void)
{
    unsigned int ui_sz;
    ui_sz = sizeof(CTPM_FW);
    if (ui_sz > 2)
    {
        return CTPM_FW[ui_sz - 2];
    }
    else
    {
        //TBD, error handling?
        return 0xff; //default value
    }
}

#define    FTS_SETTING_BUF_LEN        128

//update project setting
//only update these settings for COB project, or for some special case
int fts_ctpm_update_project_setting(void)
{
    unsigned char uc_i2c_addr;             //I2C slave address (8 bit address)
    unsigned char uc_io_voltage;           //IO Voltage 0---3.3v;    1----1.8v
    unsigned char uc_panel_factory_id;     //TP panel factory ID

    unsigned char buf[FTS_SETTING_BUF_LEN];
    FTS_BYTE reg_val[2] = {0};
    FTS_BYTE  auc_i2c_write_buf[10];
    FTS_BYTE  packet_buf[FTS_SETTING_BUF_LEN + 6];
    FTS_DWRD i = 0;
    int      i_ret;

    uc_i2c_addr = 0x0;
    uc_io_voltage = 0x0;
    uc_panel_factory_id = 0x5a;

    /*********Step 1:Reset  CTPM *****/
    /*write 0xaa to register 0xfc*/
    ft5x0x_write_reg(0xfc,0xaa);
    msleep(50);
     /*write 0x55 to register 0xfc*/
    ft5x0x_write_reg(0xfc,0x55);
    printk("[FTS] Step 1: Reset CTPM test\n");
   
    msleep(30);   

    /*********Step 2:Enter upgrade mode *****/
    auc_i2c_write_buf[0] = 0x55;
    auc_i2c_write_buf[1] = 0xaa;
    do
    {
        i ++;
        i_ret = ft5x0x_i2c_txdata(auc_i2c_write_buf, 2);
        msleep(5);
    }while(i_ret <= 0 && i < 5 );

    /*********Step 3:check READ-ID***********************/        
    cmd_write(0x90,0x00,0x00,0x00,4);
    byte_read(reg_val,2);
    if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
    {
        printk("[FTS] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
    }
    else
    {
        return ERR_READID;
    }

    cmd_write(0xcd,0x0,0x00,0x00,1);
    byte_read(reg_val,1);
    printk("bootloader version = 0x%x\n", reg_val[0]);


    /* --------- read current project setting  ---------- */
    //set read start address
    buf[0] = 0x3;
    buf[1] = 0x0;
    buf[2] = 0x78;
    buf[3] = 0x0;
    byte_write(buf, 4);
    byte_read(buf, FTS_SETTING_BUF_LEN);
    
    printk("[FTS] old setting: uc_i2c_addr = 0x%x, uc_io_voltage = %d, uc_panel_factory_id = 0x%x\n",
        buf[0],  buf[2], buf[4]);
    for (i = 0; i < FTS_SETTING_BUF_LEN; i++)
    {
        if (i % 16 == 0)     printk("\n");
        printk("0x%x, ", buf[i]);
        
    }
    printk("\n");

     /*--------- Step 4:erase project setting --------------*/
    cmd_write(0x62,0x00,0x00,0x00,1);
    msleep(100);
   
    /*----------  Set new settings ---------------*/
    buf[0] = uc_i2c_addr;
    buf[1] = ~uc_i2c_addr;
    buf[2] = uc_io_voltage;
    buf[3] = ~uc_io_voltage;
    buf[4] = uc_panel_factory_id;
    buf[5] = ~uc_panel_factory_id;
    packet_buf[0] = 0xbf;
    packet_buf[1] = 0x00;
    packet_buf[2] = 0x78;
    packet_buf[3] = 0x0;
    packet_buf[4] = 0;
    packet_buf[5] = FTS_SETTING_BUF_LEN;
    for (i = 0; i < FTS_SETTING_BUF_LEN; i++)
    {
        packet_buf[6 + i] = buf[i];
        if (i % 16 == 0)     printk("\n");
        printk("0x%x, ", buf[i]);
    }
    printk("\n");
    byte_write(&packet_buf[0],FTS_SETTING_BUF_LEN + 6);
    msleep(100);

    /********* reset the new FW***********************/
    cmd_write(0x07,0x00,0x00,0x00,1);

    msleep(200);

    return 0;
    
}



int fts_ctpm_read_project_setting(void)
{
    unsigned char uc_i2c_addr;             //I2C slave address (8 bit address)
    unsigned char uc_io_voltage;           //IO Voltage 0---3.3v;    1----1.8v
    unsigned char uc_panel_factory_id;     //TP panel factory ID

    unsigned char buf[FTS_SETTING_BUF_LEN];
    FTS_BYTE reg_val[2] = {0};
    FTS_BYTE  auc_i2c_write_buf[10];
    FTS_DWRD i = 0;
    int      i_ret;

    uc_i2c_addr = 0x70;
    uc_io_voltage = 0x0;
    uc_panel_factory_id = 0x5a;

    /*********Step 1:Reset  CTPM *****/
    /*write 0xaa to register 0xfc*/
    ft5x0x_write_reg(0xfc,0xaa);
    msleep(50);
     /*write 0x55 to register 0xfc*/
    ft5x0x_write_reg(0xfc,0x55);
    printk("[FTS] Step 1: Reset CTPM test\n");
   
    msleep(30);   

    /*********Step 2:Enter upgrade mode *****/
    auc_i2c_write_buf[0] = 0x55;
    auc_i2c_write_buf[1] = 0xaa;
    do
    {
        i ++;
        i_ret = ft5x0x_i2c_txdata(auc_i2c_write_buf, 2);
        msleep(5);
    }while(i_ret <= 0 && i < 5 );

    /*********Step 3:check READ-ID***********************/        
    cmd_write(0x90,0x00,0x00,0x00,4);
    byte_read(reg_val,2);
    if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
    {
        printk("[FTS] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
    }
    else
    {
        return ERR_READID;
    }

    cmd_write(0xcd,0x0,0x00,0x00,1);
    byte_read(reg_val,1);
    printk("bootloader version = 0x%x\n", reg_val[0]);


    /* --------- read current project setting  ---------- */
    //set read start address
    buf[0] = 0x3;
    buf[1] = 0x0;
    buf[2] = 0x78;
    buf[3] = 0x0;
    byte_write(buf, 4);
    byte_read(buf, FTS_SETTING_BUF_LEN);
    
    printk("[FTS] old setting: uc_i2c_addr = 0x%x, uc_io_voltage = %d, uc_panel_factory_id = 0x%x\n",
        buf[0],  buf[2], buf[4]);
    for (i = 0; i < FTS_SETTING_BUF_LEN; i++)
    {
        if (i % 16 == 0)     printk("\n");
        printk("0x%x, ", buf[i]);
        
    }
    printk("\n");

    /********* reset the new FW***********************/
    cmd_write(0x07,0x00,0x00,0x00,1);

    msleep(200);

    return 0;
    
}



#if CFG_SUPPORT_AUTO_UPG

int fts_ctpm_auto_upg(void)
{
    unsigned char uc_host_fm_ver;
    unsigned char uc_tp_fm_ver;
    int           i_ret;

    uc_tp_fm_ver = ft5x0x_read_fw_ver();
    uc_host_fm_ver = fts_ctpm_get_i_file_ver();
    if ( uc_tp_fm_ver == 0xa6  ||   //the firmware in touch panel maybe corrupted
         uc_tp_fm_ver < uc_host_fm_ver //the firmware in host flash is new, need upgrade
        )
    {
        msleep(100);
        printk("[FTS] uc_tp_fm_ver = 0x%x, uc_host_fm_ver = 0x%x\n",
            uc_tp_fm_ver, uc_host_fm_ver);
        i_ret = fts_ctpm_fw_upgrade_with_i_file();    
        if (i_ret == 0)
        {
            msleep(300);
            uc_host_fm_ver = ft5x0x_read_fw_ver();
            printk("[FTS] upgrade to new version 0x%x\n", uc_host_fm_ver);
        }
        else
        {
            printk("[FTS] upgrade failed ret=%d.\n", i_ret);
        }
    }

    return 0;
}

#ifdef CFG_SUPPORT_MULTI_TP



static unsigned char ft5x0x_read_fw_vid(void)
{
    unsigned char vid;
    FTS_FUN();
    i2c_smbus_read_i2c_block_data(i2c_client, 0xa8, 1, &vid);
    return(vid);
}

int ft5x0x_read_hw_id(void)
{
    FTS_FUN();
    return -1;
}

int fts_get_id(void)
{
    unsigned char fw_vid;
    unsigned char fw_id;
    unsigned char hw_id;
    unsigned char id;

    printk("[FTS] fts_get_id\n" );
    FTS_FUN();
	fw_vid = ft5x0x_read_fw_vid();
    FTS_LOG("fw_vid = %x",fw_vid);
    switch (fw_vid)
    {
        case TP_C8_TRULY_FW_VID:
            fw_id = TP_C8_TRULY_ID; 
            break;
        case TP_C8_SUNRISE_FW_VID:
            fw_id = TP_C8_SUNRISE_ID; 
            break;
        default:
            fw_id = -1; 
            break;
    }

    FTS_LOG("fw_id = %d",fw_id);

#ifdef TP_SUPPORT_HW_ID
    hw_id = ft5x0x_read_hw_id();
#else
    hw_id = -1;
#endif

#if defined(TP_SUPPORT_HW_ID) && defined(TP_SUPPORT_FW_ID)
    if(hw_id == fw_id)
        id = hw_id;
    else
        id = -1;
#elif defined(TP_SUPPORT_HW_ID)
    id = hw_id;
#elif defined(TP_SUPPORT_FW_ID)
    id = fw_id;
#else
    id = -1;
#endif

    FTS_LOG("id = %d",id);
    return id;

}

unsigned char fts_ctpm_get_i_file_ver_ext(int dev_id)
{
    unsigned int ui_sz;

#ifdef CFG_SUPPORT_MULTI_TP
    FTS_FUN1(1);
    FTS_LOG("dev_id = %d",dev_id);
    switch (dev_id)
    {
        case TP_C8_TRULY_ID:
            ui_sz = sizeof(CTPM_FW_C8_TRULY);
    FTS_FUN1(2);
            break;
        case TP_C8_SUNRISE_ID:
            ui_sz = sizeof(CTPM_FW_C8_SUNRISE);
    FTS_FUN1(3);
            break;
        default:
            ui_sz = 0;
    FTS_FUN1(4);
            break;
    }
    
    FTS_FUN1(5);
    if (ui_sz > 2)
    {

        switch (dev_id)
        {
            case TP_C8_TRULY_ID:
    FTS_FUN1(6);
                return CTPM_FW_C8_TRULY[ui_sz - 2];
                break;
            case TP_C8_SUNRISE_ID:
    FTS_FUN1(7);
                return CTPM_FW_C8_SUNRISE[ui_sz - 2];
                break;
            default:
                return 0xff; //default value
                break;
        }
    }
    else
    {
    FTS_FUN1(8);
        //TBD, error handling?
        return 0xff; //default value
    }
#else
    ui_sz = sizeof(CTPM_FW);
    FTS_FUN1(100);
    if (ui_sz > 2)
    {
        return CTPM_FW[ui_sz - 2];
    }
    else
    {
        //TBD, error handling?
        return 0xff; //default value
    }
#endif
}

char* fts_get_module_name(void)
{
    int dev_id;
    dev_id = fts_get_id(); 
    switch (dev_id) {
        case TP_C8_TRULY_ID:
    FTS_FUN1(2);
			return truly_info;
            break;
        case TP_C8_SUNRISE_ID:
    FTS_FUN1(3);
			return sr_info;
            break;
        default:
			return unknown_info;
            break;
    }
}


int fts_ctpm_fw_upgrade_with_i_file_ext(int dev_id)
{
   FTS_BYTE*     pbt_buf = FTS_NULL;
   int i_ret=-8;

    
    FTS_FUN1(1);
#ifdef CFG_SUPPORT_MULTI_TP
    switch (dev_id) {
        case TP_C8_TRULY_ID:
            pbt_buf = CTPM_FW_C8_TRULY;
    FTS_FUN1(2);
            /*call the upgrade function*/
            i_ret =  fts_ctpm_fw_upgrade(pbt_buf,sizeof(CTPM_FW_C8_TRULY));
            break;
        case TP_C8_SUNRISE_ID:
            pbt_buf = CTPM_FW_C8_SUNRISE;
    FTS_FUN1(3);
            /*call the upgrade function*/
            i_ret =  fts_ctpm_fw_upgrade(pbt_buf,sizeof(CTPM_FW_C8_SUNRISE));
            break;
        default:
            return -10;
            break;
    }

#else
    xx

    FTS_FUN1(100);
    //=========FW upgrade========================*/
   pbt_buf = CTPM_FW;
   /*call the upgrade function*/
   i_ret =  fts_ctpm_fw_upgrade(pbt_buf,sizeof(CTPM_FW));
#endif

   
   if (i_ret != 0)
   {
       printk("[FTS] upgrade failed i_ret = %d.\n", i_ret);
       //error handling ...
       //TBD
   }
   else
   {
       printk("[FTS] upgrade successfully.\n");
       fts_ctpm_auto_clb();  //start auto CLB
   }

   return i_ret;
}

int fts_ctpm_auto_upg_ext(void)
{

    unsigned char uc_host_fm_ver;
    unsigned char uc_tp_fm_ver;
    int           i_ret;
    int dev_id;

        printk("fts_ctpm_auto_upg_ext  -1\n");
#ifdef CFG_SUPPORT_MULTI_TP
    FTS_FUN1(1);
    dev_id = fts_get_id(); 
    if(dev_id<0)
        return -1;
#else
    FTS_FUN1(100);
#endif


    uc_tp_fm_ver = ft5x0x_read_fw_ver();
    uc_host_fm_ver = fts_ctpm_get_i_file_ver_ext(dev_id);
    if ( uc_tp_fm_ver == 0xa6  ||   //the firmware in touch panel maybe corrupted
         uc_tp_fm_ver < uc_host_fm_ver //the firmware in host flash is new, need upgrade
        )
    {
        msleep(100);
        printk("[FTS] uc_tp_fm_ver = 0x%x, uc_host_fm_ver = 0x%x, dev_id=%d\n",
            uc_tp_fm_ver, uc_host_fm_ver,dev_id);
    //    i_ret = fts_ctpm_fw_upgrade_with_i_file();    
        i_ret = fts_ctpm_fw_upgrade_with_i_file_ext(dev_id);    
        if (i_ret == 0)
        {
            msleep(300);
            uc_host_fm_ver = ft5x0x_read_fw_ver();
            printk("[FTS] upgrade to new version 0x%x\n", uc_host_fm_ver);
        }
        else
        {
            printk("[FTS] upgrade failed ret=%d.\n", i_ret);
        }
    }

    return 0;
}
#endif

#endif

#endif
///////////////////////////////////////////////////////////////////



static  void tpd_down(int x, int y, int p, int id) {
	// input_report_abs(tpd->dev, ABS_PRESSURE, p);
	 input_report_key(tpd->dev, BTN_TOUCH, 1);
	 input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 1);
	 input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
	 input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
	 input_report_abs(tpd->dev,ABS_MT_TRACKING_ID,id);
	 //printk("D[%4d %4d %4d] ", x, y, p);
	 input_mt_sync(tpd->dev);
	if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
	{   
	tpd_button(x, y, 1);  
	}
	 if(y > TPD_RES_Y) //virtual key debounce to avoid android ANR issue
	 {
	      msleep(50);
	      printk("ft53006 virtual key \n");
	 }
	 TPD_EM_PRINT(x, y, x, y, p-1, 1);
	// printk("[LONG][tp_down]X=%d,y=%d,id=%d\r\n",x,y,id);
 }
 
static  void tpd_up(int x, int y,int *count, int id) {
	 //if(*count>0) {
		 //input_report_abs(tpd->dev, ABS_PRESSURE, 0);
		 input_report_key(tpd->dev, BTN_TOUCH, 0);
		 //input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0);
		 //input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
		 //input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
		 //input_report_abs(tpd->dev,ABS_MT_TRACKING_ID,id);
		 //printk("U[%4d %4d %4d] ", x, y, 0);
		 input_mt_sync(tpd->dev);
		 TPD_EM_PRINT(x, y, x, y, 0, 0);
	//	 (*count)--;
     if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
     {   
        tpd_button(x, y, 0); 
     }   		 
	 	// printk("[LONG][tp_up]X=%d,y=%d,id=%d\r\n",x,y,id);
 }

 static int tpd_touchinfo(struct touch_info *cinfo, struct touch_info *pinfo)
 {

	int i = 0;
	
	char data[35] = {0};

    u16 high_byte,low_byte;
	u8 report_rate =0;

	//p_point_num = point_num;

	i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 8, &(data[0]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x08, 8, &(data[8]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x10, 8, &(data[16]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x18, 8, &(data[24]));
	//i2c_smbus_read_i2c_block_data(i2c_client, 0xa6, 1, &(data[24]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x88, 1, &report_rate);
	//TPD_DEBUG("FW version=%x]\n",data[24]);
	
	//TPD_DEBUG("received raw data from touch panel as following:\n");
	//TPD_DEBUG("[data[0]=%x,data[1]= %x ,data[2]=%x ,data[3]=%x ,data[4]=%x ,data[5]=%x]\n",data[0],data[1],data[2],data[3],data[4],data[5]);
	//TPD_DEBUG("[data[9]=%x,data[10]= %x ,data[11]=%x ,data[12]=%x]\n",data[9],data[10],data[11],data[12]);
	//TPD_DEBUG("[data[15]=%x,data[16]= %x ,data[17]=%x ,data[18]=%x]\n",data[15],data[16],data[17],data[18]);


    //    
	 //we have  to re update report rate
    // TPD_DMESG("report rate =%x\n",report_rate);
	 if (report_rate < 8)
	 {
	   report_rate = 0x8;
	   if((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
	   {
		   TPD_DMESG("I2C read report rate error, line: %d\n", __LINE__);
	   }
	 }
	 
	
	/* Device Mode[2:0] == 0 :Normal operating Mode*/
	if(data[0] & 0x70 != 0) return false; 

	/*get the number of the touch points*/
	point_num= data[2] & 0x0f;
	
	TPD_DEBUG("point_num =%d\n",point_num);
	if(point_num>CFG_MAX_TOUCH_POINTS)
		point_num=CFG_MAX_TOUCH_POINTS;
	
//	if(point_num == 0) return false;

	   //TPD_DEBUG("Procss raw data...\n");

		
		for(i = 0; i <point_num; i++)
		{
			cinfo->p[i] = (data[3+6*i] >> 6); //event flag 
			cinfo->id[i] = (data[5+6*i] >> 4); //id

	       /*get the X coordinate, 2 bytes*/
			high_byte = data[3+6*i];
			high_byte <<= 8;
			high_byte &= 0x0f00;
			low_byte = data[3+6*i + 1];
			cinfo->x[i] = high_byte |low_byte;

				//cinfo->x[i] =  cinfo->x[i] * 480 >> 11; //calibra
		
			/*get the Y coordinate, 2 bytes*/
			
			high_byte = data[3+6*i+2];
			high_byte <<= 8;
			high_byte &= 0x0f00;
			low_byte = data[3+6*i+3];
			cinfo->y[i] = high_byte |low_byte;

			  //cinfo->y[i]=  cinfo->y[i] * 800 >> 11;
		
			//cinfo->count++;
			TPD_DEBUG(" cinfo[%d]:x= %d,y= %d,p= %d\n", i, cinfo->x[i], cinfo->y[i], cinfo->p[i]);
//printk("[LONG][mtk-tpd] cinfo[%d]:x= %d,y= %d,evnet=%d,id= %d\n", i, cinfo->x[i], cinfo->y[i], cinfo->p[i],cinfo->id[i]);
			
		}
	 return true;

 };

 static int touch_event_handler(void *unused)
 {
  
    struct touch_info cinfo, pinfo;
	int idx;
#ifdef TPD_HAVE_BUTTON
	static u8 button_state=0;
#endif

    static u8 id_mask = 0;
    u8 cur_mask;
    char esd_data=0;

    static int x_history[CFG_MAX_TOUCH_POINTS];
    static int y_history[CFG_MAX_TOUCH_POINTS];
    static int event_history[CFG_MAX_TOUCH_POINTS];

	 struct sched_param param = { .sched_priority = RTPM_PRIO_TPD };
	 sched_setscheduler(current, SCHED_RR, &param);
 
	 do
	 {
	  	//mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM); 
		 set_current_state(TASK_INTERRUPTIBLE); 

		while ( tpd_halt )
		{
		    tpd_flag = 0;
		    msleep(20);
		}

		wait_event_interruptible(waiter,tpd_flag!=0);			 
		tpd_flag = 0;
		set_current_state(TASK_RUNNING);
		 
	#if defined(ESD_CHECK)	 
		i2c_smbus_read_i2c_block_data(i2c_client, 0x87, 1, &esd_data);
		//printk("[LONG]TP ESD DATA=%x\r\n",esd_data);
		if (esd_data!=0X0a)
		{
		//printk("[LONG]ESD ERROR:TP ESD DATA=%x\r\n",esd_data);
			hwPowerDown(TPD_POWER_SOURCE,"TP");
			msleep(30);
			hwPowerOn(TPD_POWER_SOURCE,VOL_2800,"TP"); 
			msleep(300);
			hrtimer_start(&ft5306_timer, ktime_set(0, 500000000), HRTIMER_MODE_REL); //check again after 500ms
			return;
		}
	#endif	

			if (tpd_touchinfo(&cinfo, &pinfo)) {
				
				printk("ft5306 point_num = %d, -1\n",point_num);

				if(point_num >0) 
				{
					for ( idx = 0 ; idx < point_num ; idx++ )
					{         
							// LCD view area
							if (cinfo.x[idx] < TPD_X_RES && cinfo.y[idx] < TPD_Y_RES)
							{
								tpd_down(TPD_WARP_X(cinfo.x[idx]), TPD_WARP_Y(cinfo.y[idx]), idx+1,cinfo.id[idx]);
							}

					}       

				}	
				else
				{
					tpd_up(cinfo.x[0], cinfo.y[0], 0,0);		 
				}    

				if ( tpd != NULL && tpd->dev != NULL )
					input_sync(tpd->dev);
		}
#if defined(ESD_CHECK)
	hrtimer_start(&ft5306_timer, ktime_set(0, 500000000), HRTIMER_MODE_REL); //check again after 500ms
#endif
 	}while(!kthread_should_stop());
 
	 return 0;
 }
 
 static int tpd_detect (struct i2c_client *client, int kind, struct i2c_board_info *info) 
 {
	 strcpy(info->type, TPD_DEVICE);	
	  return 0;
 }
 
#if defined(ESD_CHECK)
static enum hrtimer_restart ft5306_timer_func(struct hrtimer *timer)
{
	tpd_flag = 1;
	wake_up_interruptible(&waiter);
	return HRTIMER_NORESTART;
}
#endif

 static void tpd_eint_interrupt_handler(void)
 {
	 TPD_DEBUG("TPD interrupt has been triggered\n");
	 tpd_flag = 1;
	 wake_up_interruptible(&waiter);
	 
 }



#ifdef TP_INFO_PROC

static int tp_info_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char *ptr = page;
	unsigned char fw_ver;
	char *tp_info;

    FTS_FUN1(1);
	tp_info = fts_get_module_name();
    fw_ver = ft5x0x_read_fw_ver();
    ptr += sprintf( ptr, " %s\n",tp_info);
    ptr += sprintf( ptr, "TP firmware version: %x\n",fw_ver);
	TPD_DEBUG("tp_info:%s fw_ver:%x\n",tp_info,fw_ver);
	*eof = 1;
	return ( ptr - page );
}

static int tp_info_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	return 0;
}

#endif

 static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id)
 {	 
	int retval = TPD_OK;
	char data;
	u8 report_rate=0;
	int err=0;
	int reset_count = 0;

reset_proc:   
	i2c_client = client;

   
#ifdef MT6575
    //power on, need confirm with SA
   // hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_2800, "TP");
    hwPowerOn(TPD_POWER_SOURCE, VOL_2800, "TP");      //MT65XX_POWER_LDO_VGP=2.8v
#endif	
	#if 0//def TPD_CLOSE_POWER_IN_SLEEP	 
	hwPowerDown(TPD_POWER_SOURCE,"TP");
	hwPowerOn(TPD_POWER_SOURCE,VOL_2800,"TP");
	msleep(100);
	#else
	#ifdef MT6573
	mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
	msleep(100);
	#endif
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	msleep(10);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
	msleep(10);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	msleep(10);
	#endif

	mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);
 
	  mt65xx_eint_set_sens(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_SENSITIVE);
	  mt65xx_eint_set_hw_debounce(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_CN);
	  mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler, 1); 
	  mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
 
	msleep(300);//100-->200  //make sure CTP already finish startup process

	if((i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 1, &data))< 0)
	{
		TPD_DMESG("I2C transfer error, line: %d\n", __LINE__);
#ifdef TPD_RESET_ISSUE_WORKAROUND
        if ( reset_count < TPD_MAX_RESET_COUNT )
        {
            reset_count++;
            goto reset_proc;
        }
#endif
		   return -1; 
	   }

	//set report rate 80Hz
	report_rate = 0x8; 
	if((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
	{
	    if((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
	    {
		   TPD_DMESG("I2C read report rate error, line: %d\n", __LINE__);
	    }
		   
	}

	tpd_load_status = 1;

	#ifdef VELOCITY_CUSTOM_FT5206
	if((err = misc_register(&tpd_misc_device)))
	{
		printk("mtk_tpd: tpd_misc_device register failed\n");
		
	}
	#endif

    //get some register information

	i2c_smbus_read_i2c_block_data(i2c_client, FT5X0X_REG_FIRMID, 1, &data);
	TPD_DMESG("[FTS] Firmware version = 0x%x\n", data);
	i2c_smbus_read_i2c_block_data(i2c_client, FT5X0X_REG_PERIODACTIVE, 1, &data);
	TPD_DMESG("[FTS] report rate is %dHz.\n", data * 10);
	i2c_smbus_read_i2c_block_data(i2c_client, FT5X0X_REG_THGROUP, 1, &data);
	TPD_DMESG("[FTS] touch threshold is %d.\n", data * 4);

   printk("[FTS] tpd_probe.\n");
#if CFG_SUPPORT_AUTO_UPG
#ifdef CFG_SUPPORT_MULTI_TP
        fts_ctpm_auto_upg_ext();
#else
        xx
        fts_ctpm_auto_upg();
#endif
#endif    

#ifdef TP_INFO_PROC
			// Create proc file system
			tp_info_proc = create_proc_entry( TP_INFO_PROC_FILE , 0666, NULL);

			if ( tp_info_proc == NULL )
			{
				TPD_DEBUG("create_proc_entry %s failed\n", TP_INFO_PROC_FILE );
			}
			else 
			{
				tp_info_proc ->read_proc = tp_info_read_proc;
				tp_info_proc ->write_proc = tp_info_write_proc;
			}
#endif

#if CFG_SUPPORT_UPDATE_PROJECT_SETTING
    	fts_ctpm_update_project_setting();
#endif

#if CFG_DBG_RAW_DATA_READ
    	fts_raw_data_test();
#endif

#if defined(ESD_CHECK)
	hrtimer_init(&ft5306_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	ft5306_timer.function = ft5306_timer_func;
#endif

	thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
	 if (IS_ERR(thread))
		 { 
		  retval = PTR_ERR(thread);
		  TPD_DMESG(TPD_DEVICE " failed to create kernel thread: %d\n", retval);
		}

	TPD_DMESG("ft5306 Touch Panel Device Probe %s\n", (retval < TPD_OK) ? "FAIL" : "PASS");
   return 0;
   
 }

 static int __devexit tpd_remove(struct i2c_client *client)
 {
   
	 TPD_DEBUG("TPD removed\n");
 
   return 0;
 }
 
 
 static int tpd_local_init(void)
 {
  TPD_DMESG("Focaltech FT5206 I2C Touchscreen Driver (Built %s @ %s)\n", __DATE__, __TIME__);
 
    if(i2c_add_driver(&tpd_i2c_driver)!=0)
    {
	TPD_DMESG("ft5306 unable to add i2c driver.\n");
	return -1;
    }
    if(tpd_load_status == 0) 
    {
    	TPD_DMESG("ft5306 add error touch panel driver.\n");
    	i2c_del_driver(&tpd_i2c_driver);
    	return -1;
    }
	
#ifdef TPD_HAVE_BUTTON     
    tpd_button_setting(TPD_KEY_COUNT, tpd_keys_local, tpd_keys_dim_local);// initialize tpd button data
#endif   
  
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))    
    TPD_DO_WARP = 1;
    memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT*4);
    memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT*4);
#endif 

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
    memcpy(tpd_calmat, tpd_def_calmat_local, 8*4);
    memcpy(tpd_def_calmat, tpd_def_calmat_local, 8*4);	
#endif  
		
    tpd_type_cap = 1;
    return 0; 
 }

 static int tpd_resume(struct i2c_client *client)
 {
  int retval = TPD_OK;
 
   TPD_DEBUG("TPD wake up\n");
#ifdef TPD_CLOSE_POWER_IN_SLEEP	
	hwPowerOn(TPD_POWER_SOURCE,VOL_2800,"TP"); 
	msleep(300);
#else
#ifdef MT6573
	mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
#endif	
	//msleep(100);

	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);  
	msleep(10);  
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
#if defined(ACER_C8)
	msleep(30);  //300-->10,  lcm resume cost 310ms at least
#else
	msleep(300);  //350-->300
#endif
#endif
#if defined(ACER_C8)
	fts_chk_chr();
#endif

	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);  
#if defined(ESD_CHECK)
	hrtimer_start(&ft5306_timer, ktime_set(0, 500000000), HRTIMER_MODE_REL); //check again after 500ms
#endif	
     	tpd_halt = 0;
	 return retval;
 }
 
 static int tpd_suspend(struct i2c_client *client, pm_message_t message)
 {
	 int retval = TPD_OK;
	 static char data = 0x3;
 
	 TPD_DEBUG("TPD enter sleep\n");
 	tpd_halt = 1;   
#if defined(ESD_CHECK)
	hrtimer_cancel(&ft5306_timer);
#endif
	 mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
#ifdef TPD_CLOSE_POWER_IN_SLEEP	
	hwPowerDown(TPD_POWER_SOURCE,"TP");
#else
i2c_smbus_write_i2c_block_data(i2c_client, 0xA5, 1, &data);  //TP enter sleep mode
#ifdef MT6573
mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ZERO);
#endif

#endif
	 return retval;
 } 


 static struct tpd_driver_t tpd_device_driver = {
		 .tpd_device_name = "FT5306",
		 .tpd_local_init = tpd_local_init,
		 .suspend = tpd_suspend,
		 .resume = tpd_resume,
#ifdef TPD_HAVE_BUTTON
		 .tpd_have_button = 1,
#else
		 .tpd_have_button = 0,
#endif		
 };
 /* called when loaded into kernel */
 static int __init tpd_driver_init(void) {
	 TPD_DMESG("MediaTek FT5306 touch panel driver init\n");
	i2c_register_board_info(0, &ft5306_i2c_tpd, 1);
		 if(tpd_driver_add(&tpd_device_driver) < 0)
			 TPD_DMESG("add FT5306 driver failed\n");
	 return 0;
 }
 
 /* should never be called */
 static void __exit tpd_driver_exit(void) {
	 TPD_DMESG("MediaTek FT5306 touch panel driver exit\n");
	 //input_unregister_device(tpd->dev);
	 tpd_driver_remove(&tpd_device_driver);
 }
 
 module_init(tpd_driver_init);
 module_exit(tpd_driver_exit);


