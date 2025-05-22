#include "chat.h"

void handle_signal(int sig) {
    exit_flag = 1;
}

void print_help() {
    printf("\n*********************************** Chat Application *********************************\n\n");
    printf("List of commands:\n");
    printf("  help \t\t\t\t\t\t\tDisplay UI options or commands\n");
    printf("  myip \t\t\t\t\t\t\tDisplay IP address of this process\n");
    printf("  myport \t\t\t\t\t\tDisplay listening port\n");
    printf("  connect <destination_ip> <destination_port_no> \tConnect to another process\n");
    printf("  list \t\t\t\t\t\t\tList all connections\n");
    printf("  terminate <connection_id> \t\t\t\tTerminate a connection\n");
    printf("  send <connection_id> <message> \t\t\tSend a message to a connection\n");
    printf("  exit \t\t\t\t\t\t\tClose all connections & exit the application\n");
    printf("\n*********************************** Chat Application *********************************\n\n");
}

char* get_local_IP_address() {
    char host_buffer[256];
    struct hostent *host_entry;

    if (gethostname(host_buffer, sizeof(host_buffer)) == -1)
        return NULL;

    if ((host_entry = gethostbyname(host_buffer)) == NULL)
        return NULL;

    return inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
}

void print_IP_address() {
    char *IP_buffer;

    IP_buffer = get_local_IP_address();
    if (IP_buffer == NULL)
        handle_error("get_local_IP_address");

    printf("IP address: %s\n", IP_buffer);
}

void print_port_no(int port_no) {
    printf("Listening port: %d\n", port_no);
}
