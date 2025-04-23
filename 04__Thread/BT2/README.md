**BT2**. Thread Synchronization with Mutex

*Description*:
- Write a program using *pthread* and *mutex* to perform the following operations:
    1. Create a global variable counter initialized to 0.
    2. Create three threads, each thread increases the value of the counter by 1, 1,000,000 times.
    3. Use a mutex to ensure that counter incrementation is performed safely.
    4. Print the final value of the counter when all threads have completed.
    5. Question: Why is a mutex necessary in this exercise? What happens if the mutex is removed?

*Hint*:
- Use `pthread_mutex_lock` and `pthread_mutex_unlock` to lock and unlock the mutex when accessing the counter.

---

*Answer*:

**Why is a mutex necessary?**

- In a multithreaded environment, multiple threads may try to read and update a shared variable (like `counter_var`) at the same time. Without a mutex, these operations can overlap and cause race conditions, leading to incorrect results (e.g. final count < 3,000,000).

**What happens if the mutex is removed?**

- The final counter value may vary across runs and will likely be less than expected. This happens because the `counter++` operation is not atomic, and threads can overwrite each other’s changes unpredictably.
- Here is an example output when mutex isnt being used:
```bash
Thread 1: my ID: 140132674406144
Thread 3: my ID: 140132657620736
Thread 3: counter incrementing...
Thread 1: counter incrementing...
Thread 2: my ID: 140132666013440
Thread 2: counter incrementing...
Final counter value: 1024488
```