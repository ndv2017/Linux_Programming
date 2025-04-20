#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

int main(void) {
	pid_t pid;

	pid = fork();

	switch (pid) {
		case -1:
			perror("fork() failed.");
			exit(EXIT_FAILURE);

		case 0:		/* Child process */
			sleep(2);

			printf("Child process: my pid: %d\n", getpid());
			printf("Child process: I am now orphaned, my new parent pid: %d\n", getppid());

			exit(EXIT_SUCCESS);

		default:	/* Parent process */
			printf("Parent process: my pid: %d\n", getpid());
	}

	return 0;
}
