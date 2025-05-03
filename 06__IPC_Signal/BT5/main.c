/* This macro ensures that the program adheres to the POSIX.1-2008 standard.
   It enables access to POSIX-specific features, such as sigaction(), siginfo_t, 
   and SA_SIGINFO, which are used in this program for signal handling. */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE	256
#define MAX_TIMEOUT		10

volatile sig_atomic_t sigint_flag = false;
volatile sig_atomic_t sigterm_flag = false;

void sigint_Handler(int sig_num) {
	sigint_flag = true;
	printf("\nReceived signal %d (%s)\n", sig_num, strsignal(sig_num));
}

void sigterm_Handler(int sig_num) {
	sigterm_flag = true;
	printf("\nReceived signal %d (%s)\n", sig_num, strsignal(sig_num));
}

int main(void) {
	struct sigaction sa_int, sa_term;
	fd_set readfds;
	struct timeval timeout;
	int ready;

	sa_int.sa_handler = sigint_Handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;

	sa_term.sa_handler = sigterm_Handler;
	sigemptyset(&sa_term.sa_mask);
	sa_term.sa_flags = 0;

	if (sigaction(SIGINT, &sa_int, NULL) == -1) {
		perror("sigaction(SIGINT)");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTERM, &sa_term, NULL) == -1) {
		perror("sigaction(SIGTERM)");
		exit(EXIT_FAILURE);
	}

	printf("Program started! Type something or send SIGINT/SIGTERM...\n");

	while (1) {
		if (sigint_flag) {
			sigint_flag = false;
			printf("\tSIGINT received.\n");
		}

		if (sigterm_flag) {
			sigterm_flag = false;
			printf("\tExiting program due to SIGTERM.\n");
			break;
		}

		/* Set up the file descriptor set */
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);

		/* Set the timeout */
		timeout.tv_sec = MAX_TIMEOUT;
		timeout.tv_usec = 0;

		ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

		if (ready == -1) {
			if (errno == EINTR) {
				// Interrupted by signal, check flags
				continue;
			}

			perror("select() failed.");
			exit(EXIT_FAILURE);
		} else if (ready > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
			char input[MAX_INPUT_SIZE];

			if (fgets(input, sizeof(input), stdin) != NULL) {
				printf("\tYou typed: %s", input);
			}
		} else if (ready == 0) {
			printf("Timeout occurred after %d seconds.\n", MAX_TIMEOUT);
		}
	}

	return 0;
}
