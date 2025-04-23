**BT4**. Counting Even and Odd Numbers with Threads

*Description*:
- Write a program to perform the following steps:
    1. Create a random array containing 100 integers from 1 to 100.
    2. Create two threads: one thread counts even numbers and one thread counts odd numbers in the array.
    3. When both threads complete the counting, print the total number of even numbers and the total number of odd numbers.
    4. Use `pthread_join` to ensure that the program only ends when both threads have completed their tasks.

*Hint*:
- Each thread will handle a different part of the work, so no mutex is needed in this exercise.

---

*Answer*:

**Why is no mutex needed?**

- Because each thread writes to its own separate variable (even_number_count or odd_number_count). There is no shared memory conflict, so thread synchronization is not required.