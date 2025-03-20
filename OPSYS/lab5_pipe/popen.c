/*
 * példa a popen() használatára
 *
 * a popen() függvény egy új folyamatot indít, előtte azonban
 * egy csővezeteket hoz létré. Az új folyamatban (fiú) meghívja a shellt
 * a paraméterében megadott paranccsál, amely a pipe-ról
 * olvas vagy ír.
 *
 * Azt, hogy olvas vagy ír szintén a popen() paramétereiben kell megadni.
 *
 * pl.
 *      FILE * fp = popen ("grep 'alma'", "w");
 *
 *      Elindul a "grep alma" shell parancs, és a pipe amit a függvény
 *      visszaad írható lesz felső szintű függvényekkel.
 *      Az írás a parancs standard bemenetére történik.
 *      Hiba esetén NULL pointert ad vissza.
 *
 *      Az fp fájl mutatót a pclose(fp) hívással
 *      kell lezárni, a pclose() lezárja a pipe apa felöli oldalat,
 *      és elvégzi az indított fiúra vonatkozó  wait parancsot is
 *
 * fordítás:
 *      gcc -Wall popen.c -o popen
 * tesztelés:
 *      ./popen teszt.txt
 *
 */

#include "myinclude.h"
#define MAX 4096  // puffer hossza

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("használat: %s szoveg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fp;
    fp = popen("grep 'alma'", "w");
    if (fp == NULL) {
        syserr("popen");
    }

    // ebből olvasom a teszt adatokat, soronkent:
    FILE* fp1;
    if ((fp1 = fopen(argv[1], "r")) == NULL) {
        // ha az apa hibaval kilep:
        pclose(fp);  // a pclose lezárja a pipe írható felét
                     // ennek következtében a fiú kilép
                     // a pclose meghívja  a wait()-et is, így megvárja a fiút
        syserr("fopen");
    }

    char buff[MAX];
    while (fgets(buff, MAX, fp1) != NULL) {  // amíg van még sor, olvasom
        if (fprintf(fp, "%s", buff) < 0) {   // itt írom a pipe-ba
            syserr("fprintf");
        }
    }

    if (ferror(fp1) != 0) {
        printf("olvasási hiba a bemeneten\n");
    }

    fclose(fp1);

    // a pclose megvárja a popen-nel nyitott folyamatot
    // tartalmazza a wait függvényt is
    if ((pclose(fp)) < 0) {
        syserr("pclose");
    }

    exit(EXIT_SUCCESS);
}
