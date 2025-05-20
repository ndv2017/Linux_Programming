#include "network.h"
#include "chat.h"

void init_peers() {
    for (int i = 0; i < MAX_PEERS; i++) {
        peers[i].socket_fd = -1;
        peers[i].active = 0;
    }
}

int init_server(int port_no) {
    int server_fd;
    struct sockaddr_in server_addr;
    int opt = 1;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        handle_error("socket creation");

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        handle_error("setsockopt");
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);
    
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        handle_error("bind");
    
    if (listen(server_fd, 5) < 0)
        handle_error("listen");
    
    /* Set non-blocking */
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);
    
    return server_fd;
}

int get_available_peer_slot() {
    for (int i = 0; i < MAX_PEERS; i++) {
        if (!peers[i].active)
            return i;
    }
    return -1;
}

void connect_to_peer(char *command) {
    char ip[INET_ADDRSTRLEN];
    int port;
    
    /* Parse command */
    char *token = strtok(command, " ");  // Skip "connect"
    token = strtok(NULL, " ");           // IP address
    if (token == NULL) {
        printf("Invalid command format. Use: connect <destination_ip> <destination_port_no>\n");
        return;
    }
    strncpy(ip, token, INET_ADDRSTRLEN);
    
    token = strtok(NULL, " ");           // Port
    if (token == NULL) {
        printf("Invalid command format. Use: connect <destination_ip> <destination_port_no>\n");
        return;
    }
    port = atoi(token);
    
    int slot = get_available_peer_slot();
    if (slot == -1) {
        printf("Cannot connect: maximum number of peers reached.\n");
        return;
    }

    char* local_ip = get_local_IP_address();
    if ((strcmp(local_ip, ip) == 0) && (port == port_no)) {
        printf("Cannot connect to yourself. Connection refused.\n");
        return;
    }
    
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket creation");
        return;
    }
    
    /* Set socket to non-blocking */
    int flags = fcntl(sock_fd, F_GETFL, 0);
    fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
    
    struct sockaddr_in peer_addr;
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip, &peer_addr.sin_addr) <= 0) {
        printf("Invalid IP address: %s\n", ip);
        close(sock_fd);
        return;
    }
    
    /* Try to connect (non-blocking) */
    int ret = connect(sock_fd, (struct sockaddr *)&peer_addr, sizeof(peer_addr));
    if (ret < 0 && errno != EINPROGRESS) {
        perror("connect");
        close(sock_fd);
        return;
    }

    char port_msg[32];
    snprintf(port_msg, sizeof(port_msg), "PORT:%d", port_no);
    send(sock_fd, port_msg, strlen(port_msg), 0);

    /* Add to peers array */
    peers[slot].socket_fd = sock_fd;
    strncpy(peers[slot].ip, ip, INET_ADDRSTRLEN);
    peers[slot].connection_port = port;
    peers[slot].listening_port = port;
    peers[slot].active = 1;
    
    printf("\nConnection established with %s as connection ID %d\n", ip, slot);
}

void list_all_connections() {
    int active_connections = 0;
    
    printf("\nID\tIP Address\tPort\n");
    printf("---\t----------\t----\n");
    
    for (int i = 0; i < MAX_PEERS; i++) {
        if (peers[i].active) {
            int display_port = peers[i].listening_port > 0 ? peers[i].listening_port : peers[i].connection_port;
            printf("%d\t%s\t%d\n", i, peers[i].ip, display_port);
            active_connections++;
        }
    }
    
    if (active_connections == 0)
        printf("\nNo active connections.\n");
}

void terminate_connection(char *command) {
    char *token = strtok(command, " ");  // Skip "terminate"
    token = strtok(NULL, " ");           // Connection ID
    
    if (token == NULL) {
        printf("Invalid command format. Use: terminate <connection_id>\n");
        return;
    }
    
    int conn_id = atoi(token);
    if (conn_id < 0 || conn_id >= MAX_PEERS || !peers[conn_id].active) {
        printf("Invalid connection ID: %d\n", conn_id);
        return;
    }
    
    // Close connection
    close(peers[conn_id].socket_fd);
    peers[conn_id].socket_fd = -1;
    peers[conn_id].active = 0;
    
    printf("\nConnection %d terminated.\n", conn_id);
}

