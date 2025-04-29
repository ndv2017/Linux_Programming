#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_SOCK_PATH "./my_server_sock"
#define MAXBUF 100
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

void chat_func(int server_fd) {
    char recv_buffer[MAXBUF];
    char send_buffer[MAXBUF];
    struct sockaddr_un client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (1) {
        if (recvfrom(server_fd, recv_buffer, sizeof(recv_buffer), 0,
                     (struct sockaddr *)&client_addr, &client_len) == -1)
            handle_error("recvfrom");

        if (!strncmp(recv_buffer, "exit", 4)) {
            printf("Client requested exit.\n");
            break;
        }

        printf("\nMessage from client: %s\n", recv_buffer);

        printf("Respond message: ");
        fgets(send_buffer, MAXBUF, stdin);

        if (sendto(server_fd, send_buffer, strlen(send_buffer) + 1, 0,
                   (struct sockaddr *)&client_addr, client_len) == -1)
            handle_error("sendto");

        if (!strncmp(send_buffer, "exit", 4)) {
            printf("Server exit...\n");
            break;
        }
    }
}

int main(void) {
    int server_fd;
    struct sockaddr_un server_addr;

    if ((server_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
        handle_error("socket");

    if (remove(SERVER_SOCK_PATH) == -1 && errno != ENOENT)
        handle_error("remove");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCK_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("bind");

    printf("Unix Datagram Server started on %s\n", SERVER_SOCK_PATH);

    chat_func(server_fd);

    close(server_fd);
    remove(SERVER_SOCK_PATH);
    return 0;
}
