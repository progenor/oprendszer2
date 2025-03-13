/*
 * példa a dup2() függvény használatára
 * a fiú folyamat a:
 *    "tail -1 fájl1 > fájl2" parancsot futtatja 
 *
 * futtatás:
 *    ./dup a.txt b.txt
 */

#include "myinclude.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Használat: %s <fájl1> <fájl2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid;  // folyamat azonosító
    pid = fork();  // új folyamat
    if (pid < 0) {
        syserr("fork");
    }

    if (pid == 0) {
        // 1. megnyitjuk a fájlt írásra => fd
        int fd;
        if ((fd = open("a.txt", O_RDONLY, 0644)) == -1) {
            syserr("open");
        }

        fd = dup2(fd, STDIN_FILENO);
        execlp("tail", "tail", "-1", (char*)NULL);
        // ide már nem tér vissza
        syserr("execl");
    } else {
        // apa folyamat megvárja a fiút
        int status;  // kilépési állapot tárolására
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("A fiú sikeresen fejezte be a futását\n");
        }else{
            printf("A fiú hibával lépett ki\n");
        }
    }
    exit(EXIT_SUCCESS);
}
