**BT3**. Inter-Process Communication Using Signals

*Description*:
- Write a program that creates two processes (parent and child) and uses the SIGUSR1 signal for communication between them.
    1. Use `fork()` to create a child process from the parent process.
    2. The parent process will send `SIGUSR1` signal to the child process every 2 seconds.
    3. When receiving `SIGUSR1` signal, the child process will print `"Received signal from parent"`.
    4. Stop the program after sending the signal 5 times.

*Hint*:
- Use `kill()` to send signals from the parent process to the child process, and use `signal()` in the child process to catch `SIGUSR1`.

*Question*:
- Why do we need to use `kill()` to send signals in this exercise?

---

*Answer*:
- In this exercise, `kill()` is used by the parent process to send the `SIGUSR1` signal to the child process. It allows one process to deliver a specific signal to another using its PID. This is essential for IPC, where processes are independent and do not share memory or control flow. Without `kill()`, the parent has no way to notify the child that an event occurred.

<br>

- Here's the example output:
```bash
Child: my pid: 5317
Parent: my pid: 5316
Received signal 10 (User defined signal 1)
Child: Received signal from parent
        Signal count: (1/5)
Received signal 10 (User defined signal 1)
Child: Received signal from parent
        Signal count: (2/5)
Received signal 10 (User defined signal 1)
Child: Received signal from parent
        Signal count: (3/5)
Received signal 10 (User defined signal 1)
Child: Received signal from parent
        Signal count: (4/5)
Received signal 10 (User defined signal 1)
Child: Received signal from parent
        Signal count: (5/5)
Parent: child exited with status = 0
```
