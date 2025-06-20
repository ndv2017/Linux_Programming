**BT10**. Control a GPIO device using a character device driver

*Description*:
- This example demonstrates how to control a GPIO device using a character device driver in the Linux kernel.
    - Writing `on` to the device file will display the message `Turn my led on` in the kernel log, and turn on the LED connected to the GPIO pin 27.
    - Writing `off` to the device file will display the message `Turn my led off` in the kernel log, and turn off the LED connected to the GPIO pin 27.
