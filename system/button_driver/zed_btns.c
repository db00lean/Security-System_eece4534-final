#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/driver.h>
#include <linux/of_device.h>
#include <linux/kdev_t.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <asm/io.h>

#include "zed_btns_driver.h"
#include "zed_btns.h"

/////////////////////////////
/* Global structs */
/////////////////////////////

static struct kbtns_inst kbtns_global;
static struct class* kbtns_class;

/////////////////////////////
/* Chrdev file-operations */
/////////////////////////////

static ssize_t kbtns_read(struct file* f, 
                          char __user *buf, 
                          size_t count, 
                          loff_t* offset) 
{
    int ret; 
    unsigned int copied; 

    if (mutex_lock_interruptible(&kbtns_global.buffer_lock)) 
        return -ERESTARTSYS; 
    
    ret = kfifo_to_user(&kbtns_global.btns_buffer, buf, count, &copied);
    mutex_unlock(&kbtns_global.buffer_lock);

    return ret ? ret : copied;
}

static unsigned int kbtns_poll(struct file* f,
                               poll_table* wait) 
{
    if (mutex_lock_interruptible(&kbtns_global.buffer_lock)) 
        return -ERESTARTSYS;

    poll_wait(f, &kbtns_global.waitq, wait);
    if (kfifo_is_empty(&kbtns_global.btns_buffer)) {
        return 0; 
    }
    mutex_unlock(&kbtns_global.buffer_lock);

    return POLLIN | POLLRDNORM;
}

static struct file_operations kbtns_fileops = {
    .owner = THIS_MODULE,
    .read = kbtns_read, 
    .poll = kbtns_poll,
};

/////////////////////////////
/* Interrupt handlers */
/////////////////////////////

static void try_kfifo_put_locked(u8 btn_value) {
    int ret; 
    if (mutex_lock_interruptible(&kbtns_global.buffer_lock)) 
        printk(KERN_INFO "[ KBtns  ] - ERROR: failed to lock kbtns buffer when attempting to write\n");
    ret = kfifo_put(&(kbtns_global.btns_buffer), btn_value);    
    mutex_unlock(&kbtns_global.buffer_lock);

#ifdef DEBUG
    if (ret == 0) {
        printk(KERN_INFO "[ KBtns  ] - Tried putting value %u\n", btn_value);
    }
#endif
}

static void log_btn_press(void) {
    u32 btn_value = reg_read(&kbtns_global, BTN_DATA_OFFEST);
    if (btn_value != 0) {
        try_kfifo_put_locked(TO_U8(btn_value));
    }

#ifdef DEBUG
    if (IS_PRESSED(BTN_D, btn_value)) {
        printk(KERN_INFO "[ KBtns  ] - Down button pressed!\n");
    }

    if (IS_PRESSED(BTN_L, btn_value)) {
        printk(KERN_INFO "[ KBtns  ] - Left button pressed!\n");
    }

    if (IS_PRESSED(BTN_R, btn_value)) {
        printk(KERN_INFO "[ KBtns  ] - Right button pressed!\n");
    }

    if (IS_PRESSED(BTN_C, btn_value)) {
        printk(KERN_INFO "[ KBtns  ] - Center button pressed!\n");
    }
#endif
}

static irqreturn_t button_interrupt_handler(int irq, void* dev_id) {
    log_btn_press();
    clear_gpio_interrupt(&kbtns_global);
    return IRQ_HANDLED;
}

/////////////////////////////
/* Module set-up/clean-up helpers */ 
/////////////////////////////

int init_kbtns_chrdev(void) {
    int err;
    struct device* dev; 

    err = alloc_chrdev_region(&kbtns_global.devno, 0, 1, "zedbtns");
    if (err) {
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not alocate chrdev for zed_btns\n");
    }

    cdev_init(&kbtns_global.chrdev, &kbtns_fileops);

    err = cdev_add(&kbtns_global.chrdev, kbtns_global.devno, 1);
    if (err) {
        unregister_chrdev_region(kbtns_global.devno, 1);
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not add chrdev with fileops\n");
        return err; 
    }

    kbtns_class = class_create(THIS_MODULE, "zedbtns_char");
    if (IS_ERR(kbtns_class)) {
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not create zedbtns_char class\n");
        return -ENOENT; 
    }

    dev = device_create(kbtns_class, NULL, kbtns_global.devno, NULL, "zedbtns0");
    if (IS_ERR(dev)) {
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not create zedbtns0 device in /dev\n");
        return -ENOENT;
    }

    return 0; 
}

