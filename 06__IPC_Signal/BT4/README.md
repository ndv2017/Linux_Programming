**BT4**. Write a Program That Ignores SIGTSTP Signal

*Description*:
- Write a program that can ignore the SIGTSTP signal (typically `Ctrl+Z`).
    1. Use `signal()` or `sigaction()` to catch and ignore the SIGTSTP signal.
    2. When receiving the SIGTSTP signal, the program will not stop but continue running and print "SIGTSTP ignored".

*Hint*:
- Ensure the program continues running instead of being suspended when `Ctrl+Z` is pressed.

*Question*:
- What happens if the SIGTSTP signal is not handled and the user presses `Ctrl+Z`?

---

*Answer*:
- If SIGTSTP is not explicitly handled or ignored, pressing `Ctrl+Z` will cause the operating system to suspend the process by default. The shell will place the program into the background in a "stopped" state. The user can later resume it with the `fg` or `bg` command.

<br>

- Here's the output of the program:
```bash
Running... press Ctrl+Z (SIGTSTP) to test ignoring.
^Z
Received signal 20 (Stopped)
        SIGTSTP ignored!
Signal info:
        Signal number: 20
        Sender PID: 0
        Sender UID: 0
        Signal code: 128

Received signal 20 (Stopped)
        SIGTSTP ignored!
Signal info:
        Signal number: 20
        Sender PID: 4991
        Sender UID: 1000
        Signal code: 0
```

- Note that this running program receives the SIGTSTP signal twice. The first time it is sent by the terminal (`PID 0`), and the second time it is sent by the user (`PID 4991`).
