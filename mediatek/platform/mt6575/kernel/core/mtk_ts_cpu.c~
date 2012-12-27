#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/dmi.h>
#include <linux/acpi.h>
#include <linux/thermal.h>
#include <linux/platform_device.h>
#include <linux/aee.h>
#include <linux/xlog.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>

#include <mach/system.h>

#include "mach/mt6575_typedefs.h"
#include "mach/mt6575_thermal.h"

#define DVFS_F1 (1001000)   // KHz
#define DVFS_F2 ( 500500)   // KHz
#define DVFS_F3 ( 250250)   // KHz
#define DVFS_F4 ( 166833)   // KHz

extern void cpufreq_thermal_protect(int limited_freq);

static unsigned int interval = 0; /* seconds, 0 : no auto polling */
static unsigned int trip_0_temp = 120000;
static unsigned int trip_1_temp = 110000;
static unsigned int trip_2_temp = 100000;
static unsigned int trip_3_temp = 90000;
static unsigned int cl_dev_1000_state = 0;
static unsigned int cl_dev_500_state = 0;
static unsigned int cl_dev_250_state = 0;
static unsigned int cl_dev_166_state = 0;
static unsigned int cl_dev_sysrst_state = 0;
static struct thermal_zone_device *thz_dev;
static struct thermal_cooling_device *cl_dev_1000;
static struct thermal_cooling_device *cl_dev_500;
static struct thermal_cooling_device *cl_dev_250;
static struct thermal_cooling_device *cl_dev_166;
static struct thermal_cooling_device *cl_dev_sysrst;
static struct platform_device *mtktscpu_dev;
static int mtktscpu_debug_log = 1;
static int kernelmode = 0;

#define MTKTSCPU_TEMP_CRIT 120000 /* 120.000 degree Celsius */
#define THERMAL_CHANNEL 0x5

#define mtktscpu_dprintk(fmt, args...)   \
do {                                    \
    if (mtktscpu_debug_log) {                \
        xlog_printk(ANDROID_LOG_INFO, "Power/CPU_Thermal", fmt, ##args); \
    }                                   \
} while(0)

extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);
extern int IMM_IsAdcInitReady(void);

static kal_int32 g_adc_ge = 0;
static kal_int32 g_adc_oe = 0;
static kal_int32 g_o_vts = 0;
static kal_int32 g_o_vbg = 0;
static kal_int32 g_degc_cali = 0;
static kal_int32 g_adc_cali_en = 0;
static kal_int32 g_o_vts_abb = 0;
static kal_int32 g_o_slope = 0;
static kal_int32 g_o_slope_sign = 0;
static kal_int32 g_id = 0;

static kal_int32 g_ge = 0;
static kal_int32 g_oe = 0;
static kal_int32 y_vts_plus = 0;
static kal_int32 g_gain = 0;
static kal_int32 g_x_roomt = 0;
static kal_int32 y_vts_abb_plus = 0;
static kal_int32 g_x_roomt_abb = 0;

static int y_vbg_plus_repeat_times = 10;
static int y_curr_repeat_times = 10;

static bool thermal_enable = false;


