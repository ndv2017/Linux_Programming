#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

#define	MSG_SIZE_MAX	100
#define	MSG_NUM			2
#define	PID_NUM			3

const char *msg_arr[MSG_NUM] = {
	"This is message from parent...",
	"(modified by child 1)."
};

int main(void) {
	int pipe_fd_0[2];	// parent to child 1
	int pipe_fd_1[2];	// child 1 to child 2
	pid_t pid_arr[PID_NUM];
	ssize_t num_read_0, num_write_0, num_read_1, num_write_1;
	char str[MSG_SIZE_MAX];

	if (pipe(pipe_fd_0) == -1) {
		perror("pipe() failed");
		exit(EXIT_FAILURE);
	}

	if (pipe(pipe_fd_1) == -1) {
		perror("pipe() failed");
		exit(EXIT_FAILURE);
	}

	/* Create child 1 */
	pid_arr[0] = fork();
	switch (pid_arr[0]) {
		case -1:
			perror("fork() failed");
			exit(EXIT_FAILURE);

		case 0:		/* Child process 1 */
			printf("Child 1: my pid: %d\n", getpid());

			close(pipe_fd_0[1]);
			close(pipe_fd_1[0]);

			while (1) {
				num_read_0 = read(pipe_fd_0[0], str, MSG_SIZE_MAX);
				if (num_read_0 == -1) {
					perror("read() failed");
					exit(EXIT_FAILURE);
				}
				else if (num_read_0 == 0) {
					puts("pipe 1 end_of_pipe\n");
					break;
				}
				else
					printf("Child 1: received message from parent.\n");
			}

			/* Modify the message */
			strcat(str, msg_arr[1]);

			num_write_1 = write(pipe_fd_1[1], str, MSG_SIZE_MAX);
			if (num_write_1 == -1) {
				perror("write() failed");
				exit(EXIT_FAILURE);
			}

			close(pipe_fd_0[0]);
			close(pipe_fd_1[1]);
			exit(EXIT_SUCCESS);

		default:
			break;
	}

	/* Create child 2 */
	pid_arr[1] = fork();
	switch (pid_arr[1]) {
		case -1:
			perror("fork() failed");
			exit(EXIT_FAILURE);

		case 0:		/* Child process 2 */
			printf("Child 2: my pid: %d\n", getpid());

			close(pipe_fd_0[0]);
			close(pipe_fd_0[1]);
			close(pipe_fd_1[1]);

			while (1) {
				num_read_1 = read(pipe_fd_1[0], str, MSG_SIZE_MAX);
				if (num_read_1 == -1) {
					perror("read() failed");
					exit(EXIT_FAILURE);
				}
				else if (num_read_1 == 0) {
					puts("pipe 2 end_of_pipe\n");
					break;
				}
				else
					printf("Child 2: received message from child 1.\n");
			}

			printf("Child 2: displaying message:\n\t%s\n\n", str);

			close(pipe_fd_1[0]);
			exit(EXIT_SUCCESS);

		default:
			break;
	}

	/* Parent */
	close(pipe_fd_0[0]);
	close(pipe_fd_1[0]);
	close(pipe_fd_1[1]);

	num_write_0 = write(pipe_fd_0[1], msg_arr[0], MSG_SIZE_MAX);
	if (num_write_0 == -1) {
		perror("write() failed");
		exit(EXIT_FAILURE);
	}
	else
		printf("Parent: sent message to child 1.\n");

	close(pipe_fd_0[1]);

	/* Wait for both children to finish */
	wait(NULL);
	wait(NULL);

	printf("Parent: all children finished.\n");

	return 0;
}
