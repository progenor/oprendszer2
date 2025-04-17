#include "myinclude_kuld.h"
/*
 * sorokat olvas a terminalrol es atkuldi
 * uzenetsoron egy masik folyamatnak
 * szamokat irunk be
 * ha 0 -at irunk be kilep, elotte megvar meg egy uzenetet
 * a masik programtol, majd torli a sort
 *
 */

static int qid;

int main() {
    setbuf(stdout, NULL);

    // uzenetsor KEY1 kulcsra
    if ((qid = msgget(KEY1, 0666)) == -1) {
        fprintf(stderr, "A sort nem lehet lekérni, második indul hamarabb\n");
        exit(EXIT_FAILURE);
    }

    // szamokat, amig 0
    printf("írjon be egész számokat, befejezéshez írjon 0-t\n");
    while (1) {
        printf("következő szám:");

        if (fgets(mbuff.b, BUFSIZE, stdin) == NULL) {  // EOF vagy hiba
            strcpy(mbuff.b, "0\n");                    // beirjuk a 0-t ha vege
        }

        mbuff.mtype = 1;
        if (msgsnd(qid, &mbuff, strlen(mbuff.b) + 1, 0) == -1) {
            syserr("msgsnd");
        }
        // ha 0 abbahagyja
        if (!strcmp(mbuff.b, "0\n")) {
            break;
        }
    }

    // megvarja az "ok"-t, ez a 2-es tipusu uzenet
    if (msgrcv(qid, &mbuff, BUFSIZE, 2, 0) == -1)
        syserr("msgrcv");

    exit(EXIT_SUCCESS);
}