static void thermal_cal_prepare(void)
{
    kal_uint32 temp = 0;
	
    temp = DRV_Reg32(0xF1019048);
    g_adc_ge = (temp & 0x000000FF);
    mtktscpu_dprintk("temp = 0x%x, g_adc_ge = 0x%x\n", temp, g_adc_ge);
	
    temp = DRV_Reg32(0xF1019044);
    g_adc_oe = (temp & 0x000000FF);
    mtktscpu_dprintk("temp = 0x%x, g_adc_oe = 0x%x\n", temp, g_adc_oe);
	
    temp = DRV_Reg32(0xF1019040);
    //g_o_vts_abb   = ((temp & 0xFC000000) >> 26);
    g_o_vts       = ((temp & 0x03FE0000) >> 17);
    g_o_vbg       = ((temp & 0x0001FF00) >> 8);
    //g_degc_cali   = ((temp & 0x000000FE) >> 1);
    g_degc_cali   = ((temp & 0x0000007E) >> 1);
    g_adc_cali_en = ((temp & 0x00000001) >> 0);

    g_o_slope     = ((temp & 0xFC000000) >> 26);
    g_o_slope_sign= ((temp & 0x00000080) >> 7);    

    temp = DRV_Reg32(0xF1019100);
    g_id = ((temp & 0x80000000) >> 31);

    if(g_id==0)
    {
        g_o_slope = 0;
    }
    
    if(g_adc_cali_en == 1)
    {
        //thermal_enable = true;        
    }
    else
    {
        g_adc_ge = 128;
        g_adc_oe = 128;
        g_o_vts = 292;
        g_o_vbg = 167;
        g_degc_cali = 40;
        g_o_slope = 0;
        g_o_slope_sign = 0;
    }
    //mtktscpu_dprintk("[thermal_cal_prepare] thermal_enable = %d\n", thermal_enable);
	
    mtktscpu_dprintk("temp = 0x%x, g_o_vts = 0x%x, g_o_vbg = 0x%x, g_degc_cali = 0x%x, g_adc_cali_en = 0x%x, g_o_vts_abb = 0x%x, g_o_slope = 0x%x, g_o_slope_sign = 0x%x, g_id = 0x%x\n", 
        temp, g_o_vts, g_o_vbg, g_degc_cali, g_adc_cali_en, g_o_vts_abb, g_o_slope, g_o_slope_sign, g_id);

    mtktscpu_dprintk("[thermal_cal_prepare]: Done\n");
}

static void thermal_cal_prepare_2(kal_uint32 ret)
{
    kal_int32 y_vbg_plus = ret;
    kal_int32 format_1 = 0;
    kal_int32 format_2 = 0;
    kal_int32 format_3 = 0;
    kal_int32 format_3_abb = 0;
    kal_int32 format_4 = 0;
    kal_int32 format_4_abb = 0;
    
    g_ge = ((g_adc_ge - 128) * 10000 ) / 4096; // ge * 10000
    g_oe = (g_adc_oe - 128);
    
    format_1 = (y_vbg_plus - g_oe);
    format_2 = ((g_o_vbg + 1800) - g_oe);
    format_3 = ((g_o_vts + 2700) - g_oe);
    //format_3_abb = ((g_o_vts + 2700 + g_o_vts_abb - 20) - g_oe);
    format_3_abb = ((g_o_vts + 2700) - g_oe);
    
    y_vts_plus = ((format_1 * format_3) / format_2) + g_oe;
    y_vts_abb_plus = ((format_1 * format_3_abb) / format_2) + g_oe;
    g_gain = (10000 + g_ge) * format_1 / format_2; // gain * 10000
    
    format_4 = (y_vts_plus - g_oe);
    format_4_abb = (y_vts_abb_plus - g_oe);
    
    g_x_roomt = (((format_4 * 10000) / 4096) * 10000) / g_gain; // x_roomt * 10000
    g_x_roomt_abb = (((format_4_abb * 10000) / 4096) * 10000) / g_gain; // x_roomt * 10000
    
    mtktscpu_dprintk("g_ge = %d, g_oe = %d, y_vts_prime = %d, g_gain = %d, g_x_roomt = %d, y_vts_abb_prime = %d, g_x_roomt_abb = %d\n", 
        g_ge, g_oe, y_vts_plus, g_gain, g_x_roomt, y_vts_abb_plus, g_x_roomt_abb);

    mtktscpu_dprintk("[thermal_cal_prepare_2]: Done\n");
}

static kal_int32 thermal_cal_exec(kal_uint32 ret)
{
    kal_int32 t_current = 0;
    kal_int32 y_curr = ret;
    kal_int32 format_1 = 0;
    kal_int32 format_2 = 0;
    kal_int32 format_3 = 0;
    kal_int32 format_4 = 0;

    if(ret==0)
    {
        return 0;
    }
    
    format_1 = (g_degc_cali / 2);
    format_2 = (y_curr - g_oe);
    format_3 = (((((format_2) * 10000) / 4096) * 10000) / g_gain) - g_x_roomt;

    //format_4 = ((format_3 * 100) / 139); // uint = 0.1 deg
    if(g_o_slope_sign==0)
    {
        format_4 = ((format_3 * 100) / (139+g_o_slope)); // uint = 0.1 deg
    }
    else
    {
        format_4 = ((format_3 * 100) / (139-g_o_slope)); // uint = 0.1 deg
    } 
    format_4 = format_4 - (2 * format_4);
    
    t_current = (format_1 * 10) + format_4; // uint = 0.1 deg
    
    return t_current;
}

