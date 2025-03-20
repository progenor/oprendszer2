/* példaprogram a fájlrendszerbéli névvel
 * rendelkező csővezeték használatára
 *
 * ez a program olvas a csőből
 *
 * fordítás:
 *    gcc -Wall fiforead.c -o fiforead
 * használat:
 *    ./fiforead fifo1
 * fifo1 a pipe neve a fájlrendszerben
 */

#include "myinclude.h"

#define N 4096

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("hiányzó fifo név\n");
        exit(1);
    }

    // van-e ilyen nevű fájl
    // nem tudjuk, melyik indul hamarabb
    if (access(argv[1], F_OK))  // ha nem létezik
    {
        // létrehozza a névvel rendelkező csövet
        //  lásd man 3 mkfifo
        if (mkfifo(argv[1], 0644) < 0) {
            syserr("mkfifo");
        }
    }

    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        syserr("open");
    }

    char buff[N];
    ssize_t n;
    while ((n = read(fd, buff, N)) > 0) {
        // feltételezzük, hogy nyomtatható sztringet
        // kapunk
        if (write(STDOUT_FILENO, buff, n) < 0) {
            syserr("stdout");
        }
    }
    // ha hiba
    if (n < 0) {
        syserr("read");
    }
    //
    close(fd);

    // az olvasó fél letörli a fifo-t
    if (unlink(argv[1]) < 0) {
        syserr("unlink");
    }

    exit(0);
}
