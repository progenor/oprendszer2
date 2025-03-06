/*
 * Sablon: Labor 3, feladat 7.
 *
 * futtatás méréshez:
 *
 * ./kiazelso 100000
 *
 */

#include "myinclude.h"

int main(int argc, char *argv[]) {
    // ezt a két változót írjuk a fájlba
    char one[] = "1\n";
    char zero[] = "0\n";

    setbuf(stdout, NULL);

    size_t ciklus;
    // TODO: ha nincs parancssori paraméter, ciklus=1000,
    // ha van, akkor atoll() függvénnyel átalakítjuk
    // és beírjuk a ciklus-ba
    // ha 0-t kapunk hiba és kilépünk

    int fd;
    // TODO: megnyitjuk ki.txt fájlt írásra, csonkolás, új fájl

    // TODO: ellenőrzés, megnyílt-e a fájl

    // elvégezzük többször a kiírásokat
    for (size_t i = 0; i < ciklus; i++) {
        pid_t pid;
        // TODO: új folyamat, pid szám a pid változóba
        // ellenőrzés, hogy sikerült

        // fiú:
        if (pid == 0) {
            // TODO: beírja a zero változó tartalmát a fájlba, 2 karaktert

            exit(EXIT_SUCCESS);
        } else {  // apa:

            // TODO: beírja a one változó tartalmát a fájlba, 2 karaktert

            // TODO: apa megvárja a fiát
        }
    }
    exit(EXIT_SUCCESS);
}
