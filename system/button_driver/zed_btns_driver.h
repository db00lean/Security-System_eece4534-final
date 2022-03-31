#ifndef ZED_BTNS_DRIVER_H
#define ZED_BTNS_DRIVER_H

#include <linux/kfifo.h>
#include <linux/mutex.h>
#include <linux/wait.h>

#define MODULE_NAME "KBTNS_DRIVER"
#define ZEDBTNS_CHRDEV_REGION_NAME "zedbtns"
#define ZEDBTNS_CLASS_NAME "zedbtns_char"
#define ZEDBTNS_DEV_NAME "zedbtns0"
#define BTN_BASE_ADDR 0x41220000
#define BTN_BASE_GPIO_CHIP 901
#define BTN_MAX 5
#define BTN_BUF_SIZE 32

#define BTN_DATA_OFFEST (0x0)
#define BTN_IER_OFFSET (0x128)
#define BTN_GIER_OFFSET (0x11C)
#define BTN_ISR_OFFSET (0x120)

#define TO_U8(val) ((u8) (val & 0xFF))

struct kbtns_inst {
    // gpio members
    void __iomem *base; 
    int irq_num;
    
    // chardev members
    dev_t devno;
    struct cdev chrdev;
    DECLARE_KFIFO(btns_buffer, u8, BTN_BUF_SIZE);
    struct mutex buffer_lock;
    wait_queue_head_t waitq;
};

static inline u32 reg_read(struct kbtns_inst* inst, u32 reg) {
    return ioread32(inst->base + reg);
}

static inline void reg_write(struct kbtns_inst* inst, u32 reg, u32 value) {
    iowrite32(value, inst->base + reg);
}

static inline void clear_gpio_interrupt(struct kbtns_inst* inst) {
    reg_write(inst, BTN_ISR_OFFSET, 1); // isr is clear-on-write
}

static inline void enable_gpio_interrupts(struct kbtns_inst* inst) {
    reg_write(inst, BTN_GIER_OFFSET, 1 << 31);
    reg_write(inst, BTN_IER_OFFSET, 1);
}

#endif
