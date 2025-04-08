#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if ((argc < 5 && *argv[3] == 'w') || (argc < 4 && *argv[3] == 'r')) {
        fprintf(stderr, "Usage:\n\t%s <filename> <num-bytes> <r/w> \"some_content\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int num_bytes = atoi(argv[2]);
    char mode = *argv[3];

    int fd;

    if (mode == 'r') {
        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("open() failed.");
            exit(EXIT_FAILURE);
        }

        char *buffer = (char *)malloc(num_bytes + 1);
        if (buffer == NULL) {
            perror("malloc() failed.");
            close(fd);
            exit(EXIT_FAILURE);
        }

        ssize_t bytes_to_read = read(fd, buffer, num_bytes);
        if (bytes_to_read == -1) {
            perror("read() failed.");
            close(fd);
            exit(EXIT_FAILURE);
        }
        buffer[bytes_to_read] = '\0';
        printf("Read %zd bytes of file %s:\n%s\n", bytes_to_read, filename, buffer);

        free(buffer);
    } else if (mode == 'w') {
        char *content = argv[4];
        ssize_t bytes_to_write = (num_bytes > strlen(content) ? strlen(content) : num_bytes);

        fd = open(filename, O_WRONLY | O_APPEND);
        if (fd == -1) {
            perror("open() failed.");
            exit(EXIT_FAILURE);
        }

        if (write(fd, content, bytes_to_write) == -1) {
            perror("write() failed.");
            close(fd);
            exit(1);
        }
        printf("Wrote %zd bytes to file %s\n", bytes_to_write, filename);
    } else {
        fprintf(stderr, "Invalid argument. Use 'r' for read or 'w' for write.\n");
    }

    if (close(fd) == -1) {
        perror("close() failed.");
    }

    return 0;
}
