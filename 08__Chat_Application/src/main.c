#include "chat.h"

peer_t peers[MAX_PEERS];
int listen_fd = -1;
volatile sig_atomic_t exit_flag = 0;
int port_no;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <port_no>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
        port_no = atoi(argv[1]);
    
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    init_peers();
    listen_fd = init_server(port_no);
    
    print_help();
    printf("Listening on port %d...\n", port_no);

    int show_prompt_command = false;
    
    while (!exit_flag) {
        accept_connections();
        check_incoming_messages();

        if (!show_prompt_command) {
            printf("\nEnter a command: ");
            fflush(stdout);
            show_prompt_command = true;
        }

        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;  // 100ms timeout for responsive UI

        int ret = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &tv);
        if (ret < 0 && errno != EINTR)
            handle_error("select");

        if (ret > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {
            char command[MAXBUF];
            if (fgets(command, MAXBUF, stdin) == NULL)
                continue;

            command[strcspn(command, "\n")] = 0;
            
            if (strlen(command) <= 0)                                           printf("Type command...\n");
            else if (strcmp(command, "help") == 0)                              print_help();
            else if (strcmp(command, "myip") == 0)                              print_IP_address();
            else if (strcmp(command, "myport") == 0)                            print_port_no(port_no);
            else if (strncmp(command, "connect", strlen("connect")) == 0)       connect_to_peer(command);
            else if (strcmp(command, "list") == 0)                              list_all_connections();
            else if (strncmp(command, "terminate", strlen("terminate")) == 0)   terminate_connection(command);
            else if (strncmp(command, "send ", 5) == 0)                         send_message(command);
            else if (strcmp(command, "exit") == 0)                              exit_app();
            else                                                                printf("Invalid command. Please try again.\n");

            show_prompt_command = false;
        }
    }
    
    exit_app();
    
    return 0;
}