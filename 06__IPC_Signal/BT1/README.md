**BT1**. Handling SIGINT Signal

*Description*:
- Write a program that catches the SIGINT signal (sent when `Ctrl+C` is pressed) and prints a message when this signal is received.
    1. Use the `signal()` function to register a handler function for the SIGINT signal.
    2. In the handler function, print the message `"SIGINT received"` each time a SIGINT signal is received.
    3. The program should continue running until it receives SIGINT for the third time, then terminate.

*Hint*:
- Use a global counter variable to count the number of times the SIGINT signal is received and check this variable in the handler function.

*Question*:
- If we ignore the SIGINT signal, what will happen when `Ctrl+C` is pressed?

---

*Answer*:
- If we ignore the SIGINT signal, pressing `Ctrl+C` will not terminate the program. Instead, the program will continue running without any interruption. This is because the default behavior of the SIGINT signal is to terminate the process, but if we set a custom handler that ignores it, the process will not respond to the signal.
- Here's the output of the program:
```bash
^C
Received signal 2 (Interrupt)
        Count variable = 1
^C
Received signal 2 (Interrupt)
        Count variable = 2
^C
Received signal 2 (Interrupt)
        Count variable = 3
Exiting program...
```
