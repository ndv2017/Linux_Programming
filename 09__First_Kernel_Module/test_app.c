#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CDEV_PATH "/dev/m_device"

void printMenu(void) {
    printf("Choose option for file:\n");
    printf("    1: Write\n");
    printf("    2: Read\n");
    printf("    3: Exit\n");
    printf("***********************\n");
    printf("Type: ");
}

int main(void) {
    int option;
    char write_buf[1024], read_buf[1024];
    
    while (1) {
        printMenu();
        scanf("%d", &option);
        while (getchar() != '\n');

        switch (option) {
            case 1: {
                printf("Type and enter to write into file: ");
                fgets(write_buf, sizeof(write_buf), stdin);
                write_buf[strcspn(write_buf, "\n")] = 0;

                FILE *file = fopen(CDEV_PATH, "w");
                if (file == NULL) {
                    perror("Failed to open file for writing");
                    exit(EXIT_FAILURE);
                }

                /* Write to the file */
                fwrite(write_buf, sizeof(char), strlen(write_buf), file);

                fclose(file);
                printf("Data Writing ... Done!\n\n\n");
                break;
            }

            case 2: {
                FILE *file = fopen(CDEV_PATH, "r");
                if (file == NULL) {
                    perror("Failed to open file for reading");
                    exit(EXIT_FAILURE);
                }

                printf("Data Reading ... \n");
                printf("Data: ");
                while (fgets(read_buf, sizeof(read_buf), file)) {
                    printf("%s", read_buf);                        // Print each line read from the file
                }

                fclose(file);
                printf("\nDone!\n\n\n");
                break;
            }

            case 3:
                printf("Exiting application...\n");
                exit(0);
                break;

            default:
                printf("Enter Valid option\n");
                break;
        }
    }

    return 0;
}