int init_kbtns_gpio(void) {
    struct gpio_chip* chip;
    struct platform_device* pdev;
    struct device* dev;
    struct gpio_desc* gdesc;
    struct resource* res;

    gdesc = gpio_to_desc(BTN_BASE_GPIO_CHIP);
    if (!gdesc) {
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not get GPIO description for buttons!\n");
        return -ENODEV;
    }

    // get chip of btns gpio
    chip = gpiod_to_chip(gdesc);
    if (!chip) {
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not get button GPIO chip!\n");
        return -ENODEV;
    }

    dev = chip->parent; 
    pdev = to_platform_device(dev);
    kbtns_global.irq_num = platform_get_irq(pdev, 0);

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (IS_ERR(res))
    {
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not retrieve button MMIO resource from platform device!\n");
        return PTR_ERR(res);
    }

    kbtns_global.base = devm_ioremap(dev, res->start, resource_size(res));
    if (IS_ERR(kbtns_global.base))
    {
        printk(KERN_INFO "[ KBtns  ] - ERROR: could not remap button MMIO resources!\n");
        return PTR_ERR(kbtns_global.base);
    }

    return 0;
}

int init_kbtns_interrupt(void) {
    int rv; 
    enable_gpio_interrupts(&kbtns_global);
    rv = request_threaded_irq(kbtns_global.irq_num, button_interrupt_handler, 0, IRQF_TRIGGER_RISING, 0, 0);
    return rv;
}

void free_kbtns_chrdev(void) {
    cdev_del(&kbtns_global.chrdev); 
    device_destroy(kbtns_class, kbtns_global.devno);

    class_destroy(kbtns_class);
    unregister_chrdev_region(kbtns_global.devno, 1);
    printk(KERN_INFO "[ Kbtns  ] - Finished cleaning up characted device\n");
}

/////////////////////////////
/* Module init/exit functions */
/////////////////////////////

static int zed_btns_init(void) {
    int err; 

    printk(KERN_INFO "Zed btns initializing...\n");

    INIT_KFIFO(kbtns_global.btns_buffer);
    mutex_init(&kbtns_global.buffer_lock);
    init_waitqueue_head(&kbtns_global.waitq);

    err = init_kbtns_chrdev();
    if (err) {
        printk(KERN_INFO "Error initializing button chrdev\n");
        return err; 
    }
    printk(KERN_INFO "[ KBtns  ] - Succesfully initialized btn chrdev\n");

    err = init_kbtns_gpio();
    if (err) {
        free_kbtns_chrdev();
        printk(KERN_INFO "Error when initializing buttons gpio...\n");
        return err; 
    }
    printk(KERN_INFO "[ KBtns  ] - Succesfully initialized btn gpio\n");

    err = init_kbtns_interrupt(); 
    if (err) {
        free_kbtns_chrdev();
        printk(KERN_INFO "Error when setting up button interrupt handler...\n");
        return err;
    }
    printk(KERN_INFO "[ KBtns  ] - Succesfully initialized btn interrupts\n");

    printk(KERN_INFO "Zed btns initialized successfully initialized!\n");    

    return 0;
}

static void zed_btns_exit(void) {
    printk(KERN_INFO "Zed btns exiting...\n");
    free_kbtns_chrdev();
    free_irq(kbtns_global.irq_num, 0);
    printk(KERN_INFO "Zed btns exited...\n");
}

module_init(zed_btns_init);
module_exit(zed_btns_exit);

MODULE_DESCRIPTION("This module records button presses");
MODULE_LICENSE("GPL");
