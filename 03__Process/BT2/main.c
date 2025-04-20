#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(void) {
	pid_t pid;
	char *env_val;
	int index_value;
	char *command_1 = "ls";
	char *command_2 = "date";
    char *argument_list_1[] = {"ls", "-lah", (char*) NULL};
    char *argument_list_2[] = {"date", "-R", (char*) NULL};
	
	env_val = getenv("TASK_INDEX");
	if (env_val == NULL) {
		fprintf(stderr, "Environment variable TASK_INDEX not set.\n");
		exit(EXIT_FAILURE);
	}

	index_value = atoi(env_val);
	if (index_value < 1 || index_value > 2) {
		fprintf(stderr, "TASK_INDEX must be 1 (ls) or 2 (date).\n");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	switch (pid) {
		case -1:
            perror("fork() failed.");
			exit(EXIT_FAILURE);

        case 0:
            // Child process
			if (index_value == 1) {
				execvp(command_1, argument_list_1);
			} else if (index_value == 2) {
				execvp(command_2, argument_list_2);
			}

			// If execvp fails
			perror("execvp() failed.");
			exit(EXIT_FAILURE);

        default:
            // Parent process
			sleep(3); // Give child a chance to execute
			printf("Parent process: my pid: %d\n", getpid());
			printf("Parent process: my child process's pid: %d\n", pid);
	}

	return 0;
}
