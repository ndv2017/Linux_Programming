**BT3**. Using Condition Variables

*Description*:
- Write a program with two threads: *producer* and *consumer* as follows:
    1. Producer: Generates a random number from 1 to 10 and stores it in a global variable data.
    2. Consumer: Waits for the producer to provide data, then reads and prints that data.
    3. Use `pthread_cond_wait` and `pthread_cond_signal` to synchronize between producer and consumer, ensuring that the consumer only reads data when the producer has finished providing it.
    4. Repeat this process 10 times and print all the values read by the consumer.

*Hint*:
- Use `pthread_cond_wait` to make the consumer wait until there is a signal from the producer that data is ready.
