/*
 * Zombi folyamat példa
 * indítsuk el a programot:
 *
 *      gcc zombi.c -o zombi
 *      zombi &
 *
 * utána: nézzük meg többször egymás után az alábbi ps paranccsal:
 *
 * ps -o pid,ppid,stat,command
 *
 */

#include "myinclude.h"

int main() {
    pid_t child;
    child = fork();

    if (child == -1) {
        syserr("fork");
    }

    if (child == 0) {  // fiú folyamat
        sleep(10);
        exit(EXIT_SUCCESS);
        /* a fiú azonnal kilép, mivel az apa vár, a fiú
         * ettől a pillanattól kezdve zombi */
    }

    sleep(60);  // apa var
                // 60 szekundum múlva ér ide az apa, addig a fiú zombi

    int child_allapot;
    wait(&child_allapot);
    /* a wait fogadta a fiú befejezési kódját - termination status
     * ekkor a zombinak el kell tűnnie
     */
    printf("Apa alszik\n");
    sleep(30);
    /* itt még vár 30 szekundumot, megnézhetjük, hogy a zombi eltűnt */
    printf("Apa felébredt\n");

    printf("\nVége.\n");
    exit(EXIT_SUCCESS);
}
