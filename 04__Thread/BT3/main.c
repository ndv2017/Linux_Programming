#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 2

int global_variable = 0;							// shared resource
int num_of_times = 10;								// number of times to write and read

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

int random_from_1_to_10(void) {
	return rand() % 10 + 1;
}

static void *thread_handler(void *args) {
	int thread_index = *((int *)args);

	if (thread_index == 0) {						// Producer
		while (num_of_times--) {
			pthread_mutex_lock(&mutex_lock);

			printf("Count: %d\n", 10 - num_of_times);
			printf("\tThread %d - Producer: generating value and assigning to global_variable...\n", \
					thread_index + 1);

			global_variable = random_from_1_to_10();

			pthread_cond_signal(&cond_var);
			pthread_mutex_unlock(&mutex_lock);

			sleep(1);								// Let the consumer thread run
		}
	} else if (thread_index == 1) {					// Consumer
		pthread_mutex_lock(&mutex_lock);

		while (num_of_times) {
			pthread_cond_wait(&cond_var, &mutex_lock);
			printf("\tThread %d - Consumer: global_variable = %d\n", \
					thread_index + 1, global_variable);
		}

		pthread_mutex_unlock(&mutex_lock);
	}

	return NULL;
}

int main(void) {
	srand(time(NULL));

	pthread_t thread_id[NUM_THREADS];
	int thread_index[NUM_THREADS] = {0, 1};

	for (int i = NUM_THREADS - 1; i >= 0; i--) {
		if (pthread_create(&thread_id[i], NULL, thread_handler, (void *)&thread_index[i])) {
			perror("pthread_create() failed");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = NUM_THREADS - 1; i >= 0; i--) {
		if (pthread_join(thread_id[i], NULL)) {
			perror("pthread_join() failed");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
