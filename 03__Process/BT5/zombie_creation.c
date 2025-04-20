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
			printf("Child process: my pid: %d\n", getpid());

			exit(EXIT_SUCCESS);

        default:	/* Parent process */
			printf("Parent process: my pid: %d\n", getpid());
			printf("Use `ps ux | grep Z` or `top` to observe zombie process.\n");

			while (1);
	}

    return 0;
}
