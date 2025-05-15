#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

#define MSG_SIZE	40
#define MSG_NUM		2

char *msg_arr[MSG_NUM] = {
	"Hello, this is an arbitrary message...",
	"I am Viet"
};

int main(void) {
	pid_t pid;
	int pipe_fd[2];
	ssize_t num_read = 0;
	ssize_t num_write = 0;
	char buff_recv[MSG_SIZE];

	if (pipe(pipe_fd) == -1) {
		perror("pipe() failed.");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	switch (pid) {
		case -1:
			perror("fork() failed.");
			exit(EXIT_FAILURE);

		case 0:		/* Child process performs reading */
			printf("Child: my pid: %d\n", getpid());

			if (close(pipe_fd[1]) == -1) {	// Unused write end of pipe
				perror("close() child failed.");
				exit(EXIT_FAILURE);
			}

			while (1) {
				num_read = read(pipe_fd[0], buff_recv, MSG_SIZE);
				if (num_read == -1) {
					perror("read() failed.");
					exit(EXIT_FAILURE);
				}
				else if (num_read == 0) {
					puts("\npipe end_of_pipe");
					break;
				}
				else
					printf("Child: message: \n\t%s\n", buff_recv);
			}

			if (close(pipe_fd[0]) == -1) {
				perror("close() child failed.");
				exit(EXIT_FAILURE);
			}

			break;

		default:	/* Parent process performs writing */
			printf("Parent: my pid: %d\n", getpid());

			if (close(pipe_fd[0]) == -1) {	// Unused read end of pipe
				perror("close() parent failed.");
				exit(EXIT_FAILURE);
			}

			for (int i = 0; i < MSG_NUM; i++) {
				
				num_write = write(pipe_fd[1], msg_arr[i], MSG_SIZE);
				if (num_write == -1) {
					perror("write() failed.");
					exit(EXIT_FAILURE);
				}
				else
				printf("Parent: write %zd bytes to pipe.\n", num_write);
			}
	
			if (close(pipe_fd[1]) == -1) {
				perror("close() parent failed.");
				exit(EXIT_FAILURE);
			}
			
			break;
	}

	return 0;
}
