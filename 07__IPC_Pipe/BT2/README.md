**BT2**: Using Multiple Pipes for Communication Between Multiple Processes

*Description*:
- Extend the previous exercise to use multiple pipes for three processes.
    * Create two pipes: one for communication from parent process to child process 1, and another from child process 1 to child process 2.
    * The parent process sends a message to child process 1.
    * Child process 1 will receive the message, modify it (e.g., append a string), and send it to child process 2.
    * Child process 2 will receive the message and display it on the screen.

*Hint*:
- Use multiple pipes and ensure that processes are not blocked when reading and writing data.

---

*Answer*:
- The program demonstrates the use of 2 pipes for communication between 3 processes: the parent process, child process 1, and child process 2.
- The parent process sends `"This is message from parent..."` to child process 1.
- Child process 1 appends `"(modified by child 1)"` to the message and sends it to child process 2.
- Child process 2 receives the modified message and displays it on the screen.

<br>

- Below is the output of the program:

```bash
Child 1: my pid: 8374
Parent: sent message to child 1.
Child 1: received message from parent.
Child 2: my pid: 8375
pipe 1 end_of_pipe

Child 2: received message from child 1.
pipe 2 end_of_pipe

Child 2: displaying message:
        This is message from parent...(modified by child 1).

Parent: all children finished.
```
