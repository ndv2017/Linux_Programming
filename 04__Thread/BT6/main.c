#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4
#define ARRAY_ELEMENTS 1000000
#define ARRAY_ELEMENTS_PER_THREAD (ARRAY_ELEMENTS / NUM_THREADS)

int global_variable_array[ARRAY_ELEMENTS];
int final_sum_array = 0;
int total_count = 0;

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

int random_from_1_to_10(void) {
	return rand() % 10 + 1;
}

static void *thread_handler(void *args) {
	int thread_index = *((int *)args);
	int local_sum = 0;
	int local_count = 0;

	for (int i = thread_index * ARRAY_ELEMENTS_PER_THREAD; i < (thread_index + 1) * ARRAY_ELEMENTS_PER_THREAD; i++) {
		local_sum += global_variable_array[i];
		local_count++;
	}

	pthread_mutex_lock(&mutex_lock);
	final_sum_array += local_sum;
	total_count += local_count;
	pthread_mutex_unlock(&mutex_lock);

	return NULL;
}

int main(void) {
	srand(time(NULL));

	pthread_t thread_id[NUM_THREADS];
	int thread_index[NUM_THREADS] = {0, 1, 2, 3};

	for (int i = 0; i < ARRAY_ELEMENTS; i++) {
		global_variable_array[i] = random_from_1_to_10();

		// uncomment the following line to see the generated values
		// printf("%d ", global_variable_array[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_create(&thread_id[i], NULL, thread_handler, (void *)&thread_index[i])) {
			perror("pthread_create() failed");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(thread_id[i], NULL)) {
			perror("pthread_join() failed");
			exit(EXIT_FAILURE);
		}
	}

	printf("\nFinal sum of all elements of global_variable_array: %d\n", final_sum_array);
	printf("Total elements counted: %d\n", total_count);

	return 0;
}