static kal_int32 thermal_cal_exec_abb(kal_uint32 ret)
{
    kal_int32 t_current = 0;
    kal_int32 y_curr = ret;
    kal_int32 format_1 = 0;
    kal_int32 format_2 = 0;
    kal_int32 format_3 = 0;
    kal_int32 format_4 = 0;

    if(ret==0)
    {
        return 0;
    }
    
    format_1 = (g_degc_cali / 2);
    format_2 = (y_curr - g_oe);
    format_3 = (((((format_2) * 10000) / 4096) * 10000) / g_gain) - g_x_roomt_abb;

    //format_4 = ((format_3 * 100) / 139); // uint = 0.1 deg 
    if(g_o_slope_sign==0)
    {
        format_4 = ((format_3 * 100) / (139+g_o_slope)); // uint = 0.1 deg
    }
    else
    {
        format_4 = ((format_3 * 100) / (139-g_o_slope)); // uint = 0.1 deg
    }    
    format_4 = format_4 - (2 * format_4);
    
    t_current = (format_1 * 10) + format_4; // uint = 0.1 deg
    
    return t_current;
}

static int thermal_auxadc_get_data(int times)
{
    int ret = 0, data[4], i, ret_value = 0, ret_temp = 0;

    if( IMM_IsAdcInitReady() == 0 )
    {
        mtktscpu_dprintk("[thermal_auxadc_get_data]: AUXADC is not ready\n");
        return 0;
    }
    
    i = times;
    while (i--)
    {
        ret_value = IMM_GetOneChannelValue(THERMAL_CHANNEL, data, &ret_temp);
        ret += ret_temp;
        //mtktscpu_dprintk("[thermal_auxadc_get_data(ADCIN5)]: ret_temp=%d\n",ret_temp);        
        msleep(10);
    }
    
    ret = ret / times;
    
    return ret;
}

static void thermal_calibration(void)
{
    kal_uint32 ret = 0;
    
    //thermal_cal_prepare();
    
    //get VBG
    //DRV_WriteReg32(TS_CON1, 0x0002);
    //DRV_WriteReg32(TS_CON2, 0x0200);
    //msleep(1);    
    //mtktscpu_dprintk("[thermal_calibration]: Before read VBG (0x%x,0x%x,0x%x,0x%x) \n", DRV_Reg16(TS_CON0), DRV_Reg16(TS_CON1), DRV_Reg16(TS_CON2), DRV_Reg16(TS_CON3));     
    //ret = thermal_auxadc_get_data(y_vbg_plus_repeat_times); //y_vbg_plus
    //xlog_printk(ANDROID_LOG_INFO, "Power/Thermal", "[thermal_calibration]: average %d times channel 5 = %d\n", y_vbg_plus_repeat_times, ret);
    //mtktscpu_dprintk("[thermal_calibration]: After read VBG (0x%x,0x%x,0x%x,0x%x) \n", DRV_Reg16(TS_CON0), DRV_Reg16(TS_CON1), DRV_Reg16(TS_CON2), DRV_Reg16(TS_CON3)); 
    
    //get VBG - workaround
    if(g_adc_cali_en == 1)
    {
        ret = g_o_vbg + 1800;
        mtktscpu_dprintk("[thermal_calibration]: VBG = g_o_vbg(%d) + 1800 = %d \n", g_o_vbg, ret);
    }
    else
    {
        ret = 1967; // y_vbg_plus default value
        mtktscpu_dprintk("[thermal_calibration]: VBG = y_vbg_plus = %d \n", ret);            
    }
    
    if (g_adc_cali_en == 1)
    {
        thermal_cal_prepare_2(ret); // will save DEGC_cali, OE, Gain and X_roomt in memory for future use
    }
    else
    {
        thermal_cal_prepare_2(ret); // will save DEGC_cali, OE, Gain and X_roomt in memory for future use
    }
    
    mtktscpu_dprintk("[thermal_calibration]: Done\n");
}

