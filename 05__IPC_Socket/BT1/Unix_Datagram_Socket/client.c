#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_SOCK_PATH "./my_server_sock"
#define CLIENT_SOCK_PATH "./my_client_sock"
#define MAXBUF 100
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

void chat_func(int client_fd, struct sockaddr_un *server_addr) {
    char send_buffer[MAXBUF];
    char recv_buffer[MAXBUF];

    socklen_t server_len = sizeof(*server_addr);

    while (1) {
        printf("Enter the message: ");
        fgets(send_buffer, MAXBUF, stdin);

        if (sendto(client_fd, send_buffer, strlen(send_buffer) + 1, 0,
                   (struct sockaddr *)server_addr, server_len) == -1)
            handle_error("sendto");

        if (!strncmp(send_buffer, "exit", 4)) {
            printf("Client exit...\n");
            break;
        }

        if (recvfrom(client_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL) == -1)
            handle_error("recvfrom");

        if (!strncmp(recv_buffer, "exit", 4)) {
            printf("Server exit...\n");
            break;
        }

        printf("\nMessage from server: %s\n", recv_buffer);
    }
}

int main(void) {
    int client_fd;
    struct sockaddr_un client_addr, server_addr;

    if ((client_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
        handle_error("socket");

    if (remove(CLIENT_SOCK_PATH) == -1 && errno != ENOENT)
        handle_error("remove");

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_SOCK_PATH, sizeof(client_addr.sun_path) - 1);

    if (bind(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
        handle_error("bind");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCK_PATH, sizeof(server_addr.sun_path) - 1);

    printf("Client connected via Unix Datagram socket.\n");

    chat_func(client_fd, &server_addr);

    close(client_fd);
    remove(CLIENT_SOCK_PATH);
    return 0;
}
