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
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/writeback.h>
#include <asm/uaccess.h>

#include <mach/system.h>

#include "mach/mt6575_typedefs.h"
#include "mach/mt6575_thermal.h"

#include <mach/pmic_mt6329_hw_bank1.h> 
#include <mach/pmic_mt6329_sw_bank1.h> 
#include <mach/pmic_mt6329_hw.h>
#include <mach/pmic_mt6329_sw.h>
#include <mach/upmu_common_sw.h>
#include <mach/upmu_hw.h>

static unsigned int interval = 0; /* seconds, 0 : no auto polling */
static unsigned int trip_0_temp = 60000;
static unsigned int trip_1_temp = 50000;
static unsigned int cl_dev_dis_charge_state = 0;
static unsigned int cl_dev_sysrst_state = 0;
static struct thermal_zone_device *thz_dev;
static struct thermal_cooling_device *cl_dev_dis_charge;
static struct thermal_cooling_device *cl_dev_sysrst;
static struct platform_device *mtktsbattery_dev;
static int mtktsbattery_debug_log = 1;
static int kernelmode = 0;

#define mtktsbattery_TEMP_CRIT 60000 /* 60.000 degree Celsius */

#define mtktsbattery_dprintk(fmt, args...)   \
do {                                    \
    if (mtktsbattery_debug_log) {                \
        xlog_printk(ANDROID_LOG_INFO, "Power/Battery_Thermal", fmt, ##args); \
    }                                   \
} while(0)

/*
 * kernel fopen/fclose
 */
static mm_segment_t oldfs;

static void my_close(int fd)
{
    set_fs(oldfs);
    sys_close(fd);
}

static int my_open(char *fname, int flag)
{
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    return sys_open(fname, flag, 0);
}

static int get_hw_battery_temp(void)
{
    int fd;
    char buf[64];
    char *pmtdbufp = NULL;
    ssize_t pmtdsize;

    char *pvalue = NULL;
    int got_value=0;
    int new_value=0;
    char out_buf[64];
    const char *out_buf_const;
    int ret=0;
    int len=0;

    //open file and read current value
    fd = my_open("/sys/class/power_supply/battery/batt_temp", O_RDONLY);
    if (fd < 0)
    {   
        mtktsbattery_dprintk("[get_hw_battery_temp]: open file fail");
        return;
    }
    mtktsbattery_dprintk("[get_hw_battery_temp]: open file ok");
    buf[sizeof(buf) - 1] = '\0';
    pmtdsize = sys_read(fd, buf, sizeof(buf) - 1);
    pmtdbufp = buf;   
    got_value = simple_strtol(pmtdbufp,&pvalue,10);

    // close file
    my_close(fd);

    // debug
    mtktsbattery_dprintk("[get_hw_battery_temp]: got_value=%d\n", got_value);

    return got_value;
    
}


int ts_battery_at_boot_time=0;
static int mtktsbattery_get_hw_temp(void)
{
    int t_ret=0;

    mtktsbattery_dprintk("[mtktsbattery_get_hw_temp] \n");

    if(ts_battery_at_boot_time==0){            
        ts_battery_at_boot_time=1;
        mtktsbattery_dprintk("[mtktsbattery_get_hw_temp] at boot time, return 25000 as default\n");
        return 25000;
    }

    //get HW battery temp (TSBATTERY)    
    //cat /sys/class/power_supply/battery/batt_temp
    t_ret = get_hw_battery_temp();
    t_ret = t_ret * 100;    
    
    mtktsbattery_dprintk("[mtktsbattery_get_hw_temp] T_Battery, %d\n", t_ret);

    return t_ret;
}
    
static int mtktsbattery_get_temp(struct thermal_zone_device *thermal,
			       unsigned long *t)
{
    mtktsbattery_dprintk("[mtktsbattery_get_temp] \n");
    
    *t = mtktsbattery_get_hw_temp();

	return 0;
}

static int mtktsbattery_bind(struct thermal_zone_device *thermal,
			struct thermal_cooling_device *cdev)
{
    int table_val=0;

    if (cdev == cl_dev_dis_charge)
        table_val= 1;
    else if (cdev == cl_dev_sysrst)
        table_val= 0;
    else
        return 0;
    
    if (thermal_zone_bind_cooling_device(thermal, table_val, cdev)) {
	    mtktsbattery_dprintk("[mtktsbattery_bind] error binding cooling dev\n");
		return -EINVAL;
	} else {
	    mtktsbattery_dprintk("[mtktsbattery_bind] binding OK\n");
    }

	return 0;
}

static int mtktsbattery_unbind(struct thermal_zone_device *thermal,
			  struct thermal_cooling_device *cdev)
{
    int table_val=0;

    if (cdev == cl_dev_dis_charge)
        table_val= 1;
    else if (cdev == cl_dev_sysrst)
        table_val= 0;
    else
        return 0;
    
    if (thermal_zone_unbind_cooling_device(thermal, table_val, cdev)) {
	    mtktsbattery_dprintk("[mtktsbattery_unbind] error unbinding cooling dev\n");
		return -EINVAL;
	} else {
	    mtktsbattery_dprintk("[mtktsbattery_unbind] unbinding OK\n");
    }

	return 0;
}

static int mtktsbattery_get_mode(struct thermal_zone_device *thermal,
			    enum thermal_device_mode *mode)
{
    mtktsbattery_dprintk("[mtktsbattery_get_mode] %d\n", kernelmode);

    *mode = (kernelmode) ? THERMAL_DEVICE_ENABLED
			     : THERMAL_DEVICE_DISABLED;

	return 0;
}

static int mtktsbattery_set_mode(struct thermal_zone_device *thermal,
			    enum thermal_device_mode mode)
{
    mtktsbattery_dprintk("[mtktsbattery_set_mode] %d\n", mode);

    kernelmode = mode;
    
	return 0;
}

static int mtktsbattery_get_trip_type(struct thermal_zone_device *thermal, int trip,
				 enum thermal_trip_type *type)
{
    mtktsbattery_dprintk("[mtktsbattery_get_trip_type] %d\n", trip);

    if (trip == 0)
    	*type = THERMAL_TRIP_ACTIVE;
    if (trip == 1)
    	*type = THERMAL_TRIP_ACTIVE;
    
	return 0;
}

static int mtktsbattery_get_trip_temp(struct thermal_zone_device *thermal, int trip,
				 unsigned long *temp)
{
    mtktsbattery_dprintk("[mtktsbattery_get_trip_temp] %d\n", trip);

    if (trip == 0)
    	*temp = trip_0_temp;
    if (trip == 1)
    	*temp = trip_1_temp;
    
	return 0;
}

static int mtktsbattery_get_crit_temp(struct thermal_zone_device *thermal,
				 unsigned long *temperature)
{
    mtktsbattery_dprintk("[mtktsbattery_get_crit_temp] \n");
    
	*temperature = mtktsbattery_TEMP_CRIT;
    
	return 0;
}

/* bind callback functions to thermalzone */
static struct thermal_zone_device_ops mtktsbattery_dev_ops = {
	.bind = mtktsbattery_bind,
	.unbind = mtktsbattery_unbind,
	.get_temp = mtktsbattery_get_temp,
	.get_mode = mtktsbattery_get_mode,
	.set_mode = mtktsbattery_set_mode,
	.get_trip_type = mtktsbattery_get_trip_type,
	.get_trip_temp = mtktsbattery_get_trip_temp,
	.get_crit_temp = mtktsbattery_get_crit_temp,
};

/*
 * cooling device callback functions (dis_charge)
 * 1 : ON and 0 : OFF
 */
static int dis_charge_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktsbattery_dprintk("[dis_charge_get_max_state] %d\n", *state);
	return 0;
}
static int dis_charge_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_dis_charge_state;
    mtktsbattery_dprintk("[dis_charge_get_cur_state] %d\n", *state);
	return 0;
}
static int dis_charge_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktsbattery_dprintk("[dis_charge_set_cur_state] %d\n", state);    
    cl_dev_dis_charge_state = state;
    if(cl_dev_dis_charge_state == 1) {
        //dis_charge
        upmu_chr_enable(0);
        mtktsbattery_dprintk("[dis_charge_set_cur_state] disable charging\n");
    }
	return 0;
}

