/*
This is taken from Lab 3 for testing the sysfs functionality for bounding boxes
It is not necessary once the connection to the actual sysfs functionality is developed by the system management
*/

/* LAB 3 PWM Controller kernel module reference implementation */
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/of_device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <asm/io.h>

// define driver/module name
#define DRIVER_NAME "axitimer"

// INSTANCE flags
#define INSTANCE_FLAG_RUNNING 0x01

// maximum number of instances
#define MAX_INSTANCE_NUMBER 16
#define TIMER_MAX_COUNT 0xFFFFFFFF

// AXI timer constants
#define TIMER_TCSR0 0
#define TIMER_TCSR1 0x10
#define TIMER_TLR0 0x04
#define TIMER_TLR1 0x14
#define TIMER_TCR0 0x08
#define TIMER_TCR1 0x18

#define TIMER_ENALL (1 << 10)
#define TIMER_PWMA (1 << 9)
#define TIMER_GENT (1 << 2) 

#define TIMER_CLOCK_FREQ_HZ 100000000

// define structure for device instance data
struct esl_axitimer_instance
{
    void __iomem *regs; // memory-mapped registers
    unsigned int flags; // instance flags
    dev_t devno;        // device number
};

// define structure for class data (common across all instances)
struct esl_axitimer_global
{
    u32 dev_major; // device major number
    dev_t devt;

    unsigned int instance_number; // ever-increasing number for instances
};

// Instantiate class data
static struct esl_axitimer_global driver_data;

// Utility method to read registers
static inline u32 reg_read(struct esl_axitimer_instance *inst, u32 reg)
{
    return ioread32(inst->regs + reg);
}

// Utility method to write registers
static inline void reg_write(struct esl_axitimer_instance *inst, u32 reg,
                             u32 value)
{
    iowrite32(value, inst->regs + reg);
}

// Utility method to convert period from ms to the value which gets stored in the load register
static inline u32 period_convert_ms_to_reg_val(unsigned int period_ms)
{
    return U32_MAX + 2 - ((TIMER_CLOCK_FREQ_HZ * period_ms) / 1000);
}

// Utility method to convert period register value to period in ms
static inline u32 period_convert_reg_val_to_ms(u32 reg_val)
{
    return ((U32_MAX - reg_val + 2) * 1000) / TIMER_CLOCK_FREQ_HZ;
}

// Utility method to convert duty cycle percent to the value which gets stored in the load register
static inline u32 duty_convert_percent_to_reg_val(struct esl_axitimer_instance *inst, unsigned int duty_percent)
{

    u32 period_reg_val = reg_read(inst, TIMER_TLR0);
    u32 period_ms = period_convert_reg_val_to_ms(period_reg_val);

    return (U32_MAX + 2 - ( (TIMER_CLOCK_FREQ_HZ * period_ms) / ((100/duty_percent)*1000) ));
}

// utility method to convert duty cycle percent to the generate value which is stored in the load register
static inline u32 duty_convert_reg_val_to_percent(struct esl_axitimer_instance *inst, u32 reg_val)
{
    u32 period_reg_val = reg_read(inst, TIMER_TLR0);
    return period_reg_val / reg_val;
}

// initialize device
static void axitimer_initialize(struct esl_axitimer_instance *inst)
{
    // set PWMA0 (9) and GENT0 (2) bits in TCSR0 and TCSR1 registers
    reg_write(inst, TIMER_TCSR0, TIMER_PWMA | TIMER_GENT);
    reg_write(inst, TIMER_TCSR1, TIMER_PWMA | TIMER_GENT);
}

// PRELAB Q3.3
// this function should get called whenever the 'show' member of the device_attribute called esl_axitimer_state is accessed,
// which happens whenever the sysfs file is read
// when the PWM controller is idle, this function prints "idle" and returns the result of 
// the sprintf() call, which should be 4 because 4 chars were written to the buffer (null char is excluded in count)
static ssize_t esl_axitimer_state_show(struct device *dev,
                                       struct device_attribute *attr,
                                       char *buf)
{
    struct esl_axitimer_instance *inst = dev_get_drvdata(dev);

    if (inst->flags & INSTANCE_FLAG_RUNNING)
    {
        return sprintf(buf, "running\n");
    }

    return sprintf(buf, "idle\n");
}

