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

#include <mach/pmic_mt6329_hw_bank1.h> 
#include <mach/pmic_mt6329_sw_bank1.h> 
#include <mach/pmic_mt6329_hw.h>
#include <mach/pmic_mt6329_sw.h>
#include <mach/upmu_common_sw.h>
#include <mach/upmu_hw.h>

#define DVFS_F1 (1001000)   // KHz
#define DVFS_F2 ( 500500)   // KHz
#define DVFS_F3 ( 250250)   // KHz
#define DVFS_F4 ( 166833)   // KHz

extern void cpufreq_thermal_protect(int limited_freq);

static unsigned int interval = 0; /* seconds, 0 : no auto polling */
static unsigned int trip_0_temp = 150000;
static unsigned int trip_1_temp = 125000;
static unsigned int trip_2_temp = 110000;
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
static struct platform_device *mtktspmic_dev;
static int mtktspmic_debug_log = 1;
static int kernelmode = 0;

#define mtktspmic_TEMP_CRIT 150000 /* 150.000 degree Celsius */

#define mtktspmic_dprintk(fmt, args...)   \
do {                                    \
    if (mtktspmic_debug_log) {                \
        xlog_printk(ANDROID_LOG_INFO, "Power/PMIC_Thermal", fmt, ##args); \
    }                                   \
} while(0)

int ts_pmic_at_boot_time=0;
static int mtktspmic_get_hw_temp(void)
{
    kal_uint32 ret_val=0;
    kal_uint8 thermal_status=0;
    int t_ret=0;

    mtktspmic_dprintk("[mtktspmic_get_hw_temp] \n");

    if(ts_pmic_at_boot_time==0){            
        ts_pmic_at_boot_time=1;
        mtktspmic_dprintk("[mtktspmic_get_hw_temp] at boot time, return 100002 as default\n");
        return 100002;
    }

    //get HW PMIC temp (TSPMIC)    
    ret_val=pmic_read_interface(0x13,&thermal_status,0x7,4);

    if(thermal_status == 0x0) {
        t_ret = 100002;
    } else if (thermal_status == 0x1) {
        t_ret = 115002;
    } else if (thermal_status == 0x3) {
        t_ret = 140002;    
    } else if (thermal_status == 0x7) {
        t_ret = 160002;
    } else {
        mtktspmic_dprintk("[mtktspmic_get_hw_temp] error register value (%d)\n", thermal_status);
    }        
    
    mtktspmic_dprintk("[mtktspmic_get_hw_temp] T_PMIC, %d, %d\n", t_ret, thermal_status);

    return t_ret;
}
    
static int mtktspmic_get_temp(struct thermal_zone_device *thermal,
			       unsigned long *t)
{
    mtktspmic_dprintk("[mtktspmic_get_temp] \n");
    
    *t = mtktspmic_get_hw_temp();

	return 0;
}

static int mtktspmic_bind(struct thermal_zone_device *thermal,
			struct thermal_cooling_device *cdev)
{
    int table_val=0;

    if (cdev == cl_dev_500)
        table_val= 2;
    else if (cdev == cl_dev_250)
        table_val= 1;
    else if (cdev == cl_dev_166)
        table_val= 0;
    else
        return 0;
    
    if (thermal_zone_bind_cooling_device(thermal, table_val, cdev)) {
	    mtktspmic_dprintk("[mtktspmic_bind] error binding cooling dev\n");
		return -EINVAL;
	} else {
	    mtktspmic_dprintk("[mtktspmic_bind] binding OK\n");
    }

	return 0;
}

static int mtktspmic_unbind(struct thermal_zone_device *thermal,
			  struct thermal_cooling_device *cdev)
{
    int table_val=0;

    if (cdev == cl_dev_500)
        table_val= 2;
    else if (cdev == cl_dev_250)
        table_val= 1;
    else if (cdev == cl_dev_166)
        table_val= 0;
    else
        return 0;
    
    if (thermal_zone_unbind_cooling_device(thermal, table_val, cdev)) {
	    mtktspmic_dprintk("[mtktspmic_unbind] error unbinding cooling dev\n");
		return -EINVAL;
	} else {
	    mtktspmic_dprintk("[mtktspmic_unbind] unbinding OK\n");
    }

	return 0;
}

static int mtktspmic_get_mode(struct thermal_zone_device *thermal,
			    enum thermal_device_mode *mode)
{
    mtktspmic_dprintk("[mtktspmic_get_mode] %d\n", kernelmode);

    *mode = (kernelmode) ? THERMAL_DEVICE_ENABLED
			     : THERMAL_DEVICE_DISABLED;

	return 0;
}

static int mtktspmic_set_mode(struct thermal_zone_device *thermal,
			    enum thermal_device_mode mode)
{
    mtktspmic_dprintk("[mtktspmic_set_mode] %d\n", mode);

    kernelmode = mode;
    
	return 0;
}

static int mtktspmic_get_trip_type(struct thermal_zone_device *thermal, int trip,
				 enum thermal_trip_type *type)
{
    mtktspmic_dprintk("[mtktspmic_get_trip_type] %d\n", trip);

    if (trip == 0)
    	*type = THERMAL_TRIP_ACTIVE;
    if (trip == 1)
    	*type = THERMAL_TRIP_ACTIVE;
    if (trip == 2)
    	*type = THERMAL_TRIP_ACTIVE;
    
	return 0;
}

static int mtktspmic_get_trip_temp(struct thermal_zone_device *thermal, int trip,
				 unsigned long *temp)
{
    mtktspmic_dprintk("[mtktspmic_get_trip_temp] %d\n", trip);

    if (trip == 0)
    	*temp = trip_0_temp;
    if (trip == 1)
    	*temp = trip_1_temp;
    if (trip == 2)
    	*temp = trip_2_temp;  
    
	return 0;
}

static int mtktspmic_get_crit_temp(struct thermal_zone_device *thermal,
				 unsigned long *temperature)
{
    mtktspmic_dprintk("[mtktspmic_get_crit_temp] \n");
    
	*temperature = mtktspmic_TEMP_CRIT;
    
	return 0;
}

/* bind callback functions to thermalzone */
static struct thermal_zone_device_ops mtktspmic_dev_ops = {
	.bind = mtktspmic_bind,
	.unbind = mtktspmic_unbind,
	.get_temp = mtktspmic_get_temp,
	.get_mode = mtktspmic_get_mode,
	.set_mode = mtktspmic_set_mode,
	.get_trip_type = mtktspmic_get_trip_type,
	.get_trip_temp = mtktspmic_get_trip_temp,
	.get_crit_temp = mtktspmic_get_crit_temp,
};

/*
 * cooling device callback functions (cpufreq_1000)
 * 1 : ON and 0 : OFF
 */
static int cpufreq_1000_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktspmic_dprintk("[cpufreq_1000_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_1000_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_1000_state;
    mtktspmic_dprintk("[cpufreq_1000_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_1000_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktspmic_dprintk("[cpufreq_1000_set_cur_state] %d\n", state);    
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
    mtktspmic_dprintk("[cpufreq_500_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_500_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_500_state;
    mtktspmic_dprintk("[cpufreq_500_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_500_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktspmic_dprintk("[cpufreq_500_set_cur_state] %d\n", state);    
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
    mtktspmic_dprintk("[cpufreq_250_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_250_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_250_state;
    mtktspmic_dprintk("[cpufreq_250_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_250_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktspmic_dprintk("[cpufreq_250_set_cur_state] %d\n", state);    
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
    mtktspmic_dprintk("[cpufreq_166_get_max_state] %d\n", *state);
	return 0;
}
static int cpufreq_166_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_166_state;
    mtktspmic_dprintk("[cpufreq_166_get_cur_state] %d\n", *state);
	return 0;
}
static int cpufreq_166_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktspmic_dprintk("[cpufreq_166_set_cur_state] %d\n", state);    
    cl_dev_166_state = state;
    if(cl_dev_166_state == 1)
        cpufreq_thermal_protect(DVFS_F4);
    else
        cpufreq_thermal_protect(DVFS_F1);
	return 0;
}

/*
 * cooling device callback functions (mtktspmic_cooling_sysrst_ops)
 * 1 : ON and 0 : OFF
 */
static int sysrst_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = 1;    
    mtktspmic_dprintk("[sysrst_get_max_state] %d\n", *state);
	return 0;
}
static int sysrst_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{        
	*state = cl_dev_sysrst_state;
    mtktspmic_dprintk("[sysrst_get_cur_state] %d\n", *state);
	return 0;
}
static int sysrst_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
    mtktspmic_dprintk("[sysrst_set_cur_state] %d\n", state);    
    cl_dev_sysrst_state = state;
    if(cl_dev_sysrst_state == 1)
        arch_reset(0,NULL);    
	return 0;
}

/* bind fan callbacks to fan device */
static struct thermal_cooling_device_ops mtktspmic_cooling_1000_ops = {
	.get_max_state = cpufreq_1000_get_max_state,
	.get_cur_state = cpufreq_1000_get_cur_state,
	.set_cur_state = cpufreq_1000_set_cur_state,
};
static struct thermal_cooling_device_ops mtktspmic_cooling_500_ops = {
	.get_max_state = cpufreq_500_get_max_state,
	.get_cur_state = cpufreq_500_get_cur_state,
	.set_cur_state = cpufreq_500_set_cur_state,
};
static struct thermal_cooling_device_ops mtktspmic_cooling_250_ops = {
	.get_max_state = cpufreq_250_get_max_state,
	.get_cur_state = cpufreq_250_get_cur_state,
	.set_cur_state = cpufreq_250_set_cur_state,
};
static struct thermal_cooling_device_ops mtktspmic_cooling_166_ops = {
	.get_max_state = cpufreq_166_get_max_state,
	.get_cur_state = cpufreq_166_get_cur_state,
	.set_cur_state = cpufreq_166_set_cur_state,
};
static struct thermal_cooling_device_ops mtktspmic_cooling_sysrst_ops = {
	.get_max_state = sysrst_get_max_state,
	.get_cur_state = sysrst_get_cur_state,
	.set_cur_state = sysrst_set_cur_state,
};


static int mtktspmic_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    char *p = buf;
    
    p += sprintf(p, "[ mtktspmic_read] time_ms=%d\n", interval*1000);
    
    *start = buf + off;
    
    len = p - buf;
    if (len > off)
        len -= off;
    else
        len = 0;
    
    return len < count ? len  : count;
}

static ssize_t mtktspmic_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len=0,time_msec=0;
    char desc[32];
    
    len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
    if (copy_from_user(desc, buffer, len))
    {
        return 0;
    }
    desc[len] = '\0';
    
    if (sscanf(desc, "%d",&time_msec) == 1)
    {
        interval=time_msec / 1000;
        thz_dev->polling_delay = interval*1000;
	    thermal_zone_device_update(thz_dev);
        
        mtktspmic_dprintk("[mtktspmic_write] time_ms=%d\n", interval*1000);
        
        return count;
    }
    else
    {
        mtktspmic_dprintk("[mtktspmic_write] bad argument\n");
    }
    
    return -EINVAL;
}

