#ifndef CHAT_H
#define CHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "network.h"

extern volatile sig_atomic_t exit_flag;

/* Function prototypes (UI-related) */
void handle_signal(int sig);
void print_help(void);
char* get_local_IP_address(void);
void print_IP_address(void);
void print_port_no(int port_no);

#endif /* CHAT_H */