// PRELAB Q3.4
// the PWM controller sysfs file 'control' is defined and linked to this function
// further down in the code when the device_attribute 'esl_axitimer_control' is defined.
// (by not putting the parenthesis at the end of the fcn a ref to it is linked to the store member of the struct)
// 'esl_axitimer_control' gets grouped in the array of attributes 'esl_axitimer_attr_groups', and then all
// the sysfs stuff gets created in esl_axitimer_probe() when device_create_with_groups() gets called with the attribute 
// groups as one of its arguments.
static ssize_t esl_axitimer_control_store(struct device *dev,
                                          struct device_attribute *attr,
                                          const char *buf, size_t count)
{
    struct esl_axitimer_instance *inst = dev_get_drvdata(dev);

    if (!strncmp(buf, "on", count))
    {
        inst->flags |= INSTANCE_FLAG_RUNNING;

        // set ENALL bit (10) in TCSR0 register
        u32 r_val = reg_read(inst, TIMER_TCSR0);
        r_val |= TIMER_ENALL;
        reg_write(inst, TIMER_TCSR0, r_val);
    }
    else if (!strncmp(buf, "off", count))
    {
        inst->flags &= ~INSTANCE_FLAG_RUNNING;

        // clear TCSR0 register
        reg_write(inst, TIMER_TCSR0, 0x00);
        // clear TCSR1 register
        reg_write(inst, TIMER_TCSR1, 0x00);
    }
    else
    {
        return -EINVAL;
    }

    return count;
}

// example shell command for validation: echo -n 100 > period
static ssize_t esl_axitimer_pwm_period_store(struct device *dev,
                                            struct device_attribute *attr,
                                            const char *buf, size_t count)
{
    struct esl_axitimer_instance *inst = dev_get_drvdata(dev);

    unsigned long period;
    u32 period_reg_value;
    int res = kstrtoul(buf, 10, &period);
    if (!res)
    {
        // calculate timer period value to write into register
        // convert period (ms) to value that should be loaded into TLR0 reg
        period_reg_value = period_convert_ms_to_reg_val(period);

        // check that the number is within the admissible range of the AXI timer PWM period
        // (max 32 bit number is 4,294,967,295)
        if (period_reg_value > 4294967295U)
        {
            printk(KERN_INFO "Error: value is too large for AXI timer register\n");
            return -EINVAL;
        }

        // print out debug message for validation
        printk(KERN_INFO "PWM period write: %lu\n", period);

        // write period value to register
        reg_write(inst, TIMER_TLR0, period_reg_value);
    }
    else
    {
        return res; 
    }

    return count;
}

// example shell command for validation: cat period
static ssize_t esl_axitimer_pwm_period_show(struct device *dev,
                                            struct device_attribute *attr,
                                            char *buf)
{
    struct esl_axitimer_instance *inst = dev_get_drvdata(dev);

    // print out debug message for validation
    printk(KERN_INFO "PWM period read\n");
    
    // read TLR0 register 
    u32 period_reg_value = reg_read(inst, TIMER_TLR0);
    // reverse the period register value calculation to get the actual period
    u32 period = period_convert_reg_val_to_ms(period_reg_value);

    return sprintf(buf, "pwm period: %lu \t period (reg val): %lu\n", period, period_reg_value);
}

