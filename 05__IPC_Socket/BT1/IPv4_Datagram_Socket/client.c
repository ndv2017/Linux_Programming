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

void chat_func(int client_fd, struct sockaddr_in *server_addr) {
    char send_buffer[MAXBUF];
    char recv_buffer[MAXBUF];
    socklen_t addr_len = sizeof(*server_addr);

    while (1) {
        printf("Enter the message: ");
        fgets(send_buffer, MAXBUF, stdin);

        if (sendto(client_fd, send_buffer, strlen(send_buffer) + 1, 0,
                   (struct sockaddr *)server_addr, addr_len) == -1)
            handle_error("sendto");

        if (!strncmp(send_buffer, "exit", 4)) {
            printf("Client exit...\n");
            break;
        }

        if (recvfrom(client_fd, recv_buffer, sizeof(recv_buffer), 0,
                     NULL, NULL) == -1)
            handle_error("recvfrom");

        if (!strncmp(recv_buffer, "exit", 4)) {
            printf("Server exit...\n");
            break;
        }

        printf("\nMessage from server: %s\n", recv_buffer);
    }
}

int main(int argc, char const *argv[]) {
    int client_fd, port_no;
    struct sockaddr_in server_addr;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server address> <port number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port_no = atoi(argv[2]);

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
        handle_error("inet_pton");

    printf("UDP Client is running...\n");

    chat_func(client_fd, &server_addr);

    close(client_fd);
    return 0;
}
