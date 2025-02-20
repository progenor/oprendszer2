/*
 * egyszerű állománymásoló program
 * indítás: ./mycp file1 file2

 * ez a változat nem másol rá egy már létező állományra
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>

// használjuk az errno változót, amelyben a rendszerfüggvények
// a hibakódot hagyják
#include <errno.h>

#define BUFFSIZE 1024  // a használt puffer hossza

extern int errno;  // az errno külső valtozo

int main(int argc, char* argv[]) {
    int fd1, fd2, n;
    char buf[BUFFSIZE];

    // három paraméter kell legyen
    if (argc != 3) {
        printf("használat: %s f1 f2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // ha  a két név megegyezik, hiba
    if (!strcmp(argv[1], argv[2])) {
        printf("%s: a két fájlnév megegyezik!\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // az első állomány nyitása olvasásra
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        perror("open1");
        exit(EXIT_FAILURE);
    }

    // második állomány nyitása írásra, de O_EXCL opcióval
    // ha létezik a fájl, akkor hibát ad
    if ((fd2 = open(argv[2], O_CREAT | O_EXCL | O_WRONLY, 0644)) == -1) {
        perror("open2");
        // az errno változóban jelenik meg a hibakód
        // a létező hiba kódokat lásd: man 2 open /ERRORS
        if (errno == EEXIST) {
            printf("a cél fájl már  létezik\n");
        }
        exit(EXIT_FAILURE);
    }

    // másolás maximum BUFFSIZE szakaszokban
    while ((n = read(fd1, buf, BUFFSIZE)) > 0) {
        if ((write(fd2, buf, n) != n)) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (n < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    // a végén mindent fájlt lezárni !!!

    if (close(fd1) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    if (close(fd2) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
