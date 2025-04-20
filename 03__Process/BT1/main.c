#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
	pid_t pid;

	pid = fork();

	switch (pid) {
		case -1:
            perror("fork() failed.");
			exit(EXIT_FAILURE);
        case 0:
            // Child process
			printf("Child process: my pid: %d\n", getpid());
			printf("Child process: my parent process's pid: %d\n", getppid());
            break;
        default:
            // Parent process
			sleep(3); // Give child a chance to execute
			printf("Parent process: my pid: %d\n", getpid());
			printf("Parent process: my child process's pid: %d\n", pid);
	}

    return 0;
}
