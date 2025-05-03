#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#define	MAX_COUNT	10

static int counter_var = 0;
static volatile sig_atomic_t sig_flag = false;

static void signal_handler(int sig_num) {
	sig_flag = true;
	printf("Received signal %d (%s)\n", sig_num, strsignal(sig_num));
}

int main(void) {
	if (signal(SIGALRM, signal_handler) == SIG_ERR) {
		perror("signal() failed");
		exit(EXIT_FAILURE);
	}

	alarm(1);

	while (1) {
		if (sig_flag) {
			sig_flag = false;
			alarm(1);
			printf("\tTimer: %d seconds\n", ++counter_var);
		}

		if (counter_var >= MAX_COUNT)	break;
	}

	printf("Exiting program...\n");

	return 0;
}