static int mtktscpu_get_hw_temp(void)
{
    int ret = 0;
    int t_ret = 0;

    mtktscpu_dprintk("[mtktscpu_get_hw_temp] \n");

    //get HW CPU temp (TSMCU)
    DRV_WriteReg32(TS_CON1, 0x0002);
    DRV_WriteReg32(TS_CON2, 0x0000);
 	msleep(1);
   	ret = thermal_auxadc_get_data(y_curr_repeat_times);
    mtktscpu_dprintk("[mtktscpu_get_hw_temp]: TSMCU average %d times channel 5 (0x%x,0x%x,0x%x,0x%x) = %d\n",
            y_curr_repeat_times, DRV_Reg16(TS_CON0), DRV_Reg16(TS_CON1), DRV_Reg16(TS_CON2), DRV_Reg16(TS_CON3), ret);
    
    t_ret = thermal_cal_exec(ret);
    t_ret = t_ret * 100;
    mtktscpu_dprintk("[mtktscpu_get_hw_temp] T_MCU, %d\n", t_ret);

    return t_ret;
}
    
static int mtktscpu_get_temp(struct thermal_zone_device *thermal,
			       unsigned long *t)
{
    mtktscpu_dprintk("[mtktscpu_get_temp] \n");
    
    *t = mtktscpu_get_hw_temp();

	return 0;
}

static int mtktscpu_bind(struct thermal_zone_device *thermal,
			struct thermal_cooling_device *cdev)
{
    int table_val=0;

    if (cdev == cl_dev_500)
        table_val= 3;
    else if (cdev == cl_dev_250)
        table_val= 2;
    else if (cdev == cl_dev_166)
        table_val= 1;
    else if (cdev == cl_dev_sysrst)
        table_val= 0;
    else
        return 0;
    
    if (thermal_zone_bind_cooling_device(thermal, table_val, cdev)) {
	    mtktscpu_dprintk("[mtktscpu_bind] error binding cooling dev\n");
		return -EINVAL;
	} else {
	    mtktscpu_dprintk("[mtktscpu_bind] binding OK\n");
    }

	return 0;
}

static int mtktscpu_unbind(struct thermal_zone_device *thermal,
			  struct thermal_cooling_device *cdev)
{
    int table_val=0;

    if (cdev == cl_dev_500)
        table_val= 3;
    else if (cdev == cl_dev_250)
        table_val= 2;
    else if (cdev == cl_dev_166)
        table_val= 1;
    else if (cdev == cl_dev_sysrst)
        table_val= 0;
    else
        return 0;
    
    if (thermal_zone_unbind_cooling_device(thermal, table_val, cdev)) {
	    mtktscpu_dprintk("[mtktscpu_unbind] error unbinding cooling dev\n");
		return -EINVAL;
	} else {
	    mtktscpu_dprintk("[mtktscpu_unbind] unbinding OK\n");
    }

	return 0;
}

static int mtktscpu_get_mode(struct thermal_zone_device *thermal,
			    enum thermal_device_mode *mode)
{
    mtktscpu_dprintk("[mtktscpu_get_mode] %d\n", kernelmode);

    *mode = (kernelmode) ? THERMAL_DEVICE_ENABLED
			     : THERMAL_DEVICE_DISABLED;

	return 0;
}

static int mtktscpu_set_mode(struct thermal_zone_device *thermal,
			    enum thermal_device_mode mode)
{
    mtktscpu_dprintk("[mtktscpu_set_mode] %d\n", mode);

    kernelmode = mode;
    
	return 0;
}

static int mtktscpu_get_trip_type(struct thermal_zone_device *thermal, int trip,
				 enum thermal_trip_type *type)
{
    mtktscpu_dprintk("[mtktscpu_get_trip_type] %d\n", trip);

    if (trip == 0)
    	*type = THERMAL_TRIP_ACTIVE;
    if (trip == 1)
    	*type = THERMAL_TRIP_ACTIVE;
    if (trip == 2)
    	*type = THERMAL_TRIP_ACTIVE;
    if (trip == 3)
    	*type = THERMAL_TRIP_ACTIVE;
    
	return 0;
}

static int mtktscpu_get_trip_temp(struct thermal_zone_device *thermal, int trip,
				 unsigned long *temp)
{
    mtktscpu_dprintk("[mtktscpu_get_trip_temp] %d\n", trip);

    if (trip == 0)
    	*temp = trip_0_temp;
    if (trip == 1)
    	*temp = trip_1_temp;
    if (trip == 2)
    	*temp = trip_2_temp;
    if (trip == 3)
    	*temp = trip_3_temp;    
    
	return 0;
}

