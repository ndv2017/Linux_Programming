#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/* shared resource */
int counter_var = 0;

pthread_mutex_t mutex_lock;

static void counter_increment(int num_of_times) {
	pthread_mutex_lock(&mutex_lock);

	while (num_of_times--) { counter_var++; }

	pthread_mutex_unlock(&mutex_lock);
}

static void *thread_handler(void *args) {
	int thread_index = *((int *)args);

	printf("Thread %d: my ID: %ld\n", thread_index + 1, pthread_self());
	printf("Thread %d: counter incrementing...\n", thread_index + 1);
	counter_increment(1000000);

	return NULL;
}

int main(void) {
	pthread_t thread_id[3];
	int thread_index[3] = {0, 1, 2};

	if (pthread_mutex_init(&mutex_lock, NULL)) {
		perror("pthread_mutex_init() failed");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 3; i++) {
		if (pthread_create(&thread_id[i], NULL, thread_handler, (void *)&thread_index[i])) {
			perror("pthread_create() failed");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < 3; i++) {
		if (pthread_join(thread_id[i], NULL)) {
			perror("pthread_join() failed");
			exit(EXIT_FAILURE);
		}
	}

	printf("Final counter value: %d\n", counter_var);

	return 0;
}