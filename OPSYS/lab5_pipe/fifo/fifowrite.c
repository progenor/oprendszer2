
/*
 * példaprogram a fájlrendszerbéli névvel
 * rendelkező csővezeték használatára
 *
 * ez a program ír a csőbe
 * fordítás:
      gcc -Wall fifowrite.c -o fifowrite
 * használat
      ./fifowrite fifo1
 *   fifo1 a pipe neve a fájlrendszerben
 */

#include "myinclude.h"

#define N 4096

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Használat: %s fifo_név\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // van-e ilyen nevű fájl
    if (access(argv[1], F_OK)) {  // ha nem létezik
        // létrehozza a névvel rendelkező csövet
        // lásd man 3 mkfifo
        if (mkfifo(argv[1], 0644) < 0)
            syserr("mkfifo");
    }

    // megnyitja  a fifo-t írásra
    int fd;
    if ((fd = open(argv[1], O_WRONLY)) < 0) {
        syserr("open");
    }

    // az stdin-t másolja  a fifo-ba
    char buff[N];
    while (fgets(buff, N, stdin)) {
        if (write(fd, buff, strlen(buff)) < 0)
            syserr("write");
    }

    close(fd);
    exit(0);
}