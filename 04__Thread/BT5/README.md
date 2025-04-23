**BT5**. Resource Sharing With Read-Write Lock

*Description*:
- Write a program that simulates a situation with multiple reading and writing threads:
    1. Create a data variable containing an integer.
    2. Create 5 reader threads and 2 writer threads:
        - Reader threads will only read data without changing it.
        - Writer threads will increase the value of data by 1.
    3. Use a *read-write lock* to ensure multiple threads can read simultaneously, but only one thread can write at any given time.
    4. Print the value of data after all threads have completed.

*Hint*:
- Use `pthread_rwlock_rdlock` and `pthread_rwlock_wrlock` for read-write synchronization.

---

*More more information*:

- If no lock is used, readers might read inconsistent or partial updates from writers.
- If regular mutex is used, readers are forced to wait even when no data modification is happening (less efficient).
- Here is an example output:

```bash
Thread 1 - Writer: writing...
        Thread 1 - Reader: reading...
        Thread 1 - Reader: data_variable = 1
Thread 2 - Writer: writing...
        Thread 3 - Reader: reading...
        Thread 3 - Reader: data_variable = 2
        Thread 2 - Reader: reading...
        Thread 2 - Reader: data_variable = 2
        Thread 5 - Reader: reading...
        Thread 5 - Reader: data_variable = 2
        Thread 4 - Reader: reading...
        Thread 4 - Reader: data_variable = 2
Final data_variable value: 2
```
