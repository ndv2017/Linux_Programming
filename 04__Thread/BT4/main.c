#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 2

int random_arr[100];
int even_number_count = 0;
int odd_number_count = 0;

int random_from_1_to_100(void) {
	return rand() % 100 + 1;
}

static void *thread_handler(void *args) {
	int thread_index = *((int *)args);

	if (thread_index == 0) {
		for (int i = 0; i < 100; i++) {
			if (random_arr[i] % 2 == 0)	even_number_count++;
		}
	} else if (thread_index == 1) {
		for (int i = 0; i < 100; i++) {
			if (random_arr[i] % 2 != 0)	odd_number_count++;
		}
	}

	return NULL;
}

int main(void) {
	srand(time(NULL));

	pthread_t thread_id[NUM_THREADS];
	int thread_index[NUM_THREADS] = {0, 1};

	for (int i = 0; i < 100; i++) {
		random_arr[i] = random_from_1_to_100();
		printf("%d ", random_arr[i]);
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

	printf("\nEven numbers count: %d\n", even_number_count);
	printf("Odd numbers count: %d\n", odd_number_count);
	printf("Total numbers counted: %d\n", even_number_count + odd_number_count);

	return 0;
}
