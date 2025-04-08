#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>

void print_name_of_file(const char *path) {
    char path_copy[1024];
    strncpy(path_copy, path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';

    char *file_name = basename(path_copy);
    printf("\tFile name: %s\n", file_name);
}

void print_type_of_file(struct stat *file_stat) {
    printf("\tType of file: ");
    switch (file_stat->st_mode & S_IFMT) {
        case S_IFREG:   printf("regular file\n");           break;
        case S_IFDIR:   printf("directory\n");              break;
        case S_IFCHR:   printf("character device\n");       break;
        case S_IFBLK:   printf("block device\n");           break;
        case S_IFLNK:   printf("symbolic (soft) link\n");   break;
        case S_IFIFO:   printf("FIFO or pipe\n");           break;
        case S_IFSOCK:  printf("socket\n");                 break;
        default:        printf("unknown file type?\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path/to/file> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file_path = argv[1];
    struct stat file_stat;

    if (stat(file_path, &file_stat) == -1) {
        perror("stat() failed.");
        exit(1);
    }

    puts("----------------------------------------------------------");
    puts("File's information:");
    print_name_of_file(file_path);
    print_type_of_file(&file_stat);
    printf("\tLast file modification: %s", ctime(&file_stat.st_mtime));
    printf("\tSize of file: %ld bytes\n", file_stat.st_size);

    return 0;
}
