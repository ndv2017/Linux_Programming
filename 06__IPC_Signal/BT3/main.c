#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define	SEND_COUNT	5
#define DELAY_SECS	2

static volatile sig_atomic_t sig_flag = false;

static void signal_handler(int sig_num) {
	sig_flag = true;
	printf("Received signal %d (%s)\n", sig_num, strsignal(sig_num));
}

int main(void) {
	pid_t pid;
	int status;
	int received_count = 0;

	pid = fork();

	switch (pid) {
		case -1:
			perror("fork() failed.");
			exit(EXIT_FAILURE);

		case 0:		/* Child process */
			printf("Child: my pid: %d\n", getpid());

			if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
				perror("signal() failed");
				exit(EXIT_FAILURE);
			}

			while (received_count < SEND_COUNT) {
				if (sig_flag) {
					sig_flag = false;
					received_count++;
					printf("Child: Received signal from parent\n");
					printf("\tSignal count: (%d/%d)\n", received_count, SEND_COUNT);
				}
			}

			exit(EXIT_SUCCESS);

		default:	/* Parent process */
			sleep(1);	// Give child time to set up signal handler
			printf("Parent: my pid: %d\n", getpid());

			for (int i = 1; i <= SEND_COUNT; i++) {
				sleep(DELAY_SECS);
				if (kill(pid, SIGUSR1) == -1) {
					perror("kill() failed");
					exit(EXIT_FAILURE);
				}
			}

			if (waitpid(pid, &status, 0) == -1) {
				perror("waitpid() failed");
				exit(EXIT_FAILURE);
			}

			if (WIFEXITED(status)) {
				printf("Parent: child exited with status = %d\n", WEXITSTATUS(status));
			} else {
				fprintf(stderr, "Parent: child did not exit normally.\n");
				exit(EXIT_FAILURE);
			}
	}

	return 0;
}
