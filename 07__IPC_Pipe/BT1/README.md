**BT1**: Using Pipes for Parent-Child Process Communication

*Description*:
- Write a program that uses pipes for communication between two processes.
    * Create a pipe and fork a child process.
    * The parent process will send a text string into the pipe.
    * The child process will read the text string from the pipe and display it on the screen.

*Hint*:
- Use the `pipe()` function to create a pipe, `fork()` to create a child process, and `read()`/`write()` to communicate between processes.

---

*Answer*:
- The parent process sends 2 messages to the child process through the pipe.
- The child process reads the messages from the pipe and displays them on the screen.

<br>

- Below is the output of the program:

```bash
Parent: my pid: 8220
Parent: write 40 bytes to pipe.
Parent: write 40 bytes to pipe.
Child: my pid: 8221
Child: message: 
        Hello, this is an arbitrary message...
Child: message: 
        I am Viet

pipe end_of_pipe
```
