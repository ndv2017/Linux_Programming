#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_LEN 20

int main(void) {
    const char *filename = "text.txt";
    char buf1[BUF_LEN] = "12345";
    char buf2[BUF_LEN] = "6789";

    int fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("open() failed.");
        exit(EXIT_FAILURE);
    }

    /* Write the content of buf1 to the file */
    if (write(fd, buf1, strlen(buf1)) == -1) {
        perror("write() failed.");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Try to move the file pointer to the beginning of the file. */
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek() failed.");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Attempt to write the content of buf2 to the file. */
    if (write(fd, buf2, strlen(buf2)) == -1) {
        perror("write() failed.");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * However, since the file was opened in O_APPEND mode, this 
     * will not affect the location where the data is written. 
     * Data will still be appended to the end of the file.
     */

    if (close(fd) == -1) {
        perror("close() failed.");
    }

    return 0;
}