// example shell command for validation: echo -n 100 > duty
static ssize_t esl_axitimer_pwm_duty_store(struct device *dev,
                                            struct device_attribute *attr,
                                            const char *buf, size_t count)
{
    struct esl_axitimer_instance *inst = dev_get_drvdata(dev);

    unsigned long duty;
    u32 duty_reg_value;
    int res = kstrtoul(buf, 10, &duty);
    if (!res)
    {
        // calculate timer duty value to write into register
        duty_reg_value = duty_convert_percent_to_reg_val(inst, duty);

        // check that the number is within the admissible range of the AXI timer PWM duty cycle
        // (LOAD registers are 32 bits, max 32 bit number is 4,294,967,295)
        if (duty_reg_value > 4294967295U)
        {
            printk(KERN_INFO "Error: value is too large for AXI timer register\n");
            return -EINVAL;
        }

        // print out debug message for validation
        printk(KERN_INFO "PWM duty write: %lu\n", duty);

        // write duty value to register TLR1
        reg_write(inst, TIMER_TLR1, duty_reg_value);
    }
    else
    {
        return res; 
    }

    return count;
}

// example shell command for validation: cat duty
static ssize_t esl_axitimer_pwm_duty_show(struct device *dev,
                                            struct device_attribute *attr,
                                            char *buf)
{
    struct esl_axitimer_instance *inst = dev_get_drvdata(dev);

    // print out debug message for validation
    printk(KERN_INFO "PWM duty read\n");

    // read TLR1 register 
    u32 duty_reg_val = reg_read(inst, TIMER_TLR1);
    // reverse the duty register value calculation to get the actual duty cycle
    u32 duty = duty_convert_reg_val_to_percent(inst, duty_reg_val);

    return sprintf(buf, "pwm duty: %lu \t duty (reg_val): %lu\n", duty, duty_reg_val);
}



// define our own class for proper and automatic sysfs file group creation

// start / stop control
static struct device_attribute esl_axitimer_control = {
    .attr = {
        .name = "control",
        .mode = S_IWUSR, // write only
    },
    .store = esl_axitimer_control_store,
};

// current state
static struct device_attribute esl_axitimer_state = {
    .attr = {
        .name = "status",
        .mode = S_IRUGO,
    },
    .show = esl_axitimer_state_show,
};

// read/write PWM period
static struct device_attribute esl_axitimer_pwm_period = {
    .attr = {
        .name = "period",
        .mode = (S_IWUSR | S_IRUGO), // read and write permissions
    },
    .store = esl_axitimer_pwm_period_store,
    .show = esl_axitimer_pwm_period_show,
};

// read/write PWM duty cycle
static struct device_attribute esl_axitimer_pwm_duty = {
    .attr = {
        .name = "duty",
        .mode = (S_IWUSR | S_IRUGO), // read and write permissions
    },
    .store = esl_axitimer_pwm_duty_store,
    .show = esl_axitimer_pwm_duty_show,
};


// array of attributes
static struct attribute *esl_axitimer_attrs[] = {
    &esl_axitimer_control.attr,
    &esl_axitimer_state.attr,
    &esl_axitimer_pwm_period.attr,
    &esl_axitimer_pwm_duty.attr,
    NULL};

// Group all attributes to "control" folder,
// this creates /sys/class/axitimer/axitimerX/control
static struct attribute_group esl_axitimer_attr_group = {
    .name = "control", // control folder
    .attrs = esl_axitimer_attrs,
};

// array of attribute groups (sysfs files and folders)
static const struct attribute_group *esl_axitimer_attr_groups[] = {
    &esl_axitimer_attr_group, // the "control" folder
    NULL};

// define the class, creates /sys/class/axitimer
static struct class esl_axitimer_class = {
    .name = "axitimer",
    .owner = THIS_MODULE,
};

