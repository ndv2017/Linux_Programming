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

void chat_func(int client_fd) {
    char recv_buffer[MAXBUF];
    char send_buffer[MAXBUF];

    while (1) {
        if (read(client_fd, recv_buffer, sizeof(recv_buffer)) == -1)
            handle_error("read");

        if (!strncmp(recv_buffer, "exit", 4)) {
            printf("Client requested exit.\n");
            break;
        }

        printf("\nMessage from client: %s\n", recv_buffer);

        printf("Respond message: ");
        fgets(send_buffer, MAXBUF, stdin);

        if (write(client_fd, send_buffer, strlen(send_buffer) + 1) == -1)
            handle_error("write");

        if (!strncmp(send_buffer, "exit", 4)) {
            printf("Server exit...\n");
            break;
        }
    }

    close(client_fd);
}

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    if (remove(SOCK_PATH) == -1 && errno != ENOENT)
        handle_error("remove");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCK_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("bind");

    if (listen(server_fd, 5) == -1)
        handle_error("listen");

    printf("Unix stream server listening on socket path: %s\n", SOCK_PATH);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
        handle_error("accept");

    printf("Server: Got connection!\n");

    chat_func(client_fd);

    close(server_fd);
    remove(SOCK_PATH);

    return 0;
}
