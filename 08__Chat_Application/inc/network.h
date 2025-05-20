#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/select.h>
#include <stdbool.h>

#define MAXBUF          1024
#define MAX_PEERS       10
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* Structure to represent a peer connection */
typedef struct {
    int socket_fd;
    char ip[INET_ADDRSTRLEN];
    int connection_port;
    int listening_port;
    int active;
} peer_t;

extern peer_t peers[MAX_PEERS];
extern int listen_fd;
extern int port_no;

/* Function prototypes (network-related) */
void init_peers(void);
int init_server(int port_no);
int get_available_peer_slot(void);
void connect_to_peer(char *command);
void accept_connections(void);
void check_incoming_messages(void);
void list_all_connections(void);
void terminate_connection(char *command);
void send_message(char *command);
void exit_app(void);

#endif /* NETWORK_H */ 