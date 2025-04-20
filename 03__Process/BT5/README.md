**BT5**. Creating Zombie and Orphan Processes

*Description*:
- Write a program that creates a zombie process and an orphan process to understand these special types of processes.

*Requirements*:
- To create a zombie, have the child process end early while the parent process is still running, and use `ps` to observe the zombie status of the process.
- To create an orphan, have the child process sleep for a longer period than the parent process, so the child process becomes an orphan.
- Explain zombie and orphan states, and the reasons why they appear in Linux.

---

*Answer*:

To demonstrate zombie and orphan processes, two separate programs were created: [zombie_creation.c](./zombie_creation.c) and [orphan_creation.c](./orphan_creation.c)

- A **Zombie Process** is a process that has finished executing (exited) but still has an entry in the process table because its parent has not yet read its exit status using `wait()` or `waitpid()`. It remains until the parent acknowledges the child’s termination. These are harmless in small numbers but can cause problems if many accumulate.

- An **Orphan Process** is a process whose parent has exited before it, leaving it without a parent. The operating system automatically reassigns the orphan to the `init` (PID 1) process, which eventually waits for its termination and cleans it up. This behavior ensures that all processes have a valid parent in Linux.

<br>

*Why They Appear in Linux*:

- **Zombie processes** exist so the parent can retrieve the child’s exit status. They are kept temporarily in the process table until the parent calls `wait()`.
- **Orphan processes** happen when a parent exits first. Linux reassigns them to `init` (PID 1) to maintain a clean and organized process tree.

This mechanism ensures that resources are managed safely, and no process is left unmanaged in the system.
