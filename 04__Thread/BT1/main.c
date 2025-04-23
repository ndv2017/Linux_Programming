#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t thread1_id, thread2_id;

static void *thread_handler(void *args) {
	pthread_t thread_id_check = pthread_self();
	char *message = (char *)args;

	if (pthread_equal(thread_id_check, thread1_id)) {
		printf("Thread 1: my ID: %ld\n", thread_id_check);
		printf("Thread 1: message: %s\n", message);
	} else if (pthread_equal(thread_id_check, thread2_id)) {
		printf("Thread 2: my ID: %ld\n", thread_id_check);
		printf("Thread 2: message: %s\n", message);
	} else {
		printf("Error: pthread_equal failed\n");
	}
}

int main(void) {
	int ret;
	char *thread1_message = "Hello from thread 1";
	char *thread2_message = "Hello from thread 2";

	ret = pthread_create(&thread1_id, NULL, thread_handler, (void *)thread1_message)
	if (ret) {
		perror("pthread_create() failed");
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&thread2_id, NULL, thread_handler, (void *)thread2_message)
	if (ret) {
		perror("pthread_create() failed");
		exit(EXIT_FAILURE);
	}

	pthread_join(thread1_id, NULL);
	pthread_join(thread2_id, NULL);

	return 0;
}
