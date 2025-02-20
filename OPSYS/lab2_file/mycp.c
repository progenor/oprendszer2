/* 
 * egyszerű állománymásoló program
 * fordítás:
 *      gcc -Wall mycp.c -o mycp
 * indítás:
 *      ./mycp file1 file2
 *
 * A program nem ellenőrzi, hogy a két fájlnév megegyezik.
 * Miért gond ez? mi fog történni, ha ugyanazt a nevét adjuk meg
 * forrás és cél fájlnak?

 * javítsuk ki: ha ugyanazt a nevét adjuk ki és bemenetnek, ne fusson le
 * a program
*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#define BUFFSIZE 1024  // a másolási puffer mérete

int main(int argc, char* argv[]) {
    int fd1, fd2, n;
    char buf[BUFFSIZE];  //másoláshoz használt puffer

    // van-é három argumentum
    if (argc != 3) {
        fprintf(stderr, "használat: %s f1 f2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // bemeneti fajl nyitas
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        perror("open1");
        exit(EXIT_FAILURE);
    }
    // kimeneti fajl letrehozas, ha letezik torli a regit
    if ((fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1) {
        perror("open2");
        exit(EXIT_FAILURE);
    }

    // masolas amig a read nem 0-t ad vissza
    while ((n = read(fd1, buf, BUFFSIZE)) > 0) {
        if ((write(fd2, buf, n) != n)) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    
    if (n < 0) { //ha a reda hibazik
        perror("read");
        exit(EXIT_FAILURE);
    }
    // az allomayokat zarni kell kilepes elott
    if (close(fd1) == -1 ){
            perror("write");
            exit(EXIT_FAILURE);
    }
    if (close(fd2) == -1 ){
            perror("write");
            exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
