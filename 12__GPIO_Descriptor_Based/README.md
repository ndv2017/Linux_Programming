**BT12**. Control a GPIO device using a character device driver - The descriptor-based interface

*Description*:
- This example demonstrates how to control a GPIO device using a character device driver in the Linux kernel using the descriptor-based interface.
    - Writing `on` to the device file will display the message `Turn my led on` in the kernel log, and turn on the LED connected to the GPIO pin 17.
    - Writing `off` to the device file will display the message `Turn my led off` in the kernel log, and turn off the LED connected to the GPIO pin 17.