static int mtktspmic_suspend(struct device *dev)
{
    mtktspmic_dprintk("[mtktspmic_suspend] \n");

	return 0;
}

static int __devinit mtktspmic_probe(struct platform_device *device)
{
    struct proc_dir_entry *entry = NULL;
    struct proc_dir_entry *mtktspmic_dir = NULL;

    mtktspmic_dprintk("[mtktspmic_probe] \n");

    mtktspmic_dir = proc_mkdir("mtktspmic", NULL);
    if (!mtktspmic_dir)
    {
        mtktspmic_dprintk("[mtktspmic_probe]: mkdir /proc/mtktspmic failed\n");
    }
    else
    {
        entry = create_proc_entry("mtktspmic", S_IRUGO | S_IWUSR, mtktspmic_dir);
        if (entry)
        {
            entry->read_proc = mtktspmic_read;
            entry->write_proc = mtktspmic_write;
        }
    }
    
	return 0;
}

static int mtktspmic_remove(struct platform_device *device)
{
    mtktspmic_dprintk("[mtktspmic_remove] \n");
    
	return 0;
}

static const struct dev_pm_ops mtktspmic_pm_ops = {
	.suspend = mtktspmic_suspend,
	.freeze  = mtktspmic_suspend,
};

static struct platform_driver mtktspmic_driver = {
	.driver = {
		.name  = "mtktspmic",
		.owner = THIS_MODULE,
		.pm    = &mtktspmic_pm_ops,
	},
	.probe = mtktspmic_probe,
	.remove = mtktspmic_remove,
};

