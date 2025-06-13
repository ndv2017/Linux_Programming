**BT9**. Create a Character Device Driver
*Description*:
- Create a **character device driver** (*kernel space*) that performs reading and writing data to the device file corresponding to that **character device driver**. Specifically:
    - Writing `on` to the device file will display the message `Turn my led on` in the kernel log.
    - Writing `off` to the device file will display the message `Turn my led off` in the kernel log.

---

*Answer*:
- Successfully created a character device driver that allows writing to a device file, which in turn logs messages to the kernel log.
- The character device driver is implemented in the file [first_driver.c](./first_driver.c).
- There's also a test program [test_app.c](./test_app.c) to interact with the device file. The app gives the user three options:
    1. Write to the device file
    2. Read from the device file
    3. Exit application

- Here's the result of running the test program:
![Message from user to kernel](./result/result.png)