static int mtktscpu_get_crit_temp(struct thermal_zone_device *thermal,
				 unsigned long *temperature)
{
    mtktscpu_dprintk("[mtktscpu_get_crit_temp] \n");
    
	*temperature = MTKTSCPU_TEMP_CRIT;
    
	return 0;
}

/* bind callback functions to thermalzone */
static struct thermal_zone_device_ops mtktscpu_dev_ops = {
	.bind = mtktscpu_bind,
	.unbind = mtktscpu_unbind,
	.get_temp = mtktscpu_get_temp,
	.get_mode = mtktscpu_get_mode,
	.set_mode = mtktscpu_set_mode,
	.get_trip_type = mtktscpu_get_trip_type,
	.get_trip_temp = mtktscpu_get_trip_temp,
	.get_crit_temp = mtktscpu_get_crit_temp,
};

/*
 * cooling device callback functions (cpufreq_1000)
 * 1 : ON and 0 : OFF
 */
static int cpufreq_1000_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktscpu_dprintk("[cpufreq_1000_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_1000_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_1000_state;
    mtktscpu_dprintk("[cpufreq_1000_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_1000_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktscpu_dprintk("[cpufreq_1000_set_cur_state] %d\n", state);    
    cl_dev_1000_state = state;
    if(cl_dev_1000_state == 1)
        cpufreq_thermal_protect(DVFS_F1);
    else
        cpufreq_thermal_protect(DVFS_F1);
	return 0;
}

/*
 * cooling device callback functions (cpufreq_500)
 * 1 : ON and 0 : OFF
 */
static int cpufreq_500_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktscpu_dprintk("[cpufreq_500_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_500_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_500_state;
    mtktscpu_dprintk("[cpufreq_500_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_500_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktscpu_dprintk("[cpufreq_500_set_cur_state] %d\n", state);    
    cl_dev_500_state = state;
    if(cl_dev_500_state == 1)
        cpufreq_thermal_protect(DVFS_F2);  
    else
        cpufreq_thermal_protect(DVFS_F1);
	return 0;
}

/*
 * cooling device callback functions (cpufreq_250)
 * 1 : ON and 0 : OFF
 */
static int cpufreq_250_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktscpu_dprintk("[cpufreq_250_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_250_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_250_state;
    mtktscpu_dprintk("[cpufreq_250_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_250_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktscpu_dprintk("[cpufreq_250_set_cur_state] %d\n", state);    
    cl_dev_250_state = state;
    if(cl_dev_250_state == 1)
        cpufreq_thermal_protect(DVFS_F3);
    else
        cpufreq_thermal_protect(DVFS_F1);
	return 0;
}

/*
 * cooling device callback functions (cpufreq_166)
 * 1 : ON and 0 : OFF
 */
static int cpufreq_166_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktscpu_dprintk("[cpufreq_166_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_166_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_166_state;
    mtktscpu_dprintk("[cpufreq_166_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_166_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktscpu_dprintk("[cpufreq_166_set_cur_state] %d\n", state);    
    cl_dev_166_state = state;
    if(cl_dev_166_state == 1)
        cpufreq_thermal_protect(DVFS_F4);
    else
        cpufreq_thermal_protect(DVFS_F1);
	return 0;
}

/*
 * cooling device callback functions (mtktscpu_cooling_sysrst_ops)
 * 1 : ON and 0 : OFF
 */
static int sysrst_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktscpu_dprintk("[sysrst_get_max_state] %d\n", *state);
	return 0;
}
static int sysrst_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_sysrst_state;
    mtktscpu_dprintk("[sysrst_get_cur_state] %d\n", *state);
	return 0;
}
static int sysrst_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktscpu_dprintk("[sysrst_set_cur_state] %d\n", state);    
    cl_dev_sysrst_state = state;
    if(cl_dev_sysrst_state == 1)
        arch_reset(0,NULL);    
	return 0;
}

/* bind fan callbacks to fan device */
static struct thermal_cooling_device_ops mtktscpu_cooling_1000_ops = {
	.get_max_state = cpufreq_1000_get_max_state,
	.get_cur_state = cpufreq_1000_get_cur_state,
	.set_cur_state = cpufreq_1000_set_cur_state,
};
static struct thermal_cooling_device_ops mtktscpu_cooling_500_ops = {
	.get_max_state = cpufreq_500_get_max_state,
	.get_cur_state = cpufreq_500_get_cur_state,
	.set_cur_state = cpufreq_500_set_cur_state,
};
static struct thermal_cooling_device_ops mtktscpu_cooling_250_ops = {
	.get_max_state = cpufreq_250_get_max_state,
	.get_cur_state = cpufreq_250_get_cur_state,
	.set_cur_state = cpufreq_250_set_cur_state,
};
static struct thermal_cooling_device_ops mtktscpu_cooling_166_ops = {
	.get_max_state = cpufreq_166_get_max_state,
	.get_cur_state = cpufreq_166_get_cur_state,
	.set_cur_state = cpufreq_166_set_cur_state,
};
static struct thermal_cooling_device_ops mtktscpu_cooling_sysrst_ops = {
	.get_max_state = sysrst_get_max_state,
	.get_cur_state = sysrst_get_cur_state,
	.set_cur_state = sysrst_set_cur_state,
};


static int mtktscpu_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    
    p += sprintf(p, "[ mtktscpu_read] trip_0_temp=%d,trip_1_temp=%d,trip_2_temp=%d,trip_4_temp=%d,time_ms=%d\n", 
        trip_0_temp,trip_1_temp,trip_2_temp,trip_3_temp,interval*1000);
    
    *start = buf + off;
    
    len = p - buf;
    if (len > off)
        len -= off;
    else
        len = 0;
    
    return len < count ? len  : count;
}