/*
 * cooling device callback functions (mtktsbattery_cooling_sysrst_ops)
 * 1 : ON and 0 : OFF
 */
static int sysrst_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktsbattery_dprintk("[sysrst_get_max_state] %d\n", *state);
	return 0;
}
static int sysrst_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_sysrst_state;
    mtktsbattery_dprintk("[sysrst_get_cur_state] %d\n", *state);
	return 0;
}
static int sysrst_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktsbattery_dprintk("[sysrst_set_cur_state] %d\n", state);    
    cl_dev_sysrst_state = state;
    if(cl_dev_sysrst_state == 1)
        arch_reset(0,NULL);    
	return 0;
}

/* bind fan callbacks to fan device */
static struct thermal_cooling_device_ops mtktsbattery_cooling_dis_charge_ops = {
	.get_max_state = dis_charge_get_max_state,
	.get_cur_state = dis_charge_get_cur_state,
	.set_cur_state = dis_charge_set_cur_state,
};
static struct thermal_cooling_device_ops mtktsbattery_cooling_sysrst_ops = {
	.get_max_state = sysrst_get_max_state,
	.get_cur_state = sysrst_get_cur_state,
	.set_cur_state = sysrst_set_cur_state,
};


static int mtktsbattery_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    
    p += sprintf(p, "[ mtktsbattery_read] trip_0_temp=%d,trip_1_temp=%d,time_ms=%d\n", 
        trip_0_temp,trip_1_temp,interval*1000);
    
    *start = buf + off;
    
    len = p - buf;
    if (len > off)
        len -= off;
    else
        len = 0;
    
    return len < count ? len  : count;
}

