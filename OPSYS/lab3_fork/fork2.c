/*
 *  a fork függvény kezelése - apa és fiú folyamat
 *  külön kód részeken futnak a fork () után
 */

#include "myinclude.h"

int main() {
    printf("\n");

    // a kimenet pufferelését megszüntetjük, így a printf()
    //   függvény azonnal kiír miután meghívódik
    setbuf(stdout, NULL);

    pid_t apa_pid;
    apa_pid = getpid();  // apa pid száma

    pid_t fiu_pid;
    fiu_pid = fork();  // folyamat indítás

    if (fiu_pid < 0) {  // hibakezelés
        syserr("fork");
    }

    if (fiu_pid == 0) {
        // fiú kód része
        printf("Fiú: apa pid %d, fiú pid %d\n", (int)apa_pid, (int)getpid());

        printf("fiú vár\n");
        sleep(2);  // fiú vár 2 másodpercet

        printf("Fiú: kilép\n");
        exit(EXIT_SUCCESS);
    } else {
        /* apa kód része */
        printf("Apa: apa pid %d, fiú pid %d\n", (int)apa_pid, (int)fiu_pid);

        wait(NULL);  // megvárja a fiút, NULL a paraméter ha nem kell a kilépési érték

        printf("Apa: kilép\n");
        exit(EXIT_SUCCESS);
    }

    printf("ide nem érnek el!\n");
    return 0;
}
