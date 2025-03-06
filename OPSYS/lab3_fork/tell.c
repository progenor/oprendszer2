/*
 * nagyon egyszerű kommunikáció két folyamat között:
 *
 * a message nevű fájl jelenléte jelzi a fiú folyamatnak,
 * hogy az apa befejezett  egy feladatot és ő következik
 * amíg nincs message nevű fájl, a fiú vár
 */

#include "myinclude.h"

int main() {
    setbuf(stdout, NULL);  // a stdout-ra pufferelés nélkül írunk

    // mi történik ha másodszor futtatjuk a programot
    // és kivesszük az unlink() hívást?
    if (!access("message", F_OK)) {  // létezik a fájl?
        printf("torles\n");

        if (unlink("message") == -1) { /* töröljük  a message  nevű állományt*/
            syserr("nem lehet törölni a message nevű fájlt\n");
        }
    }

    pid_t fiu_pid;
    fiu_pid = fork();

    if (fiu_pid < 0) {
        syserr("fork");
    }
    if (fiu_pid == 0) {
        /* fiú vár addig, amíg megjelenik a fájl */
        while (access("message", F_OK) == -1) {
            usleep(300000);  // az usleep-nek mikroszekundumban 
                            //adjuk meg a várakozási időt
            // 300000 * 1 us = 300msec
        }
        printf("fiú: message megjött\n");
        sleep(1);
        printf("fiú kilép\n");
        exit(EXIT_SUCCESS); /* a fiú itt kilép */
    }
    // apa kódja
    printf("Pid számok, apa=%d fiú=%d\n", getpid(), fiu_pid);

    // apa folyamat kiír kis várakozással
    printf("apa: vár\n");
    sleep(3);  // hosszabb időt vár

    printf("apa: message létrejön\n");

    // itt létrehozza a message állományt
    int fd;
    if ((fd = open("message", O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0) {
        syserr("open message");
    }
    close(fd);
    wait(NULL);  // megvárja a fiát
    printf("apa: kilép\n");
    exit(EXIT_SUCCESS);
}