static ssize_t mtktsbattery_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len=0,trip_0=0,trip_1=0,time_msec=0;
    char desc[32];
    
    len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
    if (copy_from_user(desc, buffer, len))
    {
        return 0;
    }
    desc[len] = '\0';
    
    if (sscanf(desc, "%d %d %d",&trip_0,&trip_1,&time_msec) == 3)
    {
        trip_0_temp = trip_0;        
        trip_1_temp = trip_1;
        
        interval=time_msec / 1000;
        thz_dev->polling_delay = interval*1000;
	    thermal_zone_device_update(thz_dev);
        
        mtktsbattery_dprintk("[mtktsbattery_write] time_ms=%d\n", interval*1000);
        
        return count;
    }
    else
    {
        mtktsbattery_dprintk("[mtktsbattery_write] bad argument\n");
    }
    
    return -EINVAL;
}

static int mtktsbattery_suspend(struct device *dev)
{
    mtktsbattery_dprintk("[mtktsbattery_suspend] \n");

	return 0;
}

static int __devinit mtktsbattery_probe(struct platform_device *device)
{
    struct proc_dir_entry *entry = NULL;
    struct proc_dir_entry *mtktsbattery_dir = NULL;

    mtktsbattery_dprintk("[mtktsbattery_probe] \n");

    mtktsbattery_dir = proc_mkdir("mtktsbattery", NULL);
    if (!mtktsbattery_dir)
    {
        mtktsbattery_dprintk("[mtktsbattery_probe]: mkdir /proc/mtktsbattery failed\n");
    }
    else
    {
        entry = create_proc_entry("mtktsbattery", S_IRUGO | S_IWUSR, mtktsbattery_dir);
        if (entry)
        {
            entry->read_proc = mtktsbattery_read;
            entry->write_proc = mtktsbattery_write;
        }
    }
    
	return 0;
}

static int mtktsbattery_remove(struct platform_device *device)
{
    mtktsbattery_dprintk("[mtktsbattery_remove] \n");
    
	return 0;
}

static const struct dev_pm_ops mtktsbattery_pm_ops = {
	.suspend = mtktsbattery_suspend,
	.freeze  = mtktsbattery_suspend,
};

static struct platform_driver mtktsbattery_driver = {
	.driver = {
		.name  = "mtktsbattery",
		.owner = THIS_MODULE,
		.pm    = &mtktsbattery_pm_ops,
	},
	.probe = mtktsbattery_probe,
	.remove = mtktsbattery_remove,
};

static int mtktsbattery_check_hardware(void)
{
    mtktsbattery_dprintk("[mtktsbattery_check_hardware] \n");
    
	return 0;
}

static int mtktsbattery_register_platform(void)
{
	int err = 0;

    mtktsbattery_dprintk("[mtktsbattery_register_platform] \n");

	err = platform_driver_register(&mtktsbattery_driver);
	if (err)
		return err;

	mtktsbattery_dev = platform_device_alloc("mtktsbattery", -1);
	if (!mtktsbattery_dev) {
		err = -ENOMEM;
		goto err_device_alloc;
	}
	err = platform_device_add(mtktsbattery_dev);
	if (err)
		goto err_device_add;

	return 0;

err_device_add:
	platform_device_put(mtktsbattery_dev);
err_device_alloc:
	platform_driver_unregister(&mtktsbattery_driver);
	return err;
}

static void mtktsbattery_unregister_platform(void)
{
    mtktsbattery_dprintk("[mtktsbattery_unregister_platform] \n");

	platform_device_unregister(mtktsbattery_dev);
	platform_driver_unregister(&mtktsbattery_driver);
}

static int mtktsbattery_register_thermal(void)
{
    mtktsbattery_dprintk("[mtktsbattery_register_thermal] \n");

    /* cooling devices */
	cl_dev_dis_charge = thermal_cooling_device_register("mtktsbattery-dischr", NULL,
						 &mtktsbattery_cooling_dis_charge_ops);
    cl_dev_sysrst = thermal_cooling_device_register("mtktsbattery-sysrst", NULL,
						 &mtktsbattery_cooling_sysrst_ops);

    /* trips : trip 0~1 */
	thz_dev = thermal_zone_device_register("mtktsbattery", 2, NULL,
					      &mtktsbattery_dev_ops, 0, 0, 0, interval*1000);

	return 0;
}

static void mtktsbattery_unregister_thermal(void)
{
    mtktsbattery_dprintk("[mtktsbattery_unregister_thermal] \n");
    
    if (cl_dev_dis_charge) {
		thermal_cooling_device_unregister(cl_dev_dis_charge);
		cl_dev_dis_charge = NULL;
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

static int __init mtktsbattery_init(void)
{
	int err = 0;

    mtktsbattery_dprintk("[mtktsbattery_init] \n");

	err = mtktsbattery_register_platform();
	if (err)
		goto out_err;

	err = mtktsbattery_register_thermal();
	if (err)
		goto err_unreg;

	return 0;

err_unreg:
	mtktsbattery_unregister_thermal();
	mtktsbattery_unregister_platform();

out_err:
	return err;
}

static void __exit mtktsbattery_exit(void)
{
    mtktsbattery_dprintk("[mtktsbattery_exit] \n");
    
	mtktsbattery_unregister_thermal();
	mtktsbattery_unregister_platform();
}

module_init(mtktsbattery_init);
module_exit(mtktsbattery_exit);
