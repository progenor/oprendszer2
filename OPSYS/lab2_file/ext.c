/*
 * példaprogram az lseek használatára
 * a program a teszt.dat állomány végén túl pozicionálja az offset
 * változót és utána ír
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LEN 80

char buff[LEN];

int main(int argc, char* argv[]) {
    int n;
    long i;
    int fd;

    char* nev = "teszt.dat"; /* a fájl neve */
    buff[0] = 1;             /* ezt a kódot írjuk be az első írással */

    /* megnyitjuk írás/olvasásra, létrehozzuk mint új fájlt  ha nem létezik: O_CREAT
     * töröljük a tartalmát ha létezik: O_TRUNC
     */

    if ((fd = open(nev, O_CREAT | O_RDWR | O_TRUNC, 0644)) < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("A fájl neve %s\n", nev);

    /* 1 karakter fájlba írása */
    if ((n = write(fd, buff, 1)) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    /* előreviszem az offset-et a 101.-dik pozícióra */
    if ((i = lseek(fd, 100, SEEK_CUR)) < 0) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    printf("a seek %ld -t adott vissza\n", i);

    /* ráírok meg egy karaktert */
    *buff = 16;
    if ((n = write(fd, buff, 1)) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    /* a végső állomány hossza 102 karakter lesz */

    printf("a write %d -t adott vissza\n", n);

    /* fájl lezárva */
    if (close(fd) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    /* ismét megnyitom */
    if ((fd = open(nev, O_RDWR)) < 0) {
        perror("open2");
        exit(EXIT_FAILURE);
    }

    /* lekérem az offset-et az állomány végéről */
    if ((i = lseek(fd, 0, SEEK_END)) < 0) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    printf("az állomány hossza %ld byte\n", i);

    if (close(fd) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
