**BT4**. Monitoring Process Status

*Description*:
- Write a program that creates a child process, then the parent process uses `wait()` to wait for the child process to end and record its exit status.

*Requirements*:
- In the child process, choose the exit status by calling `exit()` with different values.
- Use `WIFEXITED()` and `WEXITSTATUS()` to confirm and record the exit code of the child process.
