#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXBUF  100
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

void chat_func(int server_fd) {
    char send_buffer[MAXBUF];
    char recv_buffer[MAXBUF];

    while (1) {
        printf("Enter the message: ");
        fgets(send_buffer, MAXBUF, stdin);

        if (write(server_fd, send_buffer, strlen(send_buffer) + 1) == -1)
            handle_error("write");

        if (!strncmp(send_buffer, "exit", 4)) {
            printf("Client exit...\n");
            break;
        }

        if (read(server_fd, recv_buffer, sizeof(recv_buffer)) == -1)
            handle_error("read");

        if (!strncmp(recv_buffer, "exit", 4)) {
            printf("Server exit...\n");
            break;
        }

        printf("\nMessage from server: %s\n", recv_buffer);
    }

    close(server_fd);
}

int main(int argc, char const *argv[]) {
    int server_fd, port_no;
    struct sockaddr_in server_addr;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server address> <port number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port_no = atoi(argv[2]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
        handle_error("inet_pton");

    if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("connect");

    printf("Connected to %s:%d\n", argv[1], port_no);

    chat_func(server_fd);

    return 0;
}
