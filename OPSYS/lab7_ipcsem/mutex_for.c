/*
 * mutex tobb folyamattal
 *
 * P - megprobalja lehuzni a szemafort
 * V - elengedi a lehuzott szemafort
 *
 * a programban egy apa N fiu folyamatot indit, mindegyik ugyanazt a
 * feladatot kell elvegezze
 *
 * a fiuk inditasat ebben a peldaban az apa "for" ciklusban vegzi
 *
 * egyszerre csak egy fiu vegezheti a feladatot
 *
 * forditas:
 *    gcc -Wall mutex_for.c randlimit.c -o mutex_for
 */

#include "myinclude.h"
#include "randlimit.h"

#define N 5 /* a fiu folyamatok szama  */

// a feladat amit a fiuk kell vegezzenek felvaltva
// egy karaktert irnak ki, mindegyik mas karaktert
// varnak t masodpercig a kritikus szakaszban
void fiu_feladata(int semid, char *str, int t, int k) {
    // szemafor műveletek
    struct sembuf up = {0, +1, 0};
    struct sembuf down = {0, -1, 0};

    for (int i = 0; i < k; i++) {
        // kivon a szemforbol
        if (semop(semid, &down, 1) == -1) syserr("semop");
        // sleep(t);
        printf("%d kiír: %s\n", getpid(), str);
        // kritikus szakasz vege visszaallitja a szemafort
        if (semop(semid, &up, 1) == -1) syserr("semop");
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int semid;
    pid_t pid, wpid;
    setbuf(stdout, NULL);
    int i, j, status, r;
    char ch[] = "a";  // ide tesszuk a kiirando karaktert az "a" helyere

    setbuf(stdout, NULL);

    // egy szemafortomb lekerese amelyben 1 szemafor lesz
    if ((semid = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
        syserr("semget");

    // a szemafor kezdeti erteke 0
    if (semctl(semid, 0, SETVAL, 1) == -1)
        syserr("semctl1");

    for (i = 0; i < N; i++) {
        pid = fork();
        if (pid == -1) syserr("fork hiba");

        if (pid == 0) {
            //
            // a random generatort inicializaljuk
            // minden folyamatban mas maggal
            srand(getpid());

            // genaralunk egy karaktert a es z kozott
            ch[0] = 'a' + randlimit(25);

            //   a fiuk 0 es 4 szekundum kozt varakoznak a kritikus szakaszban
            r = randlimit(5);
            printf("fiú indul: pid:%d karakter:%s várakozás: %d sec\n", getpid(), ch, r);
            fiu_feladata(semid, ch, r, 3);
            // a fiu mar nem ter vissza a feladat utan, kilep
            //   a fuggvenyben
        } else {
            /* az apa itt semmit sem csinal, tovabb fut uj fiut inditani */
        }
    }
    /* ide csak az apa jut el */
    for (j = 0; j < N; j++) {
        wpid = wait(&status);
        printf("a pid=%d azonosítójú fiú véget ért\n", wpid);
    }

    /* szemafor tomb torlese */
    if (semctl(semid, 1, IPC_RMID, 0) == -1)  // szemafor tomb torlese
        syserr("semctl2");

    printf("apa kilep\n");

    exit(0);
}
