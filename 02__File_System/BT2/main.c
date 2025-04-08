#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    const char *file = "text.txt";
    int fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fd2 = open(file, O_RDWR);
    int fd3 = open(file, O_RDWR);

    write(fd1, "Hello,", 6);
    write(fd2, "world", 6);
    lseek(fd2, 0, SEEK_SET);
    write(fd1, "HELLO,", 6);
    write(fd3, "Gidday", 6);

    return 0;
}
