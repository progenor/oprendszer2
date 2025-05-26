#include "../myinclude.h"
#include <math.h>

static int shmid;
static int semid;

short init[] = {1, 0};

struct sembuf up0 = {0, 1, 0};
struct sembuf down0 = {0, -1, 0};
struct sembuf down1 = {1, -1, 0};
struct sembuf up1 = {1, 1, 0};

void fiufeladat()
{
    double *p = shmat(shmid, NULL, 0);
    if (p == (void *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    double sum = 0;
    int i;
    int done = 0;

    while (!done)
    {
        semop(semid, &down1, 1);

        for (i = 0; i < 3; i++)
        {
            if (p[i] > 999)
            {
                done = 1;
                break;
            }
            sum += p[i];
        }

        semop(semid, &up0, 1);

        if (done)
            break;
    }

    printf("Final sum: %lf\n", sum);
    shmdt(p);
    exit(EXIT_SUCCESS);
}

void apaFeladat()
{
    FILE *fp;
    double *szamok;
    double szam;
    int i = 0;

    fp = fopen("be.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    szamok = shmat(shmid, NULL, 0);
    if (szamok == (void *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    while (fscanf(fp, "%lf", &szam) == 1)
    {
        semop(semid, &down0, 1);

        if (szam > 999)
        {
            szamok[i] = szam;
            semop(semid, &up1, 1);
            break;
        }

        szamok[i++] = szam;

        if (i == 3)
        {
            semop(semid, &up1, 1);
            semop(semid, &down0, 1);
            i = 0;
        }

        semop(semid, &up0, 1);
    }

    if (i > 0 && i < 3 && szam <= 999)
    {
        szamok[i] = 1000;
        semop(semid, &up1, 1);
    }

    fclose(fp);
    shmdt(szamok);
}

int main()
{
    pid_t pid;

    shmid = shmget(IPC_PRIVATE, sizeof(double) * 3, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    if (semctl(semid, 0, SETALL, init) == -1)
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        fiufeladat();
    }
    else
    {
        apaFeladat();

        if (wait(NULL) == -1)
        {
            perror("wait");
            exit(EXIT_FAILURE);
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }

        if (semctl(semid, 0, IPC_RMID) == -1)
        {
            perror("semctl");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}