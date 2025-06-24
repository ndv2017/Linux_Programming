#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>      /* For platform devices */
#include <linux/gpio/consumer.h>        /* For GPIO Descriptor */
#include <linux/of.h>                   /* For DT*/

#define DRIVER_AUTHOR   "Vion vion@gmail.com"
#define DRIVER_DESC     "GPIO Descriptor_based Driver"
#define DRIVER_VERS     "1.0.0"

/*
*
* The below mapping in device tree - bcm2710-rpi-3-b.dts:
*
*   / {
*       mgpio: mgpio {
*           compatible = "gpio-descriptor-based";
*           led17-gpios = <&gpio 17 0>;
*           status = "okay";
*       };
*   };
*
*/

#define MEM_SIZE        1024
#define MAX_BUFF_USER   15

#define GPIO_SET        1
#define GPIO_RESET      0

static struct gpio_desc *gpio_17;

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio-descriptor-based", },
    { /* sentinel */ }
};

struct my_foo_dev {
    uint8_t *p_kernel_buffer;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev_des_based;

static int mgpio_descriptor_driver_probe(struct platform_device *pdev);
static int mgpio_descriptor_driver_remove(struct platform_device *pdev);
static int m_open(struct inode *inode, struct file *file);
static int m_release(struct inode *inode, struct file *file);
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t * offset);
static ssize_t m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset);

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
    if (copy_to_user(user_buf, mdev_des_based.p_kernel_buffer + *offset, to_read)) {
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

        // Set GPIO17 HIGH
        gpiod_direction_output(gpio_17, GPIO_SET);
        pr_info("Vion: %s - %d\n", __func__, __LINE__);
        pr_info("GPIO17 set to HIGH\n");
    }
    else if (strcmp(kbuf, "off") == 0) {
        pr_info("Turn my led off\n");

        // Set GPIO17 LOW
        gpiod_direction_output(gpio_17, GPIO_RESET);
        pr_info("Vion: %s - %d\n", __func__, __LINE__);
        pr_info("GPIO17 set to LOW\n");
    }
    else {
        pr_warn("Unknown command: %s\n", kbuf);
    }

    return size;
}

static int mgpio_descriptor_driver_probe(struct platform_device *pdev) {
    // Configure GPIO17 as output
    struct device *dev = &pdev->dev;
    gpio_17 = gpiod_get(dev, "led17", GPIOD_OUT_LOW);

    if (alloc_chrdev_region(&mdev_des_based.dev_num, 0, 1, "m_cdev") < 0) {
        printk(KERN_INFO "Major number allocation is failed\n");
        return -1;
    }

    pr_info("Major = %d & Minor = %d", MAJOR(mdev_des_based.dev_num), MINOR(mdev_des_based.dev_num));

    cdev_init(&mdev_des_based.m_cdev, &fops);

    if ((cdev_add(&mdev_des_based.m_cdev, mdev_des_based.dev_num, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_num;
    }

    if ((mdev_des_based.m_class = class_create(THIS_MODULE, "m_class")) == NULL) {
        printk(KERN_INFO "Creating class failed\n");
        goto rm_device_num;
    }

    if ((device_create(mdev_des_based.m_class, NULL, mdev_des_based.dev_num, NULL, "m_device")) == NULL) {
        printk(KERN_INFO "Creating device failed\n");
        goto rm_class;
    }

    if ((mdev_des_based.p_kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL)) == 0) {
        printk(KERN_INFO "Cannot allocate memory in kernel\n");
        goto rm_device;
    }

    printk(KERN_INFO "GPIO Descriptor_based module hello\n");
    return 0;

rm_device:
    device_destroy(mdev_des_based.m_class, mdev_des_based.dev_num);
rm_class:
    class_destroy(mdev_des_based.m_class);
rm_device_num:
    unregister_chrdev_region(mdev_des_based.dev_num, 1);
    return -1;
}

static int mgpio_descriptor_driver_remove(struct platform_device *pdev) {
    // Set GPIO17 to LOW before exiting
    gpiod_direction_output(gpio_17, GPIO_RESET);
    gpiod_put(gpio_17);

    kfree(mdev_des_based.p_kernel_buffer);
    device_destroy(mdev_des_based.m_class, mdev_des_based.dev_num);
    class_destroy(mdev_des_based.m_class);
    unregister_chrdev_region(mdev_des_based.dev_num, 1);

    printk(KERN_INFO "Goodbye from GPIO Descriptor_based module\n");
    return 0;
}

static struct platform_driver mpdriver_gpio = {
    .probe = mgpio_descriptor_driver_probe,
    .remove = mgpio_descriptor_driver_remove,
    .driver = {
        .name = "gpio-descriptor-based",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};

module_platform_driver(mpdriver_gpio);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
