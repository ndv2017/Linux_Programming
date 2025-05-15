**BT3**: Counting Characters Through a Pipe

*Description*:
- Write a program that uses a pipe to count the number of characters in a string.
    * Create a pipe and fork a child process.
    * The parent process will send a character string into the pipe.

---

*Answer*:
- The parent process sends a string `"Hello, this is an arbitrary message..."` to the child process through the pipe.
- The child process reads the string from the pipe and counts the number of characters in it using the `strlen()` function.
- The child process then displays the string and the number of characters on the screen.

<br>

- **Note**: in child process, there's no need to check end-of-file (EOF) condition when reading from the pipe, as the parent process will close the pipe after writing the string.
- Below is the output of the program:

```bash
Parent: my pid: 8573
Child: my pid: 8574
Child: string message: 
Parent: write 39 bytes to pipe.
        Hello, this is an arbitrary message...
Child: number of characters in string: 
        38
```
