#include "myinclude.h"

#include <string.h>

#define BUFF_SIZE 1024

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("not enugh params in %s \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int append = 0;
    char *filename;
    if (argc == 3 && strcmp(argv[1], "-a") == 0) {
        append = 1;
        filename = argv[2];
    } else {
        filename = argv[1];
    }

    int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
    int fd;
    if ((open(filename, flags, 0644)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFF_SIZE];
    ssize_t n;
    while ((n = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, n);
        write(fd, buffer, n);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}