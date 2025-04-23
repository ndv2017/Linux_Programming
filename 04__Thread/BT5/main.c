#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS_WRITE	2
#define NUM_THREADS_READ	5

int data_variable = 0;

pthread_rwlock_t rw_lock = PTHREAD_RWLOCK_INITIALIZER;

static void *thread_write_handler(void *args) {
	int thread_index = *((int *)args);

	pthread_rwlock_wrlock(&rw_lock);
	printf("Thread %d - Writer: writing...\n", thread_index + 1);
	++data_variable;

	pthread_rwlock_unlock(&rw_lock);

	return NULL;
}

static void *thread_read_handler(void *args) {
	int thread_index = *((int *)args);

	pthread_rwlock_rdlock(&rw_lock);
	printf("\tThread %d - Reader: reading...\n", thread_index + 1);
	printf("\tThread %d - Reader: data_variable = %d\n", thread_index + 1, data_variable);

	pthread_rwlock_unlock(&rw_lock);

	return NULL;
}

int main(void) {
	pthread_t thread_write_id[NUM_THREADS_WRITE];
	pthread_t thread_read_id[NUM_THREADS_READ];
	int thread_write_index[NUM_THREADS_WRITE] = {0, 1};
	int thread_read_index[NUM_THREADS_READ] = {0, 1, 2, 3, 4};

	for (int i = 0; i < NUM_THREADS_WRITE; i++) {
		if (pthread_create(&thread_write_id[i], NULL, thread_write_handler, (void *)&thread_write_index[i])) {
			perror("pthread_create() failed");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_THREADS_READ; i++) {
		if (pthread_create(&thread_read_id[i], NULL, thread_read_handler, (void *)&thread_read_index[i])) {
			perror("pthread_create() failed");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_THREADS_WRITE; i++) {
		if (pthread_join(thread_write_id[i], NULL)) {
			perror("pthread_join() failed");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_THREADS_READ; i++) {
		if (pthread_join(thread_read_id[i], NULL)) {
			perror("pthread_join() failed");
			exit(EXIT_FAILURE);
		}
	}

	pthread_rwlock_destroy(&rw_lock);
	printf("Final data_variable value: %d\n", data_variable);

	return 0;
}
