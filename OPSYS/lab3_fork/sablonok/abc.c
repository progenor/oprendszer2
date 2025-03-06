/*
 *  Labor 3, Feladat 1 sablon
 */

#include "myinclude.h"

// n darab c karaktert ír a name nevű fájlba
int makefile(char *name, char c, int n) {
    int fd;
    // TODO: nyissuk meg írásra, új fájl, csonkolással

    // TODO: for ciklus n-szer, minden ciklus egyszer
    // kiírja a c karaktert a fájlba

    // fájl zárás
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    pid_t pid1, pid2;      // folyamat azonosítók
    int status1, status2;  // kilépési állapotok átvételéhez

    // első fiú indítása
    if ((pid1 = fork()) < 0) {
        syserr("f1");
    }
    if (pid1 == 0) {
        // TODO: első fiú kódja
        ...

            exit(EXIT_SUCCESS);  // a fiú itt mindenképp kilép, így
                                 // nem futhat rá a következő kódokra
    }

    // TODO: második fi
    // második fiú indítása
    ...

        // TODO: második fiú kódja
        ...

    // TODO: apa kódja, itt ír a fájlba
        ...

    // apa megvárja mindkét fiát
    wait(&status1);
    wait(&status2);

    exit(EXIT_SUCCESS);
}
