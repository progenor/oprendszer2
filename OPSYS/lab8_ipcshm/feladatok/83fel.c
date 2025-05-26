#include "../myinclude.h"

// Két folyamat (apa és fiú) egy közös, N egész számból (pl. N=20) álló tömbhöz fér hozzá. A
// tömböt az apa folyamat hozza létre, és feltölti 0-kal.
// Apa folyamat elindítja a fiát, utána vár 5 milliszekundumot (usleep(5000)). Ez után mindkét
// folyamat azon versenyzik, hogy számokat írjon a tömb első szabad helyére.
// Egy hozzáférés során az apa 1-eseket, a fiú 2-eseket ír az első szabad helyre a tömb elejétől kezdve.

#define K 3   // fiúk száma
#define N 10  // tömb hossza
#define MS 20 // időzítés kritikus szakasz

enum sem
{
    mutex = 0,
    empty,
    full
};

static int semid; // szemafor az apa és a fiúk közötti szinkronizáláshoz
static int shmid; // megosztott memória az apa és a fiúk közötti kommunikációhoz
static int *ix;
static int *mem; // megosztott memória mutatója

void fiu_feladata(int i)
{
    struct sembuf fdown[] = {{mutex, -1, 0}, {empty, -1, 0}};
    struct sembuf fup = {mutex, +1, 0};

    printf("start: %d fiú pid=%d, csoport=%d\n", i, getpid(), getpgrp());
    while (1)
    {
        if (semop(semid, fdown, 2) == -1)
        {
            syserr("semop");
        }
        *(mem + *ix) = i;
        (*ix)++;
        usleep(MS);
        if (semop(semid, &fup, 1) == -1)
        {
            syserr("semop");
        }
        // TODO: a fiúk feladata
        // usleep(10000);
    }
    printf("end: fiú pid=%d\n", getpid());
}

void apaFeladata()
{
    struct sembuf adown = {full, -N, 0};
    struct sembuf aup = {empty, N, 0};
    int count = 0;
    if ((count = semctl(semid, 1, GETVAL)) == -1)
    {
        syserr("semctl");
    }
    for (int c = 0; c < count; c++)
    {
        if (semop(semid, &adown, 1) == -1)
        {
            syserr("semop");
        }
        for (int i = 0; i < N; i++)
        {
            printf("apa: %d. elem: %d\n", c, *(mem + i));
        }
        printf("\n");
        *ix = 0;
        if (semop(semid, &aup, 1) == -1)
        {
            syserr("semop");
        }
    }
}

int main(int argc, char **argv)
{
    int status;
    pid_t pid;
    int children = 0;   // elindított fiúk száma
    pid_t *pids = NULL; // fiúk pid számát tároló tömb mutatója
    int exited = 0;     // kilépett fiúk száma

    setbuf(stdout, NULL);

    printf("apa pid=%d, csoport=%d\n", getpid(), getpgrp());

    if ((semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0666)) == -1)
    {
        syserr("semget");
    }

    short init[] = {1, N, 0};
    if (semctl(semid, 0, SETALL, init) == -1)
    {
        syserr("semctl");
    }

    if ((shmid = shmget(IPC_PRIVATE, sizeof(int) * (N + 1), IPC_CREAT | 0666)) == -1)
    {
        syserr("shmget");
    }

    ix = (int *)shmat(shmid, NULL, 0);
    if (ix == (int *)-1)
    {
        syserr("shmat");
    }
    *ix = 0;
    mem = ix + 1;

    int i;
    for (i = 0; i < K; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            syserr("fork hiba");
        }
        if (pid == 0)
        {
            fiu_feladata(i);
            exit(EXIT_SUCCESS);
        }
        // tömb növelése
        pids = (pid_t *)realloc(pids, (i + 1) * sizeof(pid_t));
        if (pids == NULL)
        {
            syserr("pids");
        }
        pids[children++] = pid;
    }

    usleep(5000);
    apaFeladata();

    // fiúk leállítása
    while (children--)
    {
        if (kill(pids[children], SIGTERM) < 0)
        {
            syserr("kill");
        }
    }
    int wpid;
    while (exited != K)
    {
        wpid = wait(&status);
        exited++;
        printf("a pid=%d azonosítójú fiú kilépett\n", wpid);
    }

    free(pids);

    exit(EXIT_SUCCESS);
}
