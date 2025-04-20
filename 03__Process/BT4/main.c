#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define EXIT_VALUE 5

int main(void) {
	pid_t pid;
	int status;

	pid = fork();

	switch (pid) {
		case -1:
			perror("fork() failed.");
			exit(EXIT_FAILURE);

		case 0:		/* Child process */
			printf("Child process: my pid: %d\n", getpid());

			sleep(2);
			exit(EXIT_VALUE); // Exit with predefined value

		default:	/* Parent process */
			printf("Parent process: my pid: %d\n", getpid());

			if (waitpid(pid, &status, 0) == -1) {
				perror("waitpid() failed");
				exit(EXIT_FAILURE);
			}

			if (WIFEXITED(status)) {
				printf("Parent process: child exited with status = %d\n", WEXITSTATUS(status));
			} else {
				fprintf(stderr, "Child did not exit normally.\n");
				exit(EXIT_FAILURE);
			}
	}

	return 0;
}
