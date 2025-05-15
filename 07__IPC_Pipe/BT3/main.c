#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

#define MSG_SIZE 100

const char *str = "Hello, this is an arbitrary message...";

int main(void) {
	pid_t pid;
	int pipe_fd[2];
	ssize_t num_read = 0;
	ssize_t num_write = 0;
	char *buff_recv;

	buff_recv = (char *)malloc(sizeof(char) * MSG_SIZE);

	if (pipe(pipe_fd) == -1) {
		perror("pipe() failed.");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	switch (pid) {
		case -1:
			perror("fork() failed.");
			exit(EXIT_FAILURE);

		case 0:		/* Child process */
			printf("Child: my pid: %d\n", getpid());

			close(pipe_fd[1]);

			num_read = read(pipe_fd[0], buff_recv, MSG_SIZE);
			if (num_read == -1) {
				perror("read() failed.");
				exit(EXIT_FAILURE);
			}

			printf("Child: string message: \n\t%s\n", buff_recv);
			printf("Child: number of characters in string: \n\t%zd\n", strlen(buff_recv));

			close(pipe_fd[0]);

			break;

		default:	/* Parent process */
			printf("Parent: my pid: %d\n", getpid());

			close(pipe_fd[0]);

			num_write = write(pipe_fd[1], str, strlen(str) + 1);
			if (num_write == -1) {
				perror("write() failed.");
				exit(EXIT_FAILURE);
			}

			printf("Parent: write %zd bytes to pipe.\n", num_write);

			close(pipe_fd[1]);

			break;
	}

	wait(NULL);
	free(buff_recv);
	return 0;
}
