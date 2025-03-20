/*
 *
 * az apa folyamat egy nagy szöveges fájlt akár megjeleníteni
 * átírja csővezetéken a fiának, amelyben a less nevű
 * listázó programot futtatja
 *
 * tesztelés:s
 *    gcc -Wall useless.c -o useless
 *    ./useless 1.txt
 */

#include "myinclude.h"

/* a puffer hossza */
#define BUFF 4096

int main(int argc, char *argv[]) {
    pid_t pid;

    if (argc != 2) {
        printf("használat: %s fájlnév\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    // csővezeték létrehozás
    int pfd[2];
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    if ((pid = fork()) < 0) {
        syserr("fork");
    }

    // apa folyamat, ez ír a csővezetékbe, például az 1.txt nevű
    // nagy méretű állományt másolja
    if (pid > 0) {
        // lezárja a cső olvasható végét
        if (close(pfd[0]) < 0) {
            syserr("close");
        }
        int fd;
        // bemeneti fájl
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            syserr("open");
        }

        // átírja a fájlt a csővezetékbe
        ssize_t n;
        char buff[BUFF];
        while ((n = read(fd, buff, BUFF)) > 0) {
            if (write(pfd[1], buff, n) < 0) {
                syserr("write");
            }
        }

        if (n < 0) {
            syserr("read1");
        }
        close(fd);

        // a cső lezárása fontos!
        close(pfd[1]);
        // megvárja, hogy fia is befejezze
        int status;
        if (wait(&status) < 0)
            syserr("wait");

        exit(EXIT_SUCCESS);
    }
    /* fiú folyamat kódja */
    else {
        close(pfd[1]);  // ez a folyamat olvasni fog, az írható véget lezárja

        // hozzárendelem a pipe olvasható véget az stdin-hez
        if (dup2(pfd[0], STDIN_FILENO) < 0) {
            syserr("dup2");
        }

        close(pfd[0]); /* többet nincs rá szükség */

        // elindítom a less programot, ez a standard bemenetről olvassa
        // a szövegét
        execlp("less", "less", (char *)NULL);

        /* ide már nem érhetek el */
        /* a less ki fog lépni, ha lezárjuk a pipe másik végét */
        syserr("execlp");
    }
}