static ssize_t mtktscpu_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len=0,trip_0=0,trip_1=0,trip_2=0,trip_3=0,time_msec=0;
    char desc[32];
    
    len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
    if (copy_from_user(desc, buffer, len))
    {
        return 0;
    }
    desc[len] = '\0';
    
    if (sscanf(desc, "%d %d %d %d %d",&trip_0,&trip_1,&trip_2,&trip_3,&time_msec) == 5)
    {
        trip_0_temp = trip_0;        
        trip_1_temp = trip_1;
        trip_2_temp = trip_2;
        trip_3_temp = trip_3;

        interval=time_msec / 1000;
        thz_dev->polling_delay = interval*1000;
	    thermal_zone_device_update(thz_dev);
        
        mtktscpu_dprintk("[mtktscpu_write] trip_0_temp=%d,trip_1_temp=%d,trip_2_temp=%d,trip_4_temp=%d,time_ms=%d\n", 
            trip_0_temp,trip_1_temp,trip_2_temp,trip_3_temp,interval*1000);
        
        return count;
    }
    else
    {
        mtktscpu_dprintk("[mtktscpu_write] bad argument\n");
    }
    
    return -EINVAL;
}

static int mtktscpu_suspend(struct device *dev)
{
    mtktscpu_dprintk("[mtktscpu_suspend] \n");

	return 0;
}

static int __devinit mtktscpu_probe(struct platform_device *device)
{
    struct proc_dir_entry *entry = NULL;
    struct proc_dir_entry *mtktscpu_dir = NULL;

    mtktscpu_dprintk("[mtktscpu_probe] \n");

    DRV_WriteReg16(TS_CON3, DRV_Reg16(TS_CON3) | 0x0100); // release turbo mode and enable LPF mode

    thermal_cal_prepare();
    thermal_calibration();

    mtktscpu_dir = proc_mkdir("mtktscpu", NULL);
    if (!mtktscpu_dir)
    {
        mtktscpu_dprintk("[mtktscpu_probe]: mkdir /proc/mtktscpu failed\n");
    }
    else
    {
        entry = create_proc_entry("mtktscpu", S_IRUGO | S_IWUSR, mtktscpu_dir);
        if (entry)
        {
            entry->read_proc = mtktscpu_read;
            entry->write_proc = mtktscpu_write;
        }
    }
    
	return 0;
}

static int mtktscpu_remove(struct platform_device *device)
{
    mtktscpu_dprintk("[mtktscpu_remove] \n");
    
	return 0;
}

static const struct dev_pm_ops mtktscpu_pm_ops = {
	.suspend = mtktscpu_suspend,
	.freeze  = mtktscpu_suspend,
};