// probe, or add an instance
static int esl_axitimer_probe(struct platform_device *pdev)
{
    struct esl_axitimer_instance *inst = NULL; // new instance
    struct resource *res;                      // for reading resources from device tree
    struct device *dev;
    const void *property;
    // make new device; we need device a number (major, minor)
    // use instance count as the minor, which we increase each time
    // major has already been allocated in init function
    // to take a look at majors, do cat /proc/devices
    dev_t devno = MKDEV(driver_data.dev_major, driver_data.instance_number);

    // verify if timer is marked as PWM timer, get resource from device tree
    property = of_get_property(pdev->dev.of_node, "esl,pwm-timer", NULL);
    if (!property)
    {
        // timer is not marked for PWM, fail probe
        printk(KERN_INFO "kPWM: %s not marked as PWM timer.\n",
               pdev->name);
        // return -EPERM;
    }

    // allocate new instance. explanation for this:
    //  1. we use devm_kzalloc instead of kzalloc because it has the nice property
    //     of freeing unused resources automatically when the platform driver is
    //     removed (devm = dev managed and attached to pdev->dev so when it is
    //     destroyed, data is garbage collected)
    //  2. GFP_KERNEL means we are allocating KERNEL memory, otherwise it should be
    //     GFP_USER
    inst = devm_kzalloc(&pdev->dev, sizeof(struct esl_axitimer_instance),
                        GFP_KERNEL);

    if (!inst)
    {
        // ran out of memory
        return -ENOMEM;
    }

    // PRELAB Q3.1
    // platform_get_resource(): this function is used to get a device resource.
    // - here we're passing IORESOURCE_MEM as the second param so we are getting a memory resource.
    // - returns a pointer to a device resource structure (we use this later when mapping registers to the memory resource)  
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (IS_ERR(res))
    {
        return PTR_ERR(res);
    }

    // PRELAB Q3.2
    // the point of this call is to map the registers in the esl_axitimer_inst instance to the 
    // memory resource of the device that we just got using platform_get_resource(). 
    // we need to do this so that later we can read/write the registers through the inst object
    inst->regs = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(inst->regs))
    {
        // error mapping
        return PTR_ERR(inst->regs);
    }

    // set platform driver data (our instance struct)
    platform_set_drvdata(pdev, inst);

    // create the character device, this will create:
    // 1. /dev/axitimerX
    // 2. all the sysfs stuff
    dev = device_create_with_groups(&esl_axitimer_class, &pdev->dev,
                                    devno, inst, esl_axitimer_attr_groups,
                                    "axitimer%d", driver_data.instance_number);

    if (IS_ERR(dev))
    {
        return PTR_ERR(dev);
    }

    // set dev number
    inst->devno = devno;

    // increment instance counter
    driver_data.instance_number++;

    // initialize timer
    axitimer_initialize(inst);

    printk(KERN_INFO "probed kPWM with timer %s\n", pdev->name);

    return 0;
}

// remove an instance
static int esl_axitimer_remove(struct platform_device *pdev)
{
    struct esl_axitimer_instance *inst = platform_get_drvdata(pdev);

    // cleanup and remove
    device_destroy(&esl_axitimer_class, inst->devno);

    return 0;
}

// matching table
// list of strings that indicate what platform
// device is compatible with
static struct of_device_id esl_axitimer_of_ids[] = {
    {.compatible = "xlnx,xps-timer-1.00.a"},
    {}};

// platform driver definition
static struct platform_driver esl_axitimer_driver = {
    .probe = esl_axitimer_probe,
    .remove = esl_axitimer_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = of_match_ptr(esl_axitimer_of_ids),
    },
};

// module initialization
static int esl_axitimer_init(void)
{
    int err;
    // allocate character device region, /dev/axitimerX
    err = alloc_chrdev_region(&driver_data.devt, 0, MAX_INSTANCE_NUMBER,
                              DRIVER_NAME);
    if (err)
    {
        return err;
    }

    // initialize our data
    driver_data.instance_number = 0;
    driver_data.dev_major = MAJOR(driver_data.devt);

    // register class
    class_register(&esl_axitimer_class);

    // register platform driver
    platform_driver_register(&esl_axitimer_driver);

    return 0;
}

// module removal
static void esl_axitimer_exit(void)
{
    // cleanup
    platform_driver_unregister(&esl_axitimer_driver);
    class_unregister(&esl_axitimer_class);
    unregister_chrdev_region(driver_data.devt, MAX_INSTANCE_NUMBER);
}

module_init(esl_axitimer_init);
module_exit(esl_axitimer_exit);

MODULE_DESCRIPTION("AXI Timer driver");
MODULE_LICENSE("GPL");