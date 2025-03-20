/*
 *
 * példa az fdopen függvény használatára
 * 2. - féladat megoldása a 5_pipe_feladatok.pdf -ból
 * a program átnézése előtt olvassuk el az 1. és 2. feladatokat
 * tesztelés:
 *     gcc -Wall fdopen.c -o fdőpen
 *     ./fdopen
 */

#include "myinclude.h"

int main() {
    pid_t pid;
    // a pipe függvény hozza létre a csővezetek deszkriptorait
    int pfd[2];  // csővezetek
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    if ((pid = fork()) < 0) {
        syserr("fork");
    }

    // fiú folyamat:
    if (pid == 0) {
        // szabály: a folyamat lezárja a csővezetek azon felét amelyet nem használ
        close(pfd[0]);

        // átalakítja kettes szintű fájl mutatóvá a csővezetek azonosítóját
        FILE *fp;  // fájl mutató
        fp = fdopen(pfd[1], "w");
        if (fp == NULL) {
            syserr("fdopen");
        }

        // beir egy sztringet 2. szintu fuggvennyel
        char msg[] = "Helló!\n";
        if (fprintf(fp, "%s", msg) < 0) {
            syserr("fprintf");
        }
        // az fflush a standard könyvtár pufferét üríti ki
        fflush(fp);
        fclose(fp);
        exit(EXIT_SUCCESS);
    } else {            // apa folyamat:
        close(pfd[1]);  // nem használja
        // ez a folyamat olvas: amíg a fiú nem ír a csőbe,
        // addig a folyamat blokkolva van

        // file mutató lekérés a csőre
        FILE *fp;  // fájl mutató
        fp = fdopen(pfd[0], "r");
        if (fp == NULL) {
            syserr("fdopen");
        }
        // felső szintű faljlkezelő függvény használata
        int c;
        while ((c = fgetc(fp)) != EOF) {
            putchar(c);
        }

        fclose(fp);
        // fiúra vár
        int status;  // fiú kilépési állapota
        wait(&status);
        if (WIFSIGNALED(status)) {
            printf("fiú hiba, jelzés: %d\n", WTERMSIG(status));
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("fiú hiba, exit: %d\n", WEXITSTATUS(status));
        }
        exit(EXIT_SUCCESS);
    }
}
