/*
 * HIBÁS  kód !!!
 *
 * fork() működése ha a fork visszatérített értéke nincs lekezelve
 * !!! hibás programkód
 *
 * ha elég nagyra növeljük az i-t a ciklusban, vagy végtelen ciklust
 * írunk, akkor lefagyaszthatjuk vele a rendszert, lasd:
 * https://én.wikipedia.org/wiki/Fork_bomb
 *
 * Kérdés: hányszor hajtódik végre a fork() utáni printf
 * és mi jelenik meg az stdout-on indítás után ?
 */

#include "myinclude.h"

int main() {
    printf("\naz indító shell PID száma: %d", (int)getppid());

    setbuf(stdout, NULL);  // az stdout-ra való íráskor nincs puffer
                           // tehát amit küldök azonnal megjelenik

    for (size_t i = 0; i < 2; i++) {  // két ciklus, hány folyamat keletkezik?

        fork();

        printf("\nPID=%d, PPID=%d, fork után az i értéke i=%zu",
               (int)getpid(), (int)getppid(), i);

        sleep(1);  // 1 másodpercet vár
    }

    printf("\n");
    exit(EXIT_SUCCESS);
}