static int mtktspmic_check_hardware(void)
{
    mtktspmic_dprintk("[mtktspmic_check_hardware] \n");
    
	return 0;
}

static int mtktspmic_register_platform(void)
{
	int err = 0;

    mtktspmic_dprintk("[mtktspmic_register_platform] \n");

	err = platform_driver_register(&mtktspmic_driver);
	if (err)
		return err;

	mtktspmic_dev = platform_device_alloc("mtktspmic", -1);
	if (!mtktspmic_dev) {
		err = -ENOMEM;
		goto err_device_alloc;
	}
	err = platform_device_add(mtktspmic_dev);
	if (err)
		goto err_device_add;

	return 0;

err_device_add:
	platform_device_put(mtktspmic_dev);
err_device_alloc:
	platform_driver_unregister(&mtktspmic_driver);
	return err;
}

static void mtktspmic_unregister_platform(void)
{
    mtktspmic_dprintk("[mtktspmic_unregister_platform] \n");

	platform_device_unregister(mtktspmic_dev);
	platform_driver_unregister(&mtktspmic_driver);
}

static int mtktspmic_register_thermal(void)
{
    mtktspmic_dprintk("[mtktspmic_register_thermal] \n");

    /* cooling devices */
	cl_dev_1000 = thermal_cooling_device_register("mtktspmic-freq-1000", NULL,
						 &mtktspmic_cooling_1000_ops);
	cl_dev_500 = thermal_cooling_device_register("mtktspmic-freq-500", NULL,
						 &mtktspmic_cooling_500_ops);
	cl_dev_250 = thermal_cooling_device_register("mtktspmic-freq-250", NULL,
						 &mtktspmic_cooling_250_ops);
	cl_dev_166 = thermal_cooling_device_register("mtktspmic-freq-166", NULL,
						 &mtktspmic_cooling_166_ops);
    cl_dev_sysrst = thermal_cooling_device_register("mtktspmic-sysrst", NULL,
						 &mtktspmic_cooling_sysrst_ops);

    /* trips : trip 0~2 */
	thz_dev = thermal_zone_device_register("mtktspmic", 3, NULL,
					      &mtktspmic_dev_ops, 0, 0, 0, interval*1000);

	return 0;
}

static void mtktspmic_unregister_thermal(void)
{
    mtktspmic_dprintk("[mtktspmic_unregister_thermal] \n");
    
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

static int __init mtktspmic_init(void)
{
	int err = 0;

    mtktspmic_dprintk("[mtktspmic_init] \n");

	err = mtktspmic_register_platform();
	if (err)
		goto out_err;

	err = mtktspmic_register_thermal();
	if (err)
		goto err_unreg;

	return 0;

err_unreg:
	mtktspmic_unregister_thermal();
	mtktspmic_unregister_platform();

out_err:
	return err;
}

static void __exit mtktspmic_exit(void)
{
    mtktspmic_dprintk("[mtktspmic_exit] \n");
    
	mtktspmic_unregister_thermal();
	mtktspmic_unregister_platform();
}

module_init(mtktspmic_init);
module_exit(mtktspmic_exit);
