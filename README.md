# Linux Programming Assignments

These assignments are part of my learning journey in Linux system programming.

## Repository Structure

### [01__General_Knowledge](01__General_Knowledge/)
Create static lib and shared lib using Makefile

### [02__File_System](02__File_System/)
File operations, including reading, writing, and retrieving file information.

### [03__Process](03__Process/)
Process creation, management, and handling using system calls like `fork()`.

### [04__Thread](04__Thread/)
Multi-threading assignments demonstrating thread creation, synchronization, and shared resource handling using Mutexes, Condition Variables, and Read-Write Locks.

### [05__IPC_Socket](05__IPC_Socket/)
Inter-Process Communication (IPC) using sockets. This includes both IPv4 and Unix domain sockets for stream and datagram communication.

### [06__IPC_Signal](06__IPC_Signal/)
Inter-Process Communication (IPC) using signals. This includes signal handling, ignoring signals, and using signals for inter-process communication.

### [07__IPC_Pipe](07__IPC_Pipe/)
Inter-Process Communication (IPC) using pipes. This includes unnamed pipes for parent-child communication and named pipes (FIFOs) for communication between unrelated processes.

### [08__Chat_Application](08__Chat_Application/)
A command-line peer-to-peer chat application built using TCP sockets and non-blocking I/O with `select()`. It supports multiple simultaneous connections, message exchange, and command-driven interaction. The application is modularized with a shared library and demonstrates advanced socket programming, signal handling, and dynamic linking.

---

### [Yocto_Project](Yocto_Project/)
A project to build a custom Linux distribution using the Yocto Project for the BeagleBone Black.

---

### [09__First_Kernel_Module](09__First_Kernel_Module/)
Create a character device driver that allows reading and writing to a device file, logging messages to the kernel log based on user input.

### [10__GPIO_Legacy](10__GPIO_Legacy/)
Control a GPIO device using a character device driver.

## References
- The Linux Programming Interface - Michael Kerrisk
- [Linux man pages online](https://man7.org/linux/man-pages/)
