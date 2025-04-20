**BT3**. Process Signals

*Description*:
- Write a program that creates two processes, parent and child. The parent process will send a `SIGUSR1` signal to the child process after a period of time.

*Requirements*:
- Use `signal()` to define the behavior of the child process when receiving the `SIGUSR1` signal.
- Print a message when the child process receives the signal.
