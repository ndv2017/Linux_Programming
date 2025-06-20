#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>

#define DRIVER_AUTHOR   "Vion vion@gmail.com"
#define DRIVER_DESC     "GPIO Legacy Driver"
#define DRIVER_VERS     "1.0.0"

#define GPIO_NUMBER_27          27

#define BCM2837_GPIO_BASE_ADDR  0x20200000
#define GPIO_FSEL_OFFSET        0x00
#define GPIO_SET_0_OFFSET       0x1C
#define GPIO_CLR_0_OFFSET       0x28

#define MEM_SIZE                1024
#define MAX_BUFF_USER           15

static void __iomem *gpio_base;

struct my_foo_dev {
    uint8_t *p_kernel_buffer;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev;

static int      __init first_driver_init(void);
static void     __exit first_driver_exit(void);
static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size, loff_t * offset);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset);

static struct file_operations fops = {
    .owner      = THIS_MODULE,
    .read       = m_read,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release,
};

static int m_open(struct inode *inode, struct file *file) {
    pr_info("System call open() called...!!!\n");
    return 0;
}

static int m_release(struct inode *inode, struct file *file) {
    pr_info("System call close() called...!!!\n");
    return 0;
}

static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset) {
    size_t to_read;

    pr_info("System call read() called...!!!\n");

    if (*offset >= MEM_SIZE)
        return 0;

    to_read = ((MEM_SIZE - *offset) > size) ? size : ((size_t)(MEM_SIZE - *offset));
    if (copy_to_user(user_buf, mdev.p_kernel_buffer + *offset, to_read)) {
        pr_err("Failed to copy data to user\n");
        return -EFAULT;
    }

    *offset += to_read;
    printk(KERN_INFO "Data Read: Done!\n");
    
    return to_read;
}

static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset) {
    char kbuf[MAX_BUFF_USER] = {0};
    size_t len = (size < MAX_BUFF_USER - 1) ? size : (MAX_BUFF_USER - 1);
    uint32_t reg;

    pr_info("System call write() called...!!!\n");

    if (copy_from_user(kbuf, user_buf, len)) {
        pr_err("Failed to copy data from user\n");
        return -EFAULT;
    }

    kbuf[len] = '\0';

    if (kbuf[len - 1] == '\n')
        kbuf[len - 1] = '\0';

    pr_info("Received from user: %s\n", kbuf);

    if (strcmp(kbuf, "on") == 0) {
        pr_info("Turn my led on\n");

        // Map GPIO registers into kernel virtual address space
        gpio_base = ioremap(BCM2837_GPIO_BASE_ADDR, 0x28);
        if (!gpio_base) {
            pr_err("Failed to ioremap GPIO\n");
            return -ENOMEM;
        }

        // Configure GPIO27 as output
        reg = ioread32(gpio_base + GPIO_FSEL_OFFSET + ((GPIO_NUMBER_27 / 10) * 4));
        reg &= ~(7 << ((GPIO_NUMBER_27 % 10) * 3)); // Clear current function
        reg |= (1 << ((GPIO_NUMBER_27 % 10) * 3));  // Set as output
        iowrite32(reg, gpio_base + GPIO_FSEL_OFFSET + ((GPIO_NUMBER_27 / 10) * 4));

        // Set GPIO27 HIGH
        iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_SET_0_OFFSET);
        pr_info("GPIO27 set to HIGH\n");
    }
    else if (strcmp(kbuf, "off") == 0) {
        pr_info("Turn my led off\n");

        // Set GPIO27 to LOW before exiting
        iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_CLR_0_OFFSET);
        pr_info("GPIO27 set to LOW\n");

        // Unmap GPIO memory region
        if (gpio_base)
            iounmap(gpio_base);
    }
    else {
        pr_warn("Unknown command: %s\n", kbuf);
    }

    return size;
}

static int  __init first_driver_init(void) {
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "m_cdev") < 0) {
        printk(KERN_INFO "Major number allocation is failed\n");
        return -1;
    }

    pr_info("Major = %d & Minor = %d", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    cdev_init(&mdev.m_cdev, &fops);

    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_num;
    }

    if ((mdev.m_class = class_create(THIS_MODULE, "m_class")) == NULL) {
        printk(KERN_INFO "Creating class failed\n");
        goto rm_device_num;
    }

    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_device")) == NULL) {
        printk(KERN_INFO "Creating device failed\n");
        goto rm_class;
    }

    if ((mdev.p_kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL)) == 0) {
        printk(KERN_INFO "Cannot allocate memory in kernel\n");
        goto rm_device;
    }

    printk(KERN_INFO "GPIO Legacy module hello\n");
    return 0;

rm_device:
    device_destroy(mdev.m_class, mdev.dev_num);
rm_class:
    class_destroy(mdev.m_class);
rm_device_num:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

static void  __exit first_driver_exit(void) {
    kfree(mdev.p_kernel_buffer);
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    unregister_chrdev_region(mdev.dev_num, 1);

    printk(KERN_INFO "Goodbye from GPIO Legacy module\n");
}

module_init(first_driver_init);
module_exit(first_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);  
MODULE_VERSION(DRIVER_VERS);
