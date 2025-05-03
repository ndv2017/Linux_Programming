#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

int counter_var = 0;
static volatile sig_atomic_t sig_flag = false;

static void signal_handler(int sig_num) {
	sig_flag = true;
	printf("\nSIGINT received! Signal number: %d\n", sig_num);
}

int main(void) {
	if (signal(SIGINT, signal_handler) == SIG_ERR) {
		perror("signal() failed");
		exit(EXIT_FAILURE);
	}

	while (counter_var < 3) {
		if (sig_flag) {
			sig_flag = false;
			printf("\tCount variable = %d\n", ++counter_var);
		}
	}

	printf("Exiting program...\n");

	return 0;
}
