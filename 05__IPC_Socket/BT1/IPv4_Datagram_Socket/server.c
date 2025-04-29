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
    char recv_buffer[MAXBUF];
    char send_buffer[MAXBUF];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        if (recvfrom(server_fd, recv_buffer, sizeof(recv_buffer), 0,
                     (struct sockaddr *)&client_addr, &addr_len) == -1)
            handle_error("recvfrom");

        if (!strncmp(recv_buffer, "exit", 4)) {
            printf("Client requested exit.\n");
            break;
        }

        printf("\nMessage from client: %s\n", recv_buffer);

        printf("Respond message: ");
        fgets(send_buffer, MAXBUF, stdin);

        if (sendto(server_fd, send_buffer, strlen(send_buffer) + 1, 0,
                   (struct sockaddr *)&client_addr, addr_len) == -1)
            handle_error("sendto");

        if (!strncmp(send_buffer, "exit", 4)) {
            printf("Server exit...\n");
            break;
        }
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, port_no;
    struct sockaddr_in server_addr;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port_no = atoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("bind");

    printf("UDP Server is running on port: %d\n", port_no);

    chat_func(server_fd);

    close(server_fd);
    return 0;
}