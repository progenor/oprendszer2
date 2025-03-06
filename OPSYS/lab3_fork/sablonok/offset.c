/*
 * 3. labor 6. feladat sablon
 *
 * egy paramétere van, a fájl neve
 *
 */

#include "myinclude.h"

int main(int argc, char *argv[]) {
    char buff[] = "abc";  // ezt írjuk a fájlba

    // TODO: egy paraméter ellenőrzése, kilépés ha nincs

    int fd;  // fájl azonosító
    // TODO: fájl megnyitása írásra, csonkolás és új fájl

    // TODO:  fiú indítása a fork() függvénnyel

    // fiú kódja
    pid_t pid;  // folyamatazonosító
    if (pid == 0) {
        // TODO: a fiú beírja a 3 karaktert a fájlba
        // a buff címről

        // zárja a fájlt és kilép
        if (close(fd) < 0) {
            syserr("close");
        }
        exit(EXIT_SUCCESS);  // fiú kilép
    }
    // apa kódja
    //
    // TODO: apa megvárja  a fiát
    //

    off_t L;
    // TODO: lekérdezzük az L változóba, hol van az offset a
    // fájlban: a SEEK_CUR-hoz képest 0 pozícióba visszük
    // tk. az offset nem fog elmozdulni

    // kiírjuk, hol van az offset
    printf("poz: %ld\n", L);

    // TODO: az apa is beír 3 karaktert

    close(fd);
    exit(EXIT_SUCCESS);
}