void send_message(char *command) {
    char *token = strtok(command, " ");  // Skip "send"
    token = strtok(NULL, " ");           // Connection ID
    
    if (token == NULL) {
        printf("Invalid command format. Use: send <connection_id> <message>\n");
        return;
    }
    
    int conn_id = atoi(token);
    if (conn_id < 0 || conn_id >= MAX_PEERS || !peers[conn_id].active) {
        printf("Invalid connection ID: %d\n", conn_id);
        return;
    }
    
    /* Get message (rest of the command) */
    token = strtok(NULL, "");
    if (token == NULL) {
        printf("No message provided. Use: send <connection_id> <message>\n");
        return;
    }
    
    ssize_t bytes_sent = send(peers[conn_id].socket_fd, token, strlen(token), 0);
    if (bytes_sent < 0) {
        perror("send");
        return;
    }
    
    printf("Message sent to connection %d.\n", conn_id);
}

void accept_connections() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            perror("accept");
        return;
    }
    
    /* Set client socket to non-blocking */
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    
    int slot = get_available_peer_slot();
    if (slot == -1) {
        printf("Cannot accept: maximum number of peers reached.\n");
        close(client_fd);
        return;
    }
    
    /* Add to pers array */
    peers[slot].socket_fd = client_fd;
    inet_ntop(AF_INET, &client_addr.sin_addr, peers[slot].ip, INET_ADDRSTRLEN);
    peers[slot].connection_port = ntohs(client_addr.sin_port);
    peers[slot].listening_port = 0;
    peers[slot].active = 1;

    char port_msg[32];
    snprintf(port_msg, sizeof(port_msg), "PORT:%d", port_no);
    send(client_fd, port_msg, strlen(port_msg), 0);
    
    printf("\nIncoming connection from %s accepted as connection ID %d\n", 
           peers[slot].ip, slot);
}

void check_incoming_messages() {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    
    int max_fd = -1;
    
    /* Add active peer sockets to set */
    for (int i = 0; i < MAX_PEERS; i++) {
        if (peers[i].active) {
            FD_SET(peers[i].socket_fd, &read_fds);
            if (peers[i].socket_fd > max_fd)
                max_fd = peers[i].socket_fd;
        }
    }
    
    if (max_fd == -1)
        return;
    
    /* Set timeout to make select non-blocking */
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    int ret = select(max_fd + 1, &read_fds, NULL, NULL, &tv);
    if (ret < 0) {
        if (errno != EINTR)
            perror("select");
        return;
    } else if (ret == 0)
        return;
    
    /* Check each socket for incoming data */
    for (int i = 0; i < MAX_PEERS; i++) {
        if (peers[i].active && FD_ISSET(peers[i].socket_fd, &read_fds)) {
            char buffer[MAXBUF];
            ssize_t bytes_read = recv(peers[i].socket_fd, buffer, MAXBUF - 1, 0);
            
            if (bytes_read <= 0) {
                if (bytes_read < 0)
                    perror("recv");
                else
                    printf("\nConnection %d closed by peer.\n", i);
                
                /* Close connection */
                close(peers[i].socket_fd);
                peers[i].socket_fd = -1;
                peers[i].active = 0;
            } else {
                buffer[bytes_read] = '\0';
                /* Check if this is a port message */
                if (strncmp(buffer, "PORT:", 5) == 0) {
                    int peer_listen_port = atoi(buffer + 5);
                    peers[i].listening_port = peer_listen_port;
                } else {
                    printf("\nMessage from connection %d: \"%s\"\n", i, buffer);
                }
            }
        }
    }
}

void exit_app() {
    /* Close all connections */
    for (int i = 0; i < MAX_PEERS; i++) {
        if (peers[i].active) {
            close(peers[i].socket_fd);
            peers[i].active = 0;
        }
    }
    
    /* Close listening socket */
    if (listen_fd != -1)
        close(listen_fd);
    
    printf("\nAll connections closed.\nExiting...\n");
    exit(EXIT_SUCCESS);
}
