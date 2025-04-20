**BT2**. Using `exec` and Environment Variables

*Description*: 
- Write a program where the child process replaces the running program with another program, such as `ls` or `date`, using the `execlp()` or `execvp()` function.

*Requirements*:
- Pass environment variables to the program and perform different tasks (for example, if the environment variable passed is `1`, execute the `ls` command; if it's `2`, execute the `date` command).
- Explain what happens to the original process after `exec` is called.

---

*Answer*:

**What happens after `execvp()` is called**:

When `execvp()` is successfully called in the child process, the current process image is **replaced** by the new program (like `ls` or `date`). This means the original program’s code is no longer running — only the new program exists in that process. As a result:

- The child process becomes the new program.
- Any code **after `execvp()` in the child will not run** (unless `execvp()` fails).
- The parent process is unaffected and continues its execution normally.

This makes it a powerful tool for launching new programs in child processes.