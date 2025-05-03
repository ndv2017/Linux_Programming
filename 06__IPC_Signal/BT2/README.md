**BT2**. Using SIGALRM Signal to Create a Timer

*Description*:
- Write a program that creates a timer using the SIGALRM signal.
    1. Use the `alarm()` function to set SIGALRM to trigger every second.
    2. When receiving the SIGALRM signal, the program should increment a counter and print "Timer: \<seconds> seconds".
    3. Stop the program after counting to 10 seconds.

*Hint*:
- Use `signal()` function to register a handler for SIGALRM, and set `alarm(1)` to repeat the timer every second.

*Question*:
- What happens if `alarm(1)` is not called again within the handler function?

---

*Answer*:
- In this program, `alarm(1)` is called repeatedly to reset the timer and ensure that a `SIGALRM` signal is delivered every second.  
- If `alarm(1)` is not called again (either in the loop or within the signal handler), the timer will not restart, and only a single `SIGALRM` will be delivered.  
- Since `alarm()` is a one-shot timer by default, re-calling it is essential to create a recurring timer.

<br>

- Here's the output:
```bash
Received signal 14 (Alarm clock)
        Timer: 1 seconds
Received signal 14 (Alarm clock)
        Timer: 2 seconds
Received signal 14 (Alarm clock)
        Timer: 3 seconds
Received signal 14 (Alarm clock)
        Timer: 4 seconds
Received signal 14 (Alarm clock)
        Timer: 5 seconds
Received signal 14 (Alarm clock)
        Timer: 6 seconds
Received signal 14 (Alarm clock)
        Timer: 7 seconds
Received signal 14 (Alarm clock)
        Timer: 8 seconds
Received signal 14 (Alarm clock)
        Timer: 9 seconds
Received signal 14 (Alarm clock)
        Timer: 10 seconds
Exiting program...
```