static struct platform_driver mtktscpu_driver = {
	.driver = {
		.name  = "mtktscpu",
		.owner = THIS_MODULE,
		.pm    = &mtktscpu_pm_ops,
	},
	.probe = mtktscpu_probe,
	.remove = mtktscpu_remove,
};

static int mtktscpu_check_hardware(void)
{
    mtktscpu_dprintk("[mtktscpu_check_hardware] \n");
    
	return 0;
}

static int mtktscpu_register_platform(void)
{
	int err = 0;

    mtktscpu_dprintk("[mtktscpu_register_platform] \n");

	err = platform_driver_register(&mtktscpu_driver);
	if (err)
		return err;

	mtktscpu_dev = platform_device_alloc("mtktscpu", -1);
	if (!mtktscpu_dev) {
		err = -ENOMEM;
		goto err_device_alloc;
	}
	err = platform_device_add(mtktscpu_dev);
	if (err)
		goto err_device_add;

	return 0;

err_device_add:
	platform_device_put(mtktscpu_dev);
err_device_alloc:
	platform_driver_unregister(&mtktscpu_driver);
	return err;
}

static void mtktscpu_unregister_platform(void)
{
    mtktscpu_dprintk("[mtktscpu_unregister_platform] \n");

	platform_device_unregister(mtktscpu_dev);
	platform_driver_unregister(&mtktscpu_driver);
}

static int mtktscpu_register_thermal(void)
{
    mtktscpu_dprintk("[mtktscpu_register_thermal] \n");

    /* cooling devices : adjust freq to 1000,5000,250,166MHz */
	cl_dev_1000 = thermal_cooling_device_register("mtktscpu-freq-1000", NULL,
						 &mtktscpu_cooling_1000_ops);
	cl_dev_500 = thermal_cooling_device_register("mtktscpu-freq-500", NULL,
						 &mtktscpu_cooling_500_ops);
	cl_dev_250 = thermal_cooling_device_register("mtktscpu-freq-250", NULL,
						 &mtktscpu_cooling_250_ops);
	cl_dev_166 = thermal_cooling_device_register("mtktscpu-freq-166", NULL,
						 &mtktscpu_cooling_166_ops);
    cl_dev_sysrst = thermal_cooling_device_register("mtktscpu-sysrst", NULL,
						 &mtktscpu_cooling_sysrst_ops);

    /* trips : trip 0~3 */
	thz_dev = thermal_zone_device_register("mtktscpu", 4, NULL,
					      &mtktscpu_dev_ops, 0, 0, 0, interval*1000);

	return 0;
}

static void mtktscpu_unregister_thermal(void)
{
    mtktscpu_dprintk("[mtktscpu_unregister_thermal] \n");
    
	if (cl_dev_1000) {
		thermal_cooling_device_unregister(cl_dev_1000);
		cl_dev_1000 = NULL;
	}
    if (cl_dev_500) {
		thermal_cooling_device_unregister(cl_dev_500);
		cl_dev_500 = NULL;
	}    
    if (cl_dev_250) {
		thermal_cooling_device_unregister(cl_dev_250);
		cl_dev_250 = NULL;
	}
    if (cl_dev_166) {
		thermal_cooling_device_unregister(cl_dev_166);
		cl_dev_166 = NULL;
	}
    if (cl_dev_sysrst) {
		thermal_cooling_device_unregister(cl_dev_sysrst);
		cl_dev_sysrst = NULL;
	}

	if (thz_dev) {
		thermal_zone_device_unregister(thz_dev);
		thz_dev = NULL;
	}
}

static int __init mtktscpu_init(void)
{
	int err = 0;

    mtktscpu_dprintk("[mtktscpu_init] \n");

	err = mtktscpu_register_platform();
	if (err)
		goto out_err;

	err = mtktscpu_register_thermal();
	if (err)
		goto err_unreg;

	return 0;

err_unreg:
	mtktscpu_unregister_thermal();
	mtktscpu_unregister_platform();

out_err:
	return err;
}

static void __exit mtktscpu_exit(void)
{
    mtktscpu_dprintk("[mtktscpu_exit] \n");
    
	mtktscpu_unregister_thermal();
	mtktscpu_unregister_platform();
}

module_init(mtktscpu_init);
module_exit(mtktscpu_exit);
