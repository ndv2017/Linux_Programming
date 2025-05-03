/* This macro ensures that the program adheres to the POSIX.1-2008 standard.
   It enables access to POSIX-specific features, such as sigaction(), siginfo_t, 
   and SA_SIGINFO, which are used in this program for signal handling. */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

static void siginfo_Handler(int sig_num, siginfo_t *sig_inf, void *ucontext) {
	printf("\nReceived signal %d (%s)\n", sig_num, strsignal(sig_num));
	printf("\tSIGTSTP ignored!\n");
	
	/* For learning purposes, print signal information */
	printf("Signal info:\n");
	printf("\tSignal number: %d\n", sig_inf->si_signo);
	printf("\tSender PID: %d\n", sig_inf->si_pid);
	printf("\tSender UID: %d\n", sig_inf->si_uid);
	printf("\tSignal code: %d\n", sig_inf->si_code);
}

int main(void) {
	struct sigaction sig_act;

	sig_act.sa_handler = (__sighandler_t) siginfo_Handler;
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGTSTP, &sig_act, NULL) == -1) {
		perror("sigaction() failed");
		exit(EXIT_FAILURE);
	}

	printf("Running... press Ctrl+Z (SIGTSTP) to test ignoring.\n");

	while (1) {
		if (pause() == -1 && errno != EINTR) {
			perror("pause() failed");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
