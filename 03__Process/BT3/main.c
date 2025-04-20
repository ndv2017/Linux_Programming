#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void SIGUSR1_handler(int signalNum) {
	printf("Caught SIGUSR1 signal. Signal number is: %d\n", signalNum);
}

int main(void) {
	pid_t pid;

	pid = fork();

	switch (pid) {
		case -1:
			perror("fork() failed.");
			exit(EXIT_FAILURE);

		case 0:		/* Child process */
			printf("Child process: my pid: %d\n", getpid());

			signal(SIGUSR1, SIGUSR1_handler);
			printf("Child process: waiting for SIGUSR1 signal...\n");

			pause(); // Wait for signal

			printf("Child process exiting after receiving signal.\n");
			exit(EXIT_SUCCESS);

		default:	/* Parent process */
			sleep(1); // Give child a chance to execute
			printf("Parent process: my pid: %d\n", getpid());
			printf("Parent process will send SIGUSR1 signal to child process after 3 seconds.\n");

			// Send SIGUSR1 signal to child process after 3 seconds
			sleep(3);
			kill(pid, SIGUSR1);

			wait(NULL); // Wait for child process to finish
			printf("Parent process exiting.\n");
			exit(EXIT_SUCCESS);
	}

    return 0;
}
