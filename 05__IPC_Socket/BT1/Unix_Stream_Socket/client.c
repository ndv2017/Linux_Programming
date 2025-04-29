#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH   "./mysock"
#define MAXBUF      100
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

int main(void) {
    int server_fd;
    struct sockaddr_un server_addr;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCK_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("connect");

    printf("Client: Connected to server\n");

    chat_func(server_fd);

    return 0;
}
