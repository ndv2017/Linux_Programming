**BT6**. Calculating Array Sum with Threads and Mutex

*Description*:
- Write a program to calculate the sum of a large array of 1 million integers.
    1. Divide the array into 4 equal parts.
    2. Create 4 threads, each thread calculates the sum of one part of the array.
    3. Use a global sum variable and mutex to combine results from all threads.
    4. Print the total sum of the array after the threads have completed.

*Hint*:
- Use `pthread_mutex_lock` to protect the sum variable when threads add their results.

---

*For more information*:

- Here's an example output:

```bash
Final sum of all elements of global_variable_array: 5503125
Total elements counted: 1000000
```
