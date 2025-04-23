**BT1**. Creating and Running Basic Threads

*Description*:
- Create a program using the `pthread.h` library to perform the following operations:
    1. Create two threads. Each thread will print a message string along with the thread ID (e.g., "Thread 1: Hello from thread").
    2. Use the `pthread_join` function to wait for both threads to complete before the program ends.
    3. How do you use `pthread_create` and `pthread_join`? When does a thread terminate?

*Hint*: 
- Use the `pthread_create` function to create threads and `pthread_join` to wait for threads to complete.

---

*Answer*:
- `pthread_create(&thread_id, NULL, function, arg)` creates a new thread and runs `function(arg)` (`thread_handler(args)`) in parallel.
- `pthread_join(thread_id, NULL)` waits for the specified thread to finish before continuing.
- A thread **terminates** when:
    - It returns from the function
    - It calls `pthread_exit()`
    - The entire process exits
