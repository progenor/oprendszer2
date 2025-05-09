/*
 * peldaprogram mutex szemafor hasznalatara IPC szemaforokkal
 *
 * a programban egy apa 3 fiu folyamatot indit, mindegyik ugyanazt a
 * feladatot kell elvegezze
 *
 * egyszerre csak egy fiu vegezheti a feladatot
 *
 * futtassuk le a peldat, utana vegyuk ki a
 *     sleep(t);
 * sort a fiuk feladatabol
 * hogyan valtozik meg a kritikus szakaszba valo lepes es miert igy?
 */

#include "myinclude.h"

// a feladat amit a fiuk kell vegezzenek felvaltva
// egy karaktert irnak ki (str), mindegyik mas karaktert
// varnak t masodpercig a kritikus szakaszban
// a feladatot k-szor vegzik el
void fiu_feladata(int semid, char *str, int t, int k)
{
    // szemafor műveletek
    struct sembuf up = {0, +1, 0};
    struct sembuf down = {0, -1, 0};

    pid_t pid1 = getpid();
    for (int i = 0; i < k; i++)
    {
        // kivon a szemforbol
        if (semop(semid, &down, 1) == -1)
            syserr("semop");
        // kritikus szakasz
        sleep(t);
        printf("kiir %s\n", str);

        // kritikus szakasz vege visszaallitja a szemafort
        if (semop(semid, &up, 1) == -1)
            syserr("semop");
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    int semid;
    pid_t p;
    setbuf(stdout, NULL);

    // egy szemafortomb lekerese amelyben 1 szemafor lesz
    // IPC_PRIVATE kulcsra egyszer lehet szemafort kerni, csak apa-fiu esetben hasznaljuk
    if ((semid = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
        syserr("semget");

    printf("semid=%d\n", semid);

    // a szemafor kezdeti erteke 0
    if (semctl(semid, 0, SETVAL, 1) == -1)
        syserr("semctl1");

    if ((p = fork()) == -1)
        syserr("fork");

    if (p == 0)
    {                                   // elso fiu
        fiu_feladata(semid, "a", 1, 3); // feladat utan a fiu a fuggvenyben kilep
    }

    if ((p = fork()) == -1)
        syserr("fork");

    if (p == 0)
    { // masodik fiu
        fiu_feladata(semid, "b", 1, 3);
    }

    if ((p = fork()) == -1)
        syserr("fork");

    if (p == 0)
    { // harmadik fiu
        fiu_feladata(semid, "c", 1, 3);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL); // apa mindharmat megvarja

    if (semctl(semid, 0, IPC_RMID, 0) == -1) // szemafor tomb torlese
        syserr("semctl2");

    printf("apa kilep\n");

    exit(EXIT_SUCCESS);
}
