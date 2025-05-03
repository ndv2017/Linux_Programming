**BT5**. Program Waiting for Signals While Accepting User Input

*Description*:
- Write a program that can receive signals while simultaneously allowing users to input from the keyboard.
    * Use `select()` or `poll()` to allow the program to handle signals without interrupting keyboard input.
    * When receiving SIGINT, print `"SIGINT received."`
    * If receiving SIGTERM, exit the program.

*Hint*:
- Combine signal handling and input reading to make the program more flexible.

---

*Answer*:
- Below is the output of the program.
- First we send a `SIGINT` signal using `Ctrl+C`, then type `hi` and press `Enter`. After timeout, we type `hello` and press `Enter`. Finally, we send a `SIGTERM` signal using `kill -15 <PID>` to terminate the program.

```bash
Program started! Type something or send SIGINT/SIGTERM...
^C
Received signal 2 (Interrupt)
        SIGINT received.
hi
        You typed: hi
Timeout occurred after 10 seconds.
hello
        You typed: hello

Received signal 15 (Terminated)
        Exiting program due to SIGTERM.
```
